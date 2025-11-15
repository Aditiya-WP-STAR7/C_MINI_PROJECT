// extreme_precision.cpp
// Extreme Precision Arithmetic CLI
// Features:
//  - Configurable compile-time precision (DIGITS macro, e.g. -DDIGITS=2048)
//  - High-precision special functions (using Boost.Math where available)
//  - Adaptive Romberg integration with arbitrary precision backend
//  - Simple error propagation estimation for integrals -> derived quantities
//  - CLI: choose function, integration interval, tolerance, precision (via compile-time macro)
// NOTE: To target 1000+ digits, compile with -DDIGITS=1024 (or higher)

#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/erf.hpp>
#include <boost/math/special_functions/erf_inv.hpp>
#include <boost/math/special_functions/atanh.hpp>
#include <boost/math/special_functions/expint.hpp>

#ifndef DIGITS
// default precision: 1024 decimal digits (override at compile time with -DDIGITS=...)
#define DIGITS 1024
#endif

using BigFloat = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<DIGITS>>;

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::function;

// --- Utilities ---
static std::string precision_info() {
    std::ostringstream ss;
    ss << "Configured decimal digits: " << DIGITS;
    return ss.str();
}

template<typename T>
T absT(const T &x) { return x < 0 ? -x : x; }

template<typename T>
std::string to_string_scientific(const T &x, int digits = 20) {
    std::ostringstream ss;
    ss.precision(digits);
    ss << std::scientific << x;
    return ss.str();
}

// --- Special functions (wrappers) ---
BigFloat hp_exp(const BigFloat &x) { return boost::multiprecision::exp(x); }
BigFloat hp_log(const BigFloat &x) { return boost::multiprecision::log(x); }
BigFloat hp_sin(const BigFloat &x) { return boost::multiprecision::sin(x); }
BigFloat hp_cos(const BigFloat &x) { return boost::multiprecision::cos(x); }

BigFloat hp_tgamma(const BigFloat &x) {
    // Use boost::math::tgamma; if it has overload for cpp_dec_float it will be used.
    return boost::math::tgamma(x);
}

BigFloat hp_erf(const BigFloat &x) {
    return boost::math::erf(x);
}

BigFloat hp_erf_inv(const BigFloat &x) {
    return boost::math::erf_inv(x);
}

// --- CLI-selectable target functions ---
// We provide a set of sample "advanced" integrands and functions to evaluate.
// Users can add their own functions by modifying the dispatch.

struct FunctionSpec {
    std::string id;
    std::string description;
    function<BigFloat(const BigFloat&)> f;            // single-value function
    function<BigFloat(const BigFloat&)> integrand;    // integrand as function of x for integration (if different)
    bool integrable; // whether integration is supported
};

static std::vector<FunctionSpec> available_functions = {
    {"exp(-x^2)", "Gaussian exp(-x^2)", 
        [](const BigFloat& x)->BigFloat { return hp_exp(-x*x); },
        [](const BigFloat& x)->BigFloat { return hp_exp(-x*x); },
        true
    },
    {"sin(x)/x", "Sinc: sin(x)/x (with limit 1 at x=0)",
        [](const BigFloat& x)->BigFloat { if(x==0) return BigFloat(1); return hp_sin(x)/x; },
        [](const BigFloat& x)->BigFloat { if(x==0) return BigFloat(1); return hp_sin(x)/x; },
        true
    },
    {"gamma(x)", "Gamma function: Gamma(x)",
        [](const BigFloat& x)->BigFloat { return hp_tgamma(x); },
        nullptr,
        false
    },
    {"erf(x)", "Error function erf(x)",
        [](const BigFloat& x)->BigFloat { return hp_erf(x); },
        [](const BigFloat& x)->BigFloat { return hp_erf(x); },
        true
    },
    {"custom: x^a * exp(-b*x)", "Custom paramizable: x^a * exp(-b*x)",
        nullptr, nullptr, true
    }
};

// --- Adaptive Romberg integration with arbitrary precision ---
// Romberg integration: R(k,m) table and Richardson extrapolation
// We implement adaptive subdivision: subdivide until local error < tol

template<typename Real>
struct RombergIntegrator {
    // function to integrate: f(x)
    std::function<Real(const Real&)> f;
    RombergIntegrator(std::function<Real(const Real&)> func) : f(func) {}

    // composite trapezoid with n panels (n must be power of 2 for Romberg's typical progression)
    Real trapezoid(const Real &a, const Real &b, size_t n) {
        Real h = (b - a) / Real(n);
        Real sum = (f(a) + f(b)) / Real(2);
        for (size_t i=1;i<n;i++) {
            Real x = a + Real(i) * h;
            sum += f(x);
        }
        return sum * h;
    }

    // Romberg on [a,b] with initial depth `max_depth` and tolerance `tol`.
    // Returns pair(result, estimated_error)
    std::pair<Real,Real> integrate_adaptive(const Real &a, const Real &b, int max_depth, const Real &tol) {
        // Start with single panel, recursively subdivide if needed
        return integrate_recursive(a,b,1, max_depth, tol);
    }

private:
    std::pair<Real,Real> integrate_recursive(const Real &a, const Real &b, int depth, int max_depth, const Real &tol) {
        // Compute Romberg table with refinement levels up to R_{m}
        // We'll compute R(0,0) = trapezoid with n=1 panel, R(1,0) with n=2, etc.
        // Build column 0 with successive trapezoid refinements until depth or until convergence.
        std::vector<Real> Rcol;
        Rcol.reserve(depth+4);

        // Use exponential growth in panels: n = 2^k
        for (int k=0;k<=std::min(depth+4, max_depth); ++k) {
            size_t n = size_t(1ull << k);
            Real Tk = trapezoid(a,b,n);
            Rcol.push_back(Tk);
            // Attempt Richardson extrapolation on current column
            // Build Romberg table in-place for this column
            std::vector<Real> T = Rcol;
            for (size_t m=1; m<T.size(); ++m) {
                // R[k][m] = R[k][m-1] + (R[k][m-1] - R[k-1][m-1])/(4^m - 1)
                Real numerator = T[T.size()-m] - T[T.size()-m-1];
                Real factor = boost::multiprecision::pow(Real(4), Real(m)) - Real(1);
                Real extrap = T[T.size()-m] + numerator / factor;
                T[T.size()-m] = extrap;
            }
            // T.back() holds the most extrapolated estimate using current info
            if (k>=2) {
                // estimate error by difference of last two extrapolated values (conservative)
                Real err = absT(T.back() - Rcol.back());
                // When error is small enough with respect to tol, accept.
                if (err < tol) {
                    return {T.back(), err};
                }
            }
        }

        // If not converged, try subdividing if depth < max_depth
        if (depth >= max_depth) {
            // return best estimate we have with a conservative error
            // use last trapezoid over highest panels we computed
            size_t best_n = size_t(1ull << std::min(max_depth, 6));
            Real best_est = trapezoid(a,b,best_n);
            Real estimated_error = absT(best_est - Rcol.back());
            return {best_est, estimated_error};
        } else {
            // subdivide interval into two halves and integrate each recursively
            Real mid = (a + b) / Real(2);
            auto left = integrate_recursive(a, mid, depth+1, max_depth, tol/2);
            auto right = integrate_recursive(mid, b, depth+1, max_depth, tol/2);
            Real sum = left.first + right.first;
            Real err = left.second + right.second;
            return {sum, err};
        }
    }
};

// --- Error propagation helper ---
// Simple forward error-propagation: if integral I +/- eI and we compute g(I),
// estimate g(I+e) - g(I) ~ |g'(I)| * e (first-order). We compute derivative numerically
// with high-precision central difference.
template<typename Real>
struct ErrorPropagator {
    static Real derivative(std::function<Real(const Real&)> g, const Real &x, const Real &eps) {
        Real h = eps > Real(0) ? eps : Real(1e-10);
        Real xm = x - h;
        Real xp = x + h;
        return (g(xp) - g(xm)) / (Real(2) * h);
    }

    // propagate: returns pair(value_of_g, estimated_error)
    static std::pair<Real,Real> propagate(std::function<Real(const Real&)> g, const Real &x, const Real &err) {
        Real gx = g(x);
        // choose step size proportional to error and precision
        Real h = err == Real(0) ? boost::multiprecision::pow(Real(10), Real(-int(DIGITS/2))) : err;
        Real gp = derivative(g, x, h);
        Real propagated = absT(gp) * err;
        return {gx, propagated};
    }
};

// --- CLI parsing & driver ---
void print_usage(const char* prog) {
    cout << "Extreme Precision Arithmetic CLI\n";
    cout << precision_info() << "\n";
    cout << "Usage: " << prog << " [options]\n\n";
    cout << "Options:\n";
    cout << "  --list                         List available prebuilt functions\n";
    cout << "  --function <id>                Select function id to evaluate/integrate\n";
    cout << "  --integrate a b tol maxdepth   Integrate selected function on [a,b]\n";
    cout << "  --eval x                       Evaluate selected function at x\n";
    cout << "  --precision-note               Show note about compile-time precision\n";
    cout << "\nExamples:\n";
    cout << "  " << prog << " --list\n";
    cout << "  " << prog << " --function \"exp(-x^2)\" --integrate 0 1 1e-100 16\n";
    cout << "  " << prog << " --function \"sin(x)/x\" --integrate 0 10 1e-200 18\n";
    cout << "\nNote: For very high digits (>=1000), recompile with -DDIGITS=<digits>.\n";
}

int main(int argc, char** argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }

    std::string func_id;
    bool want_list = false;
    bool want_eval = false;
    bool want_integrate = false;
    BigFloat eval_x = 0;
    BigFloat a = 0, b = 1;
    // tolerance as decimal string -> BigFloat
    BigFloat tol = boost::multiprecision::pow(BigFloat(10), BigFloat(-50));
    int maxdepth = 16;

    // Very simple CLI parsing (no heavy deps)
    for (int i=1;i<argc;i++) {
        std::string s = argv[i];
        if (s=="--list") want_list = true;
        else if (s=="--precision-note") { cout << precision_info() << "\n"; return 0; }
        else if (s=="--function" && i+1<argc) { func_id = argv[++i]; }
        else if (s=="--eval" && i+1<argc) { want_eval = true; eval_x = BigFloat(argv[++i]); }
        else if (s=="--integrate" && i+3<argc) {
            want_integrate = true;
            a = BigFloat(argv[++i]);
            b = BigFloat(argv[++i]);
            tol = BigFloat(argv[++i]);
            if (i+1<argc && argv[i+1][0] != '-') {
                maxdepth = std::stoi(argv[++i]);
            }
        } else {
            cout << "Unknown or incomplete option: " << s << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    if (want_list) {
        cout << "Available functions:\n";
        for (auto &fs : available_functions) {
            cout << "  id: \"" << fs.id << "\"\n    " << fs.description << "\n";
        }
        return 0;
    }

    if (func_id.empty()) {
        cout << "No function selected. Use --function <id> or --list to see choices.\n";
        return 1;
    }

    // find function
    FunctionSpec *chosen = nullptr;
    for (auto &fs: available_functions) {
        if (fs.id == func_id) { chosen = &fs; break; }
    }
    if (!chosen) {
        cout << "Function id not found: " << func_id << "\n";
        return 1;
    }

    cout << "Selected function: " << chosen->id << " - " << chosen->description << "\n";
    cout << precision_info() << "\n";

    if (want_eval) {
        if (!chosen->f) { cout << "Evaluation not supported for this function.\n"; return 1; }
        BigFloat v = chosen->f(eval_x);
        cout << "f(" << to_string_scientific(eval_x, 12) << ") =\n" << to_string_scientific(v, 60) << "\n";
    }

    if (want_integrate) {
        if (!chosen->integrable || !chosen->integrand) {
            cout << "Integration not supported for this function.\n";
            return 1;
        }
        cout << "Integrating over [" << to_string_scientific(a,10) << ", " << to_string_scientific(b,10) << "]\n";
        cout << "Requested tolerance: " << to_string_scientific(tol, 10) << ", max depth: " << maxdepth << "\n";

        RombergIntegrator<BigFloat> integrator(chosen->integrand);
        auto res = integrator.integrate_adaptive(a,b,maxdepth,tol);

        BigFloat integral = res.first;
        BigFloat est_err = res.second;

        cout << "Integral =\n" << to_string_scientific(integral, 80) << "\n";
        cout << "Estimated integration error = " << to_string_scientific(est_err, 40) << "\n";

        // Basic error propagation: demonstrate computing g(I) = log(I + 1) and propagate error
        auto g = [](const BigFloat &x)->BigFloat { return hp_log(x + BigFloat(1)); };
        auto propagated = ErrorPropagator<BigFloat>::propagate<BigFloat>(g, integral, est_err);
        cout << "\nExample propagation through g(I)=log(I+1):\n";
        cout << "g(I) = " << to_string_scientific(propagated.first, 60) << "\n";
        cout << "Estimated propagated error = " << to_string_scientific(propagated.second, 40) << "\n";
    }

    cout << "\nDone.\n";
    return 0;
}

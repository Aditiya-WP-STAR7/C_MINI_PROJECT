#include <iostream>
#include <iomanip>
#include <vector>
#include <complex>
#include <algorithm>
#include <fstream>
#include <limits>
#include <cmath>

using namespace std;
using cd = complex<double>;

struct HistoryEntry {
    vector<cd> coeffs;
    vector<cd> roots;
};

vector<HistoryEntry> historyLog;

// Horner Evaluation
cd horner(const vector<cd>& a, cd x) {
    cd r = a[0];
    for (size_t i = 1; i < a.size(); ++i)
        r = r * x + a[i];
    return r;
}

// Derivative-aware evaluation
pair<cd,cd> horner_with_derivative(const vector<cd>& a, cd x) {
    cd p = a[0], dp = cd(0,0);
    for (size_t i = 1; i < a.size(); ++i) {
        dp = dp * x + p;
        p = p * x + a[i];
    }
    return {p, dp};
}

// Scale polynomial for numerical stability
void scale(vector<cd>& a) {
    double mx = 0;
    for (auto &c : a) mx = max(mx, abs(c));
    if (mx > 0) for (auto &c : a) c /= mx;
}

// Generate initial guesses on circle
vector<cd> initial_guesses(int n) {
    vector<cd> x(n);
    double theta = 2.0 * M_PI / n, R = 1.2;
    for (int k = 0; k < n; ++k)
        x[k] = cd(R * cos(theta*k), R * sin(theta*k));
    return x;
}

// Aberth–Ehrlich root solver
vector<cd> aberth_ehrlich(const vector<cd>& a, double tol, int maxit = 2000) {
    int n = a.size() - 1;
    if (n == 1) return { -a[1] / a[0] };

    vector<cd> x = initial_guesses(n), dx(n);

    for (int it = 0; it < maxit; ++it) {
        bool done = true;
        for (int i = 0; i < n; ++i) {
            auto pr = horner_with_derivative(a, x[i]);
            cd p = pr.first, dp = pr.second;
            if (abs(p) < tol) { dx[i] = 0; continue; }

            cd corr = p / dp;
            cd S = 0;
            for (int j = 0; j < n; ++j) if (i != j) S += 1.0 / (x[i] - x[j]);
            dx[i] = corr / (1.0 - corr * S);

            if (abs(dx[i]) > tol) done = false;
        }
        for (int i = 0; i < n; ++i) x[i] -= dx[i];
        if (done) break;
    }
    return x;
}

// Display utilities
void print_polynomial(const vector<cd>& a) {
    cout << "P(x) = ";
    int n = a.size() - 1;
    for (int i = 0; i <= n; ++i) {
        cout << "(" << a[i] << ")";
        if (i < n) cout << " * x^" << (n - i) << " + ";
    }
    cout << "\n";
}

void print_roots(const vector<cd>& roots, const vector<cd>& coeffs) {
    cout << "\nRoots:\n";
    for (auto &r : roots)
        cout << r << "   |  |P(r)| = " << abs(horner(coeffs, r)) << "\n";
}

// Banner
void banner() {
    cout << "\n==========================================================\n";
    cout << "        Polynomial Root Solver (Aberth–Ehrlich Method)\n";
    cout << "==========================================================\n\n";
}

// Show History (FIXED)
void show_history() {
    if (historyLog.empty()) {
        cout << "\n(No history stored)\n";
        return;
    }

    cout << "\n--- Computation History ---\n";
    for (int i = 0; i < historyLog.size(); ++i) {
        cout << "\nCase #" << i+1 << ":\n";
        print_polynomial(historyLog[i].coeffs);
        print_roots(historyLog[i].roots, historyLog[i].coeffs);
    }
}

// Export to file
void export_to_file(const vector<cd>& roots) {
    ofstream out("results.txt", ios::app);
    out << "\nRoots:\n";
    for (auto &r : roots) out << r << "\n";
    out.close();
    cout << "\n(Saved to results.txt)\n";
}

int main() {
    banner();
    cout << fixed << setprecision(12);

    while (true) {
        cout << "\nMenu:\n";
        cout << "1) Compute polynomial roots\n";
        cout << "2) Evaluate polynomial at x\n";
        cout << "3) Show history\n";
        cout << "4) Exit\n";
        cout << "Select: ";

        int choice;
        cin >> choice;

        if (choice == 4) break;

        if (choice == 3) { show_history(); continue; }

        if (choice == 2) {
            int n; cout << "Degree n: "; cin >> n;
            vector<cd> a(n+1);
            cout << "Enter coefficients: ";
            for (int i = 0; i <= n; i++) cin >> a[i];
            double xr, xi;
            cout << "Enter x (real imag): ";
            cin >> xr >> xi;
            cd x(xr, xi);
            cout << "P(x) = " << horner(a, x) << "\n";
            continue;
        }

        if (choice == 1) {
            int n;
            cout << "Degree n: "; cin >> n;
            vector<cd> a(n+1);
            cout << "Enter coefficients (a_n ... a_0): ";
            for (int i = 0; i <= n; i++) cin >> a[i];

            double tol;
            cout << "Tolerance: "; cin >> tol;

            scale(a);
            vector<cd> roots = aberth_ehrlich(a, tol);
            sort(roots.begin(), roots.end(), [](cd x, cd y){ return abs(x) < abs(y); });

            print_roots(roots, a);
            historyLog.push_back({a, roots});
            export_to_file(roots);
        }
    }
}

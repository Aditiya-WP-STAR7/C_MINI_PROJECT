#include <iostream>
#include <iomanip>
#include <cmath>
#include <tuple>

using namespace std;

const int N = 3;
typedef long double ld;

ld determinant(ld M[N][N]) {
    return M[0][0]*(M[1][1]*M[2][2] - M[1][2]*M[2][1])
         - M[0][1]*(M[1][0]*M[2][2] - M[1][2]*M[2][0])
         + M[0][2]*(M[1][0]*M[2][1] - M[1][1]*M[2][0]);
}

ld trace(ld M[N][N]) {
    return M[0][0] + M[1][1] + M[2][2];
}

void commutator(ld P[N][N], ld L[N][N], ld result[N][N]) {
    ld PL[N][N] = {}, LP[N][N] = {};
    for (int i=0; i<N; ++i)
        for (int j=0; j<N; ++j)
            for (int k=0; k<N; ++k) {
                PL[i][j] += P[i][k]*L[k][j];
                LP[i][j] += L[i][k]*P[k][j];
            }
    for (int i=0; i<N; ++i)
        for (int j=0; j<N; ++j)
            result[i][j] = PL[i][j] - LP[i][j];
}

tuple<ld, ld, ld> eigenvalues(ld M[N][N]) {
    ld a = trace(M);
    ld b = M[0][0]*M[1][1] + M[0][0]*M[2][2] + M[1][1]*M[2][2]
         - M[0][1]*M[1][0] - M[0][2]*M[2][0] - M[1][2]*M[2][1];
    ld c = determinant(M);

    ld q = (3*b - a*a)/9;
    ld r = (9*a*b - 27*c - 2*a*a*a)/54;
    ld D = q*q*q + r*r;

    ld eig1, eig2, eig3;

    if (D >= 0) {
        ld sqrtD = sqrt(D);
        ld s = cbrtl(r + sqrtD);
        ld t = cbrtl(r - sqrtD);
        eig1 = s + t + a/3.0;
        eig2 = eig3 = NAN;
    } else {
        ld theta = acos(r / sqrt(-q*q*q));
        eig1 = 2*sqrt(-q)*cos(theta/3) + a/3;
        eig2 = 2*sqrt(-q)*cos((theta + 2*M_PI)/3) + a/3;
        eig3 = 2*sqrt(-q)*cos((theta + 4*M_PI)/3) + a/3;
    }

    return {eig1, eig2, eig3};
}

void printMatrix(ld M[N][N], const string& name) {
    cout << name << " = \n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << setw(15) << M[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    while (true) {
        ld L[N][N], P[N][N];

        cout << "\n=== INPUT MATRIX L (3x3) ===\n";
        for (int i=0; i<N; ++i)
            for (int j=0; j<N; ++j) {
                cout << "L[" << i << "][" << j << "] = ";
                cin >> L[i][j];
            }

        cout << "\n=== INPUT MATRIX P (3x3) ===\n";
        for (int i=0; i<N; ++i)
            for (int j=0; j<N; ++j) {
                cout << "P[" << i << "][" << j << "] = ";
                cin >> P[i][j];
            }

        ld initial_trace = trace(L);
        ld initial_det = determinant(L);
        auto [eig1_init, eig2_init, eig3_init] = eigenvalues(L);

        ld dt = 0.001;
        int steps = 100;
        for (int step = 0; step < steps; ++step) {
            ld dL[N][N];
            commutator(P, L, dL);
            for (int i=0; i<N; ++i)
                for (int j=0; j<N; ++j)
                    L[i][j] += dt * dL[i][j];
        }

        ld final_trace = trace(L);
        ld final_det = determinant(L);
        auto [eig1_final, eig2_final, eig3_final] = eigenvalues(L);

        cout << fixed << setprecision(15);
        cout << "\n=== RESULTS ===\n";
        printMatrix(L, "Final L Matrix");

        cout << "\n-- INVARIANTS --\n";
        cout << "Initial Trace       : " << initial_trace << "\n";
        cout << "Final Trace         : " << final_trace << "\n";
        cout << "Trace Difference    : " << fabsl(final_trace - initial_trace) << "\n\n";

        cout << "Initial Determinant : " << initial_det << "\n";
        cout << "Final Determinant   : " << final_det << "\n";
        cout << "Determinant Diff    : " << fabsl(final_det - initial_det) << "\n\n";

        cout << "-- EIGENVALUES --\n";
        cout << "Initial Eigenvalues : " << eig1_init << ", " << eig2_init << ", " << eig3_init << "\n";
        cout << "Final Eigenvalues   : " << eig1_final << ", " << eig2_final << ", " << eig3_final << "\n";

        char again;
        cout << "\nWould you like to run another simulation? (y/n): ";
        cin >> again;
        if (again != 'y' && again != 'Y') {
            cout << "Program terminated.\n";
            break;
        }
    }
}

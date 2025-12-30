#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// =============================
// MODEL PARAMETERS (GLOBAL)
// =============================
const int NX = 60;
const int NY = 60;
const double DX = 0.5;
const double DY = 0.5;
const double DT = 0.01;

// =============================
// HETEROGENEOUS DIFFUSION FIELD
// =============================
double diffusion(int x, int y) {
    double cx = NX / 2.0;
    double cy = NY / 2.0;
    double r = sqrt((x - cx)*(x - cx) + (y - cy)*(y - cy));
    return 0.08 + 0.02 * exp(-r / 12.0);
}

// =============================
// IMMUNOTHERAPY / RADIATION
// =============================
double therapy(double t) {
    if (t > 10 && t < 25) return 1.0;
    return 0.0;
}

// =============================
// MAIN SIMULATION FUNCTION
// =============================
void simulate(double rho, double K, double beta, int steps) {

    vector<vector<double>> u(NX, vector<double>(NY, 0.0));
    vector<vector<double>> u_next = u;

    // Initial tumor seed
    u[NX/2][NY/2] = 0.15 * K;

    for (int step = 0; step < steps; step++) {
        double time = step * DT;

        for (int i = 1; i < NX-1; i++) {
            for (int j = 1; j < NY-1; j++) {

                double D = diffusion(i, j);

                double laplacian =
                    (u[i+1][j] - 2*u[i][j] + u[i-1][j]) / (DX*DX) +
                    (u[i][j+1] - 2*u[i][j] + u[i][j-1]) / (DY*DY);

                double reaction = rho * u[i][j] * (1.0 - u[i][j] / K);
                double treatment = beta * u[i][j] * therapy(time);

                u_next[i][j] =
                    u[i][j] + DT * (D * laplacian + reaction - treatment);

                if (u_next[i][j] < 0) u_next[i][j] = 0;
            }
        }

        u = u_next;

        // Output every 50 steps
        if (step % 50 == 0) {
            double total = 0.0;
            for (auto &row : u)
                for (double v : row)
                    total += v;

            cout << "Time = " << fixed << setprecision(2) << time
                 << " | Total Tumor Burden = " << total << endl;
        }
    }

    cout << "\nFinal Tumor Density Slice (center row):\n";
    for (int j = 0; j < NY; j++) {
        cout << fixed << setprecision(3) << u[NX/2][j] << " ";
    }
    cout << endl;
}

// =============================
// MAIN CLI LOOP
// =============================
int main() {

    cout << "=============================================\n";
    cout << " Glioblastoma Tumor Growth PDE Simulator\n";
    cout << " Reaction–Diffusion Fisher–Kolmogorov Model\n";
    cout << "=============================================\n\n";

    char repeat;

    do {
        double rho, K, beta;
        int steps;

        cout << "Enter proliferation rate (rho): ";
        cin >> rho;

        cout << "Enter carrying capacity (K): ";
        cin >> K;

        cout << "Enter therapy effectiveness (beta): ";
        cin >> beta;

        cout << "Enter number of time steps: ";
        cin >> steps;

        cout << "\nRunning simulation...\n\n";
        simulate(rho, K, beta, steps);

        cout << "\nRun another simulation? (y/n): ";
        cin >> repeat;

    } while (repeat == 'y' || repeat == 'Y');

    cout << "\nProgram finished successfully.\n";
    return 0;
}

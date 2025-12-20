#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <string>

using namespace std;

/*
==========================================================
 CMB Gravitational Lensing Simulation (CLI Research Tool)
 Author : Aditiya WP
 Target : MIT / Top Global Universities
 Language : C++17 (Cxxdroid Compatible)
==========================================================
*/

constexpr int N = 64;          // Angular resolution
constexpr double PI = 3.141592653589793;
constexpr double G = 1.0;      // Scaled gravitational constant

// 2D Grid Type
using Grid = vector<vector<double>>;

// Generate Gaussian random field (Primary CMB)
Grid generatePrimaryCMB() {
    Grid cmb(N, vector<double>(N));
    default_random_engine gen;
    normal_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cmb[i][j] = dist(gen);

    return cmb;
}

// Simulated Large-Scale Structure Mass Distribution
Grid generateMassDistribution() {
    Grid rho(N, vector<double>(N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double x = (i - N / 2.0) / N;
            double y = (j - N / 2.0) / N;
            rho[i][j] = exp(-(x * x + y * y) * 20.0);
        }
    }
    return rho;
}

// Compute Lensing Potential φ from mass distribution
Grid computeLensingPotential(const Grid& rho) {
    Grid phi(N, vector<double>(N, 0.0));

    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            phi[i][j] =
                G * (
                    rho[i + 1][j] +
                    rho[i - 1][j] +
                    rho[i][j + 1] +
                    rho[i][j - 1] -
                    4.0 * rho[i][j]
                );
        }
    }
    return phi;
}

// Compute deflection field α = ∇φ
void computeDeflection(
    const Grid& phi,
    Grid& alphaX,
    Grid& alphaY
) {
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            alphaX[i][j] = (phi[i + 1][j] - phi[i - 1][j]) / 2.0;
            alphaY[i][j] = (phi[i][j + 1] - phi[i][j - 1]) / 2.0;
        }
    }
}

// Apply lensing remapping
Grid lensCMB(
    const Grid& cmb,
    const Grid& alphaX,
    const Grid& alphaY
) {
    Grid lensed(N, vector<double>(N, 0.0));

    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            int newX = int(i + alphaX[i][j]);
            int newY = int(j + alphaY[i][j]);

            if (newX >= 0 && newX < N && newY >= 0 && newY < N)
                lensed[i][j] = cmb[newX][newY];
            else
                lensed[i][j] = cmb[i][j];
        }
    }
    return lensed;
}

// Display a small patch
void displaySample(const Grid& map, const string& title) {
    cout << "\n--- " << title << " (Central Patch) ---\n";
    for (int i = N / 2 - 3; i <= N / 2 + 3; i++) {
        for (int j = N / 2 - 3; j <= N / 2 + 3; j++) {
            cout << setw(8) << fixed << setprecision(3) << map[i][j] << " ";
        }
        cout << endl;
    }
}

// Main CLI Loop
int main() {
    cout << "\n============================================\n";
    cout << " CMB Gravitational Lensing CLI Simulator\n";
    cout << " Research-Level Computational Physics Tool\n";
    cout << "============================================\n";

    char choice;
    do {
        Grid cmb = generatePrimaryCMB();
        Grid rho = generateMassDistribution();
        Grid phi = computeLensingPotential(rho);

        Grid alphaX(N, vector<double>(N, 0.0));
        Grid alphaY(N, vector<double>(N, 0.0));
        computeDeflection(phi, alphaX, alphaY);

        Grid lensedCMB = lensCMB(cmb, alphaX, alphaY);

        displaySample(cmb, "Primary CMB");
        displaySample(lensedCMB, "Lensed CMB");

        cout << "\nRe-run simulation with new realization? (y/n): ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    cout << "\nSimulation terminated. Scientific integrity preserved.\n";
    return 0;
}

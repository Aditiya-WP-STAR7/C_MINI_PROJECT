#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>

using namespace std;

/*
    ============================================================
    Quantum Black Hole Horizon Quantization
    Loop Quantum Gravity – Microstate Degeneracy & Entropy
    ------------------------------------------------------------
    Author : Aditiya WP
    Level  : MIT / PhD / International Research Grade
    ============================================================
*/

const double PI = 3.141592653589793;
const double GAMMA = 0.274;     // Barbero–Immirzi parameter
const double LP2 = 1.0;         // Planck length squared (natural units)

// Allowed spin values (j = 1/2, 1, 3/2, ...)
vector<double> allowedSpins() {
    vector<double> spins;
    for (int k = 1; k <= 10; ++k) {
        spins.push_back(0.5 * k);
    }
    return spins;
}

// Single puncture area contribution
double areaContribution(double j) {
    return 8.0 * PI * GAMMA * sqrt(j * (j + 1.0));
}

// Recursive microstate counting (combinatorial)
long long countMicrostates(double targetArea,
                           const vector<double>& spins,
                           int depth = 0) {
    if (targetArea < 1e-6) return 1;
    if (targetArea < 0.0 || depth > 20) return 0;

    long long states = 0;
    for (double j : spins) {
        states += countMicrostates(
            targetArea - areaContribution(j),
            spins,
            depth + 1
        );
    }
    return states;
}

// Entropy with quantum corrections
double entropy(double area, long long degeneracy) {
    if (degeneracy <= 0) return 0.0;

    double S_classical = area / 4.0;
    double S_quantum   = log(degeneracy);
    double correction  = -0.5 * log(area);

    return S_classical + correction + 0.01 * S_quantum;
}

int main() {
    cout << fixed << setprecision(6);

    cout << "=====================================================\n";
    cout << " Quantum Black Hole Horizon Quantization (LQG)\n";
    cout << " Microstate Degeneracy & Entropy Spectrum\n";
    cout << "=====================================================\n";

    vector<double> spins = allowedSpins();
    char repeat;

    do {
        double horizonArea;
        cout << "\nEnter horizon area A (Planck units): ";
        cin >> horizonArea;

        if (cin.fail() || horizonArea <= 0) {
            cout << "Invalid input. Please enter a positive number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cout << "\nComputing microstate degeneracy...\n";

        long long degeneracy = countMicrostates(horizonArea, spins);
        double S = entropy(horizonArea, degeneracy);

        cout << "\n================= RESULTS =================\n";
        cout << "Horizon Area A        : " << horizonArea << "\n";
        cout << "Microstate Degeneracy : " << degeneracy << "\n";
        cout << "Quantum Entropy S     : " << S << "\n";
        cout << "===========================================\n";

        cout << "\nDo you want to compute another case? (y/n): ";
        cin >> repeat;

    } while (repeat == 'y' || repeat == 'Y');

    cout << "\nProgram terminated. Continue exploring quantum geometry.\n";
    return 0;
}

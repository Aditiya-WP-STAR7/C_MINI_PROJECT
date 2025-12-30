#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>

using namespace std;

/*
==========================================================
 Non-Linear Multi-Compartment Pharmacokinetic Model
 Michaelis-Menten Metabolism
 Author Level : MIT / PhD-Level Computational Modeling
==========================================================
*/

// Structure for PK parameters
struct PKParameters {
    double Vmax;
    double Km;
    double kel;
    double k12;
    double k21;
    double k23;
    double k32;
};

// Time-dependent Vmax (enzyme induction example)
double timeDependentVmax(double t, double Vmax0) {
    return Vmax0 * (1.0 + 0.1 * sin(0.1 * t));
}

// Differential equations system
vector<double> derivatives(double t, const vector<double>& C, const PKParameters& p) {
    double Vmax_t = timeDependentVmax(t, p.Vmax);

    double C1 = C[0];
    double C2 = C[1];
    double C3 = C[2];

    double dC1 =
        - (Vmax_t * C1) / (p.Km + C1)
        - p.kel * C1
        - p.k12 * C1
        + p.k21 * C2;

    double dC2 =
        p.k12 * C1
        - p.k21 * C2
        - p.k23 * C2
        + p.k32 * C3;

    double dC3 =
        p.k23 * C2
        - p.k32 * C3;

    return {dC1, dC2, dC3};
}

// RK4 Integrator
vector<double> rk4Step(
    double t,
    double h,
    const vector<double>& C,
    const PKParameters& p
) {
    vector<double> k1 = derivatives(t, C, p);

    vector<double> C2(3);
    for (int i = 0; i < 3; i++)
        C2[i] = C[i] + 0.5 * h * k1[i];
    vector<double> k2 = derivatives(t + 0.5 * h, C2, p);

    vector<double> C3(3);
    for (int i = 0; i < 3; i++)
        C3[i] = C[i] + 0.5 * h * k2[i];
    vector<double> k3 = derivatives(t + 0.5 * h, C3, p);

    vector<double> C4(3);
    for (int i = 0; i < 3; i++)
        C4[i] = C[i] + h * k3[i];
    vector<double> k4 = derivatives(t + h, C4, p);

    vector<double> Cnext(3);
    for (int i = 0; i < 3; i++) {
        Cnext[i] =
            C[i] + (h / 6.0) *
            (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
    }

    return Cnext;
}

int main() {
    cout << fixed << setprecision(6);

    char repeat;
    do {
        PKParameters p;
        double tMax, dt;
        vector<double> C(3);

        cout << "\n=== NON-LINEAR MULTI-COMPARTMENT PK MODEL ===\n";
        cout << "Enter Initial Central Concentration C1: ";
        cin >> C[0];
        cout << "Enter Initial Peripheral Concentration C2: ";
        cin >> C[1];
        cout << "Enter Initial Deep Tissue Concentration C3: ";
        cin >> C[2];

        cout << "\nEnter Vmax: ";
        cin >> p.Vmax;
        cout << "Enter Km: ";
        cin >> p.Km;
        cout << "Enter Elimination Rate (kel): ";
        cin >> p.kel;
        cout << "Enter k12: ";
        cin >> p.k12;
        cout << "Enter k21: ";
        cin >> p.k21;
        cout << "Enter k23: ";
        cin >> p.k23;
        cout << "Enter k32: ";
        cin >> p.k32;

        cout << "\nSimulation time (hours): ";
        cin >> tMax;
        cout << "Time step dt: ";
        cin >> dt;

        cout << "\nTime\tC1\t\tC2\t\tC3\n";

        double t = 0.0;
        while (t <= tMax) {
            cout << t << "\t"
                 << C[0] << "\t"
                 << C[1] << "\t"
                 << C[2] << "\n";

            C = rk4Step(t, dt, C, p);
            t += dt;
        }

        cout << "\nRun another simulation? (y/n): ";
        cin >> repeat;

    } while (repeat == 'y' || repeat == 'Y');

    cout << "\nSimulation finished. Scientific modeling complete.\n";
    return 0;
}

double getValidatedDouble(const string& prompt) {
    double value;

    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail()) {
            return value;  // Input valid
        }

        // Handle invalid input
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input

        cout << "Invalid input. Please enter a valid numeric value.\n";
    }
}

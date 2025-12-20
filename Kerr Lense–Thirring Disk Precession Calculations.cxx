#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/*
===========================================================
KERR BLACK HOLE – LENSE-THIRRING PRECESSION SIMULATOR
Author  : Aditiya WP
Purpose : Frame-dragging analysis of accretion disk elements
Level   : General Relativity | Computational Physics | CLI
===========================================================
Geometrized Units: G = c = M = 1
*/

double omega_phi(double r, double a)
{
    return 1.0 / (pow(r, 1.5) + a);
}

double omega_theta(double r, double a)
{
    double term = 1.0 - (4.0 * a) / pow(r, 1.5)
                        + (3.0 * a * a) / (r * r);
    if (term < 0.0) return 0.0;
    return omega_phi(r, a) * sqrt(term);
}

double lense_thirring(double r, double a)
{
    return omega_phi(r, a) - omega_theta(r, a);
}

int main()
{
    cout << "==============================================\n";
    cout << " Kerr Black Hole – Lense-Thirring Precession\n";
    cout << " Frame-Dragging in Accretion Disk Elements\n";
    cout << "==============================================\n\n";

    double spin;
    cout << "Enter Kerr spin parameter a (0 < a < 1): ";
    cin >> spin;

    if (spin <= 0.0 || spin >= 1.0)
    {
        cout << "Invalid spin parameter.\n";
        return 1;
    }

    double r_min = 3.0;     // Near ISCO
    double r_max = 50.0;    // Outer disk
    double dr    = 0.5;     // Radial step

    cout << "\nComputing Lense-Thirring Precession...\n\n";

    cout << left
         << setw(10) << "Radius"
         << setw(18) << "Omega_phi"
         << setw(18) << "Omega_theta"
         << setw(18) << "Omega_LT"
         << "\n";

    cout << "---------------------------------------------------------------\n";

    // ======= MAIN NUMERICAL LOOP =======
    for (double r = r_min; r <= r_max; r += dr)
    {
        double op = omega_phi(r, spin);
        double ot = omega_theta(r, spin);
        double olt = lense_thirring(r, spin);

        cout << fixed << setprecision(6)
             << setw(10) << r
             << setw(18) << op
             << setw(18) << ot
             << setw(18) << olt
             << "\n";
    }

    cout << "\n==============================================\n";
    cout << "Simulation Completed Successfully.\n";
    cout << "Physical Meaning:\n";
    cout << "- Positive Omega_LT indicates frame-dragging\n";
    cout << "- Strongest near inner disk (relativistic regime)\n";
    cout << "==============================================\n";

    return 0;
}

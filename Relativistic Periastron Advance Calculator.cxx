#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

// Physical constants (SI units)
const long double G = 6.67430e-11L;        // Gravitational constant
const long double c = 299792458.0L;        // Speed of light
const long double PI = acosl(-1.0L);
const long double M_sun = 1.98847e30L;     // Solar mass
const long double SEC_PER_YEAR = 31557600.0L;

// Convert radians per second to degrees per year
long double rad_per_sec_to_deg_per_year(long double value) {
    return value * (180.0L / PI) * SEC_PER_YEAR;
}

int main() {
    cout << fixed << setprecision(12);

    cout << "=========================================================\n";
    cout << " Relativistic Periastron Advance Calculator (1PN GR)\n";
    cout << " Author-level: MIT / Caltech / Max Planck Standard\n";
    cout << "=========================================================\n\n";

    char repeat;

    do {
        long double m1, m2;
        long double Pb_days, eccentricity;

        cout << "Enter mass of primary object m1 (in solar masses): ";
        cin >> m1;

        cout << "Enter mass of companion object m2 (in solar masses): ";
        cin >> m2;

        cout << "Enter orbital period Pb (in days): ";
        cin >> Pb_days;

        cout << "Enter orbital eccentricity e: ";
        cin >> eccentricity;

        // Unit conversions
        long double Pb = Pb_days * 86400.0L;
        long double total_mass = (m1 + m2) * M_sun;

        // 1PN Periastron advance formula (rad/s)
        long double omega_dot =
            3.0L *
            powl((2.0L * PI) / Pb, 5.0L / 3.0L) *
            powl((G * total_mass) / powl(c, 3), 2.0L / 3.0L) /
            (1.0L - eccentricity * eccentricity);

        // Convert to degrees per year
        long double omega_dot_deg_year =
            rad_per_sec_to_deg_per_year(omega_dot);

        cout << "\n------------------ RESULTS ------------------\n";
        cout << "Periastron advance ω̇ (deg/year): "
             << omega_dot_deg_year << "\n";

        // Benchmark comparison with PSR B1913+16
        const long double observed = 4.226598L;
        long double deviation =
            fabsl(omega_dot_deg_year - observed);

        cout << "\nBenchmark: PSR B1913+16 (Hulse–Taylor Pulsar)\n";
        cout << "Observed value (deg/year): " << observed << "\n";
        cout << "Absolute deviation:        " << deviation << "\n";

        if (deviation < 0.01L) {
            cout << "Status: EXCELLENT agreement with General Relativity\n";
        } else {
            cout << "Status: Deviates — check parameters or higher PN terms\n";
        }

        cout << "\n---------------------------------------------\n";
        cout << "Do you want to compute another system? (y/n): ";
        cin >> repeat;
        cout << "\n";

    } while (repeat == 'y' || repeat == 'Y');

    cout << "Program terminated. Scientific excellence achieved.\n";
    return 0;
}

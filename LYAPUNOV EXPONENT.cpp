#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

double computeLyapunov(double r, double x0, int burnIn, int iterations) {
    double x = x0;
    double lambda = 0.0;

    for (int i = 0; i < burnIn; i++) {
        x = r * x * (1.0 - x);
    }

    for (int i = 0; i < iterations; i++) {
        x = r * x * (1.0 - x);
        double derivative = fabs(r * (1.0 - 2.0 * x));
        if (derivative == 0) derivative = 1e-10; 
        lambda += log(derivative);
    }

    return lambda / iterations;
}

int main() {
    char repeat;
    do {
        double r_start, r_end, x0;
        int steps, burnIn, iterations;

        cout << "\n=========== Chaos Logistic Map & Lyapunov Exponent ===========\n";
        cout << "  Analyze sensitivity to initial conditions (Chaos Theory)\n";
        cout << "==============================================================\n";

        cout << "Enter starting value of r: ";
        cin >> r_start;
        cout << "Enter ending value of r: ";
        cin >> r_end;
        cout << "Enter number of steps between r_start and r_end: ";
        cin >> steps;
        cout << "Enter initial value of x (0 < x < 1): ";
        cin >> x0;
        cout << "Enter burn-in iterations: ";
        cin >> burnIn;
        cout << "Enter main iterations: ";
        cin >> iterations;

        cout << "\nCalculating Lyapunov Exponents...\n\n";
        cout << fixed << setprecision(6);
        cout << "   r value     |   Lyapunov Exponent   |     Behavior\n";
        cout << "----------------------------------------------------------\n";

        for (int i = 0; i <= steps; i++) {
            double r = r_start + i * ((r_end - r_start) / steps);
            double lambda = computeLyapunov(r, x0, burnIn, iterations);
            string behavior = (lambda > 0) ? "CHAOTIC" : "STABLE";
            cout << "   " << setw(9) << r << "   |   " << setw(19) << lambda << "   |   " << behavior << endl;
        }

        cout << "\nInterpretation:\n";
        cout << "- Lambda > 0 : Indicates sensitivity to initial conditions (CHAOS)\n";
        cout << "- Lambda < 0 : Indicates convergence or stable periodic behavior\n";

        cout << "\nDo you want to try again? (Y/N): ";
        cin >> repeat;
        repeat = toupper(repeat);

    } while (repeat == 'Y');

    cout << "\nProgram ended. Thank you!\n";
    return 0;
}

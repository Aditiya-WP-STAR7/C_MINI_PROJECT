#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// ANSI escape codes for colors and styles
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";

double function2D(double x, double y) {
    return sin(x) * cos(y); 
}

double function3D(double x, double y, double z) {
    return x * y * z; 
}

double monteCarlo2D(double a, double b, double c, double d, int N) {
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        double x = a + (b - a) * (rand() / (double)RAND_MAX);
        double y = c + (d - c) * (rand() / (double)RAND_MAX);
        sum += function2D(x, y);
    }
    return ((b - a) * (d - c) * sum) / N;
}

double monteCarlo3D(double a, double b, double c, double d, double e, double f, int N) {
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        double x = a + (b - a) * (rand() / (double)RAND_MAX);
        double y = c + (d - c) * (rand() / (double)RAND_MAX);
        double z = e + (f - e) * (rand() / (double)RAND_MAX);
        sum += function3D(x, y, z);
    }
    return ((b - a) * (d - c) * (f - e) * sum) / N;
}

void printHeader() {
    cout << BOLD << CYAN;
    cout << "========================================\n";
    cout << "   Monte Carlo Integration Calculator   \n";
    cout << "========================================\n" << RESET;
}

void printMenu() {
    cout << BOLD << YELLOW;
    cout << "\nSelect the type of integral to compute:\n";
    cout << "----------------------------------------\n";
    cout << "1. Bivariate Integral (Two Variables)\n";
    cout << "2. Multivariate Integral (Three Variables)\n";
    cout << "3. Exit\n";
    cout << "----------------------------------------\n" << RESET;
    cout << "Choice: ";
}

int main() {
    srand(time(0));

    int choice;
    int N;
    double a, b, c, d, e, f;

    printHeader();

    do {
        printMenu();
        cin >> choice;

        if (choice == 1) {
            cout << BOLD << BLUE << "\nBivariate Integral Setup\n" << RESET;
            cout << "Enter x-limits (a b): ";
            cin >> a >> b;
            cout << "Enter y-limits (c d): ";
            cin >> c >> d;
            cout << "Enter the number of Monte Carlo points (N): ";
            cin >> N;
            double result = monteCarlo2D(a, b, c, d, N);
            cout << GREEN << "\nBivariate Integral Result: " 
                 << fixed << setprecision(5) << result << RESET << "\n" << endl;
        } 
        else if (choice == 2) {
            cout << BOLD << BLUE << "\nMultivariate Integral Setup\n" << RESET;
            cout << "Enter x-limits (a b): ";
            cin >> a >> b;
            cout << "Enter y-limits (c d): ";
            cin >> c >> d;
            cout << "Enter z-limits (e f): ";
            cin >> e >> f;
            cout << "Enter the number of Monte Carlo points (N): ";
            cin >> N;
            double result = monteCarlo3D(a, b, c, d, e, f, N);
            cout << GREEN << "\nMultivariate Integral Result: " 
                 << fixed << setprecision(5) << result << RESET << "\n" << endl;
        }
        else if (choice != 3) {
            cout << RED << "Invalid choice. Please try again." << RESET << endl;
        }
    } while (choice != 3);

    cout << BOLD << MAGENTA 
         << "\nThank you for using the Monte Carlo Integration Calculator!\n" 
         << RESET;
    return 0;
}
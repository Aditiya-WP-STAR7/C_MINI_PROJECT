#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

void basic_operations(int option) {
    double a, b;
    cout << "Enter first number: "; cin >> a;
    cout << "Enter second number: "; cin >> b;

    switch(option) {
        case 1: cout << "Result: " << a + b << endl; break;
        case 2: cout << "Result: " << a - b << endl; break;
        case 3: cout << "Result: " << a * b << endl; break;
        case 4:
            if (b == 0) cout << "Error: Division by zero!" << endl;
            else cout << "Result: " << a / b << endl;
            break;
    }
}

void trig_operations(int option) {
    double x;
    cout << "Enter x (radians): ";
    cin >> x;
    switch(option) {
        case 5: cout << "Result: " << sin(x) << endl; break;
        case 6: cout << "Result: " << cos(x) << endl; break;
        case 7: cout << "Result: " << tan(x) << endl; break;
    }
}

void log_exp_operations(int option) {
    double x;
    cout << "Enter x: ";
    cin >> x;
    if ((option == 8 || option == 9) && x <= 0) {
        cout << "Error: Logarithm undefined for non-positive numbers!" << endl;
        return;
    }
    switch(option) {
        case 8: cout << "Result: " << log(x) << endl; break;       // ln
        case 9: cout << "Result: " << log10(x) << endl; break;     // log base 10
        case 10: cout << "Result: " << exp(x) << endl; break;      // e^x
    }
}

void power_root_operations(int option) {
    double x, y;
    if (option == 11) {
        cout << "Enter base: "; cin >> x;
        cout << "Enter exponent: "; cin >> y;
        cout << "Result: " << pow(x, y) << endl;
    } else if (option == 12) {
        cout << "Enter x: "; cin >> x;
        if (x < 0) cout << "Error: Square root of negative number!" << endl;
        else cout << "Result: " << sqrt(x) << endl;
    }
}

void pearson_correlation() {
    int n;
    cout << "Enter number of data points: ";
    cin >> n;

    vector<double> x(n), y(n);
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;

    for (int i = 0; i < n; i++) {
        cout << "x[" << i + 1 << "]: "; cin >> x[i];
    }
    for (int i = 0; i < n; i++) {
        cout << "y[" << i + 1 << "]: "; cin >> y[i];
    }

    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
        sumY2 += y[i] * y[i];
    }

    double r = (n * sumXY - sumX * sumY) /
               (sqrt(n * sumX2 - sumX * sumX) * sqrt(n * sumY2 - sumY * sumY));
    cout << "Pearson Correlation: " << r << endl;
}

void covariance_calc() {
    int n;
    cout << "Enter number of data points: ";
    cin >> n;

    vector<double> x(n), y(n);
    double meanX = 0, meanY = 0;

    for (int i = 0; i < n; i++) {
        cout << "x[" << i + 1 << "]: "; cin >> x[i];
    }
    for (int i = 0; i < n; i++) {
        cout << "y[" << i + 1 << "]: "; cin >> y[i];
    }

    for (int i = 0; i < n; i++) {
        meanX += x[i];
        meanY += y[i];
    }
    meanX /= n; meanY /= n;

    double cov = 0;
    for (int i = 0; i < n; i++) {
        cov += (x[i] - meanX) * (y[i] - meanY);
    }

    cout << "Covariance: " << cov / n << endl;
}

void linear_regression() {
    int n;
    cout << "Enter number of data points: ";
    cin >> n;

    vector<double> x(n), y(n);
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (int i = 0; i < n; i++) {
        cout << "x[" << i + 1 << "]: "; cin >> x[i];
    }
    for (int i = 0; i < n; i++) {
        cout << "y[" << i + 1 << "]: "; cin >> y[i];
    }

    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
    }

    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    cout << "Linear Regression Equation: y = " << slope << "x + " << intercept << endl;
}

void multivariable_stats() {
    int n;
    cout << "Enter number of variables: ";
    cin >> n;

    vector<double> data(n);
    double mean = 0;
    for (int i = 0; i < n; i++) {
        cout << "x[" << i + 1 << "]: "; cin >> data[i];
        mean += data[i];
    }
    mean /= n;

    double variance = 0;
    for (int i = 0; i < n; i++) {
        variance += pow(data[i] - mean, 2);
    }
    variance /= n;
    double stddev = sqrt(variance);

    cout << "Mean = " << mean << ", Variance = " << variance << ", Std Dev = " << stddev << endl;
}

void show_menu() {
    cout << "\n============ ADVANCED SCIENTIFIC CALCULATOR ============\n"
         << " 1. Addition\n"
         << " 2. Subtraction\n"
         << " 3. Multiplication\n"
         << " 4. Division\n"
         << " 5. Sine(x)\n"
         << " 6. Cosine(x)\n"
         << " 7. Tangent(x)\n"
         << " 8. Natural Log (ln)\n"
         << " 9. Log Base 10\n"
         << "10. Exponential (e^x)\n"
         << "11. Power (x^y)\n"
         << "12. Square Root\n"
         << "13. Pearson Correlation\n"
         << "14. Covariance\n"
         << "15. Linear Regression\n"
         << "16. Mean/Variance/Std Dev\n"
         << " 0. Exit\n"
         << "=========================================================\n";
}

int main() {
    int choice;
    do {
        show_menu();
        cout << "Choose an option: ";
        cin >> choice;
        cout << endl;

        switch(choice) {
            case 1: case 2: case 3: case 4: basic_operations(choice); break;
            case 5: case 6: case 7: trig_operations(choice); break;
            case 8: case 9: case 10: log_exp_operations(choice); break;
            case 11: case 12: power_root_operations(choice); break;
            case 13: pearson_correlation(); break;
            case 14: covariance_calc(); break;
            case 15: linear_regression(); break;
            case 16: multivariable_stats(); break;
            case 0: cout << "👋 Thank you for using the Advanced Scientific Calculator!" << endl; break;
            default: cout << "Invalid option. Please try again!" << endl;
        }

        cout << "\n=========================================================\n";

    } while(choice != 0);

    return 0;
}

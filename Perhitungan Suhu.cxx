#include <iostream>

using namespace std;

double celsiusToReamur(double celsius) {
    return celsius * 0.8;
}

double celsiusToFahrenheit(double celsius) {
    return (celsius * 9/5) + 32;
}

double reamurToCelsius(double reamur) {
    return reamur / 0.8;
}

double reamurToFahrenheit(double reamur) {
    return (reamur * 9/4) + 32;
}

double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32) * 5/9;
}

double fahrenheitToReamur(double fahrenheit) {
    return (fahrenheit - 32) * 4/9;
}

int main() {
    int choice;
    double inputTemp, result;
    char repeat;

    do {
        cout << "Pilih konversi suhu :"<< endl;
        cout << "1. Celcius -> Reamur" << endl;
        cout << "2. Celcius -> Fahrenheit" << endl;
        cout << "3. Reamur -> Celcius" << endl;
        cout << "4. Reamur -> Fahrenheit" << endl;
        cout << "5. Fahrenheit -> Celcius" << endl;
        cout << "6. Fahrenheit -> Reamur" << endl;
        cout << "Masukkan pilihan Anda (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Masukkan suhu dalam Celcius: ";
                cin >> inputTemp;
                result = celsiusToReamur(inputTemp);
                cout << "Langkah-langkah perhitungan:" << endl;
                cout << "Reamur = Celcius * 0.8" << endl;
                cout << "Reamur = " << inputTemp << " * 0.8" << endl;
                cout << "Hasil: " << result << " Reamur" << endl;
                break;
            case 2:
                cout << "Masukkan suhu dalam Celcius: ";
                cin >> inputTemp;
                result = celsiusToFahrenheit(inputTemp);
                cout << "Langkah-langkah perhitungan:" << endl;
                cout << "Fahrenheit = (Celcius * 9/5) + 32" << endl;
                cout << "Fahrenheit = (" << inputTemp << " * 9/5) + 32" << endl;
                cout << "Hasil: " << result << " Fahrenheit" << endl;
                break;
            case 3:
                cout << "Masukkan suhu dalam Reamur: ";
                cin >> inputTemp;
                result = reamurToCelsius(inputTemp);
                cout << "Langkah-langkah perhitungan:" << endl;
                cout << "Celcius = Reamur / 0.8" << endl;
                cout << "Celcius = " << inputTemp << " / 0.8" << endl;
                cout << "Hasil: " << result << " Celcius" << endl;
                break;
            case 4:
                cout << "Masukkan suhu dalam Reamur: ";
                cin >> inputTemp;
                result = reamurToFahrenheit(inputTemp);
                cout << "Langkah-langkah perhitungan:" << endl;
                cout << "Fahrenheit = (Reamur * 9/4) + 32" << endl;
                cout << "Fahrenheit = (" << inputTemp << " * 9/4) + 32" << endl;
                cout << "Hasil: " << result << " Fahrenheit" << endl;
                break;
            case 5:
                cout << "Masukkan suhu dalam Fahrenheit: ";
                cin >> inputTemp;
                result = fahrenheitToCelsius(inputTemp);
                cout << "Langkah-langkah perhitungan:" << endl;
                cout << "Celcius = (Fahrenheit - 32) * 5/9" << endl;
                cout << "Celcius = (" << inputTemp << " - 32) * 5/9" << endl;
                cout << "Hasil: " << result << " Celcius" << endl;
                break;
            case 6:
                cout << "Masukkan suhu dalam Fahrenheit: ";
                cin >> inputTemp;
                result = fahrenheitToReamur(inputTemp);
                cout << "Langkah-langkah perhitungan:" << endl;
                cout << "Reamur = (Fahrenheit - 32) * 4/9" << endl;
                cout << "Reamur = (" << inputTemp << " - 32) * 4/9" << endl;
                cout << "Hasil: " << result << " Reamur" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
                break;
        }

        cout << "Apakah Anda ingin melakukan konversi lagi? (y/n): ";
        cin >> repeat;

    } while(repeat == 'y' || repeat == 'Y');

    cout << "Program selesai." << endl;

    return 0;
}

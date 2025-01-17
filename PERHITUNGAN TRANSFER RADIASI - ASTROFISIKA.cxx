#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

double transferRadiasi(double I0, const vector<double>& tau, const vector<double>& S, int n) {
    double I = I0; 
    double deltaTau;

    for (int i = 1; i < n; i++) {
        deltaTau = tau[i] - tau[i - 1];
        I = I * exp(-deltaTau) + S[i] * (1 - exp(-deltaTau));
    }
    return I;
}

int main() {
    cout << "========================================================\n";
    cout << "             Program Perhitungan Transfer Radiasi       \n";
    cout << "========================================================\n";
    cout << "Transfer Radiasi adalah proses yang menggambarkan       \n";
    cout << "bagaimana cahaya atau radiasi elektromagnetik bergerak  \n";
    cout << "melalui medium dengan ketebalan optik tertentu.         \n";
    cout << "Program ini akan menghitung intensitas radiasi akhir    \n";
    cout << "berdasarkan intensitas awal, ketebalan optik, dan fungsi\n";
    cout << "sumber radiasi pada tiap segmen lintasan.\n";
    cout << "========================================================\n\n";

    char pilihan;
    do {
        int n;
        cout << "Masukkan jumlah segmen (n): ";
        cin >> n;

        vector<double> tau(n), S(n);

        cout << "Masukkan nilai ketebalan optik (tau) untuk setiap segmen:\n";
        for (int i = 0; i < n; i++) {
            cout << "tau[" << i + 1 << "]: ";
            cin >> tau[i];
        }

        cout << "Masukkan nilai fungsi sumber radiasi (S) untuk setiap segmen:\n";
        for (int i = 0; i < n; i++) {
            cout << "S[" << i + 1 << "]: ";
            cin >> S[i];
        }

        double I0;
        cout << "Masukkan intensitas awal (I0): ";
        cin >> I0;

        double hasil = transferRadiasi(I0, tau, S, n);

        cout << "\nHasil perhitungan intensitas radiasi akhir: " << hasil << endl;

        cout << "\nApakah Anda ingin menghitung lagi? (y/n): ";
        cin >> pilihan;
        cout << endl;

    } while (pilihan == 'y' || pilihan == 'Y');

    cout << "Program selesai. Terima kasih!\n";
    return 0;
}

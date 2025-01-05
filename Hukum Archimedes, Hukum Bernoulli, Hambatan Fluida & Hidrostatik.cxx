#include <iostream>
#include <cmath>

using namespace std;

void gayaApung() {
    double rho, g, V;
    cout << "\n--- Perhitungan Gaya Apung (Hukum Archimedes) ---\n";
    cout << "Masukkan massa jenis fluida (kg/m^3): ";
    cin >> rho;
    cout << "Masukkan percepatan gravitasi (m/s^2): ";
    cin >> g;
    cout << "Masukkan volume benda yang tercelup (m^3): ";
    cin >> V;
    
    double FA = rho * g * V;
    
    cout << "Rumus: F_A = rho * g * V\n";
    cout << "Hasil perhitungan:\n";
    cout << "Gaya Apung = " << FA << " N\n";
    cout << "Gaya apung adalah gaya yang bekerja ke atas pada benda yang tercelup dalam fluida. "
         << "Dalam kehidupan sehari-hari, prinsip Archimedes ini digunakan pada perancangan kapal, balon udara, dan kapal selam.\n";
}

void hambatanFluida() {
    double Cd, rho, A, v;
    cout << "--- Perhitungan Hambatan Fluida ---";
    cout << "Masukkan koefisien drag: ";
    cin >> Cd;
    cout << "Masukkan massa jenis fluida (kg/m^3): ";
    cin >> rho;
    cout << "Masukkan luas penampang (m^2): ";
    cin >> A;
    cout << "Masukkan kecepatan fluida relatif (m/s): ";
    cin >> v;
    
    double FD = 0.5 * Cd * rho * A * pow(v, 2);
    
    cout << "Rumus: F_D = 0.5 * Cd * rho * A * v^2";
    cout << "Hasil perhitungan:\n";
    cout << "Gaya Hambat = " << FD << " N\n";
    cout << "Hambatan fluida adalah gaya yang berlawanan dengan arah gerak benda melalui fluida. "
         << "Dalam kehidupan sehari-hari, ini penting untuk desain kendaraan seperti kapal, pesawat, dan mobil agar lebih aerodinamis dan efisien.\n";
}

void tekananHidrostatik() {
    double rho, g, h;
    cout << "--- Perhitungan Tekanan Hidrostatik ---";
    cout << "Masukkan massa jenis fluida (kg/m^3): ";
    cin >> rho;
    cout << "Masukkan percepatan gravitasi (m/s^2): ";
    cin >> g;
    cout << "Masukkan kedalaman (m): ";
    cin >> h;
    
    double P = rho * g * h;
    
    cout << "Rumus: P = rho * g * h";
    cout << "Hasil perhitungan:\n";
    cout << "Tekanan Hidrostatik = " << P << " Pa\n";
    cout << "Tekanan hidrostatik adalah tekanan yang dialami oleh benda yang berada di bawah permukaan fluida. "
         << "Dalam kehidupan sehari-hari, konsep ini digunakan pada desain bendungan, kapal selam, dan tangki air.\n";
}

void hukumBernoulli() {
    double P1, rho, v1, v2, h1, h2, g;
    cout << "\n--- Perhitungan Hukum Bernoulli ---\n";
    cout << "Masukkan tekanan awal (Pa): ";
    cin >> P1;
    cout << "Masukkan massa jenis fluida (kg/m^3): ";
    cin >> rho;
    cout << "Masukkan kecepatan fluida awal (m/s): ";
    cin >> v1;
    cout << "Masukkan kecepatan fluida akhir (m/s): ";
    cin >> v2;
    cout << "Masukkan ketinggian awal (m): ";
    cin >> h1;
    cout << "Masukkan ketinggian akhir (m): ";
    cin >> h2;
    cout << "Masukkan percepatan gravitasi (m/s^2): ";
    cin >> g;
    
    double P2 = P1 + 0.5 * rho * (pow(v1, 2) - pow(v2, 2)) + rho * g * (h1 - h2);
    
    cout << "\nRumus: P2 = P1 + 0.5 * rho * (v1^2 - v2^2) + rho * g * (h1 - h2)\n";
    cout << "Hasil perhitungan:\n";
    cout << "Tekanan akhir = " << P2 << " Pa\n";
    cout << "Hukum Bernoulli menjelaskan hubungan antara tekanan, kecepatan, dan ketinggian fluida dalam aliran. "
         << "digunakan dalam desain pesawat, pipa, dan peralatan medis seperti ventilator.\n";
}

int main() {
    int pilihan;
    char ulang;
    
    do {
        cout << "\nPilih jenis perhitungan:\n";
        cout << "1. Perhitungan Gaya Apung (Hukum Archimedes)\n";
        cout << "2. Perhitungan Hambatan Fluida\n";
        cout << "3. Perhitungan Tekanan Hidrostatik\n";
        cout << "4. Hukum Bernoulli\n";
        cout << "Masukkan pilihan Anda (1-4): ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: gayaApung(); break;
            case 2: hambatanFluida(); break;
            case 3: tekananHidrostatik(); break;
            case 4: hukumBernoulli(); break;
            default: cout << "Pilihan tidak valid. Coba lagi.\n";
        }
        
        cout << "Apakah Anda ingin mengulangi program? (y/n): ";
        cin >> ulang;
        
    } while (ulang == 'y' || ulang == 'Y');
    
    cout << "Terima kasih telah menggunakan program ini.";
    return 0;
}

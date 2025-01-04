#include <iostream>
#include <complex>
#include <vector>

using namespace std;

// Fungsi untuk menampilkan matriks
void displayMatrix(vector<vector<complex<double>>> &matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "(" << real(matrix[i][j]) << "," << imag(matrix[i][j]) << "i)\t";
        }
        cout << endl;
    }
}

// Fungsi untuk menghitung konjugat transpose matriks
vector<vector<complex<double>>> conjugateTranspose(vector<vector<complex<double>>> &matrix) {
    int n = matrix.size();
    vector<vector<complex<double>>> result(n, vector<complex<double>>(n));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[j][i] = conj(matrix[i][j]); // Konjugasi elemen dan transpose
        }
    }
    return result;
}

// Fungsi untuk memeriksa apakah matriks adalah Hermitian
bool isHermitian(vector<vector<complex<double>>> &matrix) {
    int n = matrix.size();
    vector<vector<complex<double>>> conjTrans = conjugateTranspose(matrix);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != conjTrans[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Fungsi untuk memeriksa apakah matriks adalah Uniter
bool isUniter(vector<vector<complex<double>>> &matrix) {
    int n = matrix.size();
    vector<vector<complex<double>>> conjTrans = conjugateTranspose(matrix);
    vector<vector<complex<double>>> identity(n, vector<complex<double>>(n));
    
    for (int i = 0; i < n; i++) {
        identity[i][i] = 1;
    }
    
    vector<vector<complex<double>>> product(n, vector<complex<double>>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            product[i][j] = 0;
            for (int k = 0; k < n; k++) {
                product[i][j] += conjTrans[i][k] * matrix[k][j];
            }
        }
    }
    return product == identity;
}

int main() {
    int n;
    char choice;
    do {
        cout << "Masukkan ukuran matriks (n x n): ";
        cin >> n;
        
        vector<vector<complex<double>>> matrix(n, vector<complex<double>>(n));
        
        cout << "Masukkan elemen-elemen matriks (bilangan kompleks a+bi):\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                double realPart, imagPart;
                cout << "Elemen [" << i + 1 << "][" << j + 1 << "] (real, imag): ";
                cin >> realPart >> imagPart;
                matrix[i][j] = complex<double>(realPart, imagPart);
            }
        }
        
        cout << "\nMatriks yang Anda masukkan:\n";
        displayMatrix(matrix);
        
        cout << "\nKonjugat Transpose dari matriks:\n";
        vector<vector<complex<double>>> conjTrans = conjugateTranspose(matrix);
        displayMatrix(conjTrans);
        
        cout << "\nPilih jenis matriks yang ingin diperiksa:\n";
        cout << "1. Hermitian\n";
        cout << "2. Uniter\n";
        cout << "Pilihan: ";
        cin >> choice;
        
        if (choice == '1') {
            if (isHermitian(matrix)) {
                cout << "Matriks ini adalah Hermitian.\n";
            } else {
                cout << "Matriks ini bukan Hermitian.\n";
            }
        } else if (choice == '2') {
            if (isUniter(matrix)) {
                cout << "Matriks ini adalah Uniter.\n";
            } else {
                cout << "Matriks ini bukan Uniter.\n";
            }
        } else {
            cout << "Pilihan tidak valid.\n";
        }
        
        cout << "\nApakah Anda ingin mengulangi program? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');
    
    cout << "Program selesai.\n";
    return 0;
}

#include <iostream>
#include <cmath>
using namespace std;

int main() {
    double a, b, c;
    cout << "Ingrese a, b y c: ";
    cin >> a >> b >> c;

    double d = b*b - 4*a*c;

    if (d > 0) {
        double x1 = (-b + sqrt(d)) / (2*a);
        double x2 = (-b - sqrt(d)) / (2*a);
        cout << "x1 = " << x1 << endl;
        cout << "x2 = " << x2 << endl;
    } 
    else if (d == 0) {
        double x = -b / (2*a);
        cout << "x = " << x << endl;
    } 
    else {
        double real = -b / (2*a);
        double imag = sqrt(-d) / (2*a);
        cout << "x1 = " << real << " + " << imag << "i" << endl;
        cout << "x2 = " << real << " - " << imag << "i" << endl;
    }

    return 0;
}


#include <ctime>
#include <iostream>
using namespace std;

int main() {
    int a = 0, b = 1;
    int n = clock();
    for (int i = 1; i <= 2e8; i++) {
        b = a + b;
        a = a + b;
    }
    int end = clock();
    cout << a << "\n";
    cout << (double)(end - n) / CLOCKS_PER_SEC << "\n";
}
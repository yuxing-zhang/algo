/* Input requirement: 1 <= x <= 10 ^ 16 */

#include <iostream>

long f[285113][20];
long last[285113];

long db(long x) {
    if (x == 1) return 0;
    int n = 285112, i = 0;
    long s = 0;
    for (int j = 0, k; n - j > 1;) {
        if (x > last[k = (j + n) / 2] ) j = k;
        else n = k;
    }
    for (int j = n; j; j >>= 1, ++i) {}
    x -= last[n - 1];
    for (int l = 1 << i - 1, k; i; --i, l >>= 1) {
        k = 0;
        while (x > f[n][i - 1]) ++k, x -= f[n][i - 1], n -= l;
        s = s * 10 + k;
    }
    return s;
}

int main() {
    for (int i = 0; i < 20; ++i) f[0][i] = 1;
    last[0] = 1;
    for (int k = 1; k < 285113; ++k) {
        long t = 1;
        for (int i = 1; i < 20; ++i, t <<= 1) {
            int n = k;
            if (n > 9 * ((t << 1) - 1)) continue;
            f[n][i] = f[n][i - 1]; 
            if ((n -= t) < 0) continue;
            long s = 0;
            for (int j = 1; j < 10 && n >= 0; ++j, n -= t) s += f[n][i - 1]; 
            f[k][i] += s;
        }
        last[k] = last[k - 1] + f[k][19];
    }
    for (std::string s; std::cout << "input: ", std::cin >> s;)
        std::cout << db(stol(s)) << '\n';
}

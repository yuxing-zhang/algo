#include <iostream>
#include <vector>


template<class RandIter>
struct ST {
    typedef typename RandIter::value_type V;
    RandIter bg;
    int n;
    std::vector<V> v;
    V (*f)(const V&, const V&);

    ST(RandIter begin, RandIter end, V (*f)(const V&, const V&))
            : bg(begin), n(end - begin), v(4 * n), f(f) { _build(0, 0, n); }
    void _build(int i, int a, int b) {
        if (b - a == 1) { v[i] = bg[a]; return; }
        int m = (a + b) / 2, l = 2 * i + 1, r = 2 * i + 2;
        _build(l, a, m), _build(r, m, b);
        v[i] = f(v[l], v[r]);
    }
    V _query(int i, int a, int b, int qa, int qb) {
        if (qa <= a && b <= qb) return v[i];
        int m = (a + b) / 2, l = 2 * i + 1, r = 2 * i + 2;
        if (m <= qa) return _query(r, m, b, qa, qb);
        if (qb <= m) return _query(l, a, m, qa, qb);
        return f(_query(l, a, m, qa, qb), _query(r, m, b, qa, qb));
    }
    V operator()(int qa, int qb) {
        return _query(0, 0, n, qa, qb);
    }
};

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    ST st(v.begin(), v.end(), [](const int& x, const int& y) { return x + y; });
    std::cout << st(1, 3) << ' ' << st(-10, 10) << '\n';
    std::vector<std::string> vs{"aa", "bb", "cc"};
    ST sts(vs.begin(), vs.end(), std::operator+);
    std::cout << sts(0, 3) << '\n';
}

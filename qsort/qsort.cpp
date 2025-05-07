#include <iostream>
#include <vector>
#include <thread>
#include <random>

/* Threading stragegy:
 * Threads are dynamically created for the recursive calls. The template
 * argument `N` controls whether such cals will be executed in the current
 * thread or a new thread. If the length of the subsequence is greater than
 * `N`, a new thread will be created. Note that this`N` is different from
 * the template argument `N` in the `test` function. Their relationship is:
 * N_qs = L / (2 * N_test). Roughly speaking, N_test is the number of threads
 * to be created, on average.
 */

template<int N, class Iter>
void qs(Iter begin, Iter end) {
    if (begin == end) return;
    Iter l{begin + 1}, r{end - 1};
    typename Iter::value_type t;
    while (true) {
        while (l != end && *l <= *begin) l++;
        while (r != begin && *r >= *begin) r--;
        if (r < l) break;
        t = *l, *l = *r, *r = t;
    }
    t = *begin, *begin = *r, *r = t;
    std::thread th;
    if ((r - begin) > N) th = std::thread{qs<N, Iter>, begin, r};
    else qs<N>(begin, r);
    qs<N>(l, end);
    if (th.joinable()) th.join();
}

template<int N, int L, class Cont>
void test(Cont &cont) {
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::system_clock::rep dur = 0;
    std::uniform_int_distribution<> randint{0, 100};
    std::mt19937 rng;
    for (int n = 0; n < 5; ++n) {
        for (int &i : cont) i = randint(rng);
        start = std::chrono::system_clock::now();
        qs<L / (2 * N)>(cont.begin(), cont.end());
        dur += (std::chrono::system_clock::now() - start).count();
    }
    std::cout << N << ' ' << dur / 5. << std::endl;
}

int main() {
    const int L = 10000;
    std::vector<int> v(L);
    test<16, L>(v);
/*
    std::vector<int> v;
    v.reserve(16);
    for (int i = 0; i++ < 16; v.push_back(rand() % 100));
    for (int i : v) std::cout << i << ' '; std::cout << '\n';
    qs<2>(v.begin(), v.end());
    for (int i : v) std::cout << i << ' '; std::cout << '\n';
*/
}

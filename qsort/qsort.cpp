#include <iostream>
#include <vector>
#include <thread>
#include <random>

/*
 * The quicksort is followed by two multithreaded version.
 *
 * Threading stragegies:
 * (The word "array" used here refers to a general sequential data structure
 * that supports random access iterators.)
 * In qs_mt, the array is first divided into 'N' chunks. A thread is created
 * for each chunk.
 *
 * In qs_dt, hreads are dynamically created for the recursive calls. The template
 * argument `N` controls whether such calls will be executed in the current
 * thread or a new thread. A static int 'n' is defined as the length of the
 * array to be sorted divided by '2 * N'. If the length of the subsequence is
 * greater than `n`, a new thread will be created. Roughly speaking, `N`is the
 * number of threads to be created, on average.
 */
template<class Iter>
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
    qs(begin, r);
    qs(l, end);
}

template<int N, class Iter>
void qs_mt(Iter begin, Iter end) {
    Iter l, r;
    qs(begin, begin + N - 1);
    begin += N - 1;
    typename Iter::value_type t;
    std::thread ths[N - 1];
    for (int i = N - 1; i > 0; --i) {
        --begin;
        l = begin + 1, r = end - 1;
        while (true) {
            while (l != end && *l <= *begin) ++l;
            while (r != begin && *r >= *begin) --r;
            if (r < l) break;
            t = *l, *l = *r, *r = t;
        }
        t = *begin, *begin = *r, *r = t;
        ths[i - 1] = std::thread{qs<Iter>, l, end};
        end = r;
    }
    qs(begin, end);
    for (auto &t : ths) t.join();
}

// Dynamic threading
template<int N, class Iter>
void qs_dt(Iter begin, Iter end) {
    if (begin == end) return;
    Iter l{begin + 1}, r{end - 1};
    typename Iter::value_type t;
    static int n = (end - begin) / (2 * N);
    while (true) {
        while (l != end && *l <= *begin) l++;
        while (r != begin && *r >= *begin) r--;
        if (r < l) break;
        t = *l, *l = *r, *r = t;
    }
    t = *begin, *begin = *r, *r = t;
    std::thread th;
    if ((r - begin) > n) th = std::thread{qs_dt<N, Iter>, begin, r};
    else qs(begin, r);
    if ((end - l) > n) qs_dt<N>(l, end);
    else qs(l, end);
    if (th.joinable()) th.join();
}

template<class Iter>
void test(void (*qs)(Iter, Iter), Iter begin, Iter end) {
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::system_clock::rep dur = 0;
    std::uniform_int_distribution<> randint{0, 100};
    std::mt19937 rng;
    for (int n = 0; n < 5; ++n) {
        for (Iter i = begin; i != end; ++i) *i = randint(rng);
        start = std::chrono::system_clock::now();
        qs(begin, end);
        dur += (std::chrono::system_clock::now() - start).count();
    }
    std::cout << dur / 5. << std::endl;
}

int main() {
    std::vector<int> v(10000);
    test(qs_mt<16, std::vector<int>::iterator>, v.begin(), v.end());
    test(qs_dt<16, std::vector<int>::iterator>, v.begin(), v.end());
/*
    std::vector<int> v;
    v.reserve(16);
    for (int i = 0; i++ < 16; v.push_back(rand() % 100));
    for (int i : v) std::cout << i << ' '; std::cout << '\n';
    qs_dt<4>(v.begin(), v.end());
    for (int i : v) std::cout << i << ' '; std::cout << '\n';
*/
}

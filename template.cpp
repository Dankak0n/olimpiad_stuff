//v 1.0.0 beta

#define y0 _y_0_
#define y1 _y_1_

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/rope>

#undef y0
#undef y1

#pragma GCC optimize("O3,unroll-loops")

namespace gll {
    namespace cfg {
        namespace detail {
            const int BAD_INT__ = -5;
            const std::string BAD_STRING__ = "$-! ";
        }

        using ib_func_ib_ptr = std::ios_base &(*)(std::ios_base &);

        struct config_t {
            std::string ifile_name {detail::BAD_STRING__};
            std::string ofile_name {detail::BAD_STRING__};
            int is_sync {detail::BAD_INT__};
            int is_enable {detail::BAD_INT__};
            int precision_cnt {detail::BAD_INT__};
            int verbosity {detail::BAD_INT__};
            int test_cnt {detail::BAD_INT__};
            std::vector<ib_func_ib_ptr> manips {};

#define _LAST__OK_(other, x, bad) x = (other.x != bad ? other.x : x)
#define _MIN___OK_(other, x, bad) x = (x == bad ? other.x : other.x == bad ? x : x < other.x ? x : other.x)
#define _MAX___OK_(other, x, bad) x = (x == bad ? other.x : other.x == bad ? x : x > other.x ? x : other.x)

            config_t& operator+=(const config_t& other) {
                _LAST__OK_(other, ifile_name, detail::BAD_STRING__);
                _LAST__OK_(other, ofile_name, detail::BAD_STRING__);
                _LAST__OK_(other, is_sync, detail::BAD_INT__);
                _LAST__OK_(other, precision_cnt, detail::BAD_INT__);
                _LAST__OK_(other, verbosity, detail::BAD_INT__);
                _LAST__OK_(other, test_cnt, detail::BAD_INT__);
                _MAX___OK_(other, is_enable, detail::BAD_INT__);
                manips.insert(manips.end(), other.manips.begin(), other.manips.end());
                return *this;
            }
            config_t operator+(const config_t& other) const {
                config_t ret = *this;
                return ret += other;
            }
        };
        struct ifile : config_t { explicit ifile(const char* file_name) { ifile_name = file_name; } };
        struct ofile : config_t { explicit ofile(const char* file_name) { ofile_name = file_name; } };
        struct sync  : config_t { explicit sync(bool val) { is_sync = val; } };
        struct enable: config_t { explicit enable(bool val) { is_enable = val; } };
        struct sprec : config_t { explicit sprec(int _sprec) { precision_cnt = _sprec; } };
        struct verb  : config_t { explicit verb(int _verb) { verbosity = _verb; } };
        struct tcnt  : config_t { explicit tcnt(int _tcnt) { test_cnt = _tcnt; } };
        struct out   : config_t { explicit out(const std::initializer_list<ib_func_ib_ptr>& ilist) {
                manips.insert(manips.end(), ilist.begin(), ilist.end());
            }
        };
    }

    namespace env {
        cfg::config_t cfg {};
        void upd_cfg(const cfg::config_t& ncfg) {
            if (ncfg.is_enable == 1) {
                cfg += ncfg;
            }
        }
        static const int MULTITEST = -1;
        static int tid = 0;
    }

#define Logs(verb, ...) if(verb <= gll::env::cfg.verbosity) std::cout << __VA_ARGS__ << std::endl

    void init();
    void run(void(*begin_solve)(), void(*solve)(), void(*after_solve)()) {
        if (!env::cfg.ifile_name.empty() && env::cfg.ifile_name != cfg::detail::BAD_STRING__) {
            freopen(env::cfg.ifile_name.c_str(), "r", stdin);
        }
        if (!env::cfg.ofile_name.empty() && env::cfg.ofile_name != cfg::detail::BAD_STRING__) {
            freopen(env::cfg.ofile_name.c_str(), "w", stdout);
        }
        if (env::cfg.is_sync && env::cfg.is_sync != cfg::detail::BAD_INT__) {
            std::ios::sync_with_stdio(false);
            std::cin.tie(nullptr);
            std::cout.tie(nullptr);
        }
        int cnt_tests = env::cfg.test_cnt;
        if (cnt_tests == env::MULTITEST) {
            std::cin >> cnt_tests;
        } else {
            cnt_tests = cnt_tests == 0 || cnt_tests == cfg::detail::BAD_INT__ ? 1 : cnt_tests;
        }
        for (auto el : env::cfg.manips) {
            std::cout << el;
        }
        if (env::cfg.precision_cnt != cfg::detail::BAD_INT__) {
            std::cout.precision(env::cfg.precision_cnt);
        }
        Logs(1, "Start precalculation...");
        begin_solve();
        Logs(1, "End precalculation...");
        Logs(1, "Start solve, [" + std::to_string(cnt_tests) + "] tests...");
        for (; env::tid < cnt_tests; ++env::tid) {
            Logs(2, "Start test [" + std::to_string(env::tid) + "]");
            solve();
            after_solve();
            Logs(2, "\nEnd test");
        }
        Logs(1, "\nEnd solve...");
        Logs(1, "____________________________________");
        Logs(1, "Time = [" + std::to_string(clock() / (double)CLOCKS_PER_SEC) + "]");
    }
}
namespace env = gll::env;
namespace gcfg = gll::cfg;

namespace bit {
    template<typename T> inline int max1(T x) { return x ? __lg(x) : -1; }
    template<typename T> inline int min1(T x) { return x ? __builtin_ctzll(x) : 0; }
    template<typename T> inline int cnt1(T x) { return __builtin_popcountll(x); }
    template<typename T> inline int cnt0(T x) { return max1(x) + 1 - cnt1(x); }
    template<typename T> inline int get(T x, int bit) { return (x >> bit) & 1; }
    template<typename T> inline void set(T& x, int pos, int bit) { (!!bit != ((x >> pos) & 1)) ? x ^= ((T)1 << pos) : 42; }
}

namespace rnd {
    static int64_t seed = 2736489283101;
    static std::mt19937_64 rnd(seed);
    inline void set(int64_t _seed) { seed = _seed; }
    template<typename T> inline T get(T n) { return rnd() % n; }
    template<typename T> inline T get(T l, T r) { return l + get(r - l + 1); }
    template<typename T> inline void shuffle(T& a) { std::shuffle(a.begin(), a.end(), rnd); }
    template<typename T> inline void shuffle(T& a, int l, int r) { std::shuffle(a.begin() + l, a.begin() + r + 1, rnd); }
}

namespace nmt {
    template<int MX>
    struct siv {
        static int p[MX + 1];
        static bool prime(int x) {
            return p[x] == x;
        }
        static std::vector<int> divs(int x) {
            std::list<int> res {};
            while (p[x] != -1) {
                res.emplace_front(x);
                x /= p[x];
            }
            res.emplace_front(1);
            return std::vector<int> {res.begin(), res.end()};
        }
        static __gnu_pbds::gp_hash_table<int, int> fact_to_map(int x) {
            __gnu_pbds::gp_hash_table<int, int> res {};
            while (p[x] != -1) {
                int d = p[x];
                do {
                    ++res[d];
                    x /= d;
                } while (x % d == 0);
            }
            return res;
        }
        static std::vector< std::pair<int, int> > fact_to_vpr(int x) {
            std::vector< std::pair<int, int> > res {};
            while (p[x] != -1) {
                int d = p[x];
                res.emplace_back(d, 0);
                do {
                    ++res.back().second;
                    x /= d;
                } while (x % d == 0);
            }
            return res;
        }
        siv() {
            std::iota(p, p + MX + 1, 0);
            p[0] = p[1] = -1;
            for (int i = 2; i <= MX; ++i) {
                if (i == p[i]) {
                    for (int j = i + i; j <= MX; j += i) {
                        p[j] = i;
                    }
                }
            }
        }
    };
    template<int MX>
    int siv<MX>::p[MX + 1];

    template<typename T>
    bool prime(T x) {
        if (x < 2) {
            return false;
        }
        for (T i = 2; i * i <= x; ++i) {
            if (x % i == 0) {
                return false;
            }
        }
        return true;
    }
    template<typename T>
    std::vector<T> divs(T x) {
        std::list<T> res, resr;
        for (T i = 1; i * i <= x; ++i) {
            if (x % i == 0) {
                res.emplace_back(i);
                resr.emplace_front(x / i);
            }
        }
        if (!res.empty() && res.back() * res.back() == x) {
            res.pop_back();
        }
        res.insert(res.end(), resr.begin(), resr.end());
        return std::vector<T> {res.begin(), res.end()};
    }
    template<typename T>
    __gnu_pbds::gp_hash_table<T, int> fact_to_map(T x) {
        __gnu_pbds::gp_hash_table<T, int> res;
        for (T i = 2; i * i <= x; ++i) {
            while (x % i == 0) {
                ++res[i];
                x /= i;
            }
        }
        return res;
    }
    template<typename T>
    std::vector< std::pair<T, int> > fact_to_vpr(T x) {
        std::vector< std::pair<T, int> > res;
        for (T i = 2; i * i <= x; ++i) {
            if (x % i == 0) {
                res.emplace_back(i, 0);
                do {
                    ++res.back().second;
                    x /= i;
                } while (x % i == 0);
            }
        }
        if (x > 1) {
            res.emplace_back(x, 1);
        }
        return res;
    }
    template<typename T>
    T gcd(T a, T b) {
        return b ? gcd(b, a % b) : a;
    }
    template<typename T>
    T lcm(T a, T b) {
        return a / gcd(a, b) * b;
    }
    template<typename T, T BASE>
    T dpow(int power) {
        static std::vector<T> powers(1, (T)1);
        while (power >= (int)powers.size()) {
            powers.emplace_back(powers.back() * BASE);
        }
        return powers[power];
    }
}

namespace hlp {
    template<typename T> inline T sq(T x) { return x * x; }
    template<class T1, class T2> inline auto max(T1 a, T2 b) { return (a < b ? b : a); }
    template<class T1, class T2> inline auto min(T1 a, T2 b) { return (a < b ? a : b); }
    template<class T1, class T2> inline T1 maxx(T1& a, T2 b) { a = std::max(a, (T1)b); return a; }
    template<class T1, class T2> inline T1 minn(T1& a, T2 b) { a = std::min(a, (T1)b); return a; }
    inline double _get_time() { return clock() / (double)CLOCKS_PER_SEC; }
    inline bool _time_cut(double max_time) { return _get_time() > max_time; }
    namespace to4 {
        const int cnt = 4;
        const int dx[] = {0, 1, 0, -1};
        const int dy[] = {1, 0, -1, 0};
    }
    namespace to8 {
        const int cnt = 8;
        const int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
        const int dy[] = {1, 1, 0, -1, -1, -1, 0, 1};
    }
    namespace toK {
        const int cnt = 8;
        const int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
        const int dy[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    }
    template<typename T>
    inline int dsum(T x) {
        int ans = 0;
        do ans += x % 10; while (x /= 10);
        return ans;
    }
    template<typename T>
    inline std::pair<T, T> isegs(T l1, T r1, T l2, T r2) {
        l1 = std::max(l1, l2);
        r1 = std::min(r1, r2);
        return {l1, r1};
    }
    namespace alf {
        const std::string lil = "abcdefghijklmnopqrstuvwxyz";
        const std::string big = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    namespace inf {
        const int I = 2000000007;
        const int I1 = 1000000007;
        const int I2 = 2000000007;
        const int64_t L = 2000000000000000007;
    }
    const long long MOD1 = 1000000007;
    const long long MOD2 = 998244353;
}
using namespace hlp;

namespace solution {
    inline void _begin_solve_();
    inline void _solve_();
    inline void _after_solve_();
}

int main() {
    gll::init();
    gll::run(solution::_begin_solve_,
             solution::_solve_,
             solution::_after_solve_);
}

namespace kuk {
    template<typename T>
    using V = std::vector<T>;
    template<typename T1, typename T2>
    using P = std::pair<T1, T2>;
    template<typename T>
    using PP = std::pair<T, T>;
    template<typename T1, typename T2>
    using umap = __gnu_pbds::gp_hash_table<T1, T2>;
    template<typename T>
    using mset = std::multiset<T>;
    template<typename T, typename F = std::less<T> >
    using oset = __gnu_pbds::tree<T, __gnu_pbds::null_type, F, __gnu_pbds::rb_tree_tag,
            __gnu_pbds::tree_order_statistics_node_update>;
    template<typename T, typename F = std::less_equal<T> >
    using omset = __gnu_pbds::tree<T, __gnu_pbds::null_type, F, __gnu_pbds::rb_tree_tag,
            __gnu_pbds::tree_order_statistics_node_update>;
    using uint = uint32_t;
    using ll = int64_t;
    using ull = uint64_t;
    using ld = double;
    using lld = long double;
    using str = std::string;
    using vi = V<int32_t>;
    using vll = V<ll>;
    using vch = V<char>;
    using vld = V<ld>;
    using vlld = V<lld>;
    using vs = V<str>;
    using pii = PP<int32_t>;
    using pll = PP<ll>;
    using pli = P<ll, int32_t>;
    using pil = P<int32_t, ll>;
    using pld = PP<ld>;
    using plld = PP<lld>;
    using vvi = V<vi>;
    using vvll = V<vll>;
    using vpii = V<pii>;
    using vpll = V<pll>;
    using vvch = V<vch>;
    using mii = std::map<int32_t, int32_t>;
    using mic = std::map<int32_t, char>;
    using mivi = std::map<int32_t, vi>;
    using mll = std::map<ll, ll>;
    using mlc = std::map<ll, char>;
    using mlvl = std::map<ll, vll>;
    using mci = std::map<char, int32_t>;
    using mcl = std::map<char, ll>;
    using mcvi = std::map<char, vi>;
    using mcvll = std::map<char, vll>;
    using umii = umap<int32_t, int32_t>;
    using umic = umap<int32_t, char>;
    using umivi = umap<int32_t, vi>;
    using umll = umap<ll, ll>;
    using umlc = umap<ll, char>;
    using umlvl = umap<ll, vll>;
    using umci = umap<char, int32_t>;
    using umcl = umap<char, ll>;
    using umcvi = umap<char, vi>;
    using umcvll = umap<char, vll>;
}
using namespace kuk;

template<class T1, class T2> std::ostream& operator<<(std::ostream &o, std::pair<T1, T2> x) { return o << x.first << ' ' << x.second; }
template<class T1, class T2> std::istream& operator>>(std::istream &o, std::pair<T1, T2> &x) { return o >> x.first >> x.second; }
template<class T> std::ostream& operator<<(std::ostream &o, std::vector<T> &x) { for (auto &el : x) o << el << ' '; return o; }
template<class T> std::ostream& operator<<(std::ostream &o, std::set<T> &x) { for (auto &el : x) o << el << ' '; return o; }
template<class T> std::ostream& operator<<(std::ostream &o, std::multiset<T> &x) { for (auto &el : x) o << el << ' '; return o; }
template<class T1, class T2> std::ostream& operator<<(std::ostream &o, std::map<T1, T2> &x) { o << '['; for (auto &el : x) o << '(' << el.first << ", " << el.second << ")\t"; o << ']'; return o; }
template<class T1, class T2> std::ostream& operator<<(std::ostream &o, std::unordered_map<T1, T2> &x) { o << '['; for (auto &el : x) o << '(' << el.first << ", " << el.second << ")\t"; o << ']'; return o; }
template<class T> std::ostream& operator<=(std::ostream &o, std::vector<T> &x) { o << x.size() << '\n' << x; return o; }
template<class T> std::istream& operator>>(std::istream &o, std::vector<T> &x) { for (auto &el : x) o >> el; return o; }
template<class T1, class T2> std::pair<T1, T2> operator+(std::pair<T1, T2> a, std::pair<T1, T2> b) { a.first += b.first; a.second += b.second; return a; }
template<class T1, class T2> std::pair<T1, T2> operator-(std::pair<T1, T2> a, std::pair<T1, T2> b) { a.first -= b.first; a.second -= b.second; return a; }
template<class T1, class T2> void operator+=(std::pair<T1, T2> &a, std::pair<T1, T2> b) { a.first += b.first; a.second += b.second; }
template<class T1, class T2> void operator-=(std::pair<T1, T2> &a, std::pair<T1, T2> b) { a.first -= b.first; a.second -= b.second; }

template<typename T> void _input_(T& _last) {
    std::cin >> _last;
}
template<typename T1, typename... T> void _input_(T1& _begin, T&... _rest) {
    std::cin >> _begin;
    _input_(_rest...);
}

#define AUT(type, ...) type __VA_ARGS__; _input_(__VA_ARGS__)
#define INT(...) AUT(int, __VA_ARGS__)
#define LL(...) AUT(long long, __VA_ARGS__)
#define CHAR(...) AUT(char, __VA_ARGS__)
#define STR(...) AUT(std::string, __VA_ARGS__)
#define PII(...) using _PII_ = std::pair<int, int>; AUT(_PII_, __VA_ARGS__)
#define PLL(...) using _PLL_ = std::pair<long long, long long>; AUT(_PLL_, __VA_ARGS__)
#define PLD(...) using _PLD_ = std::pair<double, double>; AUT(_PLD_, __VA_ARGS__)
#define PLLD(...)using _PLLD_ = std::pair<long double, long double>;  AUT(_PLLD_, __VA_ARGS__)

#define VEC(type, cnt, a) std::vector<type> a(cnt); _input_(a)
#define VI(a, n) VEC(int, n, a)
#define VLL(a, n) VEC(long long, n, a)
#define VLD(a, n) VEC(double, n, a)
#define VLLD(a, n) VEC(long double, n, a)
#define VC(a, n) VEC(char, n, a)
#define VS(a, n) VEC(std::string, n, a)
#define VPII(a, n) using _PII_ = std::pair<int, int>; VEC(_PII_, n, a)
#define VPLL(a, n) using _PLL_ = std::pair<long long, long long>; VEC(_PLL_, n, a)
#define VVI(a, n, m) std::vector< std::vector<int> > a(n, std::vector<int>(m)); _input_(a)
#define VVC(a, n, m) std::vector< std::vector<char> > a(n, std::vector<char>(m)); _input_(a)
#define VVLL(a, n, m) std::vector< std::vector<long long> > a(n, std::vector<long long>(m)); _input_(a)

template<typename T> inline void vout(const T& _last) {
    std::cout << _last;
}
template<typename T1, typename... T> inline void vout(const T1& _begin, const T&... _rest) {
    std::cout << _begin;
    vout(_rest...);
}
inline void voutln() {
    std::cout << '\n';
}
template<typename T> inline void voutln(const T& _last) {
    std::cout << _last << '\n';
}
template<typename T1, typename... T> inline void voutln(const T1& _begin, const T&... _rest) {
    std::cout << _begin << ' ';
    voutln(_rest...);
}

#ifdef LOCAL
#undef LOCAL
#define LOCAL 1
#else
#undef LOCAL
#define LOCAL 0
#endif

#ifdef ONLINE_JUDGE
#undef ONLINE_JUDGE
#define ONLINE_JUDGE 1
#else
#undef ONLINE_JUDGE
#define ONLINE_JUDGE 0
#endif

#if !LOCAL && !ONLINE_JUDGE
#undef ONLINE_JUDGE
#define ONLINE_JUDGE 1
#endif

#define ADD_CFG(def_name, set_cfg) gll::env::upd_cfg(set_cfg + gll::cfg::enable{def_name});

#define re return
#define fi first
#define se second
#define mp make_pair
#define pb emplace_back
#define upb(a, l, r) upper_bound((a).begin() + (l), (a).begin() + (r) + 1) - (a).begin()
#define lowb(a, l, r) lower_bound((a).begin() + (l), (a).begin() + (r) + 1) - (a).begin()
#define con continue
#define pob pop_back
#define ins insert
#define ba(x) (x).back()
#define bg(x) (x).begin()
#define en(x) (x).end()
#define sz(x) (ll)((x).size())
#define all(x) (x).begin(),(x).end()
#define f0r(i,n) for(int i=0;i<(n);++i)
#define f0rr(i,n) for(int i=(n)-1;i>=0;--i)
#define rep(i,l,r) for (auto i=l;i<(r);++i)
#define repr(i,r,l) for (auto i=(r);i>(l);--i)
#define rip(x) for(int _i_=0;_i_<(x);++_i_)
#define riph(x,hash) for(int _i_##hash=0;_i_##hash<(x);++_i_##hash)
#define unq(a) (a).resize(std::unique((a).begin(),(a).end())-a.begin())
#define YES std::cout << "YES\n"
#define Yes std::cout << "Yes\n"
#define yes std::cout << "yes\n"
#define NO std::cout << "NO\n"
#define No std::cout << "No\n"
#define no std::cout << "no\n"
#define YESre std::cout << "YES\n"; return
#define Yesre std::cout << "Yes\n"; return
#define yesre std::cout << "yes\n"; return
#define NOre std::cout << "NO\n"; return
#define Nore std::cout << "No\n"; return
#define nore std::cout << "no\n"; return

using namespace std;
using namespace __gnu_pbds;
using namespace __gnu_cxx;

void gll::init() {
    ADD_CFG(LOCAL, gcfg::ifile{"input.txt"} + gcfg::ofile{""} + gcfg::verb{1})
    ADD_CFG(ONLINE_JUDGE, gcfg::ifile{""} + gcfg::ofile{""} + gcfg::verb{0})
    ADD_CFG(LOCAL || ONLINE_JUDGE, gcfg::tcnt{env::MULTITEST} + gcfg::sync{0} + gcfg::out{std::fixed} + gcfg::sprec{20})
}

//INSERT ALGOS HERE
//-----------------

void solution::_begin_solve_() {
}
void solution::_after_solve_() {
}

void solution::_solve_() {

}





//

#include <cstdio>
#include <cctype>
#include <algorithm>
#include <queue>

typedef long long LL;
const int maxn = 500000;
const LL inf = 0X9000000000000000;

template<typename __Tp, typename __Compare>
class DoublyQueue {
    public:
        void push(const __Tp &__v) {
            __Compare comp;
            while(elem.size() && comp(__v, elem.back()))
                elem.pop_back();
            elem.push_back(__v);
        }

        inline void pop() {
            elem.pop_front();
        }

        inline __Tp top() {
            return elem.front();
        }

        inline bool empty() {
            return elem.empty();
        }

        inline int size() {
            return elem.size();
        }
    private:
        std::deque<__Tp> elem;
};

int pos[maxn + 5];
LL a[maxn + 5], dp[maxn + 5];
int d, k, n;

struct MyCmp {
    inline bool operator()(int a, int b) const {
        return dp[a] >= dp[b];
    }
};

bool check(int v) {
    int llen = std::max(d - v, 1), rlen = std::min(d + v, pos[n]);
    DoublyQueue<LL, MyCmp> q;
    for(int i = 1; i <= n; ++i)
        dp[i] = 0;
    int cur = 0;
    for(int i = 1; i <= n; ++i) {
        while(cur < i && pos[i] - pos[cur] >= llen)
            q.push(cur), cur++;
        while(q.size() && pos[i] - pos[q.top()] > rlen)
            q.pop();
        if(q.empty()) dp[i] = inf;
        else dp[i] = dp[q.top()] + a[i];
        if(dp[i] >= k) return true;
    }
    return false;
}

template<typename __Tp>
void read(__Tp &v) {
    v = 0;
    char ch;
    bool nega = false;
    while(!isdigit(ch = getchar()) && ch != '-');
    if(ch == '-')
        nega = true, ch = getchar();
    do
        v = v * 10 + ch - '0';
    while(isdigit(ch = getchar()));
    if(nega) v = -v;
}

int main() {
    LL step = 0;
    read(n), read(d), read(k);
    for(int i = 1; i <= n; ++i) {
        read(pos[i]), read(a[i]);
        if(a[i] > 0) step += a[i];
    }
    if(step < k)
        return puts("-1"), 0;
    int l = 0, r = pos[n] + 5;
    while(l < r) {
        int mid = (l + r - 1) >> 1;
        if(check(mid))
            r = mid;
        else
            l = mid + 1;
    }
    printf("%d\n", r);
    return 0;
}
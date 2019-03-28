#include <cmath>
#include <cstdio>
#include <algorithm>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

typedef __gnu_pbds::tree<int,
                         int,
                         std::less<int>,
                         __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>
        balanced_tree;
typedef long long LL;

struct Node {
    LL cnt1, cnt2;

    Node(LL c1 = 0, LL c2 = 0) : cnt1(c1), cnt2(c2) {}

    Node operator+ (const Node &node) const {
        return Node(node.cnt1 + cnt1, node.cnt2 + cnt2);
    }

    operator long long () const {
        return cnt1 + cnt2;
    }
};

extern int h[100005];
struct MyCmp {
    LL val;

    MyCmp(LL v) : val(v) {}

    bool operator() (int a, int b) const {
        LL tempa = abs(h[a] - val), tempb = abs(h[b] - val);
        if (tempa != tempb)
            return tempa < tempb;
        return h[a] < h[b];
    }
};

balanced_tree s;
int  h[100005];
int  f[25][100005];
Node g[25][100005];
int f1[100005], f2[100005];
LL g1[100005], g2[100005];
int n, m, x;

void build_graph() {
    for (int i = n; i >= 1; --i) {
        static int temp[5];
        int cnt = 0;
        s.insert(std::make_pair(h[i], i));
        int pos = s.order_of_key(h[i]);
        if (pos > 0)
            temp[cnt++] = s.find_by_order(pos - 1)->second;
        if (pos > 1)
            temp[cnt++] = s.find_by_order(pos - 2)->second;
        if (pos < n - i)
            temp[cnt++] = s.find_by_order(pos + 1)->second;
        if (pos < n - i - 1)
            temp[cnt++] = s.find_by_order(pos + 2)->second;
        std::sort(temp, temp + cnt, MyCmp(h[i]));
        if (cnt > 1)
            f1[i] = temp[1],
            g1[i] = abs(h[temp[1]] - h[i]),
            f[0][i] = temp[1],
            g[0][i] = Node(g1[i]),
            f[1][i] = f2[temp[1]],
            g[1][i] = Node(g1[i], g2[f1[i]]);
        if (cnt > 0)
            f2[i] = temp[0],
            g2[i] = abs(h[temp[0]] - h[i]);
    }
    for (int i = 2; 1 << i <= n; ++i)
    for (int j = 1; j <= n; ++j)
        if (f[i - 1][f[i - 1][j]])
            f[i][j] = f[i - 1][f[i - 1][j]],
            g[i][j] = g[i - 1][j] + g[i - 1][f[i - 1][j]];
}

Node solve(int s) {
    Node ans;
    for (int i = 18; i >= 0; --i)
        if (f[i][s] && ans + g[i][s] <= x)
            ans = ans + g[i][s],
            s = f[i][s];
     return ans;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%d", h + i);
    build_graph();
    long double temp = 1.0/0.0;
    int pos = 1;
    scanf("%d", &x);
    for (int i = 1; i <= n; ++i) {
        Node ans = solve(i);
        long double tempans = (long double)ans.cnt1 / (long double)ans.cnt2;
        if (tempans < temp)
            temp = tempans,
            pos = i;
    }
    printf("%d\n", pos);
    scanf("%d", &m);
    while (m--) {
        scanf("%d %d", &pos, &x);
        Node ans = solve(pos);
        printf("%lld %lld\n", ans.cnt1, ans.cnt2);
    }
    return 0;
}

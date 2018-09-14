#include <cstdio>
#include <queue>
#include <algorithm>

struct Point {
    int dist = 0, head = 0;
    int fa = 0, fa_dist = 0;
    int cur_dist = 0;
    bool vis = false;
} pt[305];
struct Path {
    int end = 0, weight = 0;
    int next = 0;

    Path(int __end = 0, int __cost = 0, int __next = 0) :
        end(__end), weight(__cost), next(__next) {}
} ph[605];

int n, s, ptr, m;
int del[305];

void set_path(int u, int v, int w) {
    ph[++ptr] = Path(v, w, pt[u].head), pt[u].head = ptr;
    ph[++ptr] = Path(u, w, pt[v].head), pt[v].head = ptr;
}

void dfs1(int p) {
    if(pt[p].vis) return;
    pt[p].vis = true;
    for(int i = pt[p].head; i; i = ph[i].next) {
        int s = ph[i].end;
        if(pt[s].vis) continue;
        pt[s].fa = p,
        pt[s].fa_dist = ph[i].weight,
        pt[s].dist = pt[p].dist + ph[i].weight;
        dfs1(s);
    }
}

int dfs2(int p) {
    pt[p].vis = true;
    int temp = 0;
    for(int i = pt[p].head; i; i = ph[i].next) {
        int s = ph[i].end;
        if(pt[s].vis) continue;
        pt[s].dist = pt[p].dist + ph[i].weight;
        dfs2(s);
        temp = std::max(temp, pt[s].dist);
    }
    return temp;
} 

int main() {
    scanf("%d %d", &n, &s);
    for(int i = 2; i <= n; ++i)	{
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        set_path(u, v, w);
    }

    dfs1(1);
    int rt, tempx = 0;
    for(int i = 1; i <= n; ++i) {
        pt[i].vis = false;
        if(pt[i].dist > pt[tempx].dist)
            tempx = i;
    }
    rt = tempx, tempx = 0;
    for(int i = 1; i <= n; ++i)
        pt[i].dist = pt[i].fa =
        pt[i].fa_dist = 0;

    dfs1(rt);
    for(int i = 1; i <= n; ++i) {
        pt[i].vis = false;
        if(pt[i].dist > pt[tempx].dist)
            tempx = i;
    }
    int dist1 = 0, dist2 = 0, tot;
    while(pt[tempx].fa)
        dist2 += pt[tempx].fa_dist,
        del[++m] = tempx,
        pt[tempx].vis = true,
        tempx = pt[tempx].fa;
    pt[rt].vis = true, del[++m] = tempx;
    tot = dist2;

    for(int i = 1; i <= n; ++i)
        pt[i].dist = 0;
    for(int i = 1; i <= m; ++i)
        pt[del[i]].dist = dfs2(del[i]);

    std::deque<int> q;
    int ans = 2147483647;
    for(int i = 1, j = 1; i <= m; dist2 -= pt[del[i]].fa_dist, ++i) {
        pt[del[i]].cur_dist = dist2;
        while(!q.empty() && pt[q.front()].cur_dist - s > pt[del[i]].cur_dist)
            q.pop_front();
        while(j < i && tot - dist1 - dist2 > s)
            dist1 += pt[del[j++]].fa_dist;
        while(!q.empty() && pt[q.back()].dist < pt[del[i]].dist)
            q.pop_back();
        q.push_back(del[i]);
        int temp = std::max(dist1, dist2);
        temp = std::max(temp, pt[q.front()].dist);
        ans = std::min(temp, ans);
    }
    printf("%d", ans);
    return 0;
}
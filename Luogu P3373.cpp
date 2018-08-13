#include <cstdio>
#include <cctype>

typedef long long LL;

LL mod;
LL a[100005];

class SegmentTree {
    public:
        SegmentTree(LL *b, LL *e) {
            sz = e - b;
            root = new STNode;
            root->lpos = 1, root->rpos = sz;
            build(root, b);
        }

        void modify(size_t l, size_t r, LL v, int mode) {
            if(mode == 1)
                modify_add(root, l, r, v);
            else
                modify_mul(root, l, r, v);
        }

        LL query(size_t l, size_t r) {
            return query_buildin(root, l, r);
        }

        size_t size() {
            return sz;
        }
    private:
        size_t sz;

        struct STNode {
            STNode *lson = nullptr, *rson = nullptr;
            LL tag_add = 0, tag_mul = 1, val = 0;
            size_t lpos = 0, rpos = 0;

            void update() {
                val = 0;
                val += lson->val * lson->tag_mul + lson->tag_add * (lson->rpos - lson->lpos + 1);
                val += rson->val * rson->tag_mul + rson->tag_add * (rson->rpos - rson->lpos + 1);
                val %= mod;
            }

            void mark_mul(LL v) {
                if(lpos == rpos) {
                    val *= v;
                    val %= mod;
                    return;
                }
                tag_add *= v;
                tag_mul *= v;
                tag_add %= mod;
                tag_mul %= mod;
            }

            void mark_add(LL v) {
                if(lpos == rpos)
                    val += v, val %= mod;
                else
                    tag_add += v, tag_add %= mod;
            }

            void demark() {
                lson->mark_mul(tag_mul);
                rson->mark_mul(tag_mul);
                lson->mark_add(tag_add);
                rson->mark_add(tag_add);
                tag_add = 0, tag_mul = 1;
                update();
            }
        } *root;

        void build(STNode *it, LL *b) {
            if(it->lpos == it->rpos) {
                it->val = b[it->lpos];
                return;
            }
            size_t m = (it->lpos + it->rpos) >> 1;
            it->lson = new STNode,
            it->lson->lpos = it->lpos,
            it->lson->rpos = m;
            it->rson = new STNode;
            it->rson->lpos = m + 1;
            it->rson->rpos = it->rpos;
            build(it->lson, b);
            build(it->rson, b);
            it->update();
        }

        void modify_add(STNode *it, size_t l, size_t r, LL v) {
            if(l <= it->lpos && it->rpos <= r) {
                it->mark_add(v);
                return;
            }
            size_t m = (it->lpos + it->rpos) >> 1;
            it->demark();
            if(l <= m)
                modify_add(it->lson, l, r, v);
            if(r >  m)
                modify_add(it->rson, l, r, v);
            it->update();
        }

        void modify_mul(STNode *it, size_t l, size_t r, LL v) {
            if(l <= it->lpos && it->rpos <= r) {
                it->mark_mul(v);
                return;
            }
            size_t m = (it->lpos + it->rpos) >> 1;
            it->demark();
            if(l <= m)
                modify_mul(it->lson, l, r, v);
            if(r >  m)
                modify_mul(it->rson, l, r, v);
            it->update();
        }

        LL query_buildin(STNode *it, size_t l, size_t r) {
            if(it->lpos != it->rpos) {
//                printf("---%d %d %lld %lld %lld\n", it->lpos, it->rpos, it->val, it->tag_add, it->tag_mul);
                it->demark();
                it->update();
            }
            if(l <= it->lpos && it->rpos <= r)
                return (it->val * it->tag_mul % mod + it->tag_add) % mod;
            size_t m = (it->lpos + it->rpos) >> 1;
            LL ans = 0;
            if(l <= m)
                ans += query_buildin(it->lson, l, r);
            if(r >  m)
                ans += query_buildin(it->rson, l, r);
            return ans % mod;
        }
};

template<typename __Tp>
void read(__Tp &v) {
    char ch;
    bool flag = false;
    v = 0;
    while(!isdigit(ch = getchar()) && ch != '-');
    if(ch == '-') flag = true;
    do
        v = v * 10 + ch - '0';
    while(isdigit(ch = getchar()));
    if(flag) v = -v;
}

int main() {
    int n, m;
    read(n), read(m), read(mod);
    for(int i = 1; i <= n; ++i)
        read(a[i]);
    SegmentTree s(a, a + n);
    while(m--) {
        int op;
        read(op);
        switch(op) {
            case 1: {
                int x, y;
                LL k;
                read(x), read(y), read(k);
                s.modify(x, y, k, 2);
                break;
            } case 2: {
                int x, y;
                LL k;
                read(x), read(y), read(k);
                s.modify(x, y, k, 1);
                break;
            } case 3: {
                int x, y;
                read(x), read(y);
                printf("%lld\n", s.query(x, y));
                break;
            } case 4: {
                puts("-----------debug-----------");
                for(int i = 1; i <= n; ++i)
                    printf("%lld ", s.query(i, i));
                puts("\n---------------------------");
                break;
            }
        }
   }
    return 0;
}
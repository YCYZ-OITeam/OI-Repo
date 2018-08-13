#include <cstdio>

const double eps = 1e-12;

double val[21];
int n;

double f(double x) {
    double ans = val[1];
    for(int i = 1; i <= n; ++i)
        ans = ans * x + val[i + 1];
    return ans;
}

int main() {
    double l, r;
    scanf("%d %lf %lf", &n, &l, &r);
    for(int i = 1; i <= n; ++i)
        scanf("%lf", val + i);
    while(r - l >= eps) {
        double lm = l + (r - l) / 3.0;
        double rm = r - (r - l) / 3.0;
        if(f(lm) - f(rm) < -eps)
            l = lm;
        else
            r = rm;
    }
    printf("%.5lf", (l + r) / 2);
    return 0;
}
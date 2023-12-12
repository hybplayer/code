#include <bits/stdc++.h>
using namespace std;
const int N = 100010;
int q[N];
void quick_sort(int q[], int l, int r) {
    if (l >= r) return;
    int i = l - 1;
    int j = r + 1;
    int x = q[l + r >> 1];
    while (i < j) {
        do {
            i++;
        } while (q[i] < x);
        do {
            j--;
        } while (q[j] > x);
        if (i < j) swap(q[i], q[j]);
    }
    cout << i << " " << j << " " << x << " " << q[i] << endl;
    quick_sort(q, l, j);
    quick_sort(q, j + 1, r);
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &q[i]);
    quick_sort(q, 0, n - 1);
    for (int i = 0; i < n; i++) printf("%d ", q[i]);
    return 0;
}
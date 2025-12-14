/* ProbId: D2-AB-Optimal-RMQ */

#include <algorithm>
#include <climits>
#include <cstdlib>

static const int B = 9;
static int *G;
static int  N, nb;
static int *block_min_idx;
static int **st;
static int *log_2;

void warm_up(int seq[], int n) {
    G = seq;
    N = n;
    if (N <= 0) return;
    nb = (N + B - 1) / B;
    block_min_idx = (int*)std::malloc(nb * sizeof(int));
    log_2         = (int*)std::malloc((nb + 1) * sizeof(int));
    for (int bi = 0; bi < nb; ++bi) {
        int s = bi * B, e = std::min(N, s + B), mi = s;
        for (int i = s + 1; i < e; ++i)
            if (seq[i] < seq[mi]) mi = i;
        block_min_idx[bi] = mi;
    }
    log_2[1] = 0;
    for (int i = 2; i <= nb; ++i)
        log_2[i] = log_2[i >> 1] + 1;
    int K = log_2[nb] + 1;
    st = (int**)std::malloc(K * sizeof(int*));
    st[0] = (int*)std::malloc(nb * sizeof(int));
    for (int i = 0; i < nb; ++i)
        st[0][i] = i;
    for (int k = 1; k < K; ++k) {
        int span = 1 << k, half = span >> 1;
        st[k] = (int*)std::malloc((nb - span + 1) * sizeof(int));
        for (int i = 0; i + span <= nb; ++i) {
            int x = st[k - 1][i], y = st[k - 1][i + half];
            int vx = seq[block_min_idx[x]], vy = seq[block_min_idx[y]];
            st[k][i] = (vx <= vy ? x : y);
        }
    }
}

int query(int L, int R) {
    if (L < 0 || R < L || R >= N) return -1;
    int bi = L / B, bj = R / B, ans = INT_MAX;
    if (bi == bj) {
        for (int i = L; i <= R; ++i)
            ans = std::min(ans, G[i]);
        return ans;
    }
    int endL = (bi + 1) * B;
    for (int i = L; i < endL; ++i)
        ans = std::min(ans, G[i]);
    int startR = bj * B;
    for (int i = startR; i <= R; ++i)
        ans = std::min(ans, G[i]);
    if (bj - bi > 1) {
        int l = bi + 1, r = bj - 1, len = r - l + 1, k = log_2[len];
        int x = st[k][l], y = st[k][r - (1 << k) + 1];
        int vx = G[block_min_idx[x]], vy = G[block_min_idx[y]];
        ans = std::min(ans, vx <= vy ? vx : vy);
    }
    return ans;
}

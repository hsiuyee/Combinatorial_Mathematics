/* ProbId: D2-AB-Optimal-RMQ */

#include <bits/stdc++.h>
using namespace std;

// block size recommendation from hint
static const int B = 9;

// global storage for the array and RMQ structures
static int *G = nullptr;            // pointer to input array
static int  N = 0;                  // array length
static int  nb = 0;                 // number of blocks
static int *block_min_idx = nullptr; // block_min_idx[i] = index of minimum in block i
static int **st = nullptr;          // sparse table over blocks (stores block indices)
static int *log_2 = nullptr;        // precomputed floor(log2)

// Preprocess in O(n)
void warm_up(int seq[], int n) {
    G = seq;
    N = n;
    if (N <= 0) return;

    // number of blocks of size B
    nb = (N + B - 1) / B;

    // allocate
    block_min_idx = (int*)malloc(nb * sizeof(int));
    log_2         = (int*)malloc((nb + 1) * sizeof(int));

    // 1) per-block minimum
    for (int bi = 0; bi < nb; bi++) {
        int start = bi * B;
        int end   = min(N, start + B);
        int mi = start;
        for (int j = start + 1; j < end; j++) {
            if (seq[j] < seq[mi]) mi = j;
        }
        block_min_idx[bi] = mi;
    }

    // 2) build log2 table up to nb
    log_2[1] = 0;
    for (int i = 2; i <= nb; i++) {
        log_2[i] = log_2[i >> 1] + 1;
    }

    // 3) build sparse table on blocks
    int K = log_2[nb] + 1;
    st = (int**)malloc(K * sizeof(int*));
    // level 0
    st[0] = (int*)malloc(nb * sizeof(int));
    for (int i = 0; i < nb; i++) {
        st[0][i] = i;
    }
    // higher levels
    for (int k = 1; k < K; k++) {
        int span = 1 << k;
        int half = 1 << (k - 1);
        st[k] = (int*)malloc((nb - span + 1) * sizeof(int));
        for (int i = 0; i + span <= nb; i++) {
            int x = st[k - 1][i];
            int y = st[k - 1][i + half];
            int vx = seq[ block_min_idx[x] ];
            int vy = seq[ block_min_idx[y] ];
            st[k][i] = (vx <= vy ? x : y);
        }
    }
}

// Query in O(1)
int query(int L, int R) {
    if (L < 0 || R < L || R >= N) return -1;

    int bi = L / B;
    int bj = R / B;
    int ans = INT_MAX;

    // same block: brute force
    if (bi == bj) {
        for (int i = L; i <= R; i++) {
            ans = min(ans, G[i]);
        }
        return ans;
    }

    // left partial block
    int endL = (bi + 1) * B;
    for (int i = L; i < endL; i++) {
        ans = min(ans, G[i]);
    }

    // right partial block
    int startR = bj * B;
    for (int i = startR; i <= R; i++) {
        ans = min(ans, G[i]);
    }

    // full blocks in between
    if (bj - bi > 1) {
        int l = bi + 1, r = bj - 1;
        int len = r - l + 1;
        int k = log_2[len];
        int x = st[k][l];
        int y = st[k][r - (1 << k) + 1];
        int vx = G[ block_min_idx[x] ];
        int vy = G[ block_min_idx[y] ];
        ans = min(ans, min(vx, vy));
    }

    return ans;
}

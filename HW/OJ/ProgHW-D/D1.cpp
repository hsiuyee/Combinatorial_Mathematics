#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, m;
    cin >> k >> m;
    vector<long long> a(k);
    while (m--) {
        for (int i = 0; i < k; i++) {
            cin >> a[i];
        }

        string encoding;
        encoding.reserve(2 * k);
        vector<int> st;
        st.reserve(k);

        // Build the Cartesian tree encoding with a monotonic stack:
        // - Output '0' when pushing (open parenthesis)
        // - Output '1' when popping  (close parenthesis)
        for (int i = 0; i < k; i++) {
            while (!st.empty() && a[st.back()] > a[i]) {
                st.pop_back();
                encoding.push_back('1');
            }
            st.push_back(i);
            encoding.push_back('0');
        }
        while (!st.empty()) {
            st.pop_back();
            encoding.push_back('1');
        }

        // 'encoding' now has length 2*k
        cout << encoding << "\n";
    }

    return 0;
}

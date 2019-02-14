#include<iostream>
#include<vector>
#include<algorithm>

#define forn(i, n) for(int i = 0; i < (int)(n); i++)

using namespace std;

struct item {
    int rank, next_rank, position;
};

bool operator<(item &a, item &b) {
    if (a.rank != b.rank) return a.rank < b.rank;
    return a.next_rank < b.next_rank;
}

vector<int> suffix_array(vector<int> &data) {
    int n = data.size();
    vector<item> ranks;
    forn(i, n) ranks.push_back(item{data[i], -1, i});
    sort(ranks.begin(), ranks.end());
    int prefix_size = 1;
    while (prefix_size <= n) {
        vector<item> new_ranks(n);
        vector<int> inv(n);
        forn(i, n) {
            inv[ranks[i].position] = i;
            if(
                i && ranks[i].rank == ranks[i - 1].rank
                && ranks[i].next_rank == ranks[i - 1].next_rank
            ) {
                inv[ranks[i].position] = inv[ranks[i -1].position];
            }
        }
        forn(i, n) {
            new_ranks[i].rank = inv[i];
            new_ranks[i].next_rank = -1;
            if(i + prefix_size < n) {
                new_ranks[i].next_rank = inv[i + prefix_size];
            }
            new_ranks[i].position = i;
        }
        sort(new_ranks.begin(), new_ranks.end());
        ranks = new_ranks;
        prefix_size *= 2;
    }
    vector<int> res;
    forn(i, n) res.push_back(ranks[i].position);
    return res;
}

vector<int> suffix_array(string &data) {
    vector<int> new_data;
    forn(i, data.size()) new_data.push_back((int)data[i]);
    return suffix_array(new_data);
}

vector<int> lcp(vector<int> &data, vector<int> &sa) {
    int n = data.size();
    vector<int> inv(n), res(n);
    forn(i, n) inv[sa[i]] = i;
    int k = 0;
    forn(u, n) if(inv[u] + 1 < n) {
        int i = sa[inv[u]] + k;
        int j = sa[inv[u] + 1] + k;
        while(i < n && j < n && data[i] == data[j]) {
            i ++; j ++; k ++;
        }
        res[inv[u]] = k;
        if(k) k --;
    }
    return res;
}

int main() {
    string test = "abracadabra0AbRa4Cad14abra";
    vector<int> data;
    forn(i, test.size()) data.push_back((int)test[i]);
    vector<int> res = suffix_array(data);
    vector<int> l = lcp(data, res);
    forn(i, test.size()) {
        cout << res[i] << " " << l[i] << " " << test.substr(res[i]) << endl;
    }
    return 0;
}

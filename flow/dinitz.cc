#include<iostream>
#include<vector>
#include<cmath>
#include<map>
#include<algorithm>
#include<deque>

#define forn(i, n) for(int i = 0; i < (int)n; i++)
#define pb push_back
#define mp make_pair

using namespace std;

typedef long long int tint;

// x -- arista --> y
// capacidad c, capacidad residual f
// r es la arista opuesta
struct arista {
    int y, r;
    tint c, f;
    arista (int y, int r, tint c) {
        (*this).y = y;
        (*this).r = r;
        (*this).c = c;
        (*this).f = c;
    }
};

vector<vector<arista> > g;

void init(int n) {
    g.clear();
    g.resize(n);
}

void add_arista(int i, int j, tint c) {
    if(i == j) return;
    int ti = g[i].size(), tj = g[j].size();
    g[i].pb(arista(j, tj, c));
    g[j].pb(arista(i, ti, 0));
}

tint flujo(int s, int t) {
    tint res = 0;
    int n = g.size();
    while(true) {
        // corro bfs para armar los rangos
        vector<int> rango(n, n + 1);
        deque<int> cola;
        cola.pb(t);
        rango[t] = 0;
        while(!cola.empty()) {
            int x = cola.front();
            cola.pop_front();
            forn(i, g[x].size()) if(rango[g[x][i].y] > n && g[g[x][i].y][g[x][i].r].f > 0) {
                rango[g[x][i].y] = rango[x] + 1;
                cola.pb(g[x][i].y);
            }
        }
        // la red se desconecto
        if(rango[s] == n + 1) break;
        // ahora hacemos el dfs de dinitz
        vector<int> pila, estado(n), padre(n);
        pila.pb(s);
        padre[s] = s;
        while(pila.size()) {
            int x = pila.back();
            if(x == t) {
                x = padre[x];
                tint mf = g[x][estado[x] - 1].f;
                x = t;
                int mx = x;
                do {
                    x = padre[x];
                    if(g[x][estado[x] - 1].f <= mf) {
                        mf = g[x][estado[x] - 1].f;
                        mx = x;
                    }
                } while (x != s);
                x = t;
                do {
                    x = padre[x];
                    g[x][estado[x] - 1].f -= mf;
                    g[g[x][estado[x] - 1].y][g[x][estado[x] - 1].r].f += mf;
                } while(x != s);
                res += mf;
                while(pila.size() && pila.back() != mx) {
                    estado[pila.back()]--;
                    pila.pop_back();
                }
            } else if(estado[x] == g[x].size()) {
                pila.pop_back();
            } else {
                if(g[x][estado[x]].f && rango[x] > rango[g[x][estado[x]].y]) {
                    pila.pb(g[x][estado[x]].y);
                    padre[g[x][estado[x]].y] = x;
                }
                estado[x] ++;
            }
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, p;
    //// matching spoj
    cin >> n >> m >> p;
    init(n + m + 2);
    forn(i, p) {
        int a, b;
        cin >> a >> b;
        add_arista(a, b + n, 1);
    }
    forn(i, n) add_arista(0, i + 1, 1);
    forn(i, m) add_arista(i + n + 1, n + m + 1, 1);
    cout << flujo(0, n + m + 1) << "\n";
    return 0;
    ///// flujo spoj
    cin>>n>>m;
    init(n+1);
    forn(i, m) {
        int x, y, c;;
        cin >> x >> y >> c;
        add_arista(x, y, c);
        add_arista(y, x, c);
    }
    cout << flujo(1, n) << endl;
    return 0;
    //// matching coj
    while(cin>>n>>m && (n || m)) {
        init(2 * n + 2);
        forn(i, m) {
            int x, y;
            cin >> x >> y;
            add_arista(x + 1, y + n + 1, 1);
        }
        forn(i, n) add_arista(0, i + 1, 1);
        forn(i, n) add_arista(i + n + 1, 2 * n + 1, 1);
        cout << flujo(0, 2 * n + 1) << endl;
    }
}

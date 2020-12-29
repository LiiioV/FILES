#include <vector>
#include <iostream>
#include <queue>
#include <cstring>
#include <algorithm>
#include <map>
#include <algorithm>
#include <set>
using namespace std;


const int MAXN = 310;
const int INF = 1000 * 1000 * 1000;
struct edge {
	int a, b, cap, flow;
};

int a, b, n, s, t, d[MAXN], ptr[MAXN];
vector<edge> e;
vector<int> g[MAXN];
set<int> L;
set<int> R;

void add_edge(int a, int b, int cap) {
	edge e1 = { a, b, cap, 0 };
	edge e2 = { b, a, 0, 0 };
	g[a].push_back((int)e.size());
	e.push_back(e1);
	g[b].push_back((int)e.size());
	e.push_back(e2);
}

void upgrade_edge(int element, int addCap) {
	e[element].cap += addCap;
	e[element ^ 1].cap += addCap;
}

bool bfs() {
	queue<long long> qu;
	memset(d, -1, MAXN * sizeof d[0]);
	qu.push(s);
	d[s] = 0;
	while (!qu.empty()) {
		long long v = qu.front();
		qu.pop();
		for (size_t i = 0; i < g[v].size(); ++i) {
			long long id = g[v][i], to = e[id].b;
			if (d[to] == -1 && e[id].flow < e[id].cap) {
				qu.push(to);
				d[to] = d[v] + 1;
			}
		}
	}
	return d[t] != -1;
}

long long dfs(long long v, long long flow) {
	if (!flow)  return 0;
	if (v == t)  return flow;
	for (; ptr[v] < (long long)g[v].size(); ++ptr[v]) {
		long long id = g[v][ptr[v]],
			to = e[id].b;
		if (d[to] != d[v] + 1)  continue;
		long long pushed = dfs(to, std::min(flow, 1LL * e[id].cap - e[id].flow));
		if (pushed) {
			e[id].flow += pushed;
			e[id ^ 1].flow -= pushed;
			return pushed;
		}
	}
	return 0;
}

long long dinic() {
	long long flow = 0;
	while (true) {
		if (!bfs())  break;
		memset(ptr, 0, MAXN * sizeof(ptr[0]));
		ptr[t] = 0;
		while (long long pushed = dfs(s, INF))
			flow += pushed;
	}
	return flow;
}
void update() {
	for (int i = 0; i < MAXN; ++i)
		g[i] = vector<int>();
	e = vector<edge>();
	L = R = set<int>();
	return;
}

void myDf(int x) {
	for (int i = 0; i < g[x].size(); ++i) {
		edge ed = e[g[x][i]];
		if (ed.flow < ed.cap) {
			if (ed.b >= 1 && ed.b <= a && L.find(ed.b) == L.end()) {
				L.insert(ed.b);
				myDf(ed.b);
			}
			else if (ed.b >= 1 + a && ed.b <= a + b && R.find(ed.b) == R.end()) {
				R.insert(ed.b);
				myDf(ed.b);
			}
		}
	}
}

void solve() {
	cin >> a >> b;
	n = a + b + 2;
	s = 0;
	t = n - 1;
	set<int> cam;
	for (int i = 1; i <= a; ++i) {
		cam = set<int>();
		int x;
		cin >> x;
		while (x) {
			cam.insert(x);
			cin >> x;
		}
		for (int j = 1; j <= b; ++j) {
			if (cam.find(j) == cam.end())
				add_edge(i, j + a, 1);
		}
	}
	for (int i = 1; i <= a; ++i)
		add_edge(0, i, 1);
	for (int i = 1; i <= b; ++i)
		add_edge(i + a, t, 1);
	cout << n - dinic() - 2 << endl;

	myDf(s);
	cout << L.size() << " " << b - R.size() << endl;
	for (auto item : L)
		cout << item << " ";
	cout << endl;
	for (int i = 1; i <= b; ++i) {
		if (R.find(i + a) == R.end()) {
			cout << i << " ";
		}
	}
	cout << endl;
	update();


}

int main() {
	int k;
	cin >> k;
	while (k--)
		solve();
	return 0;
}

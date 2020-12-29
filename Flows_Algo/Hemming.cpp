#include <vector>
#include <iostream>
#include <queue>
#include <cstring>
#include <string>
#include <map>
#include <algorithm>
using namespace std;


const int MAXN = 2010;
const int INF = 1000 * 1000 * 1000;
struct edge {
	int a, b, cap, flow;
};

int n, d[MAXN], ptr[MAXN], source = 0, drain = 1;
vector<edge> e;
vector<int> g[MAXN];
int mp[MAXN * MAXN];


void add_edge(int a, int b, int cap = 1) {
	edge e1 = { a, b, cap, 0 };
	edge e2 = { b, a, cap, 0 };
	g[a].push_back((int)e.size());
	e.push_back(e1);
	g[b].push_back((int)e.size());
	mp[a * MAXN + b] = mp[b * MAXN + a] = (int)e.size();
	e.push_back(e2);
}

void upgrade_edge(int element, int addCap = 1) {
	e[element].cap += addCap;
	e[element ^ 1].cap += addCap;
}

bool bfs() {
	queue<long long> qu;
	memset(d, -1, MAXN * sizeof d[0]);
	d[drain] = -1;
	qu.push(source);
	d[source] = 0;
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
	return d[drain] != -1;
}

long long dfs(long long v, long long flow) {
	if (!flow)  return 0;
	if (v == drain)  return flow;
	for (; ptr[v] < (long long)g[v].size(); ++ptr[v]) {
		long long id = g[v][ptr[v]],
			to = e[id].b;
		if (d[to] != d[v] + 1)  continue;
		long long pushed = dfs(to, min(flow, 1LL * e[id].cap - e[id].flow));
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
		ptr[drain] = 0;
		while (long long pushed = dfs(source, INF))
			flow += pushed;
	}
	return flow;
}
/*
bool bfs() {
queue<int> qu;
memset(d, -1, MAXN * sizeof (d[0]));
qu.push(source);
d[source] = 0;
while (!qu.empty() && d[drain] == -1) {
int v = qu.front();
qu.pop();
for (size_t i = 0; i < g[v].size(); ++i) {
int id = g[v][i], to = e[id].b;
if (d[to] == -1 && e[id].flow < e[id].cap) {
qu.push(to);
d[to] = v;
}
}
}
return d[drain] != -1;
}

int goBack() {
int cur = drain;
int minn = INF;
while (cur != source) {
int edge = mp[d[cur] * MAXN + cur];
if (e[edge].a != d[cur])
edge ^= 1;
minn = min(minn, e[edge].cap - e[edge].flow);
cur = d[cur];
}
cur = drain;
while (cur != source) {
int edge = mp[d[cur] * MAXN + cur];
if (e[edge].a != d[cur])
edge ^= 1;
e[edge].flow += minn;
e[edge ^ 1].flow -= minn;
cur = d[cur];
}
return minn;
}

int edmond() {
int flow = 0;
while (bfs())
flow += goBack();
return flow;
}

*/
string s, p;
bool used[2007];

void eshutyun(int l) {
	//cout << "hopp " << l << endl;
	for (size_t i = 0; i < g[l].size(); ++i) {
		if (e[g[l][i]].flow != e[g[l][i]].cap && !used[e[g[l][i]].b]) {
			if (e[g[l][i]].b >= 1003)
				p[e[g[l][i]].b - 1003] = '0';
			else if (e[g[l][i]].b >= 2)
				s[e[g[l][i]].b - 2] = '0';
			else
				continue;
			used[e[g[l][i]].b] = true;
			eshutyun(e[g[l][i]].b);
		}
	}
}

void solve() {
	edge fake = { 0, 0, 0, 0 };
	e.push_back(fake);
	e.push_back(fake);
	cin >> s >> p;
	int ans = 0;
	for (size_t i = 0; i <= s.size() - p.size(); ++i) {
		for (size_t j = 0; j < p.size(); ++j) {
			if ((p[j] == '1' && s[i + j] == '0') || (p[j] == '0' && s[i + j] == '1'))
				++ans;
			else if (p[j] == '?' && s[i + j] != '?')
				mp[(j + 1003) * MAXN + s[i + j] - '0'] ? upgrade_edge(mp[(j + 1003) * MAXN + s[i + j] - '0']) : add_edge(j + 1003, s[i + j] - '0');
			else if (p[j] == '?')
				mp[(j + 1003) * MAXN + i + j + 2] ? upgrade_edge(mp[(j + 1003) * MAXN + i + j + 2]) : add_edge(j + 1003, i + j + 2);
			else if (s[i + j] == '?')
				mp[(p[j] - '0') * MAXN + i + j + 2] ? upgrade_edge(mp[(p[j] - '0') * MAXN + i + j + 2]) : add_edge(p[j] - '0', i + j + 2);
		}
	}
	ans += dinic();
	cout << ans << endl;

	used[0] = true;
	used[1] = true;
	eshutyun(0);
	for (size_t i = 0; i < s.size(); ++i)
	if (s[i] == '?')
		s[i] = '1';
	for (size_t i = 0; i < p.size(); ++i)
	if (p[i] == '?')
		p[i] = '1';
	cout << s << endl << p << endl;

}

int main() {
	solve();
	return 0;
}
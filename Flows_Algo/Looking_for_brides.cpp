#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <stack>
#include <algorithm>
#include <climits>

using namespace std;

typedef long long ll;
typedef long double ld;

struct Edge {
	int used = 0;
	int end;
	ll cost;
	Edge(int x, int y) : end(x), cost(y) {}
};


vector<Edge> vectEdge;
vector<int> g[205];
vector<vector<int>> ansVect;
int source = 1, drain;


void addEdge(int x, int y, int c) {
	g[x].push_back(vectEdge.size());
	vectEdge.emplace_back(y, c);
	g[y].push_back(vectEdge.size());
	vectEdge.emplace_back(x, c);
}

struct myGreater {
	bool operator()(const pair<ll, int> &x, const pair<ll, int> &y) const {
		if (x.first == y.first)
			return x.second > y.second;
		return x.first > y.first;
	}
};

void dfsIN(int vert, const vector<pair<ll, int>> &dist) {
	if (vert == source)
		return;
	int edgInd = dist[vert].second;
	Edge& e = vectEdge[edgInd ^ 1];
	Edge& e2 = vectEdge[edgInd];
	if (e.used)
		e.used = 0;
	else
		e2.used = 1;
	dfsIN(e.end, dist);
	return;
}

ll dijcstra() {
	vector<pair<ll, int>> dist(drain + 1, { LLONG_MAX, -1 });
	priority_queue<pair<ll, int>, vector<pair<ll, int>>, myGreater> qu;
	dist[source].first = 0;
	qu.push({ 0, source });
	while (!qu.empty()) {
		pair<ll, int> topper = qu.top();
		qu.pop();
		if (dist[topper.second].first != topper.first) continue;
		for (int i = 0; i < g[topper.second].size(); ++i) {
			const Edge &e = vectEdge[g[topper.second][i]];
			if (e.used) continue;
			ll newCost = topper.first + e.cost;
			if (vectEdge[g[topper.second][i] ^ 1].used)
				newCost -= 2 * e.cost;
			if (newCost < dist[e.end].first) {
				dist[e.end].first = newCost;
				dist[e.end].second = g[topper.second][i];
				qu.emplace(newCost, e.end);
			}
		}
	}
	if (dist[drain].first == LLONG_MAX) return -1;
	dfsIN(drain, dist);
	return dist[drain].first;
}

void dfs(int where, int which) {
	for (int i = 0; i < g[which].size(); ++i) {
		Edge &e = vectEdge[g[which][i]];
		if (e.used) {
			ansVect[where].push_back(g[which][i] / 2 + 1);
			e.used = 0;
			dfs(where, e.end);
			break;
		}
	}
}

int main() {
	int m, k, x, y, c;
	cin >> drain >> m >> k;
	while (m--) {
		cin >> x >> y >> c;
		addEdge(x, y, c);
	}
	ll realAns = 0;
	x = k;
	while (x--) {
		ll ans = dijcstra();
		if (ans == -1) {
			cout << -1 << endl;
			return 0;
		}
		realAns += ans;
	}
	x = k;
	ansVect.resize(k);
	while (x--)
		dfs(x, source);
	cout.precision(7);
	cout << fixed << (ld)realAns / k << endl;
	for (auto &elem : ansVect) {
		cout << elem.size() << " ";
		for (auto myss : elem)
			cout << myss << " ";
		cout << endl;
	}
	return 0;
}

/*
6 7 2
1 2 0
2 4 1
4 5 1
5 6 0
1 3 0
3 5 0
4 6 0

3 8 4
1 2 1
1 2 1
1 2 1
1 2 1
2 3 1
2 3 1
2 3 1
2 3 1


9 10 2
1 2 1
2 3 1
3 7 1
7 8 1
8 9 1
1 6 1
6 7 1
3 4 1
4 5 1
5 9 1

8 16 3
1 2 1
1 3 1
1 4 1
2 3 1
3 4 1
2 5 1
3 5 1
3 6 1
4 6 1
5 6 1
5 8 1
6 8 1
2 7 1
3 7 1
7 5 1
7 8 1
*/
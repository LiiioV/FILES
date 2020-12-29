#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef long double ld;
const ld INF = 1e9;


void cyclShift(vector<pair<ll, ll>> &coords, ll shift) {
	ll sz = coords.size();
	vector<pair<ll, ll>> other(sz);
	for (int i = 0; i < sz; ++i)
		other[(i + shift % sz + sz) % sz] = coords[i];
	coords = std::move(other);
}

ld operator*(const pair<ll, ll> &a, const pair<ll, ll> &b) {
	return b.first * a.second - b.second * a.first;
}

pair<ll, ll> operator-(const pair<ll, ll> &a, const pair<ll, ll> &b) {
	return{ a.first - b.first, a.second - b.second };
}

ld len(const pair<ll, ll> &a) {
	return hypotl(a.first, a.second);
}

vector<pair<ll, ll>> inputPolds(ll n) {
	vector<pair<ll, ll>> coords(n);
	ll minX = INF;
	ll mini = -1;
	for (ld i = 0; i < n; ++i) {
		cin >> coords[i].first >> coords[i].second;
		if (coords[i].first < minX) {
			minX = coords[i].first;
			mini = i;
		}
	}
	cyclShift(coords, -mini);
	return coords;
}


void processing(vector<pair<ll, ll>> &myHald, const pair<ll, ll> &elem) {
	while (true) {
		if (myHald.size() < 2) {
			myHald.push_back(elem);
			return;
		}
		pair<ll, ll> newEdge = elem - myHald[myHald.size() - 1];
		pair<ll, ll> old = myHald[myHald.size() - 1] - myHald[myHald.size() - 2];
		if (old * newEdge >= 0LL) {
			myHald.push_back(elem);
			return;
		}
		myHald.pop_back();
	}
}

vector<pair<ll, ll>> findConvexHald(vector<pair<ll, ll>> &coords) {
	vector<pair<ll, ll>> myHald;
	for (const auto& elem : coords)
		processing(myHald, elem);
	processing(myHald, coords[0]);
	return myHald;
}

ld findLenght(const vector<pair<ll, ll>> &myHald) {
	ld ans = 0.;
	for (int i = 0; i < myHald.size() - 1; ++i)
		ans += len(myHald[i + 1] - myHald[i]);
	return ans;
}


ld findL2(const vector<pair<ll, ll>> &myHald) {
	ld ans = 0.;
	for (int i = 0; i < myHald.size() - 1; ++i)
		ans += len(myHald[i + 1] - myHald[i]);
	ans += len(myHald.back() - myHald.front());
	return ans;
}


bool cw(pair<ll, ll> a, pair<ll, ll> b, pair<ll, ll> c) {
	return a.first*(b.second - c.second) + b.first*(c.second - a.second) + c.first*(a.second - b.second) < 0;
}

bool ccw(pair<ll, ll> a, pair<ll, ll> b, pair<ll, ll> c) {
	return a.first*(b.second - c.second) + b.first*(c.second - a.second) + c.first*(a.second - b.second) > 0;
}

void convex_hull(vector<pair<ll, ll>> &a) {
	if (a.size() == 1)  return;
	sort(a.begin(), a.end());
	pair<ll, ll> p1 = a[0];
	pair<ll, ll> p2 = a.back();
	vector<pair<ll, ll>> up, down;
	up.push_back(p1);
	down.push_back(p1);
	for (size_t i = 1; i<a.size(); ++i) {
		if (i == a.size() - 1 || cw(p1, a[i], p2)) {
			while (up.size() >= 2 && !cw(up[up.size() - 2], up[up.size() - 1], a[i]))
				up.pop_back();
			up.push_back(a[i]);
		}
		if (i == a.size() - 1 || ccw(p1, a[i], p2)) {
			while (down.size() >= 2 && !ccw(down[down.size() - 2], down[down.size() - 1], a[i]))
				down.pop_back();
			down.push_back(a[i]);
		}
	}
	a.clear();
	for (size_t i = 0; i<up.size(); ++i)
		a.push_back(up[i]);
	for (size_t i = down.size() - 2; i>0; --i)
		a.push_back(down[i]);
}



int main() {
	//cout << make_pair(0, 1) * make_pair(1, 0) << endl;
	ld n;
	ld l;
	cin >> n >> l;
	vector<pair<ll, ll>> coords = inputPolds(n);
	//vector<pair<ld, ld>> convexHald = findConvexHald(coords);
	convex_hull(coords);
	ld ans = acosl(-1) * 2. * l;
	//ans += findLenght(coords);
	ans += findL2(coords);
	
	cout.precision(15);
	cout << fixed << ans << endl;
	return 0;
}

/*
9 10000
20000 40000
30000 40000
30000 30000
40000 30000
40000 40000
50000 40000
50000 20000
35000 20000
20000 20000

6 0
0 0
0 1
1 1
2 2
1 1
1 0
*/
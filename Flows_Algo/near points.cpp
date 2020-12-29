#include <iostream>
#include <algorithm>
#include <memory.h>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <cmath>
#include <ctime>
#include <random>
#include <bitset>
#include <limits.h>
#include <fstream>

#define mp make_pair
#define all(a) (a).begin(), (a).end()
#define pll pair <ll, ll>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef double ld;
#define uint unsigned int
#define mp make_pair

struct Point {
	ll x, y;
	Point() : x(0), y(0) {}
	Point(ll x, ll y) : x(x), y(y) {}
	ll len2() const {	return x * x + y * y;}
	ld len() const {	return hypotl(x, y);}
	Point operator-(const Point &other) const {	return{ x - other.x, y - other.y };}
};

struct firstArg {
	bool operator() (const Point &A, const Point&B) {
		if (A.x == B.x)
			return A.y < B.y;
		return A.x < B.x;
	}
};

struct secondArg {
	bool operator() (const Point &A, const Point&B) {
		if (A.y == B.y)
			return A.x < B.x;
		return A.y < B.y;
	}
};

ll another(const vector<Point> &pp, int start, int end) {
	ll ans = LLONG_MAX;
	for (int i = start; i < end - 1; ++i)
		ans = min(ans, (pp[i].y - pp[i + 1].y) * (pp[i].y - pp[i + 1].y));
	return ans;
}


int toLower(const vector<Point> &pp, int iter1, int l, int r, ll dist2) {
	while (r - l > 1) {
		int mid = (r + l) / 2;
		if (pp[mid].x != pp[l].x || pp[mid].y >= pp[iter1].y || (pp[mid] - pp[iter1]).len() > dist2)
			r = mid;
		else l = mid;
	}
	return l;
}

ll quad(ll tiv) {
	return tiv * tiv;
}

ll DAQ(const vector<Point> &pp, int start, int end) {
	if (end - start < 2)
		return LLONG_MAX;
	if (end - start == 2)
		return (pp[start] - pp[start + 1]).len2();
	if (pp[start].x == pp[end - 1].x)
		return another(pp, start, end);
	int mid = (start + end) / 2;
	ll dist2 = min(DAQ(pp, start, mid), DAQ(pp, mid, end)); 
	vector<Point> ro;
	for (int iter = mid - 1; iter >= start && quad(pp[iter].x - pp[mid].x) < dist2; --iter)
		ro.push_back(pp[iter]);
	for (int iter = mid; iter < end && quad(pp[iter].x - pp[mid].x) < dist2; ++iter)
		ro.push_back(pp[iter]);
	sort(ro.begin(), ro.end(), secondArg());
	for (int i = 0; i < ro.size() - 1; ++i)
		for (int j = i + 1; j < ro.size() && quad(ro[i].y - ro[j].y) < dist2; ++j)
			dist2 = min(dist2, (ro[i] - ro[j]).len2());
	return dist2;
}

void solve(vector<Point> &pp) {
	ios_base::sync_with_stdio(true);
	cout.precision(8);
	sort(pp.begin(), pp.end(), firstArg());
	ll ans = DAQ(pp, 0, pp.size());
	cout << fixed << sqrt(ans) << endl;
}

int main() {
	int n;
	vector<Point> pp;
	ll x, y;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> x >> y;
		pp.emplace_back(x, y);
	}
	solve(pp);
}

/*
10
0 30
0 40
0 50
0 60
0 70
5 0
5 10
5 20
5 30
5 40

*/
#include <vector>
#include <iostream>
#include <queue>
#include <cstring>
#include <map>
#include <algorithm>
using namespace std;

const long long INF = 10000 * 1000 * 1000 + 9;
const long long MAXN = 902;
long long n, a[MAXN][MAXN], u[MAXN], v[MAXN], way[MAXN], p[MAXN];
vector<char> used;
vector<long long> minv;


long long getPotential(int x, int y) {
	return a[x][y] - u[x] - v[y];
}

void check(long long newMinv, int ind, long long &delta, int &newJ, int &currentJ) {
	if (newMinv < minv[ind]) {
		minv[ind] = newMinv;
		way[ind] = currentJ;
	}
	if (minv[ind] < delta) {
		delta = minv[ind];
		newJ = ind;
	}
}

void KunaAlgorithm(int &currentJ) {
	used[currentJ] = 1;
	int currentI = p[currentJ], newJ;
	long long delta = INF;
	for (int j = 1; j <= n; ++j){
		if (used[j]) continue;
		long long newMinv = getPotential(currentI, j);
		if (newMinv < minv[j]) {
			minv[j] = newMinv;
			way[j] = currentJ;
		}
		if (minv[j] < delta) {
			delta = minv[j];
			newJ = j;
		}
	}
	for (int j = 0; j <= n; ++j)
		used[j] ? u[p[j]] += delta, v[j] -= delta : minv[j] -= delta;
	currentJ = newJ;
	if (!p[currentJ])
		return;
	KunaAlgorithm(currentJ);
}

void hungarianAlgorithm() {
	for (int i = 1; i <= n; ++i) {
		p[0] = i;
		int currentJ = 0;
		used = vector<char>(n + 2, 0);
		minv = vector<long long>(n + 2, INF);
		KunaAlgorithm(currentJ);
		while (true) {
			p[currentJ] = p[way[currentJ]];
			currentJ = way[currentJ];
			if (!currentJ)
				break;
		}
	}
	cout << -v[0] << endl;
	for (int i = 1; i <= n; ++i)
		cout << p[i] << " " << i << endl;
}

int main() {
	cin >> n;
	for (int i = 1; i <= n; ++i)
	for (int j = 1; j <= n; ++j)
		cin >> a[i][j];
	hungarianAlgorithm();
	return 0;
}
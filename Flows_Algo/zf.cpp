#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


vector<int> Z(string &t) {
	vector<int> ans(t.size());
	int l = 0, r = 1;
	for (int i = 1; i < t.size(); ++i) {
		if (i < r)
			ans[i] = min(r - i, ans[i - l]);
		while (i + ans[i] < t.size() && t[i + ans[i]] == t[ans[i]])
			++ans[i];
		if (r < i + ans[i]) {
			l = i;
			r = ans[i] + i;
		}
	}
	return ans;
}

int getAns(string &t, string &s, vector<int> &ww) {
	int sz = t.size();
	(t += '.') += s;
	auto zf = Z(t);
	int ans = 0;
	for (int i = sz; i < t.size(); ++i)
		if (zf[i] >= sz)
			++ans, ww.push_back(i - sz);
	return ans;
}

int main() {
	string s;
	string t;
	cin >> s >> t;
	vector<int> ans;
	cout << getAns(t, s, ans) << endl;
	for (auto it : ans)
		cout << it << " ";
	cout << endl;
	return 0;
}
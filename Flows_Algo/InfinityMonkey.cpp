#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const long long mod = 1000000;

void operator*=(vector<long long> &s, long long coef) {
    long long pahac = 0;
    size_t sz = s.size();
    for (size_t i = 0; i < sz; ++i) {
        pahac += coef * s[i];
        s[i] = pahac % mod;
        pahac /= mod;
    }
    while (pahac) {
        s.push_back(pahac % mod);
        pahac /= mod;
    }
}

void sum(const vector<long long> &s, vector<long long> &t) {
    long long pahac = 0;
    size_t sz = t.size();
    for (size_t i = 0; i < sz; ++i) {
        pahac += s[i] + t[i];
        t[i] = pahac % mod;
        pahac /= mod;
    }
    size_t st = t.size();
    for (size_t i = st; i < s.size(); ++i) {
        pahac += s[i];
        t.push_back(pahac % mod);
        pahac /= mod;
    }
    if (pahac)
        t.push_back(pahac);
}

vector<int> pi(const string &s) {
    vector<int> pi(s.size());
    for (size_t i = 1; i < s.size(); ++i) {
        pi[i] = pi[i - 1];
        while (pi[i] > 0 && s[i] != s[pi[i]])
            pi[i] = pi[pi[i] - 1];
        if (s[i] == s[pi[i]])
            ++pi[i];
    }
    return pi;
}

void poww(vector<long long> &s, int sz, long long n) {
    long long x = 1;
    for (size_t i = 0; i < 6; ++i)
        x *= n;
    int end = sz / 6;
    for (size_t i = 0; i < end; ++i)
        s *= x;
    end = sz % 6;
    if (end) {
        x = 1;
        for (size_t i = 0; i < end; ++i)
            x *= n;
        s *= x;
    }
}

void print(long long x) {
    int y = mod / 10;
    while (x < y) {
        cout << 0;
        y /= 10;
    }
    if (x)
    cout << x;
}
void getAns(size_t sz, vector<int> &p, int po) {
    vector<long long> ans(1, 0);
    vector<long long> s(1, 1);
    vector<int> pows;
    for (size_t i = sz; i > 0; i = p[i - 1])
        pows.push_back(i);
    sort(pows.begin(), pows.end());
    poww(s, pows[0], po);
    sum(s, ans);
    for (size_t i = 1; i < pows.size(); ++i) {
        poww(s, pows[i] - pows[i - 1], po);
        sum(s, ans);
    }
    reverse(ans.begin(), ans.end());
    cout << ans[0];
    for (size_t i = 1; i < ans.size(); ++i)
        print(ans[i]);
    cout << endl;

}


int main()
{
    cout << floor(4.8) << "  " << floor(4.3) << "   " << ceil(4.8) << "   " << ceil(4.3);
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n;
    cin >> n;
    string s;
    cin >> s;
    if (n == 1) {
        cout << s.size() << endl;
        return 0;
    }
    vector<int> p = pi(s);
    getAns(s.size(), p, n);
    return 0;
}
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
using namespace std;

const long double EPS = 1e-12;

bool isLess(const long double &f, const long double &s) {
    return f < s - EPS;
}

pair<int, int> operator-(const pair<int, int> &f, const pair<int, int> &s) {
    return{ f.first - s.first, f.second - s.second };
}

struct Segment {
    int id;
    pair<int, int> first, second;
    Segment(int id, const pair<int, int> &f, const pair<int, int> &s) : id(id), first(f), second(s), tg_(getTangents_(f, s)) {}

    long double operator()(long double x) const {
        if (first.first == second.first)
            return first.second;
        return first.second + tg_ * (x - first.first);
    }

    bool operator<(const Segment& other) const {
        long double x = max(min(first.first, second.first), min(other.first.first, other.second.first));
        return isLess(operator()(x), other(x));
    }

private:
    const long double tg_;
    static long double getTangents_(const pair<int, int> &f, const pair<int, int> &s) {
        return static_cast<long double>(s.second - f.second) / (s.first - f.first);
    }
};

struct Event {
    int id, x;
    enum Type {
        start,
        end
    } type;
    Event(int id, int x, Type t) : id(id), x(x), type(t) {}
    bool operator<(const Event &other) const {
        return x == other.x ? type > other.type : x > other.x;
    }
};


int sign(long long x) {
    if (x == 0)
        return 0;
    return x > 0 ? 1 : -1;
}

bool intersectHelper(int l1, int r1, int l2, int r2) {
    if (l1 > r1)  swap(l1, r1);
    if (l2 > r2)  swap(l2, r2);
    return max(l1, l2) <= min(r1, r2) + EPS;
}

bool areInOrder(const pair<int, int> &a, const pair<int, int> &b, const pair<int, int> &c) {
    return sign(a.first * b.second - a.second * b.first) * sign(b.first * c.second - b.second * c.first) != -1;
}

bool isIntersect(const Segment &f, const Segment &g) {
    return areInOrder(g.first - f.first, f.second - f.first, g.second - f.first) &&
        areInOrder(f.first - g.first, g.second - g.first, f.second - g.first) &&
        intersectHelper(f.first.first, f.second.first, g.first.first, g.second.first) &&
        intersectHelper(f.first.second, f.second.second, g.first.second, g.second.second);
}

pair<int, int> solve(const vector<Segment> &mySegs) {
    priority_queue<Event, vector<Event>> myEvents;
    set<Segment> mySet;
    vector<set<Segment>::iterator> vecOfIter(mySegs.size());
    for (int i = 0; i < mySegs.size(); ++i) {
        myEvents.emplace(i, min(mySegs[i].first.first, mySegs[i].second.first), Event::start);
        myEvents.emplace(i, max(mySegs[i].first.first, mySegs[i].second.first), Event::end);
    }
    while (!myEvents.empty()) {
        Event evn = myEvents.top();
        myEvents.pop();
        set<Segment>::iterator it, it2;
        if (evn.type == Event::start) {
            it = mySet.lower_bound(mySegs[evn.id]);
            if (it != mySet.end() && isIntersect(*it, mySegs[evn.id]))
                return{ it->id, evn.id };
            if (it != mySet.begin()) {
                it2 = it;
                --it2;
                if (isIntersect(*it2, mySegs[evn.id]))
                    return{ it2->id, evn.id };
            }
            vecOfIter[evn.id] = mySet.insert(it, mySegs[evn.id]);
        }
        else {
            it = it2 = vecOfIter[evn.id];
            ++it;
            if (it2 != mySet.begin() && it != mySet.end() && isIntersect(*(--it2), *it))
                return{ it2->id, it->id };
            mySet.erase(vecOfIter[evn.id]);
        }
    }
    return{ -1, -1 };
}

int main() {
    int n, x, y, xx, yy;
    vector<Segment> mySeg;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y >> xx >> yy;
        mySeg.emplace_back(i, make_pair(x, y), make_pair(xx, yy));
    }
    pair<int, int> ans = solve(mySeg);
    if (ans.first == -1) {
        cout << "NO" << endl;
        return 0;
    }
    cout << "YES" << endl;
    cout << ans.first + 1 << " " << ans.second + 1 << endl;
    return 0;
}
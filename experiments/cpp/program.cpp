#include <map>
#include <vector>
#include <string>

using namespace std;

int main() {
    map<string, vector<int>> mvp;

    auto make_1k = [](int start) {
        vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i) {
            v.push_back(start + i);
        }
        return v;
    };

    mvp["a"] = make_1k(1);      // 1..1000
    mvp["b"] = make_1k(1001);   // 1001..2000
    mvp["c"] = make_1k(2001);   // 2001..3000
    mvp["d"] = make_1k(3001);   // 3001..4000
    mvp["e"] = make_1k(4001);   // 4001..5000
    mvp["f"] = make_1k(5001);   // 5001..6000
    mvp["g"] = make_1k(6001);   // 6001..7000

    return 0;
}

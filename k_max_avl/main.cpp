#include <iostream>
#include "avl.h"

int main() {
    avl x;
    int n;
    std::cin >> n;
    int t, k;
    while (n--) {
        std::cin >> t >> k;
        if (t == 0)
            std::cout<<x.find(k)<<std::endl;
        else if (t == 1)
            x.push(k);
        else
            x.pop(k);
    }
    return 0;
}

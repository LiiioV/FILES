#include <iostream>
#include "DLL.h"
#include "avl.h"

void console() {
    int n, m;

    std::cin >> n >> m;
    Node **place = new Node*[m + 1];
    avl x;
    Node *myRoot = new Node(n, 0);
    x.push({ n, { 0, myRoot } });

    int t;
    for (size_t i = 1; i <= m; ++i) {
        std::cin >> t;
        if (t >= 0) {
            if (x.empty()) {
                place[i] = nullptr;
                std::cout << -1 << std::endl;
                continue;
            }
            std::pair<int, std::pair<int, Node *>> maxPlace = *x.findMax();
            if (maxPlace.first < t) {
                place[i] = nullptr;
                std::cout << -1 << std::endl;
            }
            else if (maxPlace.first > t) {
                x.pop(maxPlace);
                std::cout << 1 - maxPlace.second.first << std::endl;
                Node *R = new Node(maxPlace.second.second->size - t, maxPlace.second.second->index - t);
                place[i] = maxPlace.second.second;
                maxPlace.second.second->size = t;
                maxPlace.second.second->colour = true;
                insert(R, maxPlace.second.second);
                x.push({ R->size, { R->index, R } });
            }
            else {
                x.pop(maxPlace);
                std::cout << 1 - maxPlace.second.first << std::endl;
                place[i] = maxPlace.second.second;
                maxPlace.second.second->colour = true;
            }
        }
        else {
            t *= -1;
            if (place[t] == nullptr)
                continue;
            place[t]->colour = false;
            if (place[t]->right != nullptr) {
                if (place[t]->right->colour == false) {
                    x.pop({ place[t]->right->size, { place[t]->right->index, place[t]->right } });
                    joinRight(place[t]);
                }
            }
            if (place[t]->left != nullptr) {
                if (place[t]->left->colour == false) {
                    x.pop({ place[t]->left->size, { place[t]->left->index, place[t]->left } });
                    if (myRoot == place[t]->left)
                        myRoot = place[t];
                    joinLeft(place[t]);
                }
            }
            x.push({ place[t]->size, { place[t]->index, place[t] } });
            place[t] = nullptr;
            place[i] = nullptr;
        }

    }

    destroy(myRoot);

    delete[] place;
}

int main() {
    console();
    return 0;
}

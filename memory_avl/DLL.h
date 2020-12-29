//
// Created by ADMIN on 07.04.2020.
//

#ifndef MEMORY_AVL_DLL_H
#define MEMORY_AVL_DLL_H

struct Node {
    Node(int s, int i) : left(nullptr), right(nullptr), size(s), index(i), colour(false) {};

    Node* left;
    Node* right;
    int size;
    int index;
    bool colour;
};

void destroy(Node *);

void deleteNode(Node *);

void join(Node *);

void destroy(Node *A) {
    if (A == nullptr)
        return;
    if (A->left != nullptr) {
        A->left->right = nullptr;
        delete(A->left);
    }
    if (A->right != nullptr) {
        A->right->left = nullptr;
        delete(A->right);
    }
    delete A;
}

void deleteNode(Node *N) {
    if (N->left != nullptr)
        N->left->right = N->right;
    if (N->right != nullptr)
        N->right->left = N->left;
    delete N;
}

void joinRight(Node *N) {
    N->size += N->right->size;
    deleteNode(N->right);
}

void joinLeft(Node *N) {
    N->size += N->left->size;
    N->index = N->left->index;
    deleteNode(N->left);
}

void insert(Node *New, Node *L) {
    New->left = L;
    New->right = L->right;
    L->right = New;
    if (New->right != nullptr)
        New->right->left = New;
}

#endif //MEMORY_AVL_DLL_H

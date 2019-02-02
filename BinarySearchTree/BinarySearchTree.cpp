#include <iostream>
#include "BinarySearchTree.h"

// Binary Search Tree

using namespace BST;


int main(int argc, char* argv[]){
    
    BST::Tree<int> tree;

    tree.insert(1);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);
    tree.insert(9);    
    
    tree.walk();

    return 0;
}

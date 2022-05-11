#include<iostream>
#include "cpptree.h"


int main() {
    ftree<std::string> mytree;
    mytree.addNode(0);
    mytree.addNode(1);
    mytree.addNode(2);
    mytree.addNode(3);
    mytree.addNode(3);
    mytree.addNode(2);
    mytree.addNode(3);
    mytree.addNode(7);
    mytree.addNode(7);
    mytree.addNode(8);
    mytree.addNode(2);
    mytree.addNode(2);
    mytree.addNode(9);
    mytree.addNode(13);
    mytree.addNode(9);
    mytree.addNode(6);
    mytree.addNode(16);
    mytree.addNode(13);
    mytree.addNode(16);
    mytree.addNode(17);
    mytree.addNode(16);
    mytree.addNode(17);
    mytree.addNode(13);
    mytree.addNode(23);
    mytree.addNode(24);
    mytree.setName(25, "wew lad");
    mytree.removeNode(6);
    mytree.printTree();
}
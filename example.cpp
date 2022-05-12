#include<iostream>
#include "cpptree.h"


int main() {
    cpptree<char>* mytree;
    mytree = new cpptree<char>;
    mytree->addNode(0);
    mytree->addNode(1);
    mytree->addNode(1);
    mytree->addNode(3);
    mytree->addNode(3);
    mytree->addNode(2);
    mytree->addNode(3);
    mytree->addNode(7);
    mytree->addNode(7);
    mytree->addNode(6);
    mytree->addNode(6);
    mytree->setName(1, 'H');
    mytree->setName(2, 'e');
    mytree->setName(6, 'l');
    mytree->setName(10, 'l');
    mytree->setName(11, 'o');
    mytree->setName(3, 'W');
    mytree->setName(4, 'o');
    mytree->setName(5, 'r');
    mytree->setName(7, 'l');
    mytree->setName(8, 'd');
    mytree->removeNode(9);
    //mytree->setName(25, 'wew lad');
    //mytree->removeNode(16);
    /*for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 1000; j++) {
            mytree->addNode(i);
        }
    }*/
    //mytree->removeNode(16);
    mytree->printTree();
    delete mytree;
}

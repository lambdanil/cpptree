# cpptree
C++ tree implementation using vectors


## Usage:
```cpp
#include<iostream>
#include "cpptree.h"

int main() {
    cpptree<std::string> mytree; // Set stored data type
    mytree.addNode(0); // Add to node 0
    mytree.addNode(0);
    int nodenum = mytree.addNode(2); // Return value is the new nodes number
    mytree.addNode(nodenum); // Add a new node to the node added to 2
    mytree.setName(nodenum, "Some string here"); // Set the value of nodenum (the node added to 2) to a string (or other data type)
    int new_nodenum = mytree.addNode(nodenum);
    mytree.addNode(new_nodenum);
    mytree.removeNode(new_nodenum); // removeNode removes a Node (recursively of course!)
    // When a node is removed, all nodes with a higher number move 1 number down, to fill the number gap - there's never an unused number!
    std::cout << mytree.getName(3); // Get value at node 3 (in this case a string)
    std::vector<int> children = mytree.getChildren(1); // Print all (immediate) children of node 1
    std::vector<int> allchildren = mytree.getAllChildren(1); // Print all children of node 1 (recursively)
    int parent_of_3 = mytree.getParent(3); // Get parent of node 3

    mytree.printTree();
}
```

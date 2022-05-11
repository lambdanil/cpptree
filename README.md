# cpptree
C++ tree implementation using vectors


## Usage:
```cpp
int main() {
    ftree<string> mytree;
    mytree.addNode(0); // Add to node 0
    mytree.addNode(0);
    int nodenum = mytree.addNode(2); // Return value is the new nodes number
    mytree.addNode(nodenum); // Add a new node to the node added to 2
    mytree.setName(nodenum, "Some string here"); // Set the value of nodenum (the node added to 2) to a string (or other data type)

    mytree.printTree();
}
```

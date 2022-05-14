#include<iostream>
#include "cpptree.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;

void cls();

#ifdef _WIN32
void cls() {system("cls");}
#else
void cls() {system("clear");}
#endif

void NodeAdd ();
void NodeRemove ();
void NodeAssign ();
void NodeNodes ();
void NodeAllNodes ();

void NodeAdd(cpptree<string>* my_tree) {
    cout << "\nWhich node do you want to add to?\n";
    int select;
    int newNode;
    cin >> select;
    if (my_tree->inRange(select)) {
        newNode = my_tree->addNode(select);
        cout << "\nNew node '" << newNode << "' added to node '" << select << "'.\n";
    }
    else {
        cout << "\nNode '" << select << "' doesn't exist.\n";
    }
    getchar();
    cin.ignore();
}

void NodeRemove(cpptree<string>* my_tree) {
    cout << "\nWhich node do you want to remove?\n";
    int select;
    cin >> select;
    if (my_tree->inRange(select) && select) {
        my_tree->removeNode(select);
        cout << "\nNode '" << select << "' removed.\n";
    }
    else if (select) {
        cout << "\nNode '" << select << "' doesn't exist.\n";
    }
    else {
        cout << "\nNode '0' can't be removed.\n";
    }
    getchar();
    cin.ignore();
}

void NodeAssign(cpptree<string>* my_tree) {
    cout << "\nWhich node do you want to set value of?\n";
    int select;
    cin >> select;
    if (my_tree->inRange(select)) {
        cout << "\nWhat value do you want to set? (enter any string)\n";
        while (getchar() != '\n');
        string myVal;
        getline(cin, myVal);
        my_tree->setName(select, myVal);
        cout << "\nValue assigned to node.\n";
    }
    else {
        cout << "\nNode '" << select << "' doesn't exist.\n";
    }
    cin.ignore();
}

void NodeNodes(cpptree<string>* my_tree) {
    cout << "\nWhich node do you want list?\n";
    int select;
    cin >> select;
    if (my_tree->inRange(select)) {
        vector<int> allNodes;
        allNodes = my_tree->getChildren(select);
        cout << "\nNodes under '" << select << "' are";
        for (int i = 0; i < allNodes.size(); i++) {
            cout << " " << allNodes.at(i);
        }
        cout << "\n";
    }
    else {
        cout << "\nNode '" << select << "' doesn't exist.\n";
    }
    getchar();
    cin.ignore();
}

void NodeAllNodes(cpptree<string>* my_tree) {
    cout << "\nWhich node do you want list?\n";
    int select;
    cin >> select;
    if (my_tree->inRange(select)) {
        vector<int> allNodes;
        allNodes = my_tree->getAllChildren(select);
        cout << "\nAll nodes under '" << select << "' are";
        for (int i = 0; i < allNodes.size(); i++) {
            cout << " " << allNodes.at(i);
        }
        cout << "\n";
    }
    else {
        cout << "\nNode '" << select << "' doesn't exist.\n";
    }
    getchar();
    cin.ignore();
}

int main() {
    cpptree<string> mytree;
    int cnode;
    mytree.setName(0, "This is a tree of strings.");
    cnode = mytree.addNode(0);
    mytree.setName(1, "It can contain any number of strings.");
    mytree.addNode(0);
    mytree.addNode(0);
    cnode = mytree.addNode(1);
    mytree.setName(cnode, "And any number of branches.");
    mytree.addNode(1);
    mytree.addNode(2);
    char select;
    while (true) {
        cls();
        cout << std::endl;
        mytree.printTree();
        cout << "\nSelect an option:\n  1. Add a new node\n  2. Remove a node\n  3. Assign a value to node\n";
        cout << "  4. Print all nodes under selected node\n  5. Print all nodes under selected node (recursively)\n\n";
        select = getchar();
        switch (select) {
            case '1': 
                NodeAdd(&mytree);
                break;
            case '2':
                NodeRemove(&mytree);
                break;
            case '3':
                NodeAssign(&mytree);
                break;
            case '4':
                NodeNodes(&mytree);
                break;
            case '5':
                NodeAllNodes(&mytree);
                break;
            default:
                cout << "\nNot a valid choice!\n";
                cin.ignore();
                break;
        }
    }
}

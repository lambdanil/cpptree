#include<iostream>
#include "cpptree.h"
#include<vector>
#include<fstream>

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


void append_tree_to_file(vector_tree<string> v, string filename){
	std::ofstream file;
	file.open(filename,std::ios_base::app);
    vector<int> relations = v.exportVector();
    vector<string> names = v.exportNames();
	for(unsigned long int i=0;i<relations.size();++i){
		file<<relations.at(i)<<'^';
	}
    file << std::endl;
	for(unsigned long int i=0;i<names.size();++i){
		file<<names.at(i)<<'^';
	}
	file.close();
}


void NodeAdd(vector_tree<string>* my_tree) {
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

void NodeRemove(vector_tree<string>* my_tree) {
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

void NodeAssign(vector_tree<string>* my_tree) {
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

void NodeNodes(vector_tree<string>* my_tree) {
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

void NodeAllNodes(vector_tree<string>* my_tree) {
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
    vector_tree<string> mytree;
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
            case 'q':
                append_tree_to_file(mytree, "./encounters");
            default:
                cout << "\nNot a valid choice!\n";
                cin.ignore();
                break;
        }
    }
}

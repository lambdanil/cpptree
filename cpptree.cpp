#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::string;
using std::cout;

class ftree {
    public:
        ftree () { _relations.push_back(-1);
                   _depths.push_back(0);  }

        int addTo (unsigned int parent) {
            _relations.push_back(parent);
            _depths.push_back(_depths.at(parent)+1);
            return 0;
        }

        vector<int> getChildren (int parent) {
            vector<int> found;
            found = getOccurences(parent);
            return found;
        }

        vector<int> getAllChildren (int parent) {
            vector<int> found;
            found = getOccurences(parent);
            int i  = 0;
            while (i < found.size()) {
                vector<int> nfound;
                nfound = getOccurences(found.at(i));
                found.insert(found.end(), nfound.begin(), nfound.end());
                ++i;
            }
            return found;
        }

       int printTree() {
            int depth = 0;
            int counter = -1;
            vector<int> indexes;
            indexes.push_back(0);
            while (true) {
                vector<int> found = getOccurences(counter);
                if (found.size() > indexes.at(_depths[counter])) {
                    counter = found.at(indexes.at(_depths[counter]));
                    for (int i = 0; i < _depths[counter]; i++) cout << "--";
                    cout << counter << "\n";
                    if (indexes.size() > _depths[counter]+1) {
                        indexes.at(_depths[counter]+1) = 0;
                        for (int i = _depths[counter]; i < indexes.size(); i++) {
                            indexes.at(i) = 0;
                        }
                    }
                    else
                        indexes.push_back(0);
                }
                else {
                    counter = _relations[counter];
                    indexes.at(_depths[counter]) += 1;
                }
                if (counter == -1) break;
            }
            return 0;
        }

    private:
        vector<int> _relations;
        vector<string> _names;
        vector<int> _depths;


        vector<int> getOccurences(unsigned int parent) {
            vector<int> _found;
            for (int i = 0; i < _relations.size(); i++) {
                if (_relations.at(i) == parent) 
                    _found.push_back(i);
            }
            return _found;
        }

};

int main() {
    ftree mytree;
    mytree.addTo(0);
    mytree.addTo(0);
    mytree.addTo(2);
    mytree.addTo(3);
    mytree.addTo(3);
    mytree.addTo(5);
    mytree.addTo(5);
    mytree.addTo(2);
    mytree.addTo(4);
    mytree.addTo(3);
    
    
    vector<int> children;
    children = mytree.getAllChildren(-1);
    mytree.printTree();
    /*for (int i = 0; i < children.size(); i++) {
        cout << children.at(i);
    }*/
}
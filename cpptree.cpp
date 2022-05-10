#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::string;
using std::cout;

class ftree {
    public:
        ftree () { _relations.push_back(-1);
                   _depths.push_back(0); 
                   _names.push_back(""); }

        int addTo (unsigned int parent) {
            _relations.push_back(parent);
            _depths.push_back(_depths.at(parent)+1);
            _names.push_back("");
            return _relations.size();
        }

        vector<int> getChildren (int parent) {
            vector<int> found;
            found = _getOccurences(parent);
            return found;
        }

        vector<int> getAllChildren (int parent) {
            vector<int> found;
            found = _getOccurences(parent);
            int i  = 0;
            while (i < found.size()) {
                vector<int> nfound;
                nfound = _getOccurences(found.at(i));
                found.insert(found.end(), nfound.begin(), nfound.end());
                ++i;
            }
            return found;
        }

        void setName (unsigned int node, string desc) {
            _names.at(node) = desc;
        }

       int printTree() { // this function is quite something
            int depth = 0;
            int counter = -1;
            vector<int> sorted;
            int maxdepth = INT32_MIN;
            for (int i = 0; i < _depths.size(); i++) if (_depths[i] > maxdepth) maxdepth = _depths[i];
            vector<int> indexes;
            indexes.push_back(0);
            while (true) {
                vector<int> found = _getOccurences(counter);
                if (found.size() > indexes.at(_depths[counter])) {
                    counter = found.at(indexes.at(_depths[counter]));
                    //for (int i = 0; i < _depths[counter]; i++) {cout << " "}
                    sorted.push_back(counter);
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
            char matrix[maxdepth][sorted.size()];
            for (int i = 0; i < sorted.size(); i++) {
                for (int j = 0; j < maxdepth; j++) {
                    if (j < _depths[sorted[i]])
                        matrix[i][j] = '.';
                    else
                        matrix[i][j] = ' ';
                }
            }
            for (int i = 0; i < sorted.size(); i++) {
                for (int j = 0; j < maxdepth; j++) {
                    if (matrix[i][j] == '.' && matrix[i][j+1] == ' ') matrix[i][j] = 'x';
                    if (matrix[i][j] == '.' && j == maxdepth-1) matrix[i][j] = 'x';
                }
            }
            int exs = 0;
            int nypos = 0;
            for (int i = 0; i < sorted.size(); i++) {
                for (int j = 0; j < maxdepth; j++) {
                    if (matrix[i][j] == 'x') {
                        for (int k = i+1; k < sorted.size(); k++) {
                            if (matrix[k][j] == 'x') exs++;
                        }
                    }
                    nypos = i+1;
                    while (exs != 0) {
                        if (matrix[nypos][j] == 'x') --exs;
                        else matrix[nypos][j] = 'x';
                        nypos++;
                    }
                }
            }
            for (int i = 0; i < sorted.size(); i++) {
                for (int j = 0; j < maxdepth; j++) {
                    if (matrix[i][j] == 'x' ) {
                        if (j < maxdepth-1)
                            if (matrix[i][j+1] == 'x') matrix[i][j] = '|';
                        if (matrix[i][j+1] != 'x' && matrix[i+1][j] == 'x') matrix[i][j] = '+';
                        if (matrix[i][j+1] != 'x' && matrix[i+1][j] != 'x') matrix[i][j] = 'L';
                    }
                    else if (matrix[i][j] == '.') matrix[i][j] = '.';
                }
            }
            for (int i = 0; i < sorted.size(); i++) {
                for (int j = 0; j < maxdepth; j++) {
                    if (matrix[i][j] == '+') cout << "├── ";
                    else if (matrix[i][j] == 'L') cout << "└── ";
                    else if (matrix[i][j] == '|') cout << "│   ";
                    else if (matrix[i][j] == '.') cout << "    ";
                }
                cout << sorted.at(i) << " - " << _names.at(i);
                cout << "\n";
            }
            return 0;
        }

    private:
        vector<int> _relations;
        vector<string> _names;
        vector<int> _depths;


        vector<int> _getOccurences(unsigned int parent) {
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
    mytree.setName(0,"root");
    mytree.setName(1,"base image");
    mytree.setName(2,"multiuser system");
    mytree.setName(3,"applications");
    mytree.setName(4,"MATE full desktop");
    mytree.setName(5,"Plasma full desktop");
    
    mytree.printTree();
}
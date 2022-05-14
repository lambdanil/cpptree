#ifndef CPPTREE_H_
#define CPPTREE_H_
#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::string;
using std::cout;

template <class T>
class vtree {
    public:
        vtree () { _relations.push_back(-1);
                   _names.push_back(T()); }

        int addNode (int parent) {
            _relations.push_back(parent);
            _names.push_back(T());
            return _relations.size()-1;
        }

        int getParent(int node) {
            return _relations.at(node);
        }

        void setParent(int node, int parent) {
            _relations.at(node) = parent;
        }

        vector<int> getChildren (int parent) {
            vector<int> found;
            found = _getOccurences(parent);
            return found;
        }

        bool inRange(int val) {
            if (val >= 0 && val < _relations.size()) return true;
            else return false;
        }

        T getName(int node) {
            return _names.at(node);
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

        int getSize() {
            return _relations.size();
        }

        void setName (int node, T desc) {
            _names.at(node) = desc;
        }

        void removeNode(int node) { // Removing nodes in this vector implementation is slow
            vector<int> found = getAllChildren(node);
            while (found.size() != 0) {
                _rLastNode(found.at(found.size()-1));
                found.erase(found.begin() + (found.size()-1));
                for (int i = 0; i < found.size(); i++) {
                    if (found.at(i) > found.at(found.size()-1)) {
                        found.at(i)--;
                    }
                }
            }
            _rLastNode(node);
        }

        void printTree() { // This implementation of printing the tree is a bit dumb... but it's fast enough so WONTFIX
            int maxdepth = 0;
            for (int i = 0; i < _relations.size(); i++) if (_getDepth(i) > maxdepth) maxdepth = _getDepth(i);
            vector<int> sorted = _sortedCrawl();
            
            char** matrix = new char*[sorted.size()+1];
            for(int i = 0; i < sorted.size()+1; ++i)
                matrix[i] = new char[maxdepth+1];

            for (int i = 0; i < sorted.size(); i++) {
                for (int j = 0; j < maxdepth; j++) {
                    if (j < _getDepth(sorted[i]))
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
                            if (matrix[k][j] != '.' && matrix[k][j] != 'x') break;
                            if (matrix[k][j] == 'x') exs++;
                        }
                    }
                    nypos = i+1;
                    while (exs != 0) {
                        if (matrix[nypos][j] == '.') matrix[nypos][j] = 'x';
                        else if (matrix[nypos][j] == 'x') --exs;
                        nypos++;
                    }
                }
            }
            for (int i = 0; i < sorted.size(); i++) {
                for (int j = 0; j < maxdepth; j++) {
                    if (matrix[i][j] == 'x' ) {
                        if (matrix[i][j+1] != 'x' && matrix[i+1][j] == 'x') matrix[i][j] = '+';
                        if (matrix[i][j+1] != 'x' && matrix[i+1][j] != 'x') matrix[i][j] = 'L';
                        if (j != maxdepth-1)
                            if ((matrix[i][j+1] == 'x' || matrix[i][j+1] == '.') && matrix[i+1][j] != ' ') matrix[i][j] = '|';
                    }
                }
            }
            cout << "0 - " << _names.at(0) << "\n";
            for (int i = 0; i < sorted.size(); i++) {
                for (int j = 0; j < maxdepth; j++) {
                    if (matrix[i][j] == '+') cout << "├── ";
                    else if (matrix[i][j] == 'L') cout << "└── ";
                    else if (matrix[i][j] == '|') cout << "│   ";
                    else if (matrix[i][j] == '.') cout << "    ";
                }
                cout << sorted.at(i) << " - " << _names.at(sorted.at(i));
                cout << "\n";
            }
            for(int i = 0; i < sorted.size()+1; ++i)
                delete[] matrix[i];
            delete[] matrix;
        }

    private:
        vector<int> _relations;
        vector<T> _names;

        vector<int> _getOccurences(int parent) {
            vector<int> _found;
            for (int i = 0; i < _relations.size(); i++) {
                if (_relations.at(i) == parent) 
                    _found.push_back(i);
            }
            return _found;
        }

        void _rLastNode(int node) {
            _relations.erase(_relations.begin() + node);
            _names.erase(_names.begin() + node);
            for (int i = 1; i < _relations.size(); i++) {
                if (_relations.at(i) > node) _relations.at(i)--;
            }
        }

        int _getDepth(int node) {
            int parent = getParent(node);
            int depth = 0;
            while (parent != -1) {
                parent = getParent(parent);
                depth++;
            }
            return depth;
        }

        vector<int> _sortedCrawl() { // This function doesn't scale well, only a problem when thousands of nodes are reached.
            int counter = 0;
            vector<int> sorted;
            vector<int> indexes;
            indexes.push_back(0);
            while (true) {
                vector<int> found = _getOccurences(counter);
                if (found.size() > indexes.at(_getDepth(counter))) {
                    counter = found.at(indexes.at(_getDepth(counter)));
                    sorted.push_back(counter);
                    if (indexes.size() > _getDepth(counter)+1) {
                        indexes.at(_getDepth(counter)+1) = 0;
                        for (int i = _getDepth(counter); i < indexes.size(); i++) {
                            indexes.at(i) = 0;
                        }
                    }
                    else
                        indexes.push_back(0);
                }
                else {
                    counter = _relations[counter];
                    if (counter == -1) break;
                    indexes.at(_getDepth(counter)) += 1;
                }
            }
            return sorted;
        }

};
#endif

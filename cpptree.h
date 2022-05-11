#ifndef _cpptree_h_
#define _cpptree_h_
#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::string;
using std::cout;

template <class T>
class cpptree {
    public:
        cpptree () { _relations.push_back(-1);
                   _depths.push_back(0);
                   _names.push_back(T()); }


        int addNode (unsigned int parent) {
            _relations.push_back(parent);
            _depths.push_back(_depths.at(parent)+1);
            _names.push_back(T());
            return _relations.size()-1;
        }

        int getParent(int node) {
            return _relations.at(node);
        }

        vector<int> getChildren (int parent) {
            vector<int> found;
            found = _getOccurences(parent);
            return found;
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

        void setName (unsigned int node, T desc) {
            _names.at(node) = desc;
        }

        void removeNode(unsigned int node) { // Removing nodes in this vector implementation is _very_ slow
            vector<int> found = getAllChildren(node);
            while (found.size() != 0) {
                _rLastNode(found.at(found.size()-1));
                found = getAllChildren(node);
            }
            _rLastNode(node);
        }

        void printTree() { // This implementation of printing the tree is a bit dumb... but it's fast enough so WONTFIX
            unsigned int maxdepth = 0;
            for (int i = 0; i < _depths.size(); i++) if (_depths[i] > maxdepth) maxdepth = _depths[i];
            vector<int> sorted = _sortedCrawl();
            
            char** matrix = new char*[sorted.size()+1];
            for(int i = 0; i < sorted.size()+1; ++i)
                matrix[i] = new char[maxdepth+1];

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
        vector<T> _names; // Time complexity of accesing a specific node is constant
        vector<int> _depths; // Allows skipping depth calculation when printing, this is pretty much a waste of memory actually and I should fix it later


        vector<int> _getOccurences(unsigned int parent) {
            vector<int> _found;
            for (int i = 0; i < _relations.size(); i++) {
                if (_relations.at(i) == parent) 
                    _found.push_back(i);
            }
            return _found;
        }

        void _rLastNode(unsigned int node) {
            _relations.erase(_relations.begin() + node);
            _depths.erase(_depths.begin() + node);
            _names.erase(_names.begin() + node);
            for (int i = 1; i < _relations.size(); i++) {
                if (_relations.at(i) > node) _relations.at(i)--;
            }
        }

        vector<int> _sortedCrawl() { // This function doesn't scale well, only a problem when thousands of nodes are reached.
            int counter = 0;
            vector<int> sorted;
            vector<int> indexes;
            indexes.push_back(0);
            while (true) {
                vector<int> found = _getOccurences(counter);
                if (found.size() > indexes.at(_depths[counter])) {
                    counter = found.at(indexes.at(_depths[counter]));
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
                    if (counter == -1) break;
                    indexes.at(_depths[counter]) += 1;
                }
            }
            return sorted;
        }

};
#endif

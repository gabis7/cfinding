/* 
 * File:   global.h
 * Author: gabis
 *
 * Created on 2 août 2016, 14:49
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <algorithm>
#include <list>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;


vector<vector <int> > getAdjacencyList(vector< vector <bool> > adjacencyMatrix) {
    vector<vector <int> > LR;

    for (int i = 0; i < adjacencyMatrix.size(); i++) {
        vector < int > vx;
        for (int j = 0; j < adjacencyMatrix[i].size(); j++) {
            if (adjacencyMatrix[i][j]) {
                vx.push_back(j);
            }
        } 
        LR.push_back(vx);
    }
    return LR;
}

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}



#endif /* GLOBAL_H */


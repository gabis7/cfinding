/* 
 * File:   StrongConnectedComponents.h
 * Author: gabis
 *
 * Created on 2 août 2016, 15:03
 */

#ifndef SCCRESULT_H
#define SCCRESULT_H

#include <unordered_set>

using namespace std;

class SCCResult {
public:

    SCCResult() {
    }

    SCCResult(const SCCResult& orig) {
    }

    virtual ~SCCResult() {
    }

    SCCResult(vector<vector<int> > adjL, int lId) {
        adjList = adjL;
        lowestNodeId = lId;
        if (adjList.size() > 0) {
            for (int i = lowestNodeId; i < adjList.size(); i++) {
                if (adjList[i].size() > 0) {
                    nodeIDsOfSCC.insert(i);
                }
            }
        }
    }

    vector<vector<int> >  getAdjList() {
        return adjList;
    }

    int getLowestNodeId() {
        return lowestNodeId;
    }
private:
    std::unordered_set<int> nodeIDsOfSCC;
    vector<vector<int> > adjList;
    int lowestNodeId = -1;

};
#endif // SCCRESULT_H


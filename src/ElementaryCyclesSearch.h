/* 
 * File:   ElementaryCyclesSearch.h
 * Author: gabis
 *
 * Created on 2 août 2016, 14:35
 */

#ifndef ELEMENTARYCYCLESSEARCH_H
#define ELEMENTARYCYCLESSEARCH_H

#include "StrongConnectedComponents.h"

class ElementaryCyclesSearch {
public:

    ElementaryCyclesSearch() {
    }

    ElementaryCyclesSearch(const ElementaryCyclesSearch& orig) {
    }

    virtual ~ElementaryCyclesSearch() {
    }

    /**
     * Constructor.
     *
     * @param matrix adjacency-matrix of the graph
     * @param graphNodes array of the graphnodes of the graph; this is used to
     * build sets of the elementary cycles containing the objects of the
     * original graph-representation
     */
    ElementaryCyclesSearch(vector<vector<bool> > matrix, vector<string> gNodes) {
        graphNodes = gNodes;
        adjList = getAdjacencyList(matrix);
    }

    /**
     * Returns Vector::Vector::Object with the Vectors of nodes of all elementary
     * cycles in the graph.
     *
     * @return Vector::Vector::Object with the Vectors of the elementary cycles.
     */
    vector<vector<string> > getElementaryCycles() {
        blocked.clear();
        B.clear();
        if(blocked.size()==0) blocked.resize(adjList.size());
        if(B.size()==0)B.resize(adjList.size());
        StrongConnectedComponents *sccs = new StrongConnectedComponents(adjList);
        int s = 0;

        while (true) {
            SCCResult *sccResult = sccs->getAdjacencyList(s);
            if (sccResult != NULL && sccResult->getAdjList().size() > 0) {
                vector<vector<int> > scc = sccResult->getAdjList();
                s = sccResult->getLowestNodeId();
                for (int j = 0; j < scc.size(); j++) {
                    if (scc[j].size() > 0) {
                        blocked[j] = false;
                        B[j].clear();
                    }
                }

                findCycles(s, s, scc);
                s++;
            } else {
                break;
            }
        }

        return cycles;
    }

private:

    /**
     * Vector of cycles
     */
    vector<vector<string> > cycles;

    /**
     * Adjacency-vector of graph
     */
    vector<vector <int> > adjList;

    /**
     * Graphnodes
     */
    vector<string> graphNodes;

    /**
     * Blocked nodes, used by the algorithm of Johnson
     */
    vector<bool> blocked;

    /**
     * B-Vectors, used by the algorithm of Johnson
     */
    vector<vector<int> > B;

    /**
     * Stack for nodes, used by the algorithm of Johnson
     */
    vector<int> stack;

    /**
     * Calculates the cycles containing a given node in a strongly connected
     * component. The method calls itself recursivly.
     *
     * @param v
     * @param s
     * @param adjList adjacency-list with the subgraph of the strongly connected
     * component s is part of.
     * @return true, if cycle found; false otherwise
     */
    bool findCycles(int v, int s, vector<vector <int> > adjList) {
        bool f = false;
        stack.push_back(v);
        blocked[v] = true;

        if(v == 13 || v == 9)
            v = v+1-1;
        
        for (int i = 0; i < adjList[v].size(); i++) {
            int w = adjList[v][i];
            // found cycle
            if (w == s) {
                vector<string> cycle;
                for (int j = 0; j < stack.size(); j++) {
                    int index = (stack[j]);
                    cycle.push_back(graphNodes[index]);
                }
                cycles.push_back(cycle);
                f = true;
            } else if (!blocked[w]) {
                if (findCycles(w, s, adjList)) {
                    f = true;
                }
            }
        }

        if (f) {
            unblock(v);
        } else {
            for (int i = 0; i < adjList[v].size(); i++) {
                int w = (adjList[v][i]);
                if (std::find(B[w].begin(), B[w].end(), v) == B[w].end()) {
                    B[w].push_back(v);
                }

            }
        }

        stack.erase(std::remove(stack.begin(), stack.end(), v), stack.end());
        return f;
    }

    /**
     * Unblocks recursivly all blocked nodes, starting with a given node.
     *
     * @param node node to unblock
     */
    void unblock(int node) {
        blocked[node] = false;
        //vector<int> Bnode = B[node];
        
        while (B[node].size() > 0) {
            int w = B[node][0];
            B[node].erase(B[node].begin() + 0);
            if (blocked[w]) {
                unblock(w);
            }
        }
    }

};

#endif /* ELEMENTARYCYCLESSEARCH_H */


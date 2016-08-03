/* 
 * File:   StrongConnectedComponents.h
 * Author: gabis
 *
 * Created on 2 août 2016, 15:03
 */

#ifndef STRONGCONNECTEDCOMPONENTS_H
#define STRONGCONNECTEDCOMPONENTS_H

#include <math.h>

class StrongConnectedComponents {
public:

    StrongConnectedComponents() {
    }

    StrongConnectedComponents(const StrongConnectedComponents& orig) {
    }

    virtual ~StrongConnectedComponents() {
    }

    /**
     * Constructor.
     *
     * @param adjList adjacency-list of the graph
     */
    StrongConnectedComponents(vector<vector<int> > adjList) {
        adjListOriginal = adjList;
    }

    /**
     * This method returns the adjacency-structure of the strong connected
     * component with the least vertex in a subgraph of the original graph
     * induced by the nodes {s, s + 1, ..., n}, where s is a given node. Note
     * that trivial strong connected components with just one node will not be
     * returned.
     *
     * @param node node s
     * @return SCCResult with adjacency-structure of the strong connected
     * component; null, if no such component exists
     */
    SCCResult* getAdjacencyList(int node) {
        visited.clear();
        lowlink.clear();
        number.clear();
        stack.clear();
        currentSCCs.clear();
        if(visited.size()==0)visited.resize(adjListOriginal.size());
        if(lowlink.size()==0)lowlink.resize(adjListOriginal.size());
        if(number.size()==0)number.resize(adjListOriginal.size());

        makeAdjListSubgraph(node);

        for (int i = node; i < adjListOriginal.size(); i++) {
            if (!visited[i]) {
                getStrongConnectedComponents(i);
                vector<int> nodes = getLowestIdComponent();
                if (nodes.size() > 0 && std::find(nodes.begin(), nodes.end(), node) == nodes.end() &&
                        std::find(nodes.begin(), nodes.end(), node + 1) == nodes.end()) {
                    return getAdjacencyList(node + 1);
                } else {
                    vector<vector<int> > adjacencyList = getAdjList(nodes);
                    if (adjacencyList.size() > 0) {
                        for (int j = 0; j < adjListOriginal.size(); j++) {
                            if (adjacencyList[j].size() > 0) {
                                return new SCCResult(adjacencyList, j);
                            }
                        }
                    }
                }
            }
        }
        return NULL;
    }


private:

    /**
     * Builds the adjacency-list for a subgraph containing just nodes >= a given
     * index.
     *
     * @param node Node with lowest index in the subgraph
     */
    void makeAdjListSubgraph(int node) {
        adjList.clear();
        if(adjList.size()==0)adjList.resize(adjListOriginal.size());

        for (int i = node; i < adjList.size(); i++) {
            vector<int> successors;
            for (int j = 0; j < adjListOriginal[i].size(); j++) {
                if (adjListOriginal[i][j] >= node) {
                    successors.push_back(adjListOriginal[i][j]);
                }
            }
            if (successors.size() > 0) {
                adjList[i].clear();
                if(adjList[i].size()==0) adjList[i].resize(successors.size());
                for (int j = 0; j < successors.size(); j++) {
                    int succ = successors[j];
                    adjList[i][j] = succ;
                }
            }
        }
    }

    /**
     * Calculates the strong connected component out of a set of scc's, that
     * contains the node with the lowest index.
     *
     * @return Vector::Integer of the scc containing the lowest nodenumber
     */
    vector<int> getLowestIdComponent() {
        int min = adjList.size();
        vector<int> currScc;

        for (int i = 0; i < currentSCCs.size(); i++) {
            vector<int> scc = currentSCCs[i];
            for (int j = 0; j < scc.size(); j++) {
                int node = scc[j];
                if (node < min) {
                    currScc = scc;
                    min = node;
                }
            }
        }

        return currScc;
    }

    /**
     * @return Vector[]::Integer representing the adjacency-structure of the
     * strong connected component with least vertex in the currently viewed
     * subgraph
     */
    vector<vector<int> > getAdjList(vector<int> nodes) {
        vector<vector<int> > lowestIdAdjacencyList;

        if (nodes.size() > 0) {
            lowestIdAdjacencyList.clear();
            if(lowestIdAdjacencyList.size()==0)lowestIdAdjacencyList.resize(adjList.size());
            //            for (int i = 0; i < lowestIdAdjacencyList..size(); i++) {
            //                lowestIdAdjacencyList[i] = new Vector();

            for (int i = 0; i < nodes.size(); i++) {
                int node = nodes[i];
                for (int j = 0; j < adjList[node].size(); j++) {
                    int succ = adjList[node][j];
                    if (std::find(nodes.begin(), nodes.end(), succ) != nodes.end()) {
                        lowestIdAdjacencyList[node].push_back(succ);
                    }
                }
            }
        }

        return lowestIdAdjacencyList;
    }

    /**
     * Searchs for strong connected components reachable from a given node.
     *
     * @param root node to start from.
     */
    void getStrongConnectedComponents(int root) {
        sccCounter++;
        lowlink[root] = sccCounter;
        number[root] = sccCounter;
        visited[root] = true;
        stack.push_back(root);

        for (int i = 0; i < adjList[root].size(); i++) {
            int w = adjList[root][i];
            if (!visited[w]) {
                getStrongConnectedComponents(w);
                lowlink[root] = min(lowlink[root], lowlink[w]);
            } else if (number[w] < number[root]) {
                if (std::find(stack.begin(), stack.end(), w) != stack.end()) {
                    lowlink[root] = min(lowlink[root], number[w]);
                }
            }
        }

        // found scc
        if ((lowlink[root] == number[root]) && (stack.size() > 0)) {
            int next = -1;
            vector<int> scc;

            do {
                next = stack[stack.size() - 1];
                stack.erase(stack.begin()+ stack.size() - 1);
                scc.push_back(next);
            } while (number[next] > number[root]);

            // simple scc's with just one node will not be added
            if (scc.size() > 1) {
                currentSCCs.push_back(scc);
            }
        }
    }

    /**
     * Adjacency-vector of original graph
     */
    vector<vector<int> > adjListOriginal;

    /**
     * Adjacency-list of currently viewed subgraph
     */
    vector<vector<int> > adjList;

    /**
     * Helpattribute for finding scc's
     */
    vector<bool> visited;

    /**
     * Helpattribute for finding scc's
     */
    vector<int> stack;

    /**
     * Helpattribute for finding scc's
     */
    vector<int> lowlink;

    /**
     * Helpattribute for finding scc's
     */
    vector<int> number;

    /**
     * Helpattribute for finding scc's
     */
    int sccCounter = 0;

    /**
     * Helpattribute for finding scc's
     */
    vector<vector<int> > currentSCCs;

};

#endif /* STRONGCONNECTEDCOMPONENTS_H */


#include "graph.h"

// error message
void error(string msg){
    cerr << msg << endl;
}

// DFS
VertexList dfs(const Graph& graph, Vertex startVertex){
    VertexList result;
    vector<bool> visited(graph.numVertices, false);

    visited[startVertex] = true;

    std::stack<Vertex> st;
    st.push(startVertex);

    while(!st.empty()){
        Vertex v = st.top();
        st.pop();

        result.push_back(v);

        for(auto w: graph.edges_from(v)){
            if (!visited[w]){
                visited[w] = true;
                st.push(w);
            }
        }
    }

    return result;
}

// BFS
VertexList bfs(const Graph& graph, Vertex startVertex){
    queue<Vertex> q;
    VertexList result;
    vector<bool> visited(graph.numVertices, false);
    
    q.push(startVertex);
    result.push_back(startVertex);
    visited[startVertex] = true;

    while(!q.empty()){
        Vertex v = q.front();
        q.pop();

        for (auto w: graph.edges_from(v)){
            if (!visited[w]){
                q.push(w);
                result.push_back(w);
                visited[w] = true;
            }
        }
    }
    
    return result;
}

// sort methods
Graph Graph::sort_edges() const{
    // copy the original
    Graph sorted = *this;
    sorted.numVertices = numVertices;

    // sort the graph
    sort(sorted.begin(), sorted.end(),
        [] (const Edge& a, const Edge& b){
            return a.weight < b.weight;
        });

    return sorted;
}

// Get the edges from the taget vertex
VertexList Graph::edges_from(Vertex vertex) const{
    VertexList result;
    for (const auto& e: *this){
        if (e.u == vertex){
            result.push_back(e.v);
        }
    }
    return result;
}

// Constructor of DisjointSet
DisjointSet::DisjointSet(int numVertices){
    subsets.resize(numVertices);
    for(int vertex = 0; vertex < numVertices; ++vertex){
        makeSet(vertex);
    }
}

// makeSet method
void DisjointSet::makeSet(Vertex x){
    subsets[x].parent = x;
}

// findSet method
Vertex DisjointSet::findSet(Vertex x){
    if (subsets[x].parent == x){
        return x;
    }
    else{
        return findSet(subsets[x].parent);
    }
}

// unionSets method
void DisjointSet::unionSets(Vertex x, Vertex y){
    Vertex u = findSet(x);  // find the parent vertex of x
    Vertex v = findSet(y);  // find the parent vertex of y

    if (u != v){
        // if parents of x and y are not equal
        // then two sets are disjoint, so union two sets
        subsets[u].parent = subsets[v].parent;
    }
}

// Kruskals algorithm
EdgeList Kruskals(const Graph& G){
    Graph sorted = G.sort_edges();
    EdgeList MST;   // minimum spanning tree
    DisjointSet ds(G.numVertices);  // used for compare two vertex
    int count = 0;  // the total edges in MST

    for (size_t i=0; i < sorted.size(); ++i){
        Edge e = sorted[i]; 
        // adjust two vertex are disjoint
        // if disjoint, then add to MST and make them union
        if (ds.findSet(e.u) != ds.findSet(e.v)){
            MST.push_back(e);
            ds.unionSets(e.u, e.v);
            count++;
        }
        // satisify the MST requirement
        if (count == G.numVertices-1){
            return MST;
        }
    }
    return MST;
}

// calculate the weight
int sum_weights(EdgeList const& L){
    int sum = 0;
    for (const auto & e: L){
        sum += e.weight;
    }
    return sum;
}

void file_to_graph(string filename, Graph & G){
    ifstream in(filename);
    if (!in){
        error("no such file");
        return;
    }

    in >> G;
}

string get_arg(int argc, char *argv[], string def){
    if (argc > 1){
        return argv[1];
    }
    return def;
}
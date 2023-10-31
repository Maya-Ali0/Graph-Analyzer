#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <stack>
#include <cmath>
using namespace std;

struct node
{
    vector<string> adj; // cost of edge, destination vertex
    string name;
    node() {}
    node(string s) : name(s) {}
};

class graph
{
public:
    typedef pair<string, string> Pss;
    typedef map<string, node *> vmap;
    typedef map<Pss, double> emap;
    stack <string> TPS;
    unordered_map<string, int> Distance_Array_As_A_Map;
    unordered_map<string, string> Parent_Array_for_BFSP_Map;
    unordered_map<string, bool> inprogress;
    unordered_map<string, bool> finished;
    unordered_map<string, string> parent;
    bool cycle;
    emap edges;
    vmap vertices;
    graph() {}
    void insertNode(const string &name)
    {
        bool found = false;
        vmap::iterator itr = vertices.find(name);
        if (itr == vertices.end())
        {
            node *v;
            v = new node(name);
            vertices[name] = v;
            return;
        }
        cout << "There is already such a node!" << endl;
        ;
    }
    void removeNode(string &name)
    {
        node *f = (vertices.find(name)->second);
        for (int i = 0; i < f->adj.size(); i++)
        {
            Pss edge_name = make_pair(name,f->adj[i]);
            Pss edge1_name = make_pair(f->adj[i],name);
            edges.erase(edge_name);
            edges.erase(edge1_name);
        }

        for (auto const &pair : vertices)
        {
            if (pair.first != name)
            {
                for (auto i = pair.second->adj.begin(); i != pair.second->adj.end(); ++i)
                {
                    if (*i == name)
                    {
                        pair.second->adj.erase(i);
                        break;
                    }
                }
            }
        }

        vertices.erase(name);
        delete f;
        }
    void Diaddedge(const string &u, const string &v, double w)
    {
        node *f = (vertices.find(u)->second);
        Pss edge_name = make_pair(u, v);
        edges[edge_name] = w;
        f->adj.push_back(v);
    }
    void Undiaddedge(const string &u, const string &v, double w)
    {
        node *f = (vertices.find(u)->second);
        node *t = (vertices.find(v)->second);
        Pss edge_name = make_pair(u, v);
        Pss edge2_name = make_pair(v, u);
        edges[edge2_name] = w;
        edges[edge_name] = w;
        f->adj.push_back(v);
        t->adj.push_back(u);
    }

    void Diremoveedge(string &u, string &v)
    {
        Pss edge_name = make_pair(u, v);
        edges.erase(edge_name);
        for (auto const &pair : vertices)
        {
            if (pair.first == u)
            {
                for (auto i = pair.second->adj.begin(); i != pair.second->adj.end(); ++i)
                {
                    if (*i == v)
                    {
                        pair.second->adj.erase(i);
                        break;
                    }
                }
            }
        }
    }
    void Undiremoveedge(string &u, string &v)
    {
        Pss edge_name = make_pair(u, v);
        Pss edge2_name = make_pair(v, u);
        edges.erase(edge_name);
        edges.erase(edge2_name);
        for (auto const &pair : vertices)
        {
            if (pair.first == u)
            {
                for (auto i = pair.second->adj.begin(); i != pair.second->adj.end(); ++i)
                {
                    if (*i == v)
                    {
                        pair.second->adj.erase(i);
                        break;
                    }
                }
            }
        }

        for (auto const &pair : vertices)
        {
            if (pair.first == v)
            {
                for (auto i = pair.second->adj.begin(); i != pair.second->adj.end(); ++i)
                {
                    if (*i == u)
                    {
                        pair.second->adj.erase(i);
                        break;
                    }
                }
            }
        }
    }

    void DiadjustWeight(string &u, string &v, double w)
    {
        Pss edge_name = make_pair(u, v);
        edges[edge_name] = w;
    }
    void UndiadjustWeight(string &u, string &v, double w)
    {
        Pss edge_name = make_pair(u, v);
        Pss edge1_name = make_pair(v, u);
        edges[edge_name] = w;
        edges[edge1_name] = w;
    }  
};

void DiDFSearch2(graph &G, string v, node *q)
{
    G.inprogress[v] = true;
    cout<<v<<" ";
    vector<string> p = q->adj;
    for (int i = 0; i < p.size(); i++)
    {
        string u = p[i]; // The edge is (v,u)
        if (G.inprogress[u] == false)
        {
            G.parent[u] = v;
            DiDFSearch2(G, u, G.vertices[u]); // Recursive DFSearch call
        }
        else if (G.finished[u]==false)
        {
            G.cycle = true;
        }
    }
    G.finished[v] = true;
}

void DiDFSearch(graph &G)
{

    for (auto const &pair : G.vertices)
    {
        G.inprogress[pair.first] = false;
        G.finished[pair.first] = false;
        G.parent[pair.first] = "";
    }
    for (auto const &pair : G.vertices)
    {
        if (G.inprogress[pair.first] == false)
        {
            DiDFSearch2(G, pair.first, pair.second);
        }
    }
}

void UndiDFSearch2(graph &G, string v, node *q)
{
    G.inprogress[v] = true;
    cout << v << " ";
    vector<string> p = q->adj;
    for (int i = 0; i < p.size(); i++)
    {
       string u = p[i]; // The edge is (v,u)
       if (G.inprogress[u] == false)
       {
            G.parent[u] = v;
            UndiDFSearch2(G, u, G.vertices[u]); // Recursive DFSearch call
       }
       else if (G.finished[u] == false && G.parent[v]!=u)
       {
            G.cycle = true;
       }
    }
    G.finished[v] = true;
}
void UndiDFSearch(graph &G)
{

    for (auto const &pair : G.vertices)
    {
       G.inprogress[pair.first] = false;
       G.finished[pair.first] = false;
       G.parent[pair.first] = "";
    }
    for (auto const &pair : G.vertices)
    {
       if (G.inprogress[pair.first] == false)
       {
            UndiDFSearch2(G, pair.first, pair.second);
       }
    }
}

void DiDFSearch2_no_output(graph &G, string v, node *q)
{
    G.inprogress[v] = true;
    vector<string> p = q->adj;
    for (int i = 0; i < p.size(); i++)
    {
       string u = p[i]; // The edge is (v,u)
       if (G.inprogress[u] == false)
       {
            G.parent[u] = v;
            DiDFSearch2_no_output(G, u, G.vertices[u]); // Recursive DFSearch call
       }
       else if (G.finished[u] == false)
       {
            G.cycle = true;
       }
    }
    G.finished[v] = true;
}

void DiDFSearch_no_output(graph &G)
{

    for (auto const &pair : G.vertices)
    {
       G.inprogress[pair.first] = false;
       G.finished[pair.first] = false;
       G.parent[pair.first] = "";
    }
    for (auto const &pair : G.vertices)
    {
       if (G.inprogress[pair.first] == false)
       {
            DiDFSearch2_no_output(G, pair.first, pair.second);
       }
    }
}
void Dicheck(graph &G)
{
    G.cycle = false;
    DiDFSearch_no_output(G);
    if (G.cycle == true)
    {
       cout << "There is a cycle";
    }
    else
    {
       cout << "There is no cycle";
    }
}
void UndiDFSearch2_no_output(graph &G, string v, node *q)
{
    G.inprogress[v] = true;
    vector<string> p = q->adj;
    for (int i = 0; i < p.size(); i++)
    {
       string u = p[i]; // The edge is (v,u)
       if (G.inprogress[u] == false)
       {
            G.parent[u] = v;
            UndiDFSearch2_no_output(G, u, G.vertices[u]); // Recursive DFSearch call
       }
       else if (G.finished[u] == false && G.parent[v] != u)
       {
            G.cycle = true;
       }
    }
    G.finished[v] = true;
}
void UndiDFSearch_no_output(graph &G)
{

    for (auto const &pair : G.vertices)
    {
       G.inprogress[pair.first] = false;
       G.finished[pair.first] = false;
       G.parent[pair.first] = "";
    }
    for (auto const &pair : G.vertices)
    {
       if (G.inprogress[pair.first] == false)
       {
            UndiDFSearch2_no_output(G, pair.first, pair.second);
       }
    }
}
void Undicheck(graph &G)
{
    G.cycle = false;
    UndiDFSearch_no_output(G);
    if (G.cycle == true)
    {
       cout << "There is a cycle";
    }
    else
    {
       cout << "There is no cycle";
    }
}
void DiDFSearch2_TPS(graph &G, string v, node *q)
{
    G.inprogress[v] = true;
    vector<string> p = q->adj;
    for (int i = 0; i < p.size(); i++)
    {
       string u = p[i]; // The edge is (v,u)
       if (G.inprogress[u] == false)
       {
            G.parent[u] = v;
            DiDFSearch2_TPS(G, u, G.vertices[u]); // Recursive DFSearch call
       }
       else
       {
            if (G.finished[u] == false)
            {
                G.cycle = true;
            }
       }
    }
    G.finished[v] = true;
    G.TPS.push(v);
}
void DiDFSearch_TPS(graph &G)
{

    for (auto const &pair : G.vertices)
    {
       G.inprogress[pair.first] = false;
       G.finished[pair.first] = false;
       G.parent[pair.first] = "";
    }
    for (auto const &pair : G.vertices)
    {
       if (G.inprogress[pair.first] == false)
       {
            DiDFSearch2_TPS(G, pair.first, pair.second);
       }
    }
}

void Topological_Sort(graph &G)
{
    while (G.TPS.empty() == false)
    {
       cout << G.TPS.top() << " ";
       G.TPS.pop();
    }
    return;
}

void FindPath2(string s, string t, unordered_map<string, string> parent)
{
    if ((s == t) || (t == ""))
    {
       cout << s<<" ";
    }
    else
    {
       FindPath2(s, parent[t], parent);
       cout << t<<" ";
    }
}

void FindPath(graph &G, string s, string t)
{

    for (auto const &pair : G.vertices)
    {
       G.Distance_Array_As_A_Map[pair.first] = INT_MAX;
       G.Parent_Array_for_BFSP_Map[pair.first] = "";
    }

    G.Distance_Array_As_A_Map[s] = 0;

    for( int i=0;i<G.vertices.size()-1;i++)
    
       {
            for (auto const &pair : G.edges)
            {
                if (G.Distance_Array_As_A_Map[pair.first.first] + pair.second < G.Distance_Array_As_A_Map[pair.first.second])
                {
                    G.Parent_Array_for_BFSP_Map[pair.first.second] = pair.first.first;
                    G.Distance_Array_As_A_Map[pair.first.second] = G.Distance_Array_As_A_Map[pair.first.first] + pair.second;
                }
            }
       }
       for (auto const &pair : G.edges)
       {
            if (G.Distance_Array_As_A_Map[pair.first.second] > G.Distance_Array_As_A_Map[pair.first.first] + pair.second)
            {
                cout << "There is no such path, there is a negative weight cycle!";
                return;
            }
       }
    

    return FindPath2(s, t, G.Parent_Array_for_BFSP_Map);

    return;
}
   // from here//
// Function to generate the DOT file
// Function to generate the DOT file
void generateDotFile(graph &G)
{
    ofstream dotFile("graph.dot", ofstream::trunc);
    dotFile << "digraph G {\n";

    // Write node declarations
    for (auto const &pair : G.vertices)
    {
            dotFile << "  " << pair.first << ";\n";
    }

    // Write edge declarations
    for (auto const &pair : G.edges)
    {
            dotFile << "  " << pair.first.first << " -> " << pair.first.second
                    << " [label=\"" << pair.second << "\"];\n";
    }

    dotFile << "}\n";
    dotFile.close();
}

// Function to display the graph
void displayGraph()
{
    system("dot -Tpng graph.dot -o graph.png");
    system("graph.png");
}
 // to here

    int main()
{
    cout << "Welcome to graph creator and analyser,by Maya Ali, Nael Haidar, Toufic Al Mabsout, Youri Klim." << endl;
    cout << "What type of graph would you like to create?" << endl;
    cout << "Enter the number 1 for a directed graph and the number 2 for an undirected graph." << endl;
    int n;

    cin >> n;

    typedef pair<string, string> Pss;
    typedef map<string, node *> vmap;
    typedef map<Pss, double> emap;

    graph G;
    if (n == 1)
    {
        cout << "Noted. A directed graph has been created." << endl;
    }
    else if (n == 2)
    {
        cout << "Noted. An undirected graph has been created." << endl;
    }
    else
    {
        cout << "value of input must be either 1 or 2!";
        return 0;
    }
    int m = 0;
    while (m != 12)
    {
        cout << "Enter the number of the option you would like to choose" << endl;
        cout << "Available options are: " << endl;
        cout << "(1): insert node" << endl;
        cout << "(2): remove node" << endl;
        cout << "(3): insert edge with weight" << endl;
        cout << "(4): remove an edge" << endl;
        cout << "(5): adjust edge weight" << endl;
        cout << "(6): Perform DFS on your graph" << endl;
        cout << "(7): Check for cycles" << endl;
        cout << "(8): Perform Topological sort" << endl;
        cout << "(9): Find shortest path between two nodes of your choice" << endl;
        cout << "(10): Display current graph nodes and connections" << endl;
        cout << "(11): Display the graph visually" << endl;
        cout << "(12): exit the program" << endl;
        cin >> m;

        if (m == 1)
        {
            cout << "Enter the name of the node you would like to insert: " << endl;
            string name;
            cin >> name;
            bool find = false;
            vmap::iterator itr = G.vertices.find(name);
            if (itr != G.vertices.end())
            {
                find = true;
            }
            G.insertNode(name);
            if (find == false)
            {
                cout << "Node inserted successfully!" << endl;
                cout << endl;
            }
        }
        else if (m == 2)
        {
            cout << "Specify the name of the node you would like to remove: " << endl;
            string name;
            cin >> name;
            G.removeNode(name);
            cout << "Node removed successfully!" << endl;
            cout << endl;
        }
        else if (m == 3)
        {
            cout << "Enter the names of the nodes you would like to connect" << endl;
            cout << "Enter the name of the first node: " << endl;
            string N1;
            cin >> N1;
            vmap::iterator itr = G.vertices.find(N1);
            if (itr == G.vertices.end())
            {
                cout << "There's no such node existing!" << endl;
                continue;
            }
            cout << "Enter the name of the second node: " << endl;
            string N2;
            cin >> N2;
            vmap::iterator itr1 = G.vertices.find(N2);
            if (itr1 == G.vertices.end())
            {
                cout << "There's no such node existing!" << endl;
                continue;
            }
            cout << "Enter the weight of the edge" << endl;
            double w;
            cin >> w;

            if (n == 1)
            {
                G.Diaddedge(N1, N2, w);
            }
            else if (n == 2)
            {
                G.Undiaddedge(N1, N2, w);
            }
            cout << "Edge created successfully!" << endl;
            cout << endl;
        }
        else if (m == 4)
        {
            cout << "Enter the names of the nodes you would like to disconnect" << endl;
            cout << "Enter the name of the first node: " << endl;
            string N1;
            cin >> N1;
            vmap::iterator itr = G.vertices.find(N1);
            if (itr == G.vertices.end())
            {
                cout << "There's no such node existing!" << endl;
                continue;
            }
            cout << "Enter the name of the second node: " << endl;
            string N2;
            cin >> N2;
            vmap::iterator itr2 = G.vertices.find(N2);
            if (itr2 == G.vertices.end())
            {
                cout << "There's no such node existing!" << endl;
                continue;
            }
            if (n == 1)
            {
                G.Diremoveedge(N1, N2);
            }
            else if (n == 2)
            {
                G.Undiremoveedge(N1, N2);
            }
            cout << "Edge removed successfully!" << endl;
            cout << endl;
        }
        else if (m == 5)
        {
            cout << "Enter the names of the nodes you would like to adjust the weight between them " << endl;
            cout << "Enter the name of the first node: " << endl;
            string N1;
            cin >> N1;
            vmap::iterator itr = G.vertices.find(N1);
            if (itr == G.vertices.end())
            {
                cout << "There's no such node existing!" << endl;
                continue;
            }
            cout << "Enter the name of the second node: " << endl;
            string N2;
            cin >> N2;
            vmap::iterator itr3 = G.vertices.find(N2);
            if (itr3 == G.vertices.end())
            {
                cout << "There's no such node existing!" << endl;
                continue;
            }
            cout << "Enter the weight of the edge" << endl;
            int w;
            cin >> w;
            if (n == 1)
            {
                G.DiadjustWeight(N1, N2, w);
            }
            else if (n == 2)
            {
                G.UndiadjustWeight(N1, N2, w);
            }
            cout << "Edge weight adjusted successfully!" << endl;
            cout << endl;
        }
        else if (m==6){
            cout<<"The nodes in DFS order are: ";
            DiDFSearch(G); // works anyways for any type of graph
            cout<<endl;
        }
        else if (m==7){
            if (n==1){
            Dicheck(G);}
            else
            { Undicheck(G);}
            cout<<endl;
        }
        else if (m==8){
            if (n==2){
                cout<<"It is an undirected graph, there is no topological sort for it!"<<endl;
            }
            else if (n==1){
                DiDFSearch_no_output(G);
                if (G.cycle == true){
                    cout<<"This graph contains a cycle, so there is no topological sort!"<<endl;
                }
                else{
                    DiDFSearch_TPS(G);
                    cout<<"Topological order is: ";
                    Topological_Sort(G);
                    cout<<endl;
                }
            }
        }
        else if (m==9){
            string source,destination;
            cout<<"Enter your source node: "<<endl;
            cin>>source;
            cout<<"Enter your destination node: "<<endl;
            cin>>destination;
            cout<<endl;
            FindPath(G,source,destination);
            cout<<endl;

        }
        else if(m==10){
            for (auto const &pair : G.vertices)
            {
                cout << "node " << pair.first << endl;
                for (int i = 0; i < pair.second->adj.size(); i++)
                {
                    Pss testo = make_pair(pair.first, pair.second->adj[i]);
                    cout << pair.first << "-->" << pair.second->adj[i] << " weight is " << G.edges[testo] << endl;
                }
            }
        }
        else if (m==11){
            generateDotFile(G);
            displayGraph();
        }
    }
    cout << "Goodbye !" << endl;
    return 0;
}

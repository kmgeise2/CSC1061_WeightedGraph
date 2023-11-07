/*
* File: CSC1061_WeightedGraph.cpp
* Author: Ram Longman on 4/12/2022
* Modified: 11/5/2023 by Kathy Geise
* Description: Weighted graph implementation
* Enter file name graphData.txt or WgraphData.txt
*/

#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
using namespace std;

struct node
{
    int info;
    int weight;
    node* link;
};

void Djikstra(node** AL, int*& smallestCost, int v, int*& predecessors, int vertices);
void PrintPath(int* predecessors, int start, int end);

void depthFirst(node** graph, int v, bool* visited);
void DFT(node** graph, int size);
void breadthFirst(node** graph, int v, bool* visited);
void BFT(node** graph, int size);

void ReadGraphFromFile(node**& AL, int& vertices);
void PrintAL(node** AL, int vertices);
void DestroyGraph(node**& AL, int& vertices);

void AddElement(node*& lst, int data, int weight);
void DeleteElement(node* p);
void PrintList(node* lst);

int main() {

    node** AL;
    int vertices;
    int* smallestCost, * predecessors;

    //create an adjacency list
    ReadGraphFromFile(AL, vertices);

    PrintAL(AL, vertices);

    DFT(AL, vertices);

    BFT(AL, vertices);

    Djikstra(AL, smallestCost, 0, predecessors, vertices);

    cout << "Smallest cost from vertex 0: ";
    for (int i = 0; i < vertices; i++) cout << smallestCost[i] << " ";
    cout << endl;

    PrintPath(predecessors, 0, 6);

    DestroyGraph(AL, vertices);

    return 0;
}

void Djikstra(node** AL, int*& smallestCost, int v, int*& predecessors, int vertices)
{
    //initialization
    smallestCost = new int[vertices];
    predecessors = new int[vertices];
    bool* finalized = new bool[vertices];
    for (int i = 0; i < vertices; i++) {
        smallestCost[i] = 999'999;
        predecessors[i] = -1;
        finalized[i] = false;
    }
    //starting with vertex v
    smallestCost[v] = 0;
    finalized[v] = true;
    for (int i = 0; i < vertices - 1; i++)
    {
        //update any smaller costs to neighbors of vertex
        node* curr = AL[v];
        while (curr != nullptr)
        {
            if (smallestCost[v] + curr->weight < smallestCost[curr->info]) {
                smallestCost[curr->info] = smallestCost[v] + curr->weight;
                predecessors[curr->info] = v;
            }
            curr = curr->link;
        }
        //find vertex with smallest cost to finalize
        int champ = 1'000'000, vertex;
        for (int j = 0; j < vertices; j++)
        {
            if (!finalized[j] && smallestCost[j] < champ) {
                champ = smallestCost[j];
                vertex = j;
            }
        }
        //finalize the vertex
        v = vertex;
        finalized[v] = true;
    }
}

void PrintPath(int* predecessors, int start, int end) {
    stack<int> path;
    cout << "Path from " << start << " to " << end << ": ";
    //assumes there is a path from start to end
    while (start != end) {
        path.push(end);
        end = predecessors[end];
    }
    cout << start;
    while (!path.empty()) {
        cout << " -> " << path.top();
        path.pop();
    }
}

//depth-first search/traversal. Assumes a connected graph.
void depthFirst(node** graph, int v, bool* visited) {
    cout << v << " "; //visit v
    visited[v] = true;
    node* curr = graph[v];
    while (curr != nullptr) {
        if (visited[curr->info] == false) //visit if the neighbor wasn't visited
            depthFirst(graph, curr->info, visited);
        curr = curr->link;
    }
}

void DFT(node** graph, int size)
{
    bool* visited = new bool[size];
    for (int i = 0; i < size; i++) visited[i] = false;

    cout << "Depth First Traversal: ";
    for (int v = 0; v < size; v++) {
        if (!visited[v]) {
            depthFirst(graph, v, visited);
        }
    }
    cout << endl;

    delete[] visited;
}

void breadthFirst(node** graph, int v, bool* visited)
{
    queue<int> q;
    q.push(v);
    visited[v] = true;
    while (!q.empty()) {
        int vertex = q.front();
        q.pop();
        cout << vertex << " ";
        node* curr = graph[vertex]; //get linked list of neighbors
        while (curr != nullptr) { //iterate through neighbors
            if (!visited[curr->info]) { //if neighbor wasn't visited
                q.push(curr->info);
                visited[curr->info] = true;
            }
            curr = curr->link;
        }
    }
}

void BFT(node** graph, int size)
{
    bool* visited = new bool[size];
    for (int i = 0; i < size; i++) visited[i] = false;

    cout << "Breadth First Traversal: ";
    for (int v = 0; v < size; v++) {
        if (!visited[v]) {
            breadthFirst(graph, v, visited);
        }
    }
    cout << endl;

    delete[] visited;
}

void ReadGraphFromFile(node**& AL, int& vertices)
{
    int temp = 0, weight;

    string filename;
    cout << "Enter file name (graphData.txt or WgraphData.txt): ";
    cin >> filename;

    ifstream inFile;
    inFile.open(filename);

    inFile >> vertices;
    AL = new node * [vertices];
    for (int i = 0; i < vertices; i++) {
        AL[i] = nullptr;
        node* curr = nullptr;
        while (true) {
            inFile >> temp;
            if (temp == -999) {
                break;
            }
            inFile >> weight;
            if (curr == nullptr) {
                AddElement(curr, temp, weight);
                AL[i] = curr;
            }
            else {
                AddElement(curr, temp, weight);
                curr = curr->link;
            }
        }
    }
}

//print a graph in the form of Adjacency List
void PrintAL(node** AL, int vertices)
{
    for (int i = 0; i < vertices; i++) {
        cout << i << ": ";
        if (AL[i] != nullptr)
            PrintList(AL[i]);
        else
            cout << endl;
    }
}

void DestroyGraph(node**& AL, int& vertices)
{
    for (int i = 0; i < vertices; i++) {
        while (AL[i] != nullptr && AL[i]->link != nullptr) {
            DeleteElement(AL[i]);
        }
        if (AL[i] != nullptr) {
            delete AL[i];
            AL[i] = nullptr;
        }
    }
    delete[] AL;
    AL = nullptr;
    vertices = 0;
}

//add element to linked list
void AddElement(node*& lst, int data, int weight) {
    node* element = new node;
    element->info = data;
    element->weight = weight;
    if (lst == nullptr) {
        element->link = nullptr;
        lst = element;
    }
    else {
        element->link = lst->link;
        lst->link = element;
    }
}

//delete the element p is pointing to
void DeleteElement(node* p) {
    node* q = p->link;
    p->link = q->link;
    delete q;
}

//print a linked list
void PrintList(node* lst) {
    node* curr = lst;
    while (curr != nullptr) {
        cout << "(" << curr->info << ", " << curr->weight << ") -> ";
        curr = curr->link;
    }
    cout << "nullptr" << endl;
}
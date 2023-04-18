#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;

typedef pair<int, int> pii;

struct City {
    int x, y, pount;
};

struct Comparator {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        if (a.first == b.first){
            return a.second > b.second;
        }
        return a.first < b.first;
    }
};

float calculatePlow(int a1, int a2) {
    return abs(a1 - a2);
}

void addEdge(vector<vector<pair<float, int> > >& graph, int u, int v, float weight) {
    graph[u].emplace_back(make_pair(weight, v));
    graph[v].emplace_back(make_pair(weight, u));
    reverse(graph[u].rbegin(), graph[u].rend());
    reverse(graph[v].rbegin(), graph[v].rend());
}

bool primsIter(priority_queue<pii, vector<pii>, Comparator>& pq, vector<vector<pair<float, int> > >& graph, vector<bool>& occupied, vector<int>& key, vector<int>& mst) {
    if(pq.empty()) return true;
    int u = pq.top().second;
    pq.pop();

    if (occupied[u]) return primsIter(pq, graph, occupied, key, mst);
    occupied[u] = true;
    mst.emplace_back(u);

    for (auto& edge : graph[u]) {
        int v = edge.second;
        float weight = edge.first;

        if (!occupied[v] && key[v] < weight) {
            key[v] = weight;
            pq.push(make_pair(key[v], v));
        }
    }

    return false;
}

int main(int argc, char** argv){
    // if (argc != 2) {
    //     cout << "Usage: " << argv[0] << " <input_file>" << endl;
    //     exit(1);
    // }
    
    ifstream inputFile(argv[1]);
    // ifstream inputFile("./cases/case01/input01.txt");

    int cityCount, bakeryCount;
    float threshold;
    inputFile >> cityCount >> bakeryCount >> threshold;

    vector<int> bakeries(bakeryCount);
    for (int i = 0; i < bakeryCount; i++) {
        inputFile >> bakeries[i];
    }

    vector<City> cities(cityCount);
    for (int i = 0; i < cityCount; i++) {
        inputFile >> cities[i].x >> cities[i].y >> cities[i].pount;
    }

    vector<vector<pair<float, int> > > graph(cityCount);

    for (int i = 0; i < cityCount; i++) {
        for (int j = i + 1; j < cityCount; j++) {
            float plow = calculatePlow(cities[i].pount, cities[j].pount);
            float mean = (cities[i].pount + cities[j].pount) / 2.0;
            if (plow > 0 && plow <= threshold * mean) {
                addEdge(graph, i, j, plow);
            }
        }
    }

    vector<bool> occupied(cityCount, false);
    vector<vector<int> > mst(bakeryCount);
    vector<vector<int> > key(bakeryCount, vector<int>(cityCount, -1e9));
    vector<priority_queue<pii, vector<pii>, Comparator> > pq(bakeryCount);

    for (int i = 0; i < bakeryCount; i++) {
        pq[i].push(make_pair(0, bakeries[i]));
    }

    bool isAllPqEmpty = false;

    while (!isAllPqEmpty){
        isAllPqEmpty = true;
        for (int i = 0; i < bakeryCount; i++) {
            bool isCurrentPqEmpty = primsIter(pq[i], graph, occupied, key[i], mst[i]);
            if (!isCurrentPqEmpty) isAllPqEmpty = false;
        }
    }

    ofstream outputFile("./prim.txt", ios::out);
    
    for (int i = 0; i < bakeryCount; i++) {
        outputFile << "k" << i << " " << mst[i].size() << endl;
        for (uint j = 0; j < mst[i].size(); j++) {
            outputFile << mst[i][j];
            if (j != mst[i].size() - 1) outputFile << "->";
        }
        outputFile << endl;
    }

    exit(0);
}
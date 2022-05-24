#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void addEdge(vector<int> adj[], int v, int u) {
	adj[u].push_back(v);
	adj[v].push_back(u);
}

bool repeat(vector<int> a, int x) {
	for (int i = 0; i < a.size(); ++i) {
		if (a[i] == x)
			return true;
	}
	return false;
}

// want to use to improve find_colors l8r if need be
vector<int> generate_gene(int n) {
	vector<int> gene;
	gene.resize(n, 0);
	for (int i = 1; i < n;) {
		int temp = rand() % n;
		if (!repeat(gene, temp)) {
			gene[i] = temp;
			++i;
		}
	}
	return gene;
}


void find_colors(int V, vector<int> adj[]) {
    int res[V];
    int max_c = 0;

    res[0]  = 0;
    for (int i = 1; i < V; ++i)
        res[i] = -1;

    bool used[V];
    for (int c = 0; c < V; ++c)
        used[c] = false;

    for (int v = 1; v < V; ++v)  {
        for (auto x : adj[v])
            if (res[x] != -1)
                used[res[x]] = true;

        int c = 0;
        while (used[c]==true) ++c;

        res[v] = c;
        max_c = max(c,max_c);

        for (auto x : adj[v])
            if (res[x] != -1)
                used[res[x]] = false;
    }

    cout << "Number of colors: " << max_c+1 << endl;
}

int main() {
	string s1, s2;
	ifstream File;

	int V, E;

	string name;
	cout << "File number:\n";
	cin >> name;

	File.open("./data/gc_" + name);
	File >> s1 >> s2;
	istringstream os1(s1);
	os1 >> V;

	istringstream os2(s2);
	os2 >> E;

	vector<int> adj[V];

	for (int i = 0; i < E; ++i) {
		int e1, e2;
		File >> s1 >> s2;
		istringstream os3(s1);
		os3 >> e1;
		istringstream os4(s2);
		os4 >> e2;
		addEdge(adj, e1, e2);
	}

	File.close();

	find_colors(V,adj);

	return 0;
}

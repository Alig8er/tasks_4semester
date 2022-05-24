#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
const int maxN = 1e5;
const int insz = 7 + maxN * 19;
char ibuff[insz];

void skip_d(char *&p) {
	while (*p <= ' ')
		++p;
}

int get_number(char *&p) {
	while (*p <= ' ')
		++p;

	int num = 0;
	while (*p >= '0' && *p <= '9') {
		num *= 10;
		num += *p - '0';
		++p;
	}
	return num;
}

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
	FILE *fp = fopen("./data/gc_1000_1", "r");
	fread(ibuff, 1, insz, fp);
	char *ip = ibuff;
	int V = get_number(ip); // num of vert
	int E = get_number(ip); //num of edges


	vector<int> adj[V];

	for (int i = 0; i < E; ++i) {
		int e1, e2;
		e1 = get_number(ip);
		e2 = get_number(ip);
		addEdge(adj, e1, e2);
	}

	fclose(fp);

	find_colors(V,adj);

	return 0;
}

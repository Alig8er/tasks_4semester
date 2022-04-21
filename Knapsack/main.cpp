#include <iostream>
#include <vector>
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

int main() {
	FILE *fp = fopen("./data/ks_1000_0", "r");
	fread(ibuff, 1, insz, fp);
	char *ip = ibuff;
	int n = get_number(ip);
	int K = get_number(ip);

	vector<int> v(n);
	vector<int> w(n);

	for (int i = 0; i < n; ++i) {
		int t;
		t = get_number(ip);
		v[i] = t;
		t = get_number(ip);
		w[i] = t;
	}

	/*
	cout << "n = " << n << ' ' << "K = " << K << endl;
	cout << "v = [";
	for (int i: v)
	    cout << i << ' ';
	cout << "]\n";
	cout << "w = [";
	for (int i: w)
	    cout << i << ' ';
	cout << "]\n";
	*/

	vector<vector<int> > V(n, vector<int> (K+1, 0));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j <= K; ++j) {
			if (w[i]<=j) {
				if(i>0)
					V[i][j] = max(V[i-1][j],V[i-1][j-w[i]]+v[i]);
				else
					V[i][j] = v[i];
			}
			else {
				if(i>0)
					V[i][j] = V[i-1][j];
			}
		}
	}

	/*
	for(int i = 0; i < n; ++i) {
		for (int j = 0; j <= K; ++j) {
			cout << V[i][j] << ' ';
		}
		cout << endl;
	}
	*/


	vector<int> solution;

	int x, y;
	x = n-1; y = K;
	while (V[x][y]) {
		if (V[x][y] == V[x-1][y]) {
			x = x-1;
		}
		else if (V[x][y] == v[x] + V[x-1][y-w[x]]) {
			solution.push_back(x);
			y = y-w[x];
			x = x-1;
		}
		else
			cout << "error\n";
	}

	reverse(solution.begin(), solution.end());

	cout << "Indexes: ";

	int vsum = 0, wsum = 0;
	for (int i: solution) {
		cout << i << ' ';
		vsum += v[i];
		wsum += w[i];
	}
	cout << endl;
	cout << "Total value: " << vsum << endl;
	cout << "Weight remainder: " << K - wsum << endl;

	fclose(fp);
	return 0;
}


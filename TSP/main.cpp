#include <iostream>
#include <vector>
#include <cmath>

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
	FILE *fp = fopen("./tsp_data/tsp_100_1", "r");
	fread(ibuff, 1, insz, fp);
	char *ip = ibuff;
	int n = get_number(ip);

	vector<double> x(n);
	vector<double> y(n);

	for (int i = 0; i < n; ++i) {
		int t;
		t = get_number(ip);
		x[i] = t;
		t = get_number(ip);
		y[i] = t;
	}

	/*
	cout << "x = [";
	for (int i : x)
		cout << i << ' ';
	cout << "]\n";
	cout << "y = [";
	for (int i : y)
		cout << i << ' ';
	cout << "]\n";
	*/

	cout << "Calculating distances...\n";

	vector<vector<int> > map(n, vector<int>(n, 0)); //contains SQUARED distances

	for (int i = 0; i < n; ++i) {
		cout << "Row: " << i << endl;
		for (int j = i + 1; j < n; ++j) {
			map[i][j] = map[j][i] =  pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2);
		}
	}

	cout << "Finished calculating distances.\n";

	/*
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cout << map[i][j] << ' ';
		}
		cout << endl;
	}
	*/

	fclose(fp);
	return 0;
}


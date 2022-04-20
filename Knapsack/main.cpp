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
	FILE *fp = fopen("./data/ks_19_0", "r");
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

	cout << "n = " << n << ' ' << "K = " << K << endl;
	cout << "v = [";
	for (int i: v)
	    cout << i << ' ';
	cout << "]\n";
	cout << "w = [";
	for (int i: w)
	    cout << i << ' ';
	cout << "]\n";

	fclose(fp);
	return 0;
}

/*
19 31181
1945 4990
321 1142
2945 7390
4136 10372
1107 3114
1022 2744
1101 3102
2890 7280
962 2624
1060 3020
805 2310
689 2078
1513 3926
3878 9656
13504 32708
1865 4830
667 2034
1833 4766
16553 40006
 */


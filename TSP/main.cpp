#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <ctime>

#define POP_SIZE 100

using namespace std;
const int maxN = 1e5;
const int insz = 7 + maxN * 19;
char ibuff[insz];

void skip_d(char *&p) {
	while (*p <= ' ')
		++p;
}

double get_number(char *&p) {
	while (*p <= ' ')
		++p;

	double num = 0;
	while (*p >= '0' && *p <= '9') {
		num *= 10;
		num += *p - '0';
		++p;
	}
	return num;
}

/////

class individual {
public:
	vector<int> gene;
	double fitness;
public:
	individual() {
		fitness = INT_MAX;
	}
	void print() {
		cout << "GENE: [ ";
		for (int i : gene)
			cout << i << " ";
		cout << "]\n";
		cout << "FITNESS: " << fitness << endl;
	}
	friend bool operator<(const individual &c1, const individual &c2);
	friend bool operator>(const individual &c1, const individual &c2);
};
bool operator<(const individual &c1, const individual &c2) {
	return c1.fitness < c2.fitness;
}
bool operator>(const individual &c1, const individual &c2) {
	return c1.fitness > c2.fitness;
}


bool repeat(vector<int> a, int x) {
	for (int i = 0; i < a.size(); i++) {
		if (a[i] == x)
			return true;
	}
	return false;
}

vector<int> generate_gene(int n) {
	vector<int> gene;
	gene.resize(n, 0);
	for (int i = 1; i < n;) {
		int temp = rand() % (n - 1) + 1;
		if (!repeat(gene, temp)) {
			gene[i] = temp;
			++i;
		}
	}
	return gene;
}

vector<int> mutate_gene(vector<int> gene, int n) {
	int cnt = 0;
	while (cnt<1) {
		int r = rand() % (n - 1) + 1;
		int r1 = rand() % (n - 1) + 1;
		if (r1 != r) {
			int temp = gene[r];
			gene[r] = gene[r1];
			gene[r1] = temp;
			++cnt;
		}
	}
	return gene;
}

bool lessthan(struct individual t1, struct individual t2) {
	return t1.fitness < t2.fitness;
}

double find_fitness(vector<int> gene, vector<vector<double>> &map) ///???????
		{
	double f = 0;
	for (int i = 0; i < gene.size() - 1; i++) {
		if (map[gene[i]][gene[i + 1]] == INT_MAX)
			return INT_MAX;
		f += map[gene[i]][gene[i + 1]];
	}
	return f;
}

individual solve(vector<vector<double>> &map, int n) {
	int gen = 1;
	int thres = 100;

	vector<individual> pop;
	individual temp;

	for (int i = 0; i < POP_SIZE; i++) {
		temp.gene = generate_gene(n);
		temp.fitness = find_fitness(temp.gene, map);
		pop.push_back(temp);
	}

	while (gen <= thres) {

		cout << "Current gen: " << gen << endl;

		sort(pop.begin(), pop.end(), lessthan);

		for (int i = 0; i < POP_SIZE; i++) {
			individual p1 = pop[i];

			int cnt = 0;
			while (true) {

				vector<int> new_g;
				new_g = mutate_gene(p1.gene, n); ///////////// crossing????????
				individual new_gene;
				new_gene.gene = new_g;
				new_gene.fitness = find_fitness(new_gene.gene, map);

				if (new_gene.fitness <= pop[i].fitness) {
					pop[i] = new_gene;
					break;
				}
				else if (cnt > 40) {
					break;
				}
				++cnt;
				//cout << "FAIL: " << cnt << ", i = " << i << endl;
			}
		}
		gen++;
	}

	return pop[0];

}

int main() {
	srand(time(NULL));

	FILE *fp = fopen("./tsp_data/tsp_100_5", "r"); //normal input will work until 399, after 400 scientific notation
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

	cout << "Calculating distances...\n";
	vector<vector<double>> map(n, vector<double>(n, 0)); //contains SQUARED distances
	for (int i = 0; i < n; ++i) {
		cout << "Row: " << i << endl;
		for (int j = i + 1; j < n; ++j) {
			map[i][j] = map[j][i] = pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2);
		}
	}
	cout << "Finished calculating distances.\n";

	individual A;

	A = solve(map, n);
	double sum = 0;
	for (int i = 0; i < n - 1; i++) {
		double temp;
		temp = map[A.gene[i]][A.gene[i + 1]];
		sum += sqrt(temp);
	}

	cout << "ANSWER: " << sum << endl;
	fclose(fp);
	return 0;
}

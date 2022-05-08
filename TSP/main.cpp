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
	while (cnt < 1) {
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

struct Offspring {
	vector<int> c1, c2;
};

Offspring crossover(vector<int> p1, vector<int> p2, int n) {
	vector<int> c1, c2;
	Offspring c;
	c2 = p2;
	c1 = p1;

	int r1 = rand() % (n - 4) + 1;
	int r2 = rand() % (n - 4) + 1;

	for (int i = r1 + 3, j = 0;
			(r1 + 3 + j) % n != r1 && (r1 + 3 + j) % n != r1 + 1;) {
		if (i != 0 && p1[i] != p2[r1] && p1[i] != p2[r1 + 1]
				&& p1[i] != p2[r1 + 2]) {
			if ((r1 + 3 + j) % n) {
				c2[(r1 + 3 + j) % n] = p1[i];
				++j;
			} else {
				++j;
				c2[(r1 + 3 + j) % n] = p1[i];
				++j;
			}
		}
		i = (i + 1) % n;
	}

	for (int i = r2 + 3, j = 0;
			(r2 + 3 + j) % n != r2 && (r1 + 3 + j) % n != r1 + 1;) {
		if (i != 0 && p2[i] != p1[r2] && p2[i] != p1[r2 + 1]
				&& p2[i] != p1[r2 + 2]) {
			if ((r2 + 3 + j) % n) {
				c1[(r2 + 3 + j) % n] = p2[i];
				++j;
			} else {
				++j;
				c1[(r2 + 3 + j) % n] = p2[i];
				++j;
			}
		}
		i = (i + 1) % n;
	}

	c.c2 = c2;
	c.c1 = c1;

	return c;
}

individual solve(vector<vector<double>> &map, int n) {
	int gen = 1;
	int thres = 500;

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

		for (int i = 0; i < 5; ++i) {
			Offspring c;
			int r1 = rand() % POP_SIZE;
			int r2 = rand() % POP_SIZE;
			while (r1 == r2) {
				r2 = rand() % POP_SIZE;
			}
			c = crossover(pop[r1].gene, pop[r2].gene, n);
			pop[r1].gene = c.c1;
			pop[r1].fitness = find_fitness(c.c1, map);
			pop[r2].gene = c.c2;
			pop[r2].fitness = find_fitness(c.c2, map);
		}

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
				} else if (cnt > 40) {
					break;
				}
				++cnt;
				//cout << "FAIL: " << cnt << ", i = " << i << endl;
			}
		}
		gen++;
	}

	sort(pop.begin(), pop.end(), lessthan);

	return pop[0];

}

int main() {
	srand(time(NULL));

	FILE *fp = fopen("./tsp_data/tsp_100_1", "r"); //normal input will work until 399, after 400 scientific notation
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


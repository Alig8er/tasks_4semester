#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

double l = 0;

class Point {
public:
	int id;
	double x, y;
	double d;
	double k;
public:
	void print() {
		//cout << "(" << x << ", " << y << ")";
		//cout << endl;
		//cout << "d = " << d << endl;
	}

	Point(double dem, double a, double b, int i) {
		id = i;
		d = dem;
		x = a;
		y = b;
		k = y/x;
	}

	friend bool operator<(const Point &c1, const Point &c2);
	friend bool operator>(const Point &c1, const Point &c2);

};
bool operator<(const Point &c1, const Point &c2) {
	return c1.k < c2.k;
}
bool operator>(const Point &c1, const Point &c2) {
	return c1.k > c2.k;
}

void print_queue(priority_queue<Point> q) {
    while(!q.empty()) {
        Point t = q.top();
        t.print();
        q.pop();
    }
    std::cout << '\n';
}

void tsp(vector<Point> r) {
	int n = r.size();
	vector<double> x, y;
	for (auto e : r) {
		x.push_back(e.x);
		y.push_back(e.y);
	}


}

int main() {
	string s1, s2, s3;
	ifstream File;

	int N, V;
	double c;
	N = V = c = 0;

	priority_queue<Point> Q;

	/*
	string name;
	cout << "File number:\n";
	cin >> name;
	File.open("./data/vrp_" + name);
	*/

	File.open("./data_vrp/vrp_16_3_1");

	double t;
	File >> s1 >> s2 >> s3;
	istringstream os1(s1);
	os1 >> N;
	istringstream os2(s2);
	os2 >> V;
	istringstream os3(s3);
	os3 >> c;


	double d, x, y;
	vector<double> X(N), Y(N);

	File >> s1 >> s2 >> s3;
	istringstream os11(s1);
	os11 >> d;
	istringstream os21(s2);
	os21 >> x;
	istringstream os31(s3);
	os31 >> y;

	double x0, y0;
	x0 = x;
	y0 = y;

	X[0] = Y[0] = 0;
	// shift x y to origin


	cout << N << ' ' << V << ' ' << c << endl;

	for (int i = 1; i < N; ++i) {
		File >> s1 >> s2 >> s3;
		istringstream os12(s1);
		os12 >> d;
		istringstream os22(s2);
		os22 >> x;
		istringstream os32(s3);
		os32 >> y;
		Q.push(Point(d,x-x0,y-y0,i));
	}

	File.close();


	//cout << "Origin (x0, y0) = (" << x0 << ", " << y0 << ")" << endl;
	//cout << "Queue size: " << Q.size() << endl;

	//print_queue(Q);

	vector<Point> r;
	while(!Q.empty()) {
		double sum = 0;
		while(sum < c  && !Q.empty()) {
			Point t = Q.top();
			if (sum + t.d <= c) {
				r.push_back(t);
				sum += t.d;
				Q.pop();
			}
			else break;
		}
		tsp(r);
		r.clear();
	}


	return 0;
}


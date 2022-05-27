#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

class Point {
public:
	double x, y; // coordinates
	double d; // demand
	double k; // argument
	// going from pi to -pi
public:
	void print() {
		cout << "(" << x << ", " << y << ")" << endl;
	}

	Point(double dem, double a, double b) {
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



void print_queue(priority_queue<Point> q) { // NB: pass by value so the print uses a copy
    while(!q.empty()) {
        Point t = q.top();
        t.print();
        q.pop();
    }
    std::cout << '\n';
}

int main() {
	string s1, s2, s3;
	ifstream File;

	int N, V;
	double c;
	N = V = c = 0;
	// N - number of points
	// V - max number of vehicles
	// c - cost


	priority_queue<Point> Q; // to sort points by argument

	//string name;
	//cout << "File number:\n";
	//cin >> name;
	//File.open("./data/vrp_" + name);

	File.open("./data/vrp_16_3_1");

	double t;
	File >> s1 >> s2 >> s3;
	istringstream os1(s1);
	os1 >> N;
	istringstream os2(s2);
	os2 >> V;
	istringstream os3(s3);
	os3 >> c;


	double d, x, y;

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
	// shift x y to origin


	cout << N << ' ' << V << ' ' << c << endl;

	for (int i = 0; i < N-1; ++i) {
		File >> s1 >> s2 >> s3;
		istringstream os12(s1);
		os12 >> d;
		istringstream os22(s2);
		os22 >> x;
		istringstream os32(s3);
		os32 >> y;
		Q.push(Point(d,x-x0,y-y0));
	}


	cout << "Origin (x0, y0) = (" << x0 << ", " << y0 << ")" << endl;
	cout << "Queue size: " << Q.size() << endl;

	//print_queue(Q);

	File.close();

	return 0;
}


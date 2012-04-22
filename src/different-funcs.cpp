#include <iostream>
#include <ctime>
#include <vector>
#include <boost/thread/thread.hpp>

using namespace std;

// Settings
const int n = 20000000 * 8;

// Funcs
void cpuFunc1();
void cpuFunc2();
int test1(int, int);
int test2(int, int);
int test3(int, int, int);

// Vars
int sharedMem1[2];
int sharedMem2[2];
int sharedMem3[3];

// Main
int main(int argc, char *argv[]) {
	cout << "PROGRAM [START]" << endl;

	timeval startTime, endTime;

	// MT
	gettimeofday(&startTime, 0);

	boost::thread cpu1(cpuFunc1);
	boost::thread cpu2(cpuFunc2);
	int res1, res2, res3;

	cpu1.join();
	cpu2.join();

	res1 = sharedMem1[0] + sharedMem1[1];
	res2 = sharedMem2[0] - sharedMem2[1];
	res3 = sharedMem3[0] * sharedMem3[1] * sharedMem3[2];

	/*vector<boost::thread*> threads;

	for(int i = 0; i < p; ++i) {
		boost::thread *ptr = new boost::thread(cpuFunc);
		threads.push_back(ptr);
	}

	for(int i = 0; i < p; ++i) {
		threads[i]->join();
	}*/

	gettimeofday(&endTime, 0);

	cout << "Results MT: " << res1 << ", " << res2 << ", " << res3 << endl;

	long mtimeMT, mtimeST, seconds, useconds;
	seconds  = endTime.tv_sec  - startTime.tv_sec;
	useconds = endTime.tv_usec - startTime.tv_usec;
	mtimeMT = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	cout << "PROGRAM [END]" << endl;
	cout << "Runtime MT: " << mtimeMT << " ms" << endl;

	gettimeofday(&startTime, 0);

	// ST
	int r1, r2, r3;
	for(int i = 0; i < n; ++i) {
		r1 = test1(i, i);
		r2 = test2(i * 2, i * 3);
		r3 = test3(i * 4, i * 5, i * 6);
	}

	gettimeofday(&endTime, 0);

	cout << "Results ST: " << r1 << ", " << r2 << ", " << r3 << endl;

	seconds  = endTime.tv_sec  - startTime.tv_sec;
	useconds = endTime.tv_usec - startTime.tv_usec;
	mtimeST = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	cout << "Runtime ST: " << mtimeST << " ms" << endl;

	float ratio = (mtimeST / float(mtimeMT));
	float ratioInv = (mtimeMT / float(mtimeST));
	cout << endl;
	cout << "MT is " << (ratio * 100 - 100) << "% faster." << endl;
	cout << "MT needs " << (ratioInv) << " (1/" << int(ratio + 0.5f) << ") the time ST needs." << endl;
}

int test1(int a, int b) {
	a -= 2;
	b -= 3;
	a += 5;
	b += 7;
	a /= 11;
	b /= 13;
	a *= 17;
	b *= 19;
	return a + b;
}

int test2(int a, int b) {
	a -= 1;
	b -= 2;
	return a - b;
}

int test3(int a, int b, int c) {
	a += 1;
	b += 2;
	c += a;
	return a * b * c;
}

void cpuFunc1() {
	int a1, a2, a3;

	for(int i = 0; i < n; ++i) {
		a1 = i;
		a1 -= 2;
		a1 += 5;
		a1 /= 11;
		a1 *= 17;
		sharedMem1[0] = a1;

		a2 = i * 2;
		a2 -= 1;
		sharedMem2[0] = a2;

		a3 = i * 4;
		a3 += 1;
		sharedMem3[0] = a3;
		sharedMem3[2] = (i * 6) + a3;
	}
}

void cpuFunc2() {
	int b1, b2, b3;

	for(int i = 0; i < n; ++i) {
		b1 = i;
		b1 -= 3;
		b1 += 7;
		b1 /= 13;
		b1 *= 19;
		sharedMem1[1] = b1;

		b2 = i * 3;
		b2 -= 2;
		sharedMem2[1] = b2;

		b3 = i * 5;
		b3 += 2;
		sharedMem3[1] = b3;
	}
}

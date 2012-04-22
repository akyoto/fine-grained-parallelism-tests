#include <iostream>
#include <ctime>
#include <boost/thread/thread.hpp>
#include "Step.hpp"

using namespace std;

// Settings
const int c = 100000;
const int n = 100000 / c; //20000000 * 4;

// Funcs
void cpuFunc1();
void cpuFunc2();
int test(int, int);

// Vars
int sharedResultsA[c * 3];
int sharedResultsB[c * 3];
volatile int sharedResultsAB[c * 3];
Step step1(2), step2(2), step3(2);

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

	res1 = sharedResultsA[0] + sharedResultsB[0];
	res2 = sharedResultsA[1] + sharedResultsB[1];
	res3 = sharedResultsA[2] + sharedResultsB[2];

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
		for(int h = 0; h < c; ++h) {
			r1 = test(i, i);
			r2 = test(r1, r1);
			r3 = test(r2, r2);
		}
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

int test(int a, int b) {
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

void cpuFunc1() {
	int a1, a2, a3;
	int index;

	for(int i = 0; i < n; ++i) {
		for(int h = 0; h < c; ++h) {
			a1 = i;
			a1 -= 2;
			a1 += 5;
			a1 /= 11;
			a1 *= 17;
			sharedResultsA[h * 3 + 0] = a1;
		}
		step1.wait();

		for(int h = 0; h < c; ++h) {
			index = h * 3;
			a2 = sharedResultsA[index + 0] + sharedResultsB[index + 0];
			a2 -= 2;
			a2 += 5;
			a2 /= 11;
			a2 *= 17;
			sharedResultsA[index + 1] = a2;
		}
		step2.wait();

		for(int h = 0; h < c; ++h) {
			index = h * 3;
			a3 = sharedResultsA[index + 1] + sharedResultsB[index + 1];
			a3 -= 2;
			a3 += 5;
			a3 /= 11;
			a3 *= 17;
			sharedResultsA[index + 2] = a3;
		}
		//step3.wait();
	}
}

void cpuFunc2() {
	int b1, b2, b3;
	int index;

	for(int i = 0; i < n; ++i) {
		for(int h = 0; h < c; ++h) {
			b1 = i;
			b1 -= 3;
			b1 += 7;
			b1 /= 13;
			b1 *= 19;
			sharedResultsB[h * 3 + 0] = b1;
		}
		step1.wait();

		for(int h = 0; h < c; ++h) {
			index = h * 3;
			b2 = sharedResultsA[index + 0] + sharedResultsB[index + 0];
			b2 -= 3;
			b2 += 7;
			b2 /= 13;
			b2 *= 19;
			sharedResultsB[index + 1] = b2;
		}
		step2.wait();

		for(int h = 0; h < c; ++h) {
			index = h * 3;
			b3 = sharedResultsA[index + 1] + sharedResultsB[index + 1];
			b3 -= 3;
			b3 += 7;
			b3 /= 13;
			b3 *= 19;
			sharedResultsB[index + 2] = b3;
		}
		//step3.wait();
	}
}

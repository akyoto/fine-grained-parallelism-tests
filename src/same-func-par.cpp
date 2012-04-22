#include <iostream>
#include <ctime>
#include <vector>
#include <boost/thread/thread.hpp>

using namespace std;

// Settings
const int n = 20000000 * 4;

// Funcs
void cpuFunc1();
void cpuFunc2();
int test(int, int);

// Vars
int sharedResultsA[3];
int sharedResultsB[3];

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

	cout << "Results: " << res1 << ", " << res2 << ", " << res3 << endl;

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
		r1 = test(i, i);
		r2 = test(i * 2, i * 2);
		r3 = test(i * 3, i * 3);
	}

	gettimeofday(&endTime, 0);

	cout << "Results: " << r1 << ", " << r2 << ", " << r3 << endl;

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

	for(int i = 0; i < n; ++i) {
		a1 = i;
		a1 -= 2;
		a1 += 5;
		a1 /= 11;
		a1 *= 17;
		sharedResultsA[0] = a1;

		a2 = i * 2;
		a2 -= 2;
		a2 += 5;
		a2 /= 11;
		a2 *= 17;
		sharedResultsA[1] = a2;

		a3 = i * 3;
		a3 -= 2;
		a3 += 5;
		a3 /= 11;
		a3 *= 17;
		sharedResultsA[2] = a3;
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
		sharedResultsB[0] = b1;

		b2 = i * 2;
		b2 -= 3;
		b2 += 7;
		b2 /= 13;
		b2 *= 19;
		sharedResultsB[1] = b2;

		b3 = i * 3;
		b3 -= 3;
		b3 += 7;
		b3 /= 13;
		b3 *= 19;
		sharedResultsB[2] = b3;
	}
}

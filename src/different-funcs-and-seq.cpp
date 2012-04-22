#include <iostream>
#include <ctime>
#include <vector>
#include <boost/thread/thread.hpp>
#include "threadpool.hpp"

using namespace std;
using namespace boost::threadpool;

// Settings
const int p = 2;
const int num = 200000000;
const int n = num / 6000; //1000000;
const int m = num / n;

//const int iter = n * m;

// Funcs
void cpuFunc1();
void cpuFunc2();
void cpuFunc3();
void cpuFunc4();
int test1(int, int);
int test2(int, int);
int test3(int, int, int);
int test4(int, int);

// Vars
int res1, res2, res3, res4;
int sharedMem1[2];
int sharedMem2[2];
int sharedMem3[3];
int sharedMem4[2];

// Main
int main(int argc, char *argv[]) {
	timeval startTime, endTime;
	pool cpu(2);

	// MT
	gettimeofday(&startTime, 0);

	for(int rep = 0; rep < m; ++rep) {
		cpu.schedule(&cpuFunc1);
		cpu.schedule(&cpuFunc2);
		cpu.wait();
		res1 = sharedMem1[0] + sharedMem1[1];
		res2 = sharedMem2[0] - sharedMem2[1];
		res3 = sharedMem3[0] * sharedMem3[1] * sharedMem3[2];
		cpu.schedule(&cpuFunc3);
		cpu.schedule(&cpuFunc4);
		cpu.wait();
		res4 = (sharedMem4[0] + sharedMem4[1]) / 1000000;
	}

	/*
	boost::thread cpu1(cpuFunc1);
	boost::thread cpu2(cpuFunc2);

	cpu1.join();
	cpu2.join();

	res1 = sharedMem1[0] + sharedMem1[1];
	res2 = sharedMem2[0] - sharedMem2[1];
	res3 = sharedMem3[0] * sharedMem3[1] * sharedMem3[2];

	//res4 = (sharedMem4[0] + sharedMem4[1]) / 1000000;
	boost::thread cpu3(cpuFunc3);
	boost::thread cpu4(cpuFunc4);
	cpu3.join();
	cpu4.join();

	res4 = (sharedMem4[0] + sharedMem4[1]) / 1000000;
	*/

	/*vector<boost::thread*> threads;

	for(int i = 0; i < p; ++i) {
		boost::thread *ptr = new boost::thread(cpuFunc);
		threads.push_back(ptr);
	}

	for(int i = 0; i < p; ++i) {
		threads[i]->join();
	}*/

	gettimeofday(&endTime, 0);

	cout << "Result MT: " << res4 << endl;

	long mtimeMT, mtimeST, seconds, useconds;
	seconds  = endTime.tv_sec  - startTime.tv_sec;
	useconds = endTime.tv_usec - startTime.tv_usec;
	mtimeMT = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	cout << "Runtime MT: " << mtimeMT << " ms" << endl;
	cout << endl;

	gettimeofday(&startTime, 0);

	// ST
	int r1, r2, r3, r4;

	for(int rep = 0; rep < m; ++rep) {
		for(int i = 0; i < n; ++i) {
			// --- PARALLEL ---
			r1 = test1(i, i);
			r2 = test2(i * 2, i * 3);
			r3 = test3(i * 4, i * 5, i * 6);
			// --- SEQUENTIAL ---
			r4 = test4(r1, r2 + r3);
		}
	}

	gettimeofday(&endTime, 0);

	cout << "Result ST: " << r4 << endl;

	seconds  = endTime.tv_sec  - startTime.tv_sec;
	useconds = endTime.tv_usec - startTime.tv_usec;
	mtimeST = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	cout << "Runtime ST: " << mtimeST << " ms" << endl;

	float ratio = (mtimeST / float(mtimeMT));
	float ratioInv = (mtimeMT / float(mtimeST));
	cout << endl;
	cout << "N: " << n << endl;
	cout << "M: " << m << endl;
	cout << endl;
	cout << "Speedup: " << ratio << " / " << p << endl;
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

int test4(int a, int b) {
	a += 1;
	a *= 2;
	b += 3;
	return (a + b) / 1000000;
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

void cpuFunc3() {
	int a1;

	for(int i = 0; i < n; ++i) {
		a1 = res1;
		a1 += 1;
		a1 *= 2;
		sharedMem4[0] = a1;
	}
}

void cpuFunc4() {
	int b1;

	for(int i = 0; i < n; ++i) {
		b1 = res2 + res3;
		b1 += 3;
		sharedMem4[1] = b1;
	}
}

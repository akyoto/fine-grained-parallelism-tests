#include <iostream>
#include <ctime>
#include <vector>
#include <boost/thread/thread.hpp>

using namespace std;

class CountDown {
public:
	char counter;
	boost::mutex mut;

	CountDown(int start) : counter(start) {

	}

	void wait() {
		{
			boost::lock_guard<boost::mutex> lock(mut);
			counter -= 1;
		}

		while(counter > 0) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}
	}
};

// Shared
const int p = 2;
const int combine = 10;
const int n = 500000000;
const int num = n / combine / p;

typedef int TestType;
volatile TestType x = 0;
volatile TestType x2 = 0;
boost::mutex xMut;

int a = 2;
int b = 2;
int c = 2;
int d = 1;

// Steps
CountDown step1(4);

void cpuFunc() {
	//cout << "CPU [START]" << endl;
	//step1.wait();

	/*TestType tmp = 0;
	TestType old = 0;

	tmp = x;
	old = tmp;*/

	for(int i = 0; i < num; ++i) {
		for(int h = 0; h < combine; ++h) {
			/*tmp += a;
			tmp *= b;
			tmp /= c;
			tmp -= d;*/
		}
		__sync_fetch_and_add(&x, combine);
	}

	/*TestType diff = tmp - old;
	{
		boost::lock_guard<boost::mutex> lock(xMut);
		x += diff;
	}*/

	/*if(!__sync_bool_compare_and_swap(&x, old, tmp)) {

	}*/
	//cout << "CPU [END]" << endl;
}

int main(int argc, char *argv[]) {
	cout << "PROGRAM [START]" << endl;

	timeval startTime, endTime;

	// MT
	gettimeofday(&startTime, 0);

	vector<boost::thread*> threads;

	for(int i = 0; i < p; ++i) {
		boost::thread *ptr = new boost::thread(cpuFunc);
		threads.push_back(ptr);
	}
	//boost::thread cpu3(cpuFunc3);
	//boost::thread cpu4(cpuFunc4);
	int len = threads.size();
	for(int i = 0; i < len; ++i) {
		threads[i]->join();
	}
	//cpu3.join();
	//cpu4.join();

	gettimeofday(&endTime, 0);

	long mtimeMT, mtimeST, seconds, useconds;
	seconds  = endTime.tv_sec  - startTime.tv_sec;
	useconds = endTime.tv_usec - startTime.tv_usec;
	mtimeMT = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	cout << "PROGRAM [END]" << endl;
	cout << "Result: " << x << " (" << (num * p * combine - x) << " missing)" << endl;
	cout << "Runtime: " << mtimeMT << " ms" << endl;

	gettimeofday(&startTime, 0);

	// ST
	const int iter = num * p;
	for(int i = 0; i < iter; ++i) {
		for(int h = 0; h < combine; ++h) {
			/*x2 += a;
			x2 *= b;
			x2 /= c;
			x2 -= d;*/
		}
		x2 += combine;
	}

	gettimeofday(&endTime, 0);

	seconds  = endTime.tv_sec  - startTime.tv_sec;
	useconds = endTime.tv_usec - startTime.tv_usec;
	mtimeST = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	cout << "Result: " << x2 << " (" << (num * p * combine - x2) << " missing)" << endl;
	cout << "Runtime: " << mtimeST << " ms" << endl;

	cout << endl;
	cout << "Combine: " << combine << endl;
	cout << "MT is " << ((mtimeST / float(mtimeMT)) * 100 - 100) << "% faster." << endl;
	cout << "MT needs " << ((mtimeMT / float(mtimeST))) << "x the time ST needs." << endl;
}

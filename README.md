fine-grained-parallelism-tests
==============================

Testing fine grained parallelism with C++ and Boost.

	Runtime fine grained parallelism:    993 ms
	Runtime coarse grained parallelism: 1189 ms
	Runtime single threaded:            1887 ms

	N: 200000000 (calculations per thread)
	M: 1 (number of repeated thread scheduling)
	P: 2 (dual core)

	Speedup: 1.9003
	FG.MT is 90.0302% faster.
	FG.MT needs 0.526232 (1/2) the time ST needs.

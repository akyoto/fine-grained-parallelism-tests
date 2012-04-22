fine-grained-parallelism-tests
==============================

Testing fine grained parallelism with C++ and Boost.

	Runtime fine grained parallelism:    895 ms
	Runtime coarse grained parallelism: 1196 ms
	Runtime single threaded:            1704 ms

	N: 200000000 (calculations per thread)
	M: 1 (number of repeated thread scheduling)
	P: 2 (dual core)

	FG.MT = Fine grained multithreading
	CG.MT = Coarse grained multithreading
	ST =    Single threaded

	Speedup: 1.90391 / 2

	FG.MT needs 0.748328 (1/2) the time CG.MT needs.
	FG.MT needs 0.525235 (1/2) the time ST needs.

	FG.MT is 33.6313% faster than CG.MT.
	FG.MT is 90.3911% faster than ST.

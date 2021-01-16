all: test1 test2 test3 bench bench_diss bench_looptune

test1: test_enqueue_dequeue_order.cc barrier.h Node.h scq.h scp.h  bllock1.h bllock2.h
	g++-8 -o test1.out -pthread -std=c++17 -latomic -O3 test_enqueue_dequeue_order.cc barrier.h Node.h scq.h scp.h  bllock1.h bllock2.h

test2: test_mixed_operators.cc barrier.h Node.h scq.h scp.h bllock1.h bllock2.h
	g++-8 -o test2.out -pthread -std=c++17 -latomic -O3 test_mixed_operators.cc barrier.h Node.h scq.h scp.h bllock1.h bllock2.h

test3: test_all_elements.cc barrier.h Node.h scq.h scp.h bllock1.h bllock2.h
	g++-8 -o test3.out -pthread -std=c++17 -latomic -O3 test_all_elements.cc barrier.h Node.h scq.h scp.h bllock1.h bllock2.h

bench: bench.cc barrier.h Stats.h Node.h scq.h scp.h bllock1.h bllock2.h rnd.h
	g++-8 -o bench.out bench.cc barrier.h Stats.h Node.h scq.h scp.h bllock1.h bllock2.h rnd.h -pthread -std=c++17 -latomic -O3

bench_diss: bench_diss.cc barrier.h Stats.h scq_bench.h rnd.h
	g++-8 -o  bench_diss.out  bench_diss.cc barrier.h Stats.h scq_bench.h rnd.h -pthread -std=c++17 -latomic -O3

bench_looptune: bench_looptune.cc barrier.h Stats.h Node.h scq_looptune.h rnd.h
	g++-8 -o bench_looptune.out bench_looptune.cc barrier.h Stats.h Node.h scq_looptune.h rnd.h -pthread -std=c++17 -latomic -O3

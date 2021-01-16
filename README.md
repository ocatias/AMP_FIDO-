This is an implementation and evaluation of ["A Scalable, Portable, and Memory-Efficient Lock-Free FIFO Queue"](https://arxiv.org/abs/1908.04511) by Ruslan Nikolaev. This project was done for the "Advanced Multiprocessor Programing" course at TU Wien. 

To build all the programs calling the makefile should be enough. If this for some reason does not work
then here are all the commands to compile the separate programs:

BENCHMARKING PROGRAM
g++-8 bench.cc barrier.h Stats.h Node.h scq.h scp.h bllock1.h bllock2.h rnd.h -pthread -std=c++17 -latomic -O3

BENCHMARKING FOR DIFFERENT SPIN NUMBERS
g++-8 bench_looptune.cc barrier.h Stats.h Node.h scq_looptune.h rnd.h -pthread -std=c++17 -latomic -O3

BENCHMARKING THE INNER WORKINGS OF THE SCQ
g++-8  bench_diss.cc barrier.h Stats.h scq_bench.h rnd.h -pthread -std=c++17 -latomic -O3

TESTING ENQUEUE DEQUEUE ORDER
g++-8 test_enqueue_dequeue_order.cc barrier.h Node.h scq.h scp.h  bllock1.h bllock2.h -pthread -std=c++17 -latomic -O3

TESTING MIXED OPERATORS
g++-8 test_mixed_operators.cc barrier.h Node.h scq.h scp.h bllock1.h bllock2.h -pthread -std=c++17 -latomic -O3

TESTING ALL ELEMENTS
g++-8 test_all_elements.cc barrier.h Node.h scq.h scp.h bllock1.h bllock2.h -pthread -std=c++17 -latomic -O3



I will briefly explain what all of these do:
BENCHMARKING PROGRAM is what I used to create the benchmarks of the throughput.
To run it compile and call: ./a.out type a b
type: of operations either "pairs" or "enqdeq"
a: enqueue probability (float from 0 to 1, gets ignored if the type is "pairs")
b: prefill percentage (float from 0 to 2 where 0 is nothing prefilled and 2 is 100% of the queue prefilled)
Note: if the queue cannot deal with enqueuing a value to a full queue (in this case it will spin until it can enqueue the value)
so do not set the prefill percentage to high.

BENCHMARKING FOR DIFFERENT SPIN NUMBERS
Can be run exactly with the same parameters as "BENCHMARKING PROGRAM".
Executes the specified operations with varying numbers of "spin" numbers (loop repititions
that a dequeuer waits for if it arrives before an entry is stored).

BENCHMARKING THE INNER WORKINGS OF THE SCQ
To run compile it and call: ./a.out type a
type: of operations either "pairs" or "enqdeq"
a: prefill percentage (float from 0 to 2 where 0 is nothing prefilled and 2 is 100% of the queue prefilled)
(If you want to use other enqueue/dequeue probabilities then this needs to be changed in the "probabilities" array in the bench_diss.cc code)

All of the benchmarks save their result in the "Resilts" folder. Note that they override files of the same experiment.
This is why my final results are stored in "Eval/Results". To make the graphs use gnuplot and load the corresponding scripts:
i.e. call "gnuplot" and then "load plot_diss.p" (and then restart gnuplot and call another script), the pictures will then be in the "Picture" folder.

TESTS
All of the tests can be just run without any parameters. If one wants to test other properties then they
need to change the code directly. The way the test are configured right now is not the only way I tested the queues.
Note: if you change some of the queue capacities you need to make sure that the queues can store big enough values
or store enough entries otw. the tests will not work.

LIST OF TEST:
test_mixed_operators: Multiple threads enqueue and dequeue elements and we check if
  the queue actually returns an element.

test_enqueue_dequeue_order: Multiple threads enqueue elements, afterwards all elements in the queue
  get sequentially dequeued. We check if no element gets returned before an element that gets
  enqueued later, by the same thread.

test_all_elements: All threads randomly enqueue or dequeue elements (50% chance for each to happen).
  We collect all elements that get enqueued or dequeued and check wether the enqueued elements are the
  same as the dequeued elements


REPLICATING MY BENCHMARKS
These are all of the main throughput benchmarks:
./bench.out enqdeq 0 0
./bench.out pairs 0 0
./bench.out pairs 0 0.9
./bench.out enqdeq 0.5 0
./bench.out enqdeq 0.5 0.9
./bench.out enqdeq 0.01 0
./bench.out enqdeq 0.3 0
./bench.out enqdeq 0.3 0.9



Cyclic_remapping.ipynb
Is the code used to test whether the cyclic remapping function actually returns valid incdices.
This code can be run by an up to date version of the "Jupyter Notebook".

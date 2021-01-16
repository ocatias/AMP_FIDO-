# AMP FIFO Q

This is an implementation and evaluation of ["A Scalable, Portable, and Memory-Efficient Lock-Free FIFO Queue"](https://arxiv.org/abs/1908.04511) by Ruslan Nikolaev. This project was done for the "Advanced Multiprocessor Programing" course at TU Wien. The report can be read [here](https://github.com/ocatias/AMP_FIFO_Q/blob/master/Report/report.pdf).



## Building the Code
To build all the programs calling the **makefile** should be enough. If this for some reason does not work
then here are all the commands to compile the separate programs:

- **Benchmarking program:** g++-8 bench.cc barrier.h Stats.h Node.h scq.h scp.h bllock1.h bllock2.h rnd.h -pthread -std=c++17 -latomic -O3

- **Benchmark for different spin numbers:** g++-8 bench_looptune.cc barrier.h Stats.h Node.h scq_looptune.h rnd.h -pthread -std=c++17 -latomic -O3

- **Benchmark the inner workings of scq:** g++-8  bench_diss.cc barrier.h Stats.h scq_bench.h rnd.h -pthread -std=c++17 -latomic -O3

- **Testing enqueue dequeue order:** g++-8 test_enqueue_dequeue_order.cc barrier.h Node.h scq.h scp.h  bllock1.h bllock2.h -pthread -std=c++17 -latomic -O3

- **Testing mixed operations:** g++-8 test_mixed_operators.cc barrier.h Node.h scq.h scp.h bllock1.h bllock2.h -pthread -std=c++17 -latomic -O3

- **Testing all elements:** g++-8 test_all_elements.cc barrier.h Node.h scq.h scp.h bllock1.h bllock2.h -pthread -std=c++17 -latomic -O3

## Benchmarks
I will briefly explain what all of the built benchmarks do:
- **Benchmarking program** is what I used to create the benchmarks of the throughput.  
To run it compile and call: ./a.out type a b  
type: of operations either "pairs" or "enqdeq"  
a: enqueue probability (float from 0 to 1, gets ignored if the type is "pairs")  
b: prefill percentage (float from 0 to 2 where 0 is nothing prefilled and 2 is 100% of the queue prefilled)  
Note: if the queue cannot deal with enqueuing a value to a full queue (in this case it will spin until it can enqueue the value)
so do not set the prefill percentage to high.  

- **Benchmark for different spin numbers**
Can be run exactly with the same parameters as "Benchmarkign program".
Executes the specified operations with varying numbers of "spin" numbers (loop repititions
that a dequeuer waits for if it arrives before an entry is stored).

- **Benchmark the inner workings of scq**
To run compile it and call: ./a.out type a  
type: of operations either "pairs" or "enqdeq"  
a: prefill percentage (float from 0 to 2 where 0 is nothing prefilled and 2 is 100% of the queue prefilled)  
(If you want to use other enqueue/dequeue probabilities then this needs to be changed in the "probabilities" array in the bench_diss.cc code)

All of the benchmarks save their result in the "Results" folder. Note that they override files of the same experiment.
This is why my final results are stored in "Eval/Results". To make the graphs use gnuplot and load the corresponding scripts:
i.e. call "gnuplot" and then "load plot_diss.p" (and then restart gnuplot and call another script), the pictures will then be in the "Picture" folder.

### Replicating the Benchmarks
These are all of the main throughput benchmarks (take a look at the report for more details):
- ./bench.out enqdeq 0 0
- ./bench.out pairs 0 0
- ./bench.out pairs 0 0.9
- ./bench.out enqdeq 0.5 0
- ./bench.out enqdeq 0.5 0.9
- ./bench.out enqdeq 0.01 0
- ./bench.out enqdeq 0.3 0
- ./bench.out enqdeq 0.3 0.9


## Tests
All of the tests can be just run without any parameters. If one wants to test other properties then they
need to change the code directly. The way the test are configured right now is not the only way I tested the queues.
Note: if you change some of the queue capacities you need to make sure that the queues can store big enough values
and store enough entries otherwise the tests will fail.

### List of Tests:
- **test_mixed_operators:** Multiple threads enqueue and dequeue elements and we check if
  the queue actually returns an element.

- **test_enqueue_dequeue_order:** Multiple threads enqueue elements, afterwards all elements in the queue
  get sequentially dequeued. We check if no element gets returned before an element that gets
  enqueued later, by the same thread.

- **test_all_elements:** All threads randomly enqueue or dequeue elements (50% chance for each to happen).
  We collect all elements that get enqueued or dequeued and check wether the enqueued elements are the
  same as the dequeued elements

- **Cyclic_remapping.ipynb:**
  Is the code used to test whether the cyclic remapping function actually returns valid incdices.
  This code can be run by an up to date version of the "Jupyter Notebook".

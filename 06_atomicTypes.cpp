#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include<atomic>
using namespace std;

/*

**** integer operations and threads ****
integer operations are performed as single instruction
simplified computer architecture 
processor (registers) <---Data---> RAM 
data copied from RAM into register to do operation and copy back to RAM
++count involves thress operations: copy the value from RAM to processor's register
increment value in the processor register; update the count variable in RAM 
atomic variables 
can declare count as atomic, prevents data race 
exercise 1: wrtie program run task 10 concurrent threads;
• A data race occurs, because multiple threads access the count variable, at
least one thread modifies it, and there is no "happens before" relationship
between the thread accesses
• Even though incrementing an integer is an uninterruptible operation, the
value of the variable needs to be fetched from RAM and stored there, and
threads can be interrupted between these operations
• This causes threads to interfere with each other when accessing the count
variable, which causes the answer to be wrong

exercise 2: avoid the data race with a mutex [Done]


**** atomic types ****
c++ <atomic>, trivally copyable
usually bool, integer, pointers 
atomic<int> x{0}; //must be intialzied 
x =2; y = x; 

volatile keyword
the value may be chane without explictly modified; typically used when accessing hardware; 
prevents compiler optimizations which assumes the value does not change 
exercise 3: use atomic variable type 
• Non-atomic version
• There is a data race. Threads interfere with each other and use incorrect
values when incrementing the counter
• Atomic version
• Incrementing the counter is done as an uninterruptible operation and the
new value of the counter is immediately visible to all threads. This ensures
the correct result
• Volatile version
• The volatile keyword has no significance in multi-threaded C++ programs. This
is equivalent to the non-atomic version

**** atomic operations ****
atomic member functions 
store(): replace the value with its argument 
load(): return the value
operator=()
operator T()
exchange(): replace the value and return the previous value 
integer: fetch_add() fetch_sub()
atomic_flag: lock free 
    clear(): set to false
    test_and_set(): set flag to true and returns the previous value 
    operator=()
must initialize an atomic_flag instance to false 
atomic_flag lock = ATOMIC_FLAG_INIT; //initialize to false
void task(){ //spin lock
    while(lock.test_and_set()){ }//if true, some other threads is in the critical section, loop and try again
    //if return false; this thread has set the flag and it is safe to proceed
  critical section 
    
    lock.clear(); //finally set the flag to false to allow other thread to enter the critical section 
}
pros and cons of spin lock: 
spin thread remains active, so it can continue immediately when it get the lock. 
mutex, the thread may need to be reloaded or woken up 
processor-intensive: only sutiable for protecting very short critical sections, or under very low cotention 
many operting systems implement mutex as "hybrid mutex": spin very briefly, if the thread has not been able to run, then put it to sleep 
acquire and release semantis 
    read flag as clear-> acquire access
    set the flag as clear -> release access; release the result of modification; next thread reads the flag as clear will acquire the modificaiton
impose an ordering on access to the variable -> no data race 
exercise 4: function of spin lock 

**** lock-free programming ****
lock free programming allow threads to execute critical sections without assistance from the operating system locking facilities 
avoid the drawbacks of locks:
    race condtion 
    deadlock 
    high overhead 
    lack of scalability
    code complexity and increase overhead 
    loack of composability 
double-checked locking 
    atomic<some_type*> ptr{nullptr};
    some_type* sp = ptr;
    sp->do_it();

lazy initialization 
 In this exercise, make the lazy initialization thread-safe by using an atomic type

*/
//exercise 1: concurrency task 
// int counter{0};
//exercise 3: atomic type
// atomic<int> counter{0};
//if volatile
volatile int counter{0};
mutex mt;
void task(){
    for (int i=0; i<100'000; ++i){
        // lock_guard<mutex> lg(mt);
        // mt.lock();
        ++counter;
        // mt.unlock();
    }
}

//exercise 4: spin lock 
atomic_flag af = ATOMIC_FLAG_INIT;
void taskSpin(int n){
    while (af.test_and_set()) {}
    this_thread::sleep_for(50ms);
    cout << "I am a task with argument " << n <<endl;
    af.clear();
}

//exercise 5: atomic lazy initialization 
class someType{
public:
    void doSomething(){};
};

someType* ptr{nullptr};
void atomicLazy(){
    if (!ptr){
        ptr = new someType;
    }
    ptr->doSomething();
}
int main(){
    vector<thread> threads;
    // for (int i=0; i<10; ++i){
    //     threads.push_back(thread{task});
    // }
    // for (auto& thread : threads)
    //     thread.join();

    // //exercise 4: spin lock 
    // for (int i=0; i<10; i++){
    //     threads.push_back(thread{taskSpin,i});
    // }
    // for (auto& thread : threads)
    //     thread.join();
    // cout <<"the conunter is : " << counter <<endl;

    //exercise 5: lazy initialization
    thread t1{atomicLazy};
    t1.join();

    return 0;
}
#include<iostream>
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


**** atomic types ****
c++ <atomic>, trivally copyable
usually bool, integer, pointers 
atomic<int> x{0}; //must be intialzied 
x =2; y = x; 

volatile keyword
the value may be chane without explictly modified; typically used when accessing hardware; 
prevents compiler optimizations which assumes the value does not change 

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

*/
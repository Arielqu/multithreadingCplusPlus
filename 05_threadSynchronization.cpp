#include<iostream>
#include<thread>
#include<string>
#include<condition_variable>

using namespace std;
/*
**** condition variables ****
problem: worker b cannot start until worker a has finished 
condition variable coordinates two or more threads while allowing critical
sections to be protected by mutexes. 
processing thread: tells condition variable it is waiting (create a unique_lock to lock the mutex, call wait on the condition variable)
fetching thread: sends a notificaiton to the condition variable when it finishes( create a lock_guard to acquire a lock)
condition variable: wake up the processing thread to run 
<condition_variable> 
wait() takes an arugment of type unique_lock<mutex>, unlocks its argument and blocks the thread until a notificaiton is received 
timeout version wait_for() and wait_until()
notify_one() sends a notificaiton to a single waiting thread
notify_all() sends a notificaiton to all waiting threads 
example 1: a reader and a writer
processing thread (read): unique_lock to lock the mutex,because it needs to be able to unlock the mutex. it is done inside the wait() call
fetching thread(write): lock_guard. only needs to mutex to protect the critical section. 

exercise 1: main function starts a reader thread and a writer thread. 

****  condition variablesw with predicate ****
lost wakeup: if the writing thread notifies the condition variable before the reading thread
calls wait(), the reading thread is blocked forever
spurious wakeup: the waiting thrad will be woken up even though the writing thread has not notified condition variable 
wait() with predicate as the second arugment. 
a shared bool variab le; 
cv.wait(lk, []{return flag;});
when wait is called, calls the predicate. if the return true: already sent a notificaiton lost wakeup 
if return false: normal release the lock and wait for notisfication 
always use a predicate with condition variable 

condition variables are useful when multiple threads are waiting for the same event


****  futures ****
thread doesnot provide a way to return a value from a thrad 
future and promise to transfer data between threads; 
no explicit locking is needed 

future and promises use a producer-consumer model 
producer thread generates data; promise sets value 
cosumer wait for data; future waits until the value is avaiable
consumer thread gets the value: future object returns the value from the shared state 

future: a value that has not yet been computed; the most important data structure in c++ concurrency 
future object is not usually created directly: obtained from std::promise

<future>. 
get() blocks unitl operation is completed 
wait() blocks until operation is complete 
promise<int> p; 
auto f{p.get_future()};
this setup the shared state; when the consumer is ready to receive the result, calls the future get() 
****  promises ****
promise is template class
get_future()
set_value()
set_exception()
exception handling: producer an store exceptions in the shared state using the promise set_exception() 
try block and catch block 

****  promises with multiple waiting threads ****
single producer and multiple consumers 
shared_future allow mutiple consumers to wait for a result from a producer thread 
not normally create a shred_future directly
move an exisiting object 
    promise<int>p; 
    future<int> f=p.get_future();
    shared_future<int> sf1{std::move(f)};
can call shared() on the future to get a shared_future from it 
    shared_future<int> sf1{f.share()};
can obtain a shared_future directly from the promise
    shared_future<int> sf1{p.get_future()}; **



****  ****
****  ****



*/

//exercise 1: condition variable read and write
mutex mt;
condition_variable cv;
string sdata{"empty"};

void write(){
    //the fetching thread
    cout << "writing data...\n";
    std::this_thread::sleep_for(5s);
    {
        //acquire lock
        lock_guard<mutex> lg(mt);
        sdata = "has content now.";
    }
    cv.notify_one();
}

void read(){
    unique_lock<mutex> ul(mt); //acquire lock
    cv.wait(ul); //unlock mutex and wait to be notified 
    //notification received, wake up and lock mutex
    cout << "Data is: " <<sdata <<endl;

}

int main(){
    //exercise 1 read and write with condition variable 
    // thread t1{read};
    // thread t2{write};
    // t1.join();
    // t2.join();
    //reverse the order, write thread first and wait for 1 second to start the read
    thread t1{write};
    thread t2{read};
    t1.join();
    t2.join();

}
#include<iostream>
#include<thread>
#include<string>
#include<condition_variable>
#include<future>
using namespace std;
//g++ 05_threadSynchronization.cpp  -pthread -std=c++17
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

• Now reverse the order of the threads, so that the writer thread is
started first. Add a sleep (say, half a second) before starting the
reader thread. What happens?
•
The writing thread completes before the reading thread runs
The reading thread has not called wait() on the condition variable
The condition variable does not have any waiting threads to notify
The notification is "lost"
By the time the reading threads calls wait(), the writing thread has completed
The condition variable does not recei

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

When the writer thread calls notify_all(), the condition variable will wake up
all three reader threads. In each reader, wait() returns, the mutex is locked
and the thread can display the modified value before it exits
• When the writer thread calls notify_one() three times, the condition variable
will also wake up all three reader threads
• Calling notify_one() once will cause one reader thread to be woken up. The
other two readers will continue to sleep. If no further notifications are sent to
the condition variable, the program will be blocked indefinitely
• In all three cases, the choice of which thread to wake up, and the order in
which to wake them up, is made by the scheduler. Different executions may
result in different output


****  futures ****
thread doesnot provide a way to return a value from a thread 
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
wait() blocks until operation is completed
promise<int> p; 
auto f{p.get_future()};
this setup the shared state; when the consumer is ready to receive the result, calls the future get() 
****  promises ****
promise is template class
get_future()
set_value()
set_exception()
exception handling: producer an store exceptions in the shared state using the promise set_exception() 
• Normally, an exception handler which rethrows an exception will use
either a copy of the original exception, or a reference to it
• Why does this not work with std::promise?
• In a multi-threaded program, each thread has its own execution stack
• std::promise is used to transfer an exception from one thread to another
• We cannot use a reference to an exception, because the address of the
exception is only valid in the original thread
• If we use a copy of the exception, we risk "slicing" the exception. In this case,
the target thread would only get the base class part of the exception
try block and catch block 
exercise : write muti-threaded program uses the producer-consumer model with std::promise and std::future 
• Write a multi-threaded program which uses the producer-consumer
model with std::promise and std::future
• The producer thread of this program throws an exception which is
handled in the consumer thread


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

exercise: use mutiple consumer threads with shared_future instead of future
*/

//exercise 1: condition variable read and write
mutex mt;
condition_variable cv;
string sdata{"empty"};
bool condition {false}; //flag for predicate. default false 

void write(){
    //the fetching thread
    cout << "writing data...\n";
    std::this_thread::sleep_for(2s);
    {
        //acquire lock
        lock_guard<mutex> lg(mt);
        sdata = "writer is busy\n";
        condition=true; //set the flag to true
    }
    cout << "write is finished.\n";
    cv.notify_one();
    cv.notify_one();
    cv.notify_one();
    // cv.notify_all();
}

void read(){
    unique_lock<mutex> ul(mt); //acquire lock
    // cv.wait(ul); //unlock mutex and wait to be notified 
    //with a predicate
    cout << "thread id before wait: " << this_thread::get_id() <<endl;;
    cv.wait(ul,[]{return condition;});//lambda predicate that checks the flag
    cout << "thread id after wait: " << this_thread::get_id() <<endl;
    //notification received, wake up and lock mutex
    cout << "Read reads the Data is: " <<sdata <<endl;

}

//exercise of future and promise 
void producer(promise<int>& px){    //producer with promise
    try{
        int x{100};
        this_thread::sleep_for(1s);
        // throw std::out_of_range("oops");
        cout << "producer set value:\n" ;
        px.set_value(x); //set result
    } catch(...){
        px.set_exception(current_exception());
    }
}

void consumer(shared_future<int>& fx){ //consumer with future
    cout << "future calling get()\n";
    cout << "thread " << this_thread::get_id() << "calling get().\n";
    try{
        int x = fx.get();//get result
        lock_guard<mutex> lg{mt};
        cout << "thread " << this_thread::get_id() <<  "has future returns from calling get()\n";
        cout << "thread " << this_thread::get_id() << "gets the answer is: " << x << endl;
    } catch(exception& e){
        cout << "Exception caught: " <<e.what() << endl;
    }
    
    
}

int main(){
    //exercise 1 read and write with condition variable 
    // thread t1{read};
    // thread t2{write};
    // t1.join();
    // t2.join();
    // //reverse the order, write thread first and wait for 1 second to start the read
    
    // thread t1{write};
    // thread t2{read};
    // t1.join();
    // t2.join();
    
    //condition variable with predicate 
    //read first and then write 
    // thread t1{write};
    // thread t2{read}; thread t3{read}; thread t4{read};
    // t1.join();
    // t2.join();t3.join(); t4.join();

    //future and promise 
    promise<int> p;
    shared_future<int> sf1 = p.get_future();
    shared_future<int> sf2 = sf1;

    thread fut1{consumer, std::ref(sf1)};
    thread fut2{consumer, std::ref(sf2)};
    thread prom{producer, std::ref(p)};
    fut1.join();fut2.join();
    prom.join();

}
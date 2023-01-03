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
****  futures ****
****  promises ****
****  promises with multiple waiting threads ****
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
#include<thread>
#include<iostream>
#include<chrono>
#include<vector>
#include<string>
#include<mutex>
using namespace std;
/*
mutex: cretial section; lock and unlock 
mutex and threads: can only access ther resource if the mutex is unlocked; this avoids data race 
C++ mutex <mutex>
A mutex object must be defined outside any thread functions, but visible in them : global/static variagble with global
thread fucntion; a class data memeber with member thread functions
lock(); try_lock(); unlock()
mutexes and data 
class Vector{
mutex m;
vector<int> v;
public: 
void push_back(const int& i){
    m.lock();
    v.push_back(i);
    m.unlock();
}
};

std::lock_guard 
throw exception inside the critial region, the lock never unlocked 
1. with try and catch; in the catch unlock
Drawbacks of mutex: requires a unlock for every lock 
Mutex wrapper classes: RAII  

lock_guard: very basic wrapper only has constructor with lock and destructor to unlock 
std::lock_guard<mutex> lk(print_mutex);
//critical region 
this_thread::sleep_for (50ms); //non critical region 
//destroy lock_guard-release lock 

non-critial region is also locked for lock_guard 

scoped_lock: exactly the same as lock_guard, except it can lock more than one mutex at the same time 
std::scoped_lock lk(m1,m2,...) //no need for template parameter in c++ 17 


std::unique_Lock
the same basic feature as lock_guard: construcctor and destroctor
it has an unlock() member function, can all after the critical region to avoid blocking other threads while execute non-critial code 
if function returns before unlock(), destructor will ensure the mutex is unlocked 

is more flexbile than lock_guard 
options to lock: call lock(); try to get, dont wait if unsuccessful; try to lock only wait for certain time; try to get, wait unitl a certain time point
not lock (locked later), transfer ownership of exisiting lock (move)

constructor take second argument" 
defer_lock; adopt_lock; try_lock

cannot be copied but can be moved 

recommendations: 
lock_guard to lock a single mutex for an entire scope
scoped_lock to lock more than one mutex 
unique_lock need to unlock within the scope
unique_lock if need extra flexibility

time out and mutex 
recursive_mutex 
if try to lock and mutex which has already locked, undefined behavior
std::recursive_mutex lock() member function can be called repeatedly; 
for each lock(), must be an unlock() call
works with lock_guard, unique_lock and scoped_lock 
    unique_lock<recursive_mutex> lk(m);
normally there are better solutions (improve the code design)

timed mutexes
try_lock(); try_lock_for();try_lock_until(): return true or false depending on wheather aquire the lock 

mutexes timeout caveats
chrono library" system_clock; steady_lock; 

*/

//e1: printString
//add a mutex as global mutex instance
mutex task_mutex;

void printStr(string s){
    try{
        //print 3 times
        for (int i=0; i<3; i++){
            //lock
            task_mutex.lock();
            cout << "\nprint string s: \n";
            //print char by char 
            for (int j=0; j<s.length(); j++)
                cout << s[j] << " ";
            //throw an exception 
            throw std::exception();
            //unlock
            task_mutex.unlock();
            this_thread::sleep_for(500ms);
        }
    }   catch(...){
        cout << "\nCaught exception.\n";
    }

}


//e2 thread safe Vector class
class Vector{
private:
    vector<int> v;
    mutex vec_mutex;
public:
    void push_back(int n);
    void print();
};

// Vector vec;
vector<int> vec;

void func(){
    for (int i=0; i<5; i++){
        vec.push_back(i);
        std::this_thread::sleep_for(50ms);
    }
}

void Vector::push_back(int n){
    //lock the mutex 
    vec_mutex.lock();
    v.push_back(n);
    //unlock
    vec_mutex.unlock();

}

void Vector::print(){
    //vec_mutex.lock();
    cout << "\nprint v:\n ";
    for (const auto& x: v) 
        cout << x << " ";
    //vec_mutex.unlock();
}

void funcTryLock1(){
    //lock and sleep for 500ms
    cout<<"\nTry lock func1: try to get the lock ";
    task_mutex.lock();
    cout<<"\nTry lock func1: has the lock ";
    this_thread::sleep_for(500ms);
    cout<<"\nTry lock func1 releases the lock ";
    task_mutex.unlock();    
}
    

void funcTryLock2(){
    //sleep for 100 and try_lock
    
    this_thread::sleep_for(100ms);
    cout<<"\nTry lock func2: try to get the lock\n ";
    while (!task_mutex.try_lock()){
        // cout <<"a\n";
        cout<<"\nTry lock func2: could not get the lock\n";
        this_thread::sleep_for(100ms);
    }
    cout<<"\nTry lock func2 has the lock ";
    task_mutex.unlock();
}
    
    
int main(){
    //**** Mutex ****//
    /*
    describe critical section; mutex in C++
    • "Critical section" describes code that must execute without being interfered with by other parts of the program
    • In multi-threaded programs, critical sections usually arise when shared data can be modified by different threads
    • A "mutex" is a "mutual exclusion object" • It has two states, locked and unlocked
    • How can mutexes be used with critical sections?
        • We can associate a mutex with a critical section
        • Each thread that accesses the critical section locks the mutex before entering it and unlocks it on leaving the critical section
        • If the mutex is already locked when the thread is about to enter the critical section, the thread must wait until the mutex is unlocked
        • If all threads respect this convention, each thread's access "happens before" the next thread's access
        • Correct use of mutexes prevents data races
    Briefly describe the C++ std::mutex
        • std::mutex is a class which implements a mutual exclusion object
        • It is defined in <mutex>
        • It has three main member functions
        • lock() tries to lock the mutex. If it is already locked, it waits until the mutex is unlocked, then locks it
        • try_lock tries to lock the mutex. If it is already locked, it returns immediately
        • unlock() unlocks the mutex
    exercise1: write the scarmbeded output using a mutex: with a mutex the print is fine. 
    exercise2: alter the program exception is thrown betwween output and unlock call; add catch handler at the end of the loop
    exercise3: mutexes and data: implement a simple thread-safe wrapper for the std::vectorclass: int; push_back; print function
                call push_back 5 time with a 50 ms sleep each call' global Vector instance starts ten of these threads and print all elements of vector
                repalce the Vector with a standard vector<int>
                • With the threadsafe Vector class, the program runs normally
                • With std::vector, the program exhibits undefined behaviour (on my system, it crashed and did not display any output)
                • This is because a data race can occur if multiple threads access the same std::vector object
                • In Vector, accesses to the underlying vector are protected by a mutex, which imposes a "happens before" relationship between thread accesses
                • A data race cannot happen with a shared Vector object, but can happen with an std::vector
    exercise4: try_lock(): runs two task in seperate threads; first task locks a mutex, sleep for 500ms and release the lock
                            the second sleeps for 100ms, call try_lock() to lock the mutex, sleeps again for 100ms and call again 
    */
//    thread t1{printStr, "abc"};
//    thread t2{printStr, "def"};
//    thread t3{printStr, "xyz"};

//    t1.join();
//    t2.join();
//    t3.join();

//    vector<thread> threads;
//    for (int i=0; i<10; i++)
//     threads.push_back(thread{func});
//    for (auto& t: threads)
//     t.join();
// //    vec.print();
//    cout << vec.size();
//    for (auto i : vec)
//     cout << i << ", ";

    //try lock
    thread t1{funcTryLock1};
    thread t2{funcTryLock2};
    t1.join();
    t2.join();

   //**** ****//
   //**** ****//
   //**** ****//
   //**** Lock Guard ****//
   //**** ****//
   //**** ****//
   //**** ****//
   //**** Unique Lock ****//
   //**** ****//
   //**** ****//
   //**** Timeouts and Mutexes ****//
   //**** ****//
   //**** ****//

   //**** Shared Mutexes ****//
   //**** ****//
   //**** ****//
   //**** ****//
   //**** Shared Data Initialization ****//
   //**** ****//
   //**** ****//
   //**** ****//
   //**** Trhead-local Data ****//
   //**** ****//
   //**** ****//
   //**** Lazy Initialization ****//
   //**** ****//
   //**** Deadlock ****//
   //**** ****//
   //**** Livelock ****//

   return 0;
}
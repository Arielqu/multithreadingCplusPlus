#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <random>
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

// e1: printString
// add a mutex as global mutex instance
mutex task_mutex;

void printStr(string s)
{
    try
    {
        // print 3 times
        for (int i = 0; i < 3; i++)
        {
            // lock
            task_mutex.lock();
            cout << "\nprint string s: \n";
            // print char by char
            for (int j = 0; j < s.length(); j++)
                cout << s[j] << " ";
            // throw an exception
            throw std::exception();
            // unlock
            task_mutex.unlock();
            this_thread::sleep_for(500ms);
        }
    }
    catch (...)
    {
        cout << "\nCaught exception.\n";
    }
}

// exercise 1: lock guard
void lgPrintStr(string s)
{
    // use a lock guard
    try
    {
        for (int i = 0; i < 3; i++)
        {
            lock_guard<mutex> lg(task_mutex);
            cout << "\nprint string s: \n";
            for (int j = 0; j < s.length(); j++)
                cout << s[j] << " ";
            // throw an exception
            throw std::exception();
            this_thread::sleep_for(500ms);
        }
    }
    catch (...)
    {
        cout << "lock guard caught exception.\n";
    }
}

// exercise 1: unique_lock
void ulPrintStr(string s)
{

    // use a lock guard
    //  try{
    for (int i = 0; i < 3; i++)
    {
        unique_lock<mutex> ul(task_mutex);
        cout << "\nprint string s: \n";
        for (int j = 0; j < s.length(); j++)
            cout << s[j] << " ";
        // throw an exception
        //  throw std::exception();
        ul.unlock();
        this_thread::sleep_for(500ms);
    }

    // } catch(...){
    //     cout << "lock guard caught exception.\n";
    // }
}
// e2 thread safe Vector class
class Vector
{
private:
    vector<int> v;
    mutex vec_mutex;

public:
    void push_back(int n);
    void print();
};

// Vector vec;
vector<int> vec;

void func()
{
    for (int i = 0; i < 5; i++)
    {
        vec.push_back(i);
        std::this_thread::sleep_for(50ms);
    }
}

void Vector::push_back(int n)
{
    // lock the mutex
    vec_mutex.lock();
    v.push_back(n);
    // unlock
    vec_mutex.unlock();
}

void Vector::print()
{
    // vec_mutex.lock();
    cout << "\nprint v:\n ";
    for (const auto &x : v)
        cout << x << " ";
    // vec_mutex.unlock();
}

void funcTryLock1()
{
    // lock and sleep for 500ms
    cout << "\nTry lock func1: try to get the lock ";
    task_mutex.lock();
    cout << "\nTry lock func1: has the lock ";
    this_thread::sleep_for(500ms);
    cout << "\nTry lock func1 releases the lock ";
    task_mutex.unlock();
}

void funcTryLock2()
{
    // sleep for 100 and try_lock

    this_thread::sleep_for(100ms);
    cout << "\nTry lock func2: try to get the lock\n ";
    while (!task_mutex.try_lock())
    {
        // cout <<"a\n";
        cout << "\nTry lock func2: could not get the lock\n";
        this_thread::sleep_for(100ms);
    }
    cout << "\nTry lock func2 has the lock ";
    task_mutex.unlock();
}

timed_mutex tm;

void tryLockFor1()
{
    // lock and sleep for 500ms
    cout << "\nTry lock func1: try to get the lock ";
    tm.lock();
    cout << "\nTry lock func1: has the lock ";
    this_thread::sleep_for(500ms);
    cout << "\nTry lock func1 releases the lock ";
    tm.unlock();
}

void tryLockFor2()
{
    // sleep for 100 and try_lock
    this_thread::sleep_for(100ms);
    cout << "\nTry lock func2: try to get the lock\n ";

    while (!tm.try_lock_for(100ms))
    {
        // cout <<"a\n";
        cout << "\nTry lock func2: could not get the lock\n";
        // this_thread::sleep_for(100ms); => no need to sleep
    }
    cout << "\nTry lock func2 has the lock ";
    task_mutex.unlock();
}

void tryLockUntil2()
{
    this_thread::sleep_for(100ms);
    cout << "\nTry lock func2: try to get the lock\n ";
    auto deadline = chrono::steady_clock::now() + 90ms;

    while (!tm.try_lock_until(deadline))
    {
        // cout <<"a\n";
        deadline += 90ms;
        cout << "\nTry lock func2: could not get the lock\n";
        // this_thread::sleep_for(100ms); => no need to sleep
    }
    cout << "\nTry lock func2 has the lock ";
    task_mutex.unlock();
}

// exercise 1: shared mutex for multiple readers and one writer
//  shared_mutex sm;
// use mutex
mutex sm;
void read(int i)
{
    // lock_guard<shared_mutex> lg(sm);
    lock_guard<mutex> lg(sm);
    cout << i << "reading the book.\n";
}

void write(int i)
{
    // shared_lock<shared_mutex> lg(sm);
    unique_lock<mutex> sl(sm);
    cout << i << "writing the book.\n";
    this_thread::sleep_for(2s);
}

// singleton class
class Singleton
{
    // no copy and move opertor
    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton &operator=(Singleton &&) = delete;

public:
    Singleton() { std::cout << "initializing singlton.\n"; }
};

Singleton &get_singleton()
{
    static Singleton single;
    return single;
}

void task()
{
    Singleton &s = get_singleton();
    cout << &s << endl;
}

// thread-local variables
/*
1. global variable with thread_local: same numbers 
2. move into the function: same result; same numbers generated 
3. static instead of thread_local: different numbers
4. normal local variable in func: same numbers 
*/

// thread_local mt19937 mt;

void threadLocal()
{
    mt19937 mt;
    // static mt19937 mt;
    // thread_local mt19937 mt;
    uniform_real_distribution<double> dist(0, 1);
    cout << "\nthe random numbers are: " << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << dist(mt) << ",";
    }
}

//write a thread-safe double-checked locking 
class Ctype{
    public:
    void do_something(){};
};
Ctype* ptr { nullptr};
mutex dm;
std::once_flag ptr_flag;
void doubleCheck(){
    
    // if(!ptr){
    //     lock_guard<mutex> lk(dm);
    //     if (!ptr)
    //      ptr = new Ctype;
    // }
    std::call_once(ptr_flag,[](){ptr=new Ctype;}); //how to use call one funciton

    static Ctype *nptr{nullptr};//lazily initialized with static variable
    if (!nptr){
        nptr = new Ctype;
    }
       
    ptr->do_something();
}

//deadlock 
mutex dLock1, dLock2;
void deadLock1(){
    cout <<"thread deadlock 1 locking mutex 1.\n";
    unique_lock<mutex> lk(dLock1);
    cout <<"thread deadlock 1 locked mutex 1.\n";
    this_thread::sleep_for(100ms);
    cout << "thread deadlock 1 locking mutex 2.\n";
    unique_lock<mutex>lk1(dLock2);
    cout << "thread deadlock 1 locked mutex 2.\n";
    this_thread::sleep_for(100ms);
    cout << "thread deadlock 1 released locks.\n";

}

void deadLock2(){
    cout <<"thread deadlock 2 locking mutex 2.\n";
    unique_lock<mutex> lk(dLock2);
    cout <<"thread deadlock 2 locked mutex 2.\n";
    this_thread::sleep_for(100ms);
    cout << "thread deadlock 2 locking mutex 1.\n";
    unique_lock<mutex>lk1(dLock1);
    cout << "thread deadlock 2 locked mutex 1.\n";
    this_thread::sleep_for(100ms);
    cout << "thread deadlock 2 released locks.\n";

}

void deadLock3(){
    unique_lock<mutex> lk1(dLock1, std::defer_lock);
    unique_lock<mutex> lk2(dLock2,std::defer_lock);//associate mutex with lock, but dont lock it yet 
    lock(lk1,lk2);
    cout << "Done.\n";

}

void deadLock4(){
    unique_lock<mutex> lk2(dLock2, std::defer_lock);
    unique_lock<mutex> lk1(dLock1,std::defer_lock);//associate mutex with lock, but dont lock it yet 
    lock(lk1,lk2);
    cout << "Done.\n";
}


//live lock 
timed_mutex liveLock1, liveLock2;

void funcLiveLock1(){
    this_thread::sleep_for(10ms);
    bool locked{false};
    while (!locked){
        lock_guard lk(liveLock1); //lock mutex 1
        this_thread::sleep_for(1s);
        cout << "after you, lily.\n";
        locked = liveLock2.try_lock_for(5ms);
    }
    if (locked)
        cout << "thread 1 has both mutexes\n";
}

void funcLiveLock2(){
    bool locked{false};
    while (!locked){
        lock_guard lk(liveLock2); //lock mutex 1
        this_thread::sleep_for(1s);
        cout << "after you, micheal.\n";
        locked = liveLock1.try_lock_for(5ms);
    }
    if (locked)
        cout << "thread 2 has both mutexes\n";
}

//no live lock 
void funcLiveLock3(){
    std::this_thread::sleep_for(10ms);
    cout << "after you, lily\n";
    scoped_lock lk(liveLock1, liveLock2);
    std::this_thread::sleep_for(1s);
    cout << "thread 1 has locked both mutexes.\n";
}

void funcLiveLock4(){
    cout << "after you, michael\n";
    scoped_lock lk(liveLock1, liveLock2);
    std::this_thread::sleep_for(1s);
    cout << "thread 2 has locked both mutexes.\n";
}

int main()
{
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

    // //try lock
    // thread t1{funcTryLock1};
    // thread t2{funcTryLock2};
    // t1.join();
    // t2.join();

    //**** Lock Guard ****//
    /* exception in critial sections
    Explain what happens if an exception is thrown in a critical section
     • The program will immediately leave the current scope and start executing an
     exception handler
     • If the critical section is protected by calling lock() on std::mutex, the unlock()
     call will never be executed
     • All threads which are waiting to acquire a lock on the mutex will be blocked
     • All threads which are waiting or joined on these threads will be blocked
     • Usually, the entire program is deadlocked

     • What approaches can programmers use to manage this situation?
     • C++ provides a number of "wrapper" classes that use the RAII idiom to
     manage mutexes
     • Examples: lock_guard, scope_guard
     • These take advantage of the fact that, when an exception is thrown, the
     destructors are called for every object in scope
     • These wrapper classes lock the mutex in their constructor and unlock it in
     their destructor
     • This guarantees that the mutex will always be unlocked if the function returns

     • Suggest some situations other than exceptions being thrown in which
     these approaches could be useful
     • Thread function with multiple return paths ("return" statements)
     • Loop which locks a mutex and has "break" or "continue" statements
     • The programmer keeps forgetting to call unlock(!)

     • Suggest one feature that could usefully be added to std::lock_guard
     • A member function to unlock the mutex
     • This would give programmers more control over when the mutex is unlocked,
     while still having the fall-back of release on destruction
     • It would avoid the problem of blocking other threads while executing code
     after the critical region

     exercise 1: • Rewrite the program from the last exercise to use a lock_guard
     instead of locking and unlocking a mutex directly


    */
    //    thread t1{lgPrintStr, "abc"};
    //    thread t2{lgPrintStr, "def"};
    //    thread t3{lgPrintStr, "xyz"};

    //    t1.join();
    //    t2.join();
    //    t3.join();

    //**** Unique Lock ****//
    /*
    • Putting in an explicit call to unlock() instead of implicitly unlocking the mutex
     at the end of the scope makes the code clearer
     • There is some extra overhead to using unique_lock, but in this program it will
     not be noticeable
    exericese 1: write with unique_lock instead of lock_guard
    */

    //    thread t1{ulPrintStr, "abc"};
    //    thread t2{ulPrintStr, "def"};
    //    thread t3{ulPrintStr, "xyz"};

    //    t1.join();
    //    t2.join();
    //    t3.join();

    //**** Timeouts and Mutexes ****//
    /*
    recursive_mutex
    • Briefly describe std::recursive_mutex
     • std::recursive_mutex is a mutex that can be locked more than once without
     being unlocked
     • (For std::mutex, this would cause undefined behaviour)
     • For each call to lock(), there must be a corresponding call to unlock()
     • Is this a feature that programmers should use regularly?
     • No, it should only be used when necessary
     • If there is a possibility that a mutex may be locked more than once, this
     usually indicates a problem with the design of the code
     • However, it can occasionally be useful, e.g. for recursive functions that need
     to lock a mutex before calling themselves

     timed_mutex
     Briefly describe std::timed_mutex
     • std::timed_mutex is similar to std::mutex
     • It has additional member functions that wait with a timeout to acquire a lock
     on the mutex
     • try_lock_for() will wait until a certain time interval elapses to acquire a lock if
     not immediately available
     • try_lock_until() will wait until a certain time point to acquire a lock if not
     immediately available
     • Is there another way to obtain this functionality?
     • std::unique_lock has the same member functions
     • They can be called using any type of mutex as the lock's parameter

     exercise1: rewrite the try_lock program to use try_lock_for() and try_lock_until()

    */
    // //try lock
    // thread t1{tryLockFor1};
    // thread t2{tryLockUntil2};
    // t1.join();
    // t2.join();

    //**** Shared Mutexes ****//
    /*
    briefly explain what is shared mutex:
    A shared mutex can be locked in two different ways
     • Exclusive lock. If a thread has an exclusive lock on a shared mutex, no other
     thread can acquire a lock until the first thread releases the lock
     • Shared lock. If a thread has a shared lock on a shared mutex, other threads
     can acquire a shared lock without having to wait for the first thread to release
     it
     • If a thread wishes to acquire an exclusive lock, it must wait until all the
     threads which have a shared lock release their locks

     What is meant by "single writer, multiple readers"?
     • A situation where many threads access shared data, but only a few threads
     modify it
     • With std::mutex, each thread would have exclusive access to the data, forcing
     other threads to wait for access
     • It is safe to have multiple threads making interleaved reads (provided there
     are no modifying threads which could conflict and cause a data race)
     • Giving every thread an exclusive lock causes an unnecessary drop in
     performance

     Explain briefly how this can be implemented without data races using
     a shared mutex
     • The reading threads acquire a shared lock on the mutex and the writing
     threads acquire an exclusive lock
     • This allows many threads to read at the same time, when no threads are
     writing
     • A thread cannot write until all the reading threads have released their locks
     • When a thread is writing, no threads can read until the writing thread
     releases its lock
     • We can never have a situation in which reading and writing threads conflict

     exercise 1: Write a program which uses a shared mutex and has two task functions
     • The first task function acquires an exclusive lock on the shared mutex and
     sleeps for two seconds
     • The second task function acquires a shared lock on the shared mutex and
     does not sleep
     • The program creates five threads with the shared lock, then two
     threads with the exclusive lock, then another five threads with the
     shared lock
     • Add suitable print statements. Explain the results
     • (Note: you may have to force your compiler into C++17 mode)

     The output from the reader threads is scrambled up when using
     shared mutex, but not with std::mutex
     • With a shared mutex, reader threads do not have exclusive access to their
     critical region and their execution can interleave
     • With std::mutex, reader threads have exclusive access to the critical region.
     Only one reader thread can execute its critical region at a time
     • The output from the writer threads is never scrambled up
     • The writer threads have exclusive access to their critical section in both cases
    */
    //   vector<thread> threads;
    //   for (int i=0; i<5; i++){
    //     threads.push_back(thread{read,i});//not good?
    //   }
    //   for (int i=0; i< 2; i++){
    //     threads.push_back(thread{write,i});
    //   }
    //   for (int i=5; i<10; i++){
    //     threads.push_back(thread{read,i});
    //   }

    //   for (int i=0; i<threads.size();i++){
    //     threads[i].join();
    //   }

    //**** Shared Data Initialization ****//
    /*
    singleton class:
    what is singleton class: only has one global instance in the program
    how is implemented:
    copy and move operators are deleted;
    a global function is called to get the instance of the class
    if the instance has not been initialized, the function initializes it before returning it
    otherwise, the function returns the instance directly
    */
    //   vector<thread> threads;
    //   for (int i=0; i<10; i++){
    //     threads.push_back(thread{task});
    //   }
    //   for (auto& t: threads)
    //     t.join();

    //**** Trhead-local Data ****//
    // thread t1{threadLocal};
    // t1.join();
    // this_thread::sleep_for(1s);
    // thread t2{threadLocal};
    // t2.join();

    //**** Lazy Initialization ****//
    /*
    what is lazy intialization: only initialize when needed 
    • Lazy initialization means that a variable is not initialized until the first time it is used
    • Usually we use "eager" initialization, in which a variable is initialized when it is declared
    • Give an example of a situation where lazy initialization could be useful
    • Lazy initialization can be useful when initializing an object has high overhead costs

    • What is meant by double-checked locking? Explain how it is intended
    to work
    • Double-checked locking attempts to perform thread-safe lazy initialization
    while avoiding unnecessary locking
    • The variable is checked twice: once before locking, and once afterwards
    • The first check ensures that the mutex is only locked if the variable is
    uninitialized
    • This leaves potential for a data race, in case the thread is interrupted between
    performing the first check and locking the mutex
    • The variable is checked again, under the lock, to make sure that another
    thread has not initialized the variable while this thread was interrupted

    What issues can arise with double-checked locking in C++?
    • A typical case is where the variable is a pointer
    • Initialization involves calling the new operator and the check is whether the
    pointer is null
    • In C++, the compiler is allowed to reorder operations when optimizing the
    code
    • This can result in the pointer value being assigned before the object has been
    created
    • If the thread is interrupted at this point, other threads will see the variable as
    initialized
    • This will cause them to skip over the lock and second check and use the
    uninitialized object

    • Briefly describe how to avoid these issues
    • Use std::call_once to execute the initialization code. This will ensure that the
    code is only executed once, by one thread, which will not be interrupted
    • Use a static local variable (if the variable does not need to be accessed
    outside the function)
    • Compile under C++17 or later, where the object is guaranteed to be
    constructed before new saves its address

    exercise 1: Write a function that performs thread-safe double-checked locking,
    using one of the techniques you mentioned in your previous answer
    */
   //double checking locking 
//    thread t1{doubleCheck};
//    thread t2{doubleCheck};
//    t1.join(); t2.join();


    //**** Deadlock ****//
    /*
    Explain what is meant by deadlock
    • A thread is deadlocked when it cannot run
    • Give an example of how deadlock can occur
    • Thread A acquires a lock on mutex 1 and waits for a lock on mutex 2
    • Thread B acquires a lock on mutex 2 and waits for a lock on mutex 1
    • Suggest two ways to avoid deadlock
    • Always acquire locks in the same order
    • Use language features which can acquire multiple locks in a single operation

    exercise 1: • Write a program which causes two threads to deadlock
    • Implement your solutions. Verify that the program is no longer
    affected by deadlock
    */
   //deadlock example:
//    thread t1{deadLock1};
//    thread t2{deadLock2};
//    t1.join();
//    t2.join();
//deferred lock
//    thread t1{deadLock3};
//    thread t2{deadLock4};
//    t1.join();
//    t2.join();
    //**** ****//
    //**** Livelock ****//
    /*
    What is meant by the term livelock? How does it differ from
deadlock?
• A thread is livelocked when it is able to run, but cannot make any progress
• In deadlock, the thread cannot run at all
• Briefly describe a situation where livelock can occur
• Livelock can occur when trying to avoid deadlock
• Instead of blocking indefinitely when they cannot get a lock, threads wait and
retry
• If the lock is not available, the threads will retry indefinitely
• The threads are able to run, but cannot make any progress

Suggest how livelock could be avoided in your program
• Use scoped_lock to acquire both locks, or unique_lock with lock()

What is meant by thread priority? Is it supported in C++?
• The priority of a thread is a number assigned to it by the operating system
• A thread with high priority will be scheduled to run more often
• A thread with low priority will sleep or be interrupted more often
• C++ does not directly support thread priority
• Thread priority can be set by calling an operating system API
• The native_handle member function of the std::thread object will return the
data which is needed for this call

• What is meant by priority inversion?
• Priority inversion occurs when a 
high priority thread has to wait for a low
priority thread
• The low priority thread will be interrupted more often than the high priority
thread
• The high priority thread cannot do anything else while it is waiting for the low
priority thread
• In effect, the high priority thread runs at the same speed as a low priority
thread

• What is meant by resource starvation?
• Resource starvation occurs when a thread cannot get the resources it needs
to run
• Give some examples
• Deadlock and livelock, in which the thread cannot obtain a lock it needs
• A thread which cannot start because of insufficient operating system
resources
• In a badly designed scheduler, a low priority thread which does not run
because there is always a higher priority thread

    */
   //live lock
   thread t1{funcLiveLock3};
   this_thread::sleep_for(10ms);
   thread t2{funcLiveLock4};
   t1.join();
   t2.join();


    return 0;
}
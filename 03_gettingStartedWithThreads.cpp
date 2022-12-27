
//launching a thread 
// main thread create additional threads
#include<thread>
#include<iostream>
#include<chrono>
#include<vector>
using namespace std;

void hello(){
    cout<<"hello world\n";
}

//pass by value
void hello2(string s){
    cout << s << endl;
}

//pass by reference 
void hello3(string& s){
    s = "xyz";
}

//pass by move 
void hello4(string&& s){
    cout << "the ownership of " << s << " transferred to thread" << endl;
}

//class 
class greeter{
public: 
    void operator()(){cout <<"hello from member function. \n";} //////functor////////
};

void getSum(const int a, const int b){
    cout << a << " + " << b << " = " << a+b << endl;
}

//functor 
class GetSum{
public: 
    void Sum(const int a, const int b){
        cout << a << " + " << b << " = " << a+b << endl;
    }
};

void doubleVal(int& x) {x *=2;}

class GetDouble{
public:
    void Double(int& x) {x *= 2; }
};

void threadFunc(std::thread t){
    cout << "\nThread function id: " << t.get_id() << endl;
    t.join(); //join be called by final owner of the thread object 
}

std::thread threadFunc2(){
    thread t{[](){cout << "\nThread function 2.\n";}};
    return t;
}

// void threadExcep(){
//     cout << "\nThorw exception.\n";
//     throw std::exception();
// }

void threadExcep2(){
    try {
        cout << "\nThorw exception.\n";
        throw std::exception();

    } catch(...){
        cout << "\nthread got exception.\n";
    }
    
}


//RAII approach for thread 
class ThreadGuard{
private:
    thread mt;
public:
    explicit ThreadGuard(thread t): mt (std::move(t)) {}

    ~ThreadGuard(){
        if (mt.joinable())
            mt.join();
        }
    ThreadGuard(const ThreadGuard&) = delete; //delete copy constructor 
    ThreadGuard& operator=(const ThreadGuard&)=delete;
    ThreadGuard(ThreadGuard&&) noexcept = default;//default move construcctor
    ThreadGuard& operator= (ThreadGuard&&) noexcept =default;
};


//multiple threads 
void mulThreads(int n){
    this_thread::sleep_for(chrono::seconds(n));
    cout << "thread " << n << endl;
}
int main(){
    /**** hello thread program ****/
    // thread t1{hello2, "hello thread.\n"};
    // cout << "Doing somework.\n";
    // string s {"abc"};
    // thread t2{hello3, std::ref(s)};
    // thread t3{hello4, std::move(s)};
    // greeter g;
    // thread t4{&greeter::hello, &g}; //how to use a member function
    // greeter g;
    // thread t1{g};//use a function functor
    // t1.join();
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    // cout << "after the t2, the s: " << s <<endl;

    //**** thread function with arguments ****//
    // write function with arguments; two ints, prints values and sum 
    // thread t1{getSum, 1,2};
    // //functor 
    // GetSum s;
    // thread t1{&GetSum::Sum, &s, 1,4};
    // //lambda expression 
    // thread t1{[](int a, int b) {cout << a+b;}, 2, 4};
    // t1.join();

    //**** thread function with reference arguments ****//
    // create x, double it with thread 
    // int x = 5;
    // cout << "the original: " << x << endl;
    // //regular 
    // thread t1{doubleVal,std::ref(x)};

    // //functor
    // GetDouble d;
    // thread t1{&GetDouble::Double, &d, std::ref(x)};

    // //lambda
    // thread t1{[](int& x) {x *=2;}, std::ref(x)};

    // t1.join();
    // cout << "doubled value: " << x << endl;

    // //**** thread function with pass by move ****//
    // thread t1{[](int&& x) {cout << "inside t1; x=" << x;}, std::move(x)}; //?

    // t1.join();
    // cout << "after move value: " << x << endl;


    // //**** Pausing a thread ****//
    // cout << "\nMain thread id: " << this_thread::get_id() ;
    // thread t1{[](){
    // cout << "\nt1 id: " << this_thread::get_id();
    // this_thread::sleep_for(2s);
    // cout << "\npause a thread.";}};

    // cout << "\nt1 id before join: " << t1.get_id();
    // t1.join();
    // cout << "\nt1 id after join: " << t1.get_id();

    //**** thread objects and functions ****//
    // takes a thread object as argument and prints out the objects thread id 
    // thread t1{[](){cout << "The thread func.";}};

    // threadFunc(std::move(t1));

    //t1.join(); //no; call join in the final owner


    //**** thread objects and functions 2 ****//
    //call the function and get the returned thread 
    // thread t;
    // t = threadFunc2();
    // cout << "thread funcs 2, id: " << t.get_id();
    // t.join();

    //**** thread and exceptions ****//
    //thread function throws an unhandled exception
    // try{
    //     thread t{threadExcep};
    //     t.join();
    // } catch (...){
    //     cout << "\nMain catch exception.\n";

    // }
    // thread t{threadExcep2};
    // t.join();

    //**** managing a thread ****//
    // try{
    //     ThreadGuard tg{ std::thread(hello)};
    //     throw std::exception();

    // } catch(...){
    //     std::cout << "exception caught.\n";

    // }

    //**** multiple threads ****//
    //start three threads: print and sleep for different amount of time before print the message 
    vector<std::thread> vs;
    for (int i=0; i<4; i ++){
        vs.push_back(thread{mulThreads,i});
    }

    for (auto &t: vs){
        if (t.joinable()) t.join();
    }

    /*
    • Interleaved execution
    • A thread can be suspended, allowing another thread to run. This gives the appearance
    that the threads execute in between each other
    • Memory location
    • A scalar object, such as a variable, pointer or a scalar member of a class or struct. Also a
    compound object which does not perform internal synchronization, such as a Standard
    Library container
    • Data race
    • Multiple threads access the same memory location, with at least one thread modifying
    it, without any synchronization between the threads
    • Race condition
    • The result of a program execution depends on the order in which threads are scheduled
    */

    //**** ****//

    //**** ****//

    return 0;
}


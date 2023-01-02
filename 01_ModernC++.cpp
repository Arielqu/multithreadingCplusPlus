#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include<random>

using namespace std;
// two overloaded functions
// one taking int by value
void func(int n)
{
    cout << "by value: " << n << endl;
}

void func(int *n)
{
    cout << "by reference: " << n << endl;
}

void rValue(int&& n){
    cout << "the rvalue n is: " << n << endl;
}

void printVal(vector<string> vs){
    // cout << "\n Print val : ";
    // for (auto x: vs) cout << x;
    // cout <<endl;
}

void rValOver(const int& n){cout <<"\nby const int& \n";}

void rValOver(int&& n){cout <<"\nby int&&\n";}

class test{
    public:
    test()=default;
    test(const test&)=delete; //no copy
    test& operator=(const test&) = delete;//no copy assignment

    test(test&&) noexcept = default; //move
    test& operator=(test&&) noexcept = default; //move assignment 

};

int main()
{
    //**** Universal intialization ****//
    int x{7};
    string s{"Let us begin"};
    // int y{7.7}; //error
    //  int y = 7.7; //only warning

    cout << "x: " << x << "; s: " << s << endl;

    //**** Universal intialization contd ****//
    vector<int> v{4, 2, 3, 5, 1};
    string hello{'H', 'e', 'l', 'l', 'o'};
    cout << "v: ";
    for (const auto &x : v)
        cout << x;
    cout << "hello: " << hello << endl;

    //**** nullptr ****//
    // nullptr should be used instead of NULL to represent a null pointer. Its type is
    // compatible with any pointer, but cannot be converted to int
    // func(NULL);//error
    func(nullptr);

    //**** std::chrono ****//
    // c++11
    std::chrono::seconds t0{3}; //? like this?
    // c++14
    auto t1{2s};
    auto t2{20ms};
    auto t3{50us};

    //**** automatic type deduction ****//
    auto a{6};
    vector<string> vs{"a", "b"};
    auto b = vs.begin();
    int aa{6};
    vector<string>::iterator bb = vs.begin();

    //**** auto with qualifiers ****//
    const auto &c = a;

    //**** auto and for loop ****//
    vector<int> vi{4, 2, 5, 3, 1};
    for (auto &x : vi)
        x += 2;
    cout << "\n The auto for loop array: ";
    for (const auto &x : vi)
        cout << x;

    for (vector<int>::iterator it = vi.begin(); it != vi.end(); ++it)
        *it += 2;
    cout << "\nvector elements: ";
    for (vector<int>::const_iterator it = vi.begin(); it != vi.end(); ++it)
        cout << *it << endl;

    //**** lambda expression ****//
    // A lambda expression is an anonymous, inline function. It is used to create a local function,
    // mainly for passing as an argument to a function call or as a return value

    //double the value 

    vector<int> vecInt{1, 2, 3, 4, 5};
    cout << "\n the count of odd: " << count_if(vecInt.begin(), vecInt.end(), [](int x)
                                                { return x % 2 != 0; }) << endl;

    //**** capture ****//
    //[=]: by value; [&]: by reference

    //**** capture and objects ****//
    // a member function to capture the data memeber of the object
    // [this]() {/*use refernce to data memebers and variable*;/}

    //**** lambda expression with capture ****//
    int testVal = 3; 
    cout << "\n the count of 3: " << count_if(vecInt.begin(), vecInt.end(), [testVal](int x)
                                                { return x % testVal != 0; });

    //**** lvalue and rvalue ****//
    // • An lvalue is a value whose address can be taken using the & operator
    // • An rvalue is a value whose address cannot be taken using the & operator
    //lvalue: reference; rvalue: & is not legal; int x: lvalue; 6: rvalue; func():rvalue

    //**** lvalue and rvalue refernes ****//
    // lavalue reference: copy &
    // rvalue refernce: move && 

    //**** rvalue referene ****//
    // write a function has rvalue reference, passing rvalue to it ; pass lvalue to it 
    rValue(4);
    int val{3};
    rValue(move(val)); //use move for rvalue

    //**** Pass by value or by move ****//
    //write function printVal 
    vector<string> vs1 {"hello", "world"};
    cout << "\n before:"; 
    for (auto x: vs1) cout << x;
    printVal(vs1);
    cout << "\n after: " ;
    for (auto x:vs1) cout << x;

    //pass by rvalue; will delete 
    printVal(std::move(vs1));
    cout << "\n after pass by rvalue: " ;
    for (auto x:vs1) cout << x;


    //**** rvalue referenfe and overloading ****//
    // write a function takes int by const referene and an overloaded with an int by rvalue reference
    int rOver{1};
    rValOver(rOver); //reference
    rValOver(std::move(rOver));//rvalue reference
    rValOver(4);//rvalue

    //**** move only types ****//
    // unique_ptr; fstream => can only transfer ownership, cannot copy
    // Move-only types are useful when a class owns a resource and manages its lifetime.
    //  In order to give an object sole ownership of the resource, copying cannot be allowed.
    // However, the resource can be transferred from one object to another, in which case the target object becomes
    // the new owner of the resource.


    //**** move operators ****//
    // prototypes of move constructor and move assignment operator of calss called myclass
    // myclass(myclass&& other) noexcept; //move constructor
    // myclass& operator=(myclass&& other) noexcept; //move assignment operator


    // //**** delete and defaulted operators ****//
    // test(const test& other)=delete;
    // test(const test& other)=default;
    test t11,t12;
    // test t13(t11);//cannot copy
    // t12 = t11; //cannot 

    test t14(std::move(t11));
    t12 = std::move(t11);


    //**** random number ****//
    // <random> 
    // random number engine: generates a sequence of random numbers
    // distribution object: scales a sequence of numbers to fit inot a given statistical distribution
    //print out 10 random integers between 0 and 100
    //create engineer instance 
    mt19937 mt;
    // ints in range of 0 to 100
    uniform_int_distribution<int> uid(0,100);

    for (int i=0; i<10; i++)
        cout << "\n 10 number from 0 to 100: " << uid(mt) <<endl;

    uniform_real_distribution<double> did (0,1);
    for (int i=0; i<10; i++)
        cout << "\n 10 number from 0 to 1: " << did(mt) <<endl;



    
    return 0;
}
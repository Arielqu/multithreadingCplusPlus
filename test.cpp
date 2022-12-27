#include <iostream>
#include <thread>
using namespace std;

int cnt{0};
const int N = 100000;

void increaseCnt (){
    for (int i=0; i<N; i++){
        cnt ++;     
    }

}
int main()
{
    //FizzBuzz(100);
    thread t1{increaseCnt};
    thread t2{increaseCnt};
    thread t3{increaseCnt};


    
    t1.join();   
    t2.join();    
    t3.join();    

    cout << "final val: " << cnt << endl;
    return 0;   
}
#include <string>
#include <vector>

using namespace std;

void func(vector<string>) {
	
}

int main() {
	vector<string> vec(1000000, "abc");      // Vector of 1 million strings
	//func(std::move(vec));                        // Pass vec by value to a function
	func(vec);                        // Pass vec by value to a function
}
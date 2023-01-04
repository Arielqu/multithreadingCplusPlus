#include "singleton.h"
#include <thread>
#include <vector>

using namespace std;

void task() {
	singleton& s = get_singleton();
	cout << &s << endl;
}

int main() {
	vector<thread> threads;
	for (int i = 0; i < 10; ++i)
		threads.push_back(thread{task});
	for (auto& t : threads)
		t.join();
}
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int counter {0};

void task() {
	for (int i = 0; i < 100'000; ++i) ++counter;
}

int main() {
	std::vector<std::thread> tasks;
	for (int i = 0; i < 10; ++i) tasks.push_back(std::thread{task});
	for (auto& t: tasks) t.join();
	cout << counter << endl;
}

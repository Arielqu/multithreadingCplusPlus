#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>

class singleton {
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;
	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
	public:
	singleton() { std::cout << "Initializing singleton" << std::endl;}
};

singleton& get_singleton();

#endif //SINGLETON_H
#include <mutex>

class some_type {
	// ...
public:
	void do_it() { /*...*/ }
};

std::mutex process_mutex;

void process() {
	static some_type *ptr{nullptr};            // Variable to be lazily initialized
    if (!ptr) {
            ptr = new some_type;   // Initialize ptr
    }
     ptr->do_it();
}

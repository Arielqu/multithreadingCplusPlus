#include "singleton.h"

singleton& get_singleton() {
	static singleton single;
	return single;
}

#ifndef trace_h
#define trace_h
#include <iostream>

#include <stdlib.h>

#define LOG(x) {std::cout << x << std::flush; }
#define TRACE(x) {if (getenv("TRACE")) { std::cout << x << std::flush; }}

#endif

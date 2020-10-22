CXX ?= g++
CFLAGS ?= -Wall -I src

test: catch.cpp catch.hpp src/base64.hpp
	$(CXX) $(CFLAGS) catch.cpp -o catch
	./catch

clean:
	rm catch

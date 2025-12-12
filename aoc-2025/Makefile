CXX=clang++
VPATH=../src/main
#CXXFLAGS?=-Og -g -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG=1
#CXXFLAGS?=-Og -g -fsanitize=address
CXXFLAGS?=-O3

CXXFLAGS+=-std=c++23 -stdlib=libc++ -Wall -Wno-deprecated-declarations -I/opt/homebrew/include
LDFLAGS=-lgmpxx -lgmp -L/opt/homebrew/lib

cpp/10z3/main: LDFLAGS+="-lz3"

clean:
	rm -rf build

CXX=clang++
VPATH=../src/main
#CXXFLAGS?=-Og -g -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG=1
#CXXFLAGS?=-Og -g -fsanitize=address
CXXFLAGS?=-O3

CXXFLAGS+=-std=c++23 -stdlib=libc++ -Wall -Wno-deprecated-declarations -I/opt/homebrew/include
LDFLAGS+=-L/opt/homebrew/lib

cpp/11b/main: LDFLAGS+=-lgmpxx -lgmp
cpp/24/main: LDFLAGS+=-lgmpxx -lgmp

clean:
	rm -rf build

fmt:
	clang-format -i src/main/cpp/*.h src/main/cpp/*/*.cpp

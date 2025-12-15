CXX=clang++
VPATH=../src/main
#CXXFLAGS?=-Og -g -D_GLIBCXX_DEBUG
#CXXFLAGS?=-Og -g -fsanitize=address
CXXFLAGS?=-O3

CXXFLAGS+=-std=c++23 -stdlib=libc++ -Wall -Wno-deprecated-declarations -I/opt/homebrew/include
LDFLAGS+=-L/opt/homebrew/lib

clean:
	rm -rf build

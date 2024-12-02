VPATH=../src/main
#CXXFLAGS?=-std=c++20 -Wall -g -D_GLIBCXX_DEBUG
CXXFLAGS?=-std=c++20 -Wall -g -fsanitize=address
#CXXFLAGS?=-std=c++20 -Wall -O3
LDFLAGS=-lgmpxx -lgmp -L/opt/homebrew/lib

clean:
	rm -rf build/*

all: clean compile test

BINARY=Project3

clean:
	rm -f cmake-build-debug/$(BINARY)

compile: FileChecker.cpp FileChecker.h FileCopier.cpp FileCopier.h FileModifyException.h main.cpp Project3NetworkingModifier.cpp Project3NetworkingModifier.h Util.cpp Util.h
	g++ -std=c++14 -I . -pthread -o cmake-build-debug/$(BINARY) FileChecker.cpp FileCopier.cpp main.cpp Project3NetworkingModifier.cpp Util.cpp

test:
	cd cmake-build-debug && ./$(BINARY) R

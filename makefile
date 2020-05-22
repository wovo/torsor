ifeq ($(OS),Windows_NT)
   CPP := C:\Program Files\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\bin\c++
   CPPX := C:\mingw64-10\bin\c++ -std=c++20 -Ilibrary
   DELETE := rm
else
   CPP := c++
   DELETE := rm
endif

CPPX ?= $(CPP) -std=c++17 -fconcepts -Ilibrary

.PHONY: run fail tests build docs clean

test-compilation.exe: library/torsor.hpp tests/test-compilation.cpp
	$(CPPX) tests/test-compilation.cpp -o test-compilation.exe 

test-compilation-concepts.exe: library/torsor.hpp tests/test-compilation-concepts.cpp
	$(CPPX) tests/test-compilation-concepts.cpp -o test-compilation-concepts.exe 

test-runtime.exe: library/torsor.hpp tests/test-runtime.cpp
	$(CPPX) tests/test-runtime.cpp -o test-runtime.exe 

build: 
	$(CPPX) tests/test-error-messages.cpp -o test-compiler-messages.exe 
   
clean:
	$(DELETE) test-compilation.exe test-compilation-concepts.exe
	$(DELETE) test-runtime.exe test-compiler-messages.exe
   
run: test-runtime.exe
	./test-runtime.exe

fail: test-compilation.exe test-compilation-concepts.exe
	./test-compilation.exe 
	./test-compilation-concepts.exe 
   
tests: run fail

docs: 
	Doxygen documentation/Doxyfile
	pandoc -V geometry:a4paper -s -o documentation/readme.pdf readme.md

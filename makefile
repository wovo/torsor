ifeq ($(OS),Windows_NT)
   CPP := C:\Program Files\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\bin\c++
   CPP := C:\Program Files\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\c++
else
   CPP := c++
endif

CPPX := $(CPP) -std=c++17 -fconcepts -Ilibrary

.PHONY: run fail tests build docs 

test-compilation.exe: library/torsor.hpp tests/test-compilation.cpp
	$(CPPX) tests/test-compilation.cpp -o test-compilation.exe 

test-compilation-concepts.exe: library/torsor.hpp tests/test-compilation-concepts.cpp
	$(CPPX) tests/test-compilation-concepts.cpp -o test-compilation-concepts.exe 

test-runtime.exe: library/torsor.hpp tests/test-runtime.cpp
	$(CPPX) tests/test-runtime.cpp -o test-runtime.exe 

build: 
	$(CPPX) tests/test-error-messages.cpp -o test-compiler-messages.exe 

run: test-runtime.exe
	./test-runtime.exe

fail: test-compilation.exe test-compilation-concepts.exe
	./test-compilation.exe 
	./test-compilation-concepts.exe 
   
tests: run fail

docs: 
	Doxygen documentation/Doxyfile
	pandoc -V geometry:a4paper -s -o documentation/readme.pdf readme.md

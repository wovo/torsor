ifeq ($(OS),Windows_NT)
   CPP := C:\Program Files\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\bin\c++
else
   CPP := c++
endif

CPPX := $(CPP) -std=c++17 -fconcepts -Ilibrary

.PHONY: tests docs

test-compilation.exe: library/torsor.hpp tests/test-compilation.cpp
	$(CPPX) tests/test-compilation.cpp -o test-compilation.exe 

test-runtime.exe: library/torsor.hpp tests/test-runtime.cpp
	$(CPPX) tests/test-runtime.cpp -o test-runtime.exe 

tests: test-compilation.exe test-runtime.exe
	./test-compilation.exe && ./test-runtime.exe

docs: 
	Doxygen documentation/Doxyfile
	pandoc -V geometry:a4paper -s -o documentation/readme.pdf readme.md

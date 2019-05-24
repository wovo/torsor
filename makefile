ifeq ($(OS),Windows_NT)
   CPP := C:\Program Files\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\bin\c++
else
   CPP := c++
endif

.PHONY: build run docs

build: tests.exe

tests.exe: library/torsor.hpp tests/tests.cpp
	$(CPP) tests/tests.cpp -Ilibrary -o tests.exe -std=c++17 -fconcepts

run: main.exe
	./main.exe

docs: 
	Doxygen documentation/Doxyfile
	pandoc -s -o documentation/readme.pdf readme.md

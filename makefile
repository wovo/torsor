ifeq ($(OS),Windows_NT)
   CPP := C:\Program Files\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\bin\c++
else
   CPP := c++
endif

.PHONY: tests docs

tests.exe: library/torsor.hpp tests/tests.cpp
	$(CPP) tests/tests.cpp -Ilibrary -o tests.exe -std=c++17 -fconcepts

tests: tests.exe
	./tests.exe

docs: 
	Doxygen documentation/Doxyfile
	pandoc -V geometry:a4paper -s -o documentation/readme.pdf readme.md

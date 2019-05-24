CPP := C:\Program Files\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\bin\c++

.PHONY: build run docs

build: main.exe

main.exe: library/torsor.hpp tests/main.cpp
	$(CPP) tests/main.cpp -Ilibrary -o main.exe -std=c++17 -fconcepts

run: main.exe
	./main.exe

docs: 
	Doxygen documentation/Doxyfile
	pandoc -s -o documentation/readme.pdf readme.md
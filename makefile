all: prog4pages

prog4pages: prog4pages.o
	g++ prog4.cpp -o prog4swap

prog4pages.o: prog4.cpp
	g++ -c prog4.cpp

clean:
	rm -f *.o prog4swap

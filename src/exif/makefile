all: test

exif.o: exif.cpp
	g++ -g3 -pedantic -Wall -ansi -c exif.cpp

test: exif.o test.cpp
	g++ -g3 -Wall -pedantic -ansi -o test exif.o test.cpp

clean:
	rm -f *.o test

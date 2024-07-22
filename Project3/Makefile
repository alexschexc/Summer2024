CPP = g++ -g

all: pipegrep

pipegrep: *.o
	$(CPP) $^ -o $@

*.o: *.cpp
	$(CPP) -I. -c $^ -o $@

clean:
	rm -f pipegrep *.o

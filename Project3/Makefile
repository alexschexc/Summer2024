CPP = g++ -g

all: ACS200009MyLock

ACS200009MyLock: *.o
	$(CPP) $^ -o $@

*.o: *.cpp
	$(CPP) -I. -c $^ -o $@

clean:
	rm -f ACS200009MyLock *.o

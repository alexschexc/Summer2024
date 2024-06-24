CPP = g++

all: ACS200009PrefixSum

ACS200009PrefixSum: *.o
	$(CPP) $^ -o $@

*.o: *.cpp
	$(CPP) -I. -c $^ -o $@

clean:
	rm -f ACS200009PrefixSum *.o

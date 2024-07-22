CPP = g++ -g
CFLAGS = -I. -pthread
LDFLAGS = -pthread

all: ACS200009MyLock

ACS200009MyLock: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
        $(CPP) $(LDFLAGS) $^ -o $@

%.o: %.cpp
        $(CPP) $(CFLAGS) -c $< -o $@

clean:
        rm -f ACS200009MyLock *.o

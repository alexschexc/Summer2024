#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <unistd.h>
#include <sys/shm.h>

int* createSharedIntArray(key_t k, int size);
int** createSharedTempArray(key_t k, int size, int core);
void hs(int i, int p, int** x);
void arriveAndWait(int* wall, int i, int m);
bool hasZero(int* wall, int m);

int main() {
    //Initialization
    int m = 8;
    int n = 6;
    //Create shared memory arrays for input array, output array, temp arrays, and wall array
    key_t in = 123;
    key_t out = 456;
    key_t w = 789;
    key_t x = 246;
    int* input = createSharedIntArray(in, n);
    int* output = createSharedIntArray(out, n);
    int* wall = createSharedIntArray(w, m);
    int** temp = createSharedTempArray(x, n, m);
    //Initialize input array
    for (int i = 0; i < n; i++) {
        input[i] = i;
    }
    //Initialize wall array
    for (int i = 0; i < m; i++) {
        wall[i] = 0;
    }
    
    //Initialize temp array
    temp = new int*[m];
    for (int i = 0; i < m; i++) {
        temp[i] = new int[n];
    }
    for (int i = 0; i < n; i++) {
        temp[0][i] = input[i];
    }
    //Main process
    for (int p = 1; p < floor(log2(n)); p++) {
        for (int i = 0; i < m; i++) {
            fork(); //Forkity fork fork
            //Call Hillis and Steele Algorithm here
            hs(i, p, temp);
        }
    }
    //Copy final temp to output
    for (int i = 0; i < n; i++) {
        output[i] = temp[-1][i];
    }
    //Print output array
    for (int i = 0; i < n; i++) {
        std::cout << output[i] << std::endl;
    }
}

int* createSharedIntArray(key_t k, int size) {
    int id = shmget(k, size*sizeof(int), 0666 | IPC_CREAT);
    return (int*)shmat(id, NULL, 0);
}

int** createSharedTempArray(key_t k, int size, int core) {
    int id = shmget(k, size*core*sizeof(int), 066 | IPC_CREAT);
    return (int**)shmat(id, NULL, 0);
}

void hs(int i, int p, int** x) {
    if (i < pow(2, p - 1)) {
        x[p][i] = x[p - 1][i];
    }
    else {
        x[p][i] = x[p - 1][i - (int)pow(2, p - 1)] + x[p - 1][i];
    }
}

void arriveAndWait(int* wall, int i, int m) {
    wall[i] = 1;
    while (hasZero(wall, m)) {
        ;
    }
}

bool hasZero(int* wall, int m) {
    for (int  i = 0; i < m; i++) {
        if (wall[i] == 0) {
            return true;
        }
    }
    return false;
}
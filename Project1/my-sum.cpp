#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <bits/stdc++.h> 


using namespace std;



inline bool file_exists(const string& name);
void hAndSAlgo (int id, int processes, int (&arr)[]);
bool arriveAndWait(bool arr[], int p);
void errormsg( char *msg );
void forkProcesses(int numForks,pid_t* shared_pid); 


int main(void) {


  //1. Create variable and take in arguments
  string A = ""; // Input File name
  string B = ""; // Output File Name
  int n = 0; // num of input elements
  int m = 0; // num of cores for process

  cout << "Input File Name?: ";
  cin >> A;
  if(!file_exists(A)){
    cout << "ERROR File does not exist!";
    exit(1);
  }

  cout << "Output File Name?: ";
  cin >> B;

  cout << "Number of elements being put in Array?: ";
  cin >> n;
  if (n <= 0) {
   cout << "ERROR value less than 0 (zero)";
   exit(1);
  } 
  const int p = n;

  cout << "Number of cores desired for process?: ";
  cin >> m;
  if (m <= 0) {
   cout << "ERROR value less than 0 (zero)";
   exit(1); 
  }
// 1.5 Memory space Estimations:
int processes = ceil( 1 + log2 (n));
int a;
int b;
a = n * processes * sizeof(int);
b = m * sizeof(int);
const int ARRAY_MEMORY_SIZE = a;
const int BARRIER_MEMORY_SIZE = b;
const int TOTAL_MEMORY_SIZE = ARRAY_MEMORY_SIZE + BARRIER_MEMORY_SIZE;

  //2. populate entry array
  int* arr = new int[p];
  string line = "";
  ifstream newFile(A);
  int cnt = 0;
  int x;
  if(newFile.is_open())
    {
      string line;
      while (getline(newFile, line))
	{
	 x = stoi(line);
	 arr[cnt++] = x;
	}
      newFile.close();
    }
  else cout << "Unable to open file";
  // arr[] should now be populated
  // this has been successfully tested with for loop cout output


  //3. shared memory creation 
  int* barrierArray = new int[m];
  for (int k = 0; k < m; k++){
    barrierArray[k] = 0;
  }
  int segment_id = 0;

  segment_id = shmget( IPC_PRIVATE, TOTAL_MEMORY_SIZE, S_IRUSR | S_IWUSR );

  if ( segment_id < 0 ) errormsg( "ERROR in creating a shared memory segment\n" );

  fprintf( stdout, "1st Segment id = %d\n", segment_id );
  cout << 1;
  int *sharedArray = (int *)shmat(segment_id,NULL,0);
  cout << 2;
  if (sharedArray == (void*)-1) {
    cerr << "Failed to Attach memory segment.\n";
    shmctl(segment_id, IPC_RMID, NULL);
    exit(1); 
  }
  cout << "hello";

  memcpy(sharedArray, arr, ARRAY_MEMORY_SIZE);
  memcpy((int*)((char*)sharedArray) + ARRAY_MEMORY_SIZE, barrierArray, BARRIER_MEMORY_SIZE);
 
  int* sharedAlgoArray = sharedArray;
  int* sharedBarrArray = (int*)((char*)sharedArray + ARRAY_MEMORY_SIZE);
  
  cout << "input array \n:";
  for (int i =0; i < n; i++){
    cout << sharedAlgoArray[i] << " ";
  }
  cout << endl;
  
  cout << "barrier array \n:";
  for (int i =0; i < n; i++){
    cout << sharedBarrArray[i] << " ";
  }
  cout << endl;

  int cores;
  //4. generate m processes to handle the algorithm
  if(n == m){ // same number of cores and elements
    cores = m;
    forkProcesses(cores,sharedArray);
  }
  else if(n < m){ //more cores than elements
    cores = m - n; // reduce number of requested cores to number of needed cores for processing
    forkProcesses(cores,sharedArray);

  }
  else{ // if n > m  i.e. there are more elements than cores
    if(n % m == 0){
      cores = m; 
      forkProcesses(cores,sharedArray);
    }
    else if(n % m !=0){
      cores = m; 
      forkProcesses(cores,sharedArray);

    }
  }

  //5. run the algorithms

  //6. output results
  return 0;
}
void errormsg( char *msg ) {
   perror( msg );
   exit(1);
}
inline bool file_exists (const string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

//*7. Hillis and Steele Algorithm
void hAndSAlgo (int id, int processes, int (&arr)[]){
int n = ceil(log2(processes));


for(int p = 1;p <= n; p++){
 for(int i = 0; i < n;){
   if(i < pow(2, p-1)){
     arr[i] = arr [p-1];
   }
   else{
     int a;
     a = pow(2, p-1);
     arr[p] = arr[p-1] + arr[p-1]; //[i-a]
   }

 }
 }
}

//*8. Barrier Algorithm
int arriveAndWait(int *arr, int p, int cnt){
  int status;
  for (int i = 0; i < p; i++){
    if (arr[i] < cnt) {
      wait(&status);
    }
  }
  
  return *arr;
}

//* Process forking function
void forkProcesses(int numForks, pid_t* shared_pid) {
    for (int i = 0; i < numForks; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Fork failed: " << strerror(errno) << endl;
            exit(1);
        } else if (pid == 0) {
            // Child process
            cout << "This is child process " << i << " with PID " << getpid() << " and previous PID " << *shared_pid << endl;

            // Update the shared memory with the current child's PID
            *shared_pid = getpid();
            exit(1); // Exit to prevent child from forking again
        } else {
            // Parent process
            wait(nullptr); // Wait for the child process to complete
        }
    }
}


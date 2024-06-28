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
void hAndSAlgo (int child_id, int m, int n, int** arr, int* barrArray);
int arriveAndWait(int arr[], int m, int child_id);
void errormsg( char *msg );
void forkProcesses(int n,int numForks,pid_t* shared_pid,int** algoArray,int* barrArray); 


int main(void) {
  int debugLvl = 0;

//## 1. Create variable and take in arguments ###############################
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
  cout << "Debug Level: ";
  cin >> debugLvl;

//## 1.5 Memory space Estimations: #########################################
int processes = ceil( 1 + log2 (n));
int a;
int b;
a = n * processes * sizeof(int);
b = m * sizeof(int);
const int ARRAY_MEMORY_SIZE = a;
const int BARRIER_MEMORY_SIZE = b;
const int TOTAL_MEMORY_SIZE = ARRAY_MEMORY_SIZE + BARRIER_MEMORY_SIZE;
if(debugLvl >= 2 ){
cout << "\nn: " << n << "\n";
cout << "\nm: " << m << "\n";
cout << "\nprocesses: " << processes << "\n";
cout << "\na: " << a << "\n";
cout << "\nb: " << b << "\n";
cout << "\nArray Memory Size: " << ARRAY_MEMORY_SIZE << "\n";
cout << "\nBarrier Memory Size: " << BARRIER_MEMORY_SIZE << "\n";
cout << "\nTotal Memory Size: " << TOTAL_MEMORY_SIZE << "\n";
}
//## 2. populate entry array ###############################################
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

//## 3. shared memory creation #############################################
  int* barrierArray = new int[m];
  for (int k = 0; k < m; k++){
    barrierArray[k] = 0;
  }
  int segment_id = 0;

  segment_id = shmget( IPC_PRIVATE, TOTAL_MEMORY_SIZE, S_IRUSR | S_IWUSR );

  if ( segment_id < 0 ) errormsg( "ERROR in creating a shared memory segment\n" );
if(debugLvl >= 2 )  cout << "\n" << 1 << "\n";
  fprintf( stdout, "Segment id = %d\n", segment_id );
if(debugLvl >= 2 ){
  cout << "\n" << 2 << "\n";
  cout << "\n" << TOTAL_MEMORY_SIZE << "\n";
  errno = 0;}
  int *sharedArray = (int *)shmat(segment_id,NULL,0);
  
if(debugLvl >= 2 ){
  cout << "\n" << (void*)-1 << "\n";
  perror("Failed to attach shared memory segment");
  cout << "\nb: " << b << "\n";
  cerr << "Error code: " << errno << endl; 
  fprintf( stdout, "Pointer = %d\n", sharedArray);
  cout << 3;
}
  if (sharedArray == (void*)-1) {
    perror("Failed to Attach memory segment.\n");
    shmctl(segment_id,IPC_RMID,NULL);
    exit(1); 
  }

  memcpy(sharedArray, arr, p*sizeof(int));
  memcpy(((char*)sharedArray) + ARRAY_MEMORY_SIZE, barrierArray, BARRIER_MEMORY_SIZE);
 
  int** sharedAlgoArray = (int**)sharedArray;
  int* sharedBarrArray = (int*)((char*)sharedArray + ARRAY_MEMORY_SIZE);
 
  cout << "arr " << arr << endl;
  cout << "sharedArray " << sharedArray << endl;
  cout << "sharedAlgoArray " << sharedAlgoArray << endl;
  cout << "sharedBarrArray " << sharedBarrArray << endl;
  cout << flush;
 
/*  cout << "input array \n:";
  for (int i =0; i < n; i++){
    cout << sharedAlgoArray[0][i] << " ";
  }
  cout << endl << flush;
  
  cout << "barrier array \n:";
  for (int i =0; i < m; i++){
    cout << sharedBarrArray[i] << " ";
  }
  cout << endl;*/
  

//## 4. generate m processes to handle the algorithm #######################
  int cores; // this will hold the needed number of processes to execute the algorithm
  if(n == m){ // same number of cores and elements
    cores = m;
    forkProcesses(n,cores,sharedArray,sharedAlgoArray,sharedBarrArray);
  }
  else if(n < m){ //more cores than elements
    cores = m - n; // reduce number of requested cores to number of needed cores for processing
    forkProcesses(n,cores,sharedArray,sharedAlgoArray,sharedBarrArray);

  }
  else{ // if n > m  i.e. there are more elements than cores
    if(n % m == 0){
      cores = m;
      cout << "\ncores:" << cores <<"\n";
      forkProcesses(n,cores,sharedArray,sharedAlgoArray,sharedBarrArray);
    }
/*    else if(n  m ){
      cores = m; 
      forkProcesses(cores,sharedArray,sharedalgoarray,sharedbarrarray);

    }*/
  }
//cout << "\n" << 1 ;
/*//## 5. run the algorithms #################################################
hAndSAlgo(n, sharedAlgoArray);



//## 6. output results #####################################################*/
  return 0;
}

//## *7. Hillis and Steele Algorithm #######################################
void hAndSAlgo (int child_id, int m, int n, int** arr, int* barrArray){
  
int* arrnew = (int *) arr;

int x = ceil(log2(n));  // algorithm 'n' == processes
int arrSeg = n/m;
int start = child_id * arrSeg;
int end = (child_id == m -1) ? n : start + arrSeg;

for(int p = 1;p <= x; p++){
  
  for(int i = start; i < end; i++){ 
    if(i < pow(2, p-1)){
     *(arrnew + (p*n) + i) = *(arrnew + (p-1)*n + i);
    }
    else{
     int y = pow(2, p-1);
     *(arrnew + p*n + i) = *(arrnew + (p-1)*n + (i-y)) + *(arrnew + (p-1)*n + i);
    }
  }
  arriveAndWait(barrArray,m,child_id); // calls the barrier array function
 }
 
 for(int i = 0; i < n; ++i) {
   cout << *(arrnew + (x*n) + i) << " ";
 }
 cout << endl;

}

//## *8. Barrier Algorithm #################################################
int arriveAndWait(int *barrArray, int m, int child_id) {
  barrArray[child_id]++;
  for(int i = 0;i < m;++i){
    while(barrArray[i] < barrArray[child_id]);
    }
   
  return *barrArray;
}

//## *Process forking function #############################################
void forkProcesses(int n, int m, pid_t* shared_pid, int** algoArray, int* barrArray) {
    for (int i = 0; i < m; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Fork failed: " << strerror(errno) << endl;
            exit(1);
        } else if (pid == 0) {
            // Child process
            cout << "This is child process " << i << " with PID " << getpid() << " and previous PID " << *shared_pid << endl;
	    hAndSAlgo(i,m,n,algoArray,barrArray);
            // Update the shared memory with the current child's PID
            _exit(0); // Exit to prevent child from forking again
        }
	
    }
    for(int i = 0; i < m; ++i){
      wait(NULL);
    }
}
void errormsg( char *msg ) {
   perror( msg );
   exit(1);
}
inline bool file_exists (const string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

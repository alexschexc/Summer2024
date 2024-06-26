#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <bits/stdc++.h> 


using namespace std;

inline bool file_exists(const string& name);
void hAndSAlgo(int id, int p, int i);
bool barrier(int arr[], int n);
const int MEMORY_SIZE = 4096;
const int BUFFER_SIZE = 256;
void errormsg( char *msg );

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
    exit(0);
  }

  cout << "Output File Name?: ";
  cin >> B;

  cout << "Number of elements being put in Array?: ";
  cin >> n;
  if (n <= 0) {
   cout << "ERROR value less than 0 (zero)";
   exit(0);
  } 
  const int p = n;

  cout << "Number of cores desired for process?: ";
  cin >> m;
  if (m <= 0) {
   cout << "ERROR value less than 0 (zero)";
   exit(0); 
  }

  //2. populate entry array
  int arr[p];
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


  //3. generate m processes to handle the algorithm
  int segment_id = 0;

  char buffer[BUFFER_SIZE];

  segment_id = shmget( IPC_PRIVATE, MEMORY_SIZE, S_IRUSR | S_IWUSR );

  if ( segment_id < 0 ) errormsg( "ERROR in creating a shared memory segment\n" );

  fprintf( stdout, "Segment id = %d\n", segment_id );
  //4. run the algorithms

  //5. output results
  return 0;
}
void errormsg( char *msg ) {
   perror( msg );
   exit( 1 );
}
inline bool file_exists (const string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

//*6. Hillis and Steele Algorithm
void hAndSAlgo (int id, int p, int i){
  
}

//*7. Barrier Algorithm
void arriveAndWait(){
}

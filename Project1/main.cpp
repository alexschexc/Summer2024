#include <iostream>
#include <fstream>
//Added math library
#include <cmath>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

//Since you already did namespace std; it should just be string not std::string
inline bool file_exists (const string& name);
//Changed parameters to int id, int p, and int i based on pseudocode found in PDF
void hAndSAlgo (int id, int p, int i);
bool barrier(int arr[], int n);


int main()
{
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
  //Since it is an output file, it will not exist initially, removing the block of code
  /*if(!file_exists(B)){
    cout << "ERROR File does not exist!";
    exit(0);
  }*/

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
      while (cnt < arr[p] && newFile >> x)
	{
	 arr[cnt++] = x;
	}
      newFile.close();
    }
  else cout << "Unable to open file";

  // arr[] should now be populated

//3. generate m processes to handle the algorithm


  
//4. run the algorithms


//5. output results


  return 0;
}
//*6. Hillis and Steele Algorithm
void hAndSAlgo (int id, int p, int i){
  
}

//*7. Barrier Algorithm
//Method should be called arriveAndWait(), should be void and blocks the process
void arriveAndWait()
/*bool barrier(int arr[], int n){
  const int x = n;
  bool guardTwo[x] = {false} ;
  int cnt = 0;

  return guardTwo[];
}*/

inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

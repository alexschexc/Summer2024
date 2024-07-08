#include <iostream>



using namespace std;
void PAlTournTree(){}
void TASLock(){}
void FAILock(){}

int main(){

int lockChoice;
int n;

cout << "Make your lock selection (0 for Tournament-Tree, 1 for Test-And-Set, 2 for Fetch-And-Increment) : \n"; 

cin >> lockChoice;

cout << "Choose your \"n\" value  : \n"; 

cin >> n;

switch(lockChoice) {
  case 0: // Peterson's Algo Tournament Tree lock, each node is an instance of P's Algo

    break;
 case 1: // Test-And-Set based lock

   break;
 case 2: // Fetch-And-Increment based lock

   break;
 default:
   exit(1);

}

}
void PAlTournTree(){}
void TASLock(){}
void FAILock(){}

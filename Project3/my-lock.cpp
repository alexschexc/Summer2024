#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <cmath>
// shared counter with a large loop


void PAlTournTree(int n, int numIncrements);
void TASLock(int n, int numIncrements);
void FAILock(int n,int numIncrements);
int smallestPowerOf2(int n);
void treeAcquire (std::vector<std::atomic<bool>>& flag, std::vector<std::atomic<int>>& turn, int myid, int treeSize);
void treeRelease(std::vector<std::atomic<bool>>& flag, int myid, int treeSize);

int main(){
int lockChoice;
int n;

std::cout << "Make your lock selection (0 for Tournament-Tree, 1 for Test-And-Set, 2 for Fetch-And-Increment) : \n"; 

std::cin >> lockChoice;


const int numIncrements = 1;
switch(lockChoice) {
 case 0: // Peterson's Algo Tournament Tree lock, each node is an instance of P's Algo
   std::cout << "Choose your \"n\" value  : \n"; 
   std::cin >> n;

   PAlTournTree(n, numIncrements);
   break;
 case 1: // Test-And-Set based lock
   std::cout << "Choose your \"n\" value  : \n"; 
   std::cin >> n;

   TASLock(n, numIncrements);
   break;
 case 2: // Fetch-And-Increment based lock
   std::cout << "Choose your \"n\" value  : \n"; 
   std::cin >> n;

   FAILock(n, numIncrements);
   break;
 default:
   std::cerr << "Invalid lock Choice" << std:: endl;
   exit(1);

}

}
void PAlTournTree(int n, int numIncrements){
  int elements = smallestPowerOf2(n) - 1;
  std::vector<std::atomic<bool>> flag(elements);
  std::vector<std::atomic<int>> turn (elements);
  
  // init flag and turn vectors
  for(auto& f : flag) {
    f.store(false, std::memory_order_relaxed);
  }
  for(auto& t : turn) {
    t.store(0, std::memory_order_relaxed);
  }

  int counter = 0;
  std::vector<std::thread> threads;

  auto criticalSection = [&flag, &turn, &counter, elements](int myid){
    treeAcquire(flag, turn, myid, elements);
    // Critical Section
    ++counter;
    treeRelease(flag, myid, elements);
  };

  for(int i = 0; i < n; ++i){
    threads.push_back(std::thread([&, i]() {
      for (int j = 0; j < numIncrements; ++j){
	criticalSection(i % 2);
      }
    }));
  }
  for(auto& thread : threads){
    thread.join();
  }
  std::cout << "Final counter value: " << counter << std::endl;
}
void TASLock(int n, int numIncrements){
  std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;
  int counter = 0;
  std::vector<std::thread> threads;

  auto criticalSection = [&lock_flag, &counter]() {
    while(lock_flag.test_and_set(std::memory_order_acquire)){
    }
    ++counter;
    lock_flag.clear(std::memory_order_release);
  };

  for(int i = 0; i < n; ++i){
    threads.push_back(std::thread([criticalSection, numIncrements](){
      for (int j = 0; j < numIncrements; ++j){
	criticalSection();
      }
    }));
  }
  
  for (auto& thread : threads) {
    thread.join();
  }

  std::cout << "Final counter value: " << counter << std::endl;
}
void FAILock(int n, int numIncrements){
  std::atomic<int> counter{0};
  int final_counter = 0;
  std::vector<std::thread> threads;

  auto criticalSection = [&counter, &final_counter]() {
    counter.fetch_add(1, std::memory_order_acquire);
    ++final_counter;
  };
  
  for (int i = 0; i < n; ++i) {
    threads.push_back(std::thread([criticalSection, numIncrements]() {
      for (int j = 0; j < numIncrements; ++j) {
	criticalSection();
      }
    }));
  }

  for (auto& thread : threads){
    thread.join();
  }
  std::cout << "Final counter value: " << final_counter << std::endl;
}

int smallestPowerOf2(int n){
  if(n < 1) return 0;

  int res = 1;
  while(res < n){
    res <<= 1;
  }
  return res;

}

void treeAcquire (std::vector<std::atomic<bool>>& flag, std::vector<std::atomic<int>>& turn, int myid, int treeSize){
  int node = myid + treeSize;
  while (node > 0) {
    flag[node] = true;
    int parent = (node - 1) / 2;
    turn[parent] = node & 2;
    while(flag[parent * 2 + 1 - node % 2] && turn[parent] == node % 2){
      std::this_thread::yield();
    }
    node = parent;
  
  }
}

void treeRelease(std::vector<std::atomic<bool>>& flag, int myid, int treeSize){
  int node = myid + treeSize;
  while(node > 0){
    flag[node] = false;
    node = (node -1) / 2;
  }

}

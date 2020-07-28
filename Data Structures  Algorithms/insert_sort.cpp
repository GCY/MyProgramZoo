#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <random>

void insert_sort(std::vector<int> &array)
{
   for(int i = 1;i < array.size();++i){
      int current = array[i];
      int j = i - 1;
      while(array[j] > current && j >= 0){
	 array[j+1] = array[j];
	 --j;
      }
      array[j+1] = current;
   }
}

int main( int argc, char** argv )
{
   std::random_device rd;
   std::default_random_engine gen = std::default_random_engine(rd());
   std::uniform_int_distribution<int> dis(1,100);


   std::vector<int> array;
   array.reserve(30);

   for(size_t i = 0;i < array.capacity();++i){
      array.push_back(dis(gen));
   }

   for(auto &iter : array){
      std::cout << iter << " ";
   }
   std::cout << std::endl;

   auto start = std::chrono::steady_clock::now();
   insert_sort(array);
   auto end = std::chrono::steady_clock::now();

   for(auto &iter : array){
      std::cout << iter << " ";
   }
   std::cout << std::endl;   

   std::chrono::duration<double> elapsed_seconds = end-start;

   std::cout << "insert_sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed_seconds).count() << "us" << std::endl;   

   return 0;
}

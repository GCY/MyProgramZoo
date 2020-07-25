#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <random>

void bouble_sort(std::vector<int> && array)
{
   for(size_t i = 0;i < array.size();++i){
      for(size_t j = 0;j < array.size()-1;++j){
	 if(array[j] > array[j+1]){
	    array[j] ^= array[j+1];
	    array[j+1] ^= array[j];
	    array[j] ^= array[j+1];
	 }

      }
   }
}

int main( int argc, char** argv )
{
   std::random_device rd;
   std::default_random_engine gen = std::default_random_engine(rd());
   std::uniform_int_distribution<int> dis(1,100);


   std::vector<int> array;
   array.reserve(20);

   for(size_t i = 0;i < array.capacity();++i){
      array.push_back(dis(gen));
   }

   for(auto &iter : array){
      std::cout << iter << " ";
   }
   std::cout << std::endl;

   auto start = std::chrono::steady_clock::now();
   bouble_sort(std::move(array));
   auto end = std::chrono::steady_clock::now();

   std::chrono::duration<double> elapsed_seconds = end-start;

   std::cout << "bouble_sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed_seconds).count() << "us" << std::endl;   

   for(auto &iter : array){
      std::cout << iter << " ";
   }
   std::cout << std::endl;
   

   return 0;
}

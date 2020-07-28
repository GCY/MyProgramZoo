#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <random>

void swap(std::vector<int> &array,int l,int r)
{
   int temp = array[r];
   array[l] ^= temp;
   temp ^= array[l];
   array[l] ^= temp;
   array[r] = temp;
}

int32_t Partition_v1(std::vector<int> &array,uint32_t front,uint32_t end)
{
   int32_t pivot = front + ((end - front) / 2);
   int32_t pivot_value = array[pivot];
   int32_t i = front;
   int32_t j = end;

   while(i <= j){
      while(array[i] < pivot_value){
	 ++i;
      }
      while(array[j] > pivot_value){
	 --j;
      }
      if(i <= j){
	 swap(array,i,j);
	 ++i;
	 --j;
      }
   }

   return i;
}

void quick_sort_v1(std::vector<int> &array,uint32_t front,uint32_t end)
{
   if(front < end){
      int32_t pivot = Partition_v1(array,front,end);
      quick_sort_v1(array,front,pivot-1);
      quick_sort_v1(array,pivot,end);
   }
}

int32_t Partition_v2(std::vector<int> &array,uint32_t front,uint32_t end)
{
   uint32_t pivot_value = array[end];
   int i = (front-1);
   for(int j = front;j <= end-1;++j){
      if(array[j] <= pivot_value){
	 ++i;
	 swap(array,i,j);
      }
   }

   ++i;
   swap(array,i,end);
   return i;
}

void quick_sort_v2(std::vector<int> &array,uint32_t front,uint32_t end)
{
   if(front < end){
      int32_t pivot = Partition_v1(array,front,end);
      quick_sort_v2(array,front,pivot-1);
      quick_sort_v2(array,pivot,end);
   }
}

int main( int argc, char** argv )
{
   std::random_device rd;
   std::default_random_engine gen = std::default_random_engine(rd());
   std::uniform_int_distribution<int> dis(1,100);


   std::vector<int> array;
   array.reserve(10);

   for(size_t i = 0;i < array.capacity();++i){
      array.push_back(dis(gen));
   }

   for(auto &iter : array){
      std::cout << iter << " ";
   }
   std::cout << std::endl;

   auto start = std::chrono::steady_clock::now();
   //quick_sort_v1(array,0,array.size()-1);
   quick_sort_v2(array,0,array.size()-1);
   auto end = std::chrono::steady_clock::now();

   for(auto &iter : array){
      std::cout << iter << " ";
   }
   std::cout << std::endl;   

   std::chrono::duration<double> elapsed_seconds = end-start;

   std::cout << "quick_sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed_seconds).count() << "us" << std::endl;   

   return 0;
}

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <random>

const int Max = 9999999;

void merge(std::vector<int> &Array, int front, int mid, int end){

   // 利用 std::vector 的constructor, 
   // 把array[front]~array[mid]放進 LeftSub[]
   // 把array[mid+1]~array[end]放進 RightSub[]
   std::vector<int> LeftSub(Array.begin()+front, Array.begin()+mid+1),
      RightSub(Array.begin()+mid+1, Array.begin()+end+1);

   LeftSub.insert(LeftSub.end(), Max);      // 在LeftSub[]尾端加入值為 Max 的元素
   RightSub.insert(RightSub.end(), Max);    // 在RightSub[]尾端加入值為 Max 的元素

   int idxLeft = 0, idxRight = 0;

   for (int i = front; i <= end; i++) {

      if (LeftSub[idxLeft] <= RightSub[idxRight] ) {
	 Array[i] = LeftSub[idxLeft];
	 idxLeft++;
      }
      else{
	 Array[i] = RightSub[idxRight];
	 idxRight++;
      }
   }
}

void merge_sort(std::vector<int> &array, int front, int end){
   // front與end為矩陣範圍
   if (front < end) {                   // 表示目前的矩陣範圍是有效的
      int mid = (front+end)/2;         // mid即是將矩陣對半分的index
      merge_sort(array, front, mid);    // 繼續divide矩陣的前半段subarray
      merge_sort(array, mid+1, end);    // 繼續divide矩陣的後半段subarray
      merge(array, front, mid, end);   // 將兩個subarray做比較, 並合併出排序後的矩陣
   }
}

int main( int argc, char** argv )
{
   std::random_device rd;
   std::default_random_engine gen = std::default_random_engine(rd());
   std::uniform_int_distribution<int> dis(1,10000);


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
   merge_sort(array,0,array.size()-1);
   auto end = std::chrono::steady_clock::now();

   for(auto &iter : array){
      std::cout << iter << " ";
   }
   std::cout << std::endl;   

   std::chrono::duration<double> elapsed_seconds = end-start;

   std::cout << "merge_sort time: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed_seconds).count() << "us" << std::endl;   

   return 0;
}

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <vector>

const static uint32_t DEFAULT_SIZE = 10;

template<typename T>
class CircularQueue
{
   public:
      CircularQueue(){}
      CircularQueue(uint32_t size):array(new T[size]),capacity(size){}

      ~CircularQueue(){delete [] array;}

      CircularQueue(const CircularQueue&) = delete;
      CircularQueue& operator=(const CircularQueue&) = delete;

      uint32_t GetCapacity(){
	 return capacity;
      }
      uint32_t GetSize(){
	 return size;
      }  

      bool Push(T data){

	 if(IsFull()){ReAlloc();}
	 ++size;

	 back = (back + 1)%capacity;
	 array[back] = data;

	 return true;
      }

      uint64_t Pop(){
	 if(IsEmpty()){return -1;}
	 --size;

	 front = (front + 1)%capacity;

	 uint64_t return_value = array[front];
	 array[front] = 0;

	 return return_value;
      }

      bool IsEmpty(){
	 return (front == back);
      }

      bool IsFull(){
	 return (((back + 1) % capacity) == front);
      }

      int64_t At(uint32_t index){
	 if(index > size){return -1;}
	 return array[index];
      }

      uint32_t GetUseSize(){
	 uint32_t use_size;
	 if (front < back) {
	    use_size = back - front;
	 }
	 else {
	    use_size = capacity - (front - back);
	 }

	 return use_size;	 
      }

      void ReAlloc(){
	 T *temp = new T[capacity * 2];

	 uint32_t i = front , j = 1;
	 for(; j <= GetUseSize();++j){
	    temp[j] = array[++i % capacity];
	 }

	 delete [] array;
	 array = temp;

	 front = 0;
	 back = GetUseSize();
	 capacity *= 2;
      }

   private:
      uint32_t capacity{DEFAULT_SIZE};
      uint32_t size{0};

      uint32_t front{0};
      uint32_t back{0};
      T *array{ []()->auto{return new T[DEFAULT_SIZE];}() };
};

int main( int argc, char** argv )
{
   
   std::ios::sync_with_stdio(false);
   std::cin.tie(0);

   CircularQueue<uint32_t> queue(5);

   std::cout << queue.GetCapacity() << " " << queue.GetSize() << std::endl;

   for(int i = 0;i < queue.GetCapacity();++i){
      std::cout << queue.At(i) << std::endl;
   }

   std::cout << "================" << std::endl;

   for(int i = 1;i <= queue.GetCapacity()-1;++i){
      queue.Push(i*i);
   }

   for(int i = 0;i < queue.GetCapacity();++i){
      std::cout << queue.At(i) << std::endl;
   }

   std::cout << "================" << std::endl;

   std::cout << "Pop: " << queue.Pop() << std::endl;
   std::cout << "Pop: " << queue.Pop() << std::endl;
   std::cout << "Pop: " << queue.Pop() << std::endl;
   std::cout << "Pop: " << queue.Pop() << std::endl;

   std::cout << "Push 22,33,44" << std::endl;
   queue.Push(22);
   queue.Push(33);
   queue.Push(44);
   queue.Push(55);

   for(int i = 0;i < queue.GetCapacity();++i){
      std::cout << queue.At(i) << std::endl;
   }

   std::cout << "================" << std::endl;
   std::cout << "Push 66,77,88,99" << std::endl;
   queue.Push(66);
   queue.Push(77);
   queue.Push(88);
   queue.Push(99);
   for(int i = 0;i < queue.GetCapacity();++i){
      std::cout << queue.At(i) << std::endl;
   }

   return 0;
}

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <vector>

const static uint32_t DEFAULT_SIZE = 10;

template<typename T>
class ArrayStack
{
   public:
      ArrayStack(){}
      ArrayStack(uint32_t size):array(new T[size]),capacity(size){}

      ~ArrayStack(){delete [] array;}

      ArrayStack(const ArrayStack&) = delete;
      ArrayStack& operator=(const ArrayStack&) = delete;

      void Push(T data){
	 if(size == capacity){ReAlloc();}
	 ++size;
	 array[++top] = data;
      }

      int64_t Pop(){
	 if(top == -1){return -1;}
	 --size;
	 return array[top--];
      }

      int64_t At(uint32_t index){
	 if(index >= size){return -1;}
	 return array[index];
      }

      T GetTop(){
	 if(top == -1){return; }
	 return array[top];
      }
      uint32_t GetCapacity(){
	 return capacity;
      }
      uint32_t GetSize(){
	 return size;
      }    

      bool IsEmpty(){return (size == 0);}

      void ReAlloc(){
	 capacity *= 2;
	 T *temp = new T[capacity];
	 
	 for(uint32_t i = 0;i < size;++i){
	    temp[i] = array[i];
	 }

	 delete [] array;
	 array = temp;
      }

   private:
      uint32_t capacity{DEFAULT_SIZE};
      uint32_t size{0};
      int32_t top{-1};      
      T *array{ []()->auto{return new T[DEFAULT_SIZE];}() };
};

int main( int argc, char** argv )
{
   std::ios::sync_with_stdio(false);
   std::cin.tie(0);

   ArrayStack<uint32_t> stack{15};

   for(int i = 1;i <= stack.GetCapacity();++i){
      stack.Push(i*i);
   }

   std::cout << stack.GetCapacity() << " " << stack.GetSize() << std::endl;
   std::cout << "================" << std::endl;
   stack.Push(9999);
   std::cout << "Push 9999 : " << stack.GetCapacity() << " " << stack.GetSize() << std::endl;
   std::cout << "================" << std::endl;

   for(uint32_t i = 0; i < stack.GetSize();++i){
      std::cout << stack.At(i) << std::endl;
   }

   std::cout << "================" << std::endl;
   std::cout << "Pop: " << stack.Pop() << std::endl;
   std::cout << stack.GetCapacity() << " " << stack.GetSize() << std::endl;
   std::cout << "================" << std::endl;

   while(!stack.IsEmpty()){
      std::cout << stack.Pop() << std::endl;
   }
   std::cout << stack.GetCapacity() << " " << stack.GetSize() << std::endl;

   return 0;
}

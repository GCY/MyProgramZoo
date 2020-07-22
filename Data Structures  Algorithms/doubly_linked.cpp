#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <vector>

template<typename T>
class Node
{
   public: 
      Node(){};
      Node(T data){this->data = data;}   
      
      T GetData(){return data;}
      Node<T>* GetAddress(){return this;}

      Node<T> *prev{ [](){return nullptr;}() };
      Node<T> *next{ [](){return nullptr;}() };
      T data{ []()->T{return T();}() };
};

template<typename T>
class Link
{
   public:

      Link(){}

      ~Link(){
	 Node<T> *current = first;
	 while(current != nullptr){
	    Node<T> *temp = current->next;
	    delete current;
	    current = temp;
	 }
      }

      Link(const Link&) = delete;
      void operator=(const Link&) = delete;

      void PushBack(T data) noexcept;
      void PushFront(T data) noexcept;

      void Insert(uint32_t index,T data) noexcept;

      void Reverse() noexcept;

      uint32_t GetSize() noexcept{return size;}

      Node<T>* At(uint32_t index) noexcept;

      void Delete(uint32_t index) noexcept;

   private:
      Node<T> *first{ [](){return nullptr;}() };
      Node<T> *last{ [](){return nullptr;}() };
      uint32_t size{0};
};

template<typename T>
void Link<T>::PushBack(T data) noexcept
{
   ++size;
   Node<T>* new_node = new Node<T>(data);
   if(first == nullptr){
      first = new_node;
      return ;
   }

   Node<T>* current = first;
   while(current->next != nullptr){
      current = current->next;
   }
   current->next = new_node;
   new_node->prev = current;
   last = new_node;
   return ;
}

template<typename T>
void Link<T>::PushFront(T data) noexcept
{
   ++size;
   Node<T> *new_node = new Node<T>(data);
   new_node->next = first;
   first->prev = new_node;
   first = new_node;
   return ;
}


template<typename T>
Node<T>* Link<T>::At(uint32_t index) noexcept
{
   if(index >= size){return nullptr;}

   if(index < (size/2)){
      Node<T> *current = first;
      for(;index;--index){
	 current = current->next;
      }
      return current;
   }
   else{
      Node<T> *current = last;
      uint32_t r_index = (size-1) - index;
      for(;r_index;--r_index){
	 current = current->prev;
      }
      return current;
   }
   
}

template<typename T>
void Link<T>::Delete(uint32_t index) noexcept
{
   if(index >= size){return ;}

   Node<T> *current = nullptr;
   if(index < (size/2)){
      current = first;
      for(;index;--index){
	 current = current->next;
      }
   }
   else{
      current = last;
      uint32_t r_index = (size-1) - index;
      for(;r_index;--r_index){
	 current = current->prev;
      }
   }

   if(current->prev != nullptr && current->next != nullptr){
      current->prev->next = current->next;
   }
   else if(current->prev == nullptr){
      first = current->next;
      current->next->prev = nullptr;
   }
   else if(current->next == nullptr){
      last = current->prev;
      current->prev->next = nullptr;
   }

   delete current;
   --size;
   
}

template<typename T>
void Link<T>::Reverse() noexcept
{
   
   if(first == nullptr || first->next == nullptr){return;}

   Node<T> *prev = nullptr;
   Node<T> *current = first;
   Node<T> *next = first->next;
   last = first;
   while(current->next != nullptr){
      current->next = prev;
      current->prev = next;
      prev = current;
      current = next;
      next = next->next;
   }

   current->next = prev;
   first = current;
}

template<typename T>
void Link<T>::Insert(uint32_t index,T data) noexcept
{
   
   if(index >= size){return ;}
   Node<T> *new_node = new Node<T>(data);

   if(index == 0){
      new_node->next = first;
      first = new_node;
      return ;
   }
   if(index == size-1){
      new_node->prev = last;
      last = new_node;
      return ;
   }

   Node<T> *current = nullptr;
   if(index < (size/2)){
      current = first;
      for(;index;--index){
	 current = current->next;
      }
   }
   else{
      current = last;
      uint32_t r_index = (size-1) - index;
      for(;r_index;--r_index){
	 current = current->prev;
      }
   } 

   new_node->prev = current->prev;
   new_node->next = current;
   current->prev = new_node;
   new_node->prev->next = new_node;

/*
   Node<T> *prev = nullptr;
   Node<T> *current = first;
   for(;index;--index){
      prev = current;
      current = current->next;
   }

   prev->next = new_node;
   new_node->next = current;
  */ 
}


int main( int argc, char** argv )
{
   Link<uint32_t> link;
   //std::cout << link.GetData() << " " << link.GetAddress() << std::endl;
   for(uint32_t i = 1;i <= 10;++i){
      link.PushBack(i*i);
   }
   for(uint32_t i = 1;i <= 10;++i){
      link.PushFront(i+i);
   }   

   for(uint32_t i = 0;i < link.GetSize();++i){
      std::cout << i << ". " << link.At(i)->GetData() << " , " << link.At(i)->GetAddress() << std::endl;
   }

   std::cout << "================" << std::endl;

   link.Delete(0);
   link.Delete(4);
   link.Delete(link.GetSize()-1);
   for(uint32_t i = 0;i < link.GetSize();++i){
      std::cout << i << ". "  << link.At(i)->GetData() << " , " << link.At(i)->GetAddress() << std::endl;
   }

   std::cout << "================" << std::endl;

   link.Reverse();
   link.PushFront(99999);
   link.Insert(5,66666);
   link.Insert(0,33333);
   link.Insert(link.GetSize()-1,88888);

   for(uint32_t i = 0;i < link.GetSize();++i){
      std::cout << i << ". "  << link.At(i)->GetData() << " , " << link.At(i)->GetAddress() << std::endl;
   }
   
}

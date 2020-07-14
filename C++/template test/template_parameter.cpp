#include <iostream>
#include <vector>
#include <deque>

template<typename RET,typename T>
RET Compare(T p1,T p2)
{
   return (p1 > p2);
}

template <typename T,template<typename ELEM,typename ALLOC = std::allocator<ELEM> > class CONT = std::vector>
class TT
{
   public:
      TT(){}
      void Reserve(unsigned int);
   private:
      CONT<T> data;
};

template <typename T,template<typename,typename> class CONT>
void TT<T,CONT>::Reserve(unsigned int size)
{
   data.resize(size,T());
}

int main(int argc,char**argv)
{
   /*function template return and parameter*/
   std::cout << Compare<bool>(12,22) << std::endl;  

   /*TT*/
   TT<int,std::deque> tt;
   tt.Reserve(100);   
}

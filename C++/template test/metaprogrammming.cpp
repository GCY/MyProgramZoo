#include <iostream>
#include <vector>
#include <deque>

template<unsigned long N>
class MetaFibonacci
{
   public:
      static const unsigned long val = MetaFibonacci<N-1>::val + MetaFibonacci<N-2>::val;
};

template<>
class MetaFibonacci<1>
{
   public:
      static const unsigned long val = 1;
};

template<>
class MetaFibonacci<2>
{
   public:
      static const unsigned long val = 1;
};

unsigned long Fibonacci(unsigned long val)
{
   //if(val == 0){return 0;}
   if(val <= 2){return 1;}
   return Fibonacci(val - 2) + Fibonacci(val - 1);
}

int main(int argc,char**argv)
{
   /*Metaprogrammming*/
   unsigned long val;
   std::cin >> val;
   std::cout << Fibonacci(val) << std::endl;
   std::cout << MetaFibonacci<19>::val << std::endl;    
}

#include <iostream>

#include "boost/asio/io_context.hpp"

#define BIND_FUNC(FUNC,n) FUNC##n

#define PRINT(str)	\
   std::cout << str << std::endl;

typedef int (*FP)(int);

int display1(int n)
{
   PRINT(std::string("display 1 :") + std::to_string(n));
   return n;
}

int display2(int n)
{
   PRINT(std::string("display 2 :") + std::to_string(n));
   return n;
}


class OBJ1
{
   public:
      OBJ1(){PRINT("OBJ1 construct");}
};

class OBJ2
{
   public:
      OBJ2(){PRINT("OBJ2 construct");}
};

class OBJ3:public OBJ2
{
   public:
      OBJ3(){PRINT("OBJ3 construct");}
   private:
      OBJ1 obj1;
};

class COOL
{
   public:
      COOL& Cool(){
	 PRINT("COOL");
	 return *this;
      }
};

class V1
{
   public:
      virtual void FUNC() = 0;
      //~V1(){PRINT("V1 deconstruct");}
      virtual ~V1(){PRINT("V1 deconstruct");}
};

class V2:public V1
{
   public:
      virtual void FUNC(){}
      ~V2(){PRINT("V2 deconstruct");}
};

int main(int argc,char**argv)
{
   std::ios::sync_with_stdio(false);
   std::cin.tie(0);

   // ch6
   OBJ3 obj3;

   //ch7
   static const uint32_t SIZE = 10;
   //int (**fpp)(int) = new (int (*[SIZE])(int));
   int (**fpp)(int) = new FP[SIZE];
   for(int i = 0;i < SIZE;i++){
      uint32_t id = i%2;
      if(id){
	 fpp[i] = BIND_FUNC(display,2);
      }
      else{
	 fpp[i] = BIND_FUNC(display,1);
      }
   }
   for(int i = 0;i < SIZE;i++){
      fpp[i](i);
   }  
   delete [] fpp;

   COOL cool;
   cool.Cool().Cool();

   V1 *v = new V2;
   delete v;


  boost::asio::io_context io_context;
  io_context.run();
  
  return 0;
}

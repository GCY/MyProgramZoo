#include <iostream>
#include <vector>
#include <deque>

template<typename T>
T g(T , T x = 30)
{
   return x;
}
template<typename T>
T g(bool,T x)
{
   return x;
}
template<>
int g(int,int y)
{
   return y/2;
}

class Animal {
 public:
    virtual void say () const = 0;
    virtual ~Animal() {}
};

template <typename T>
class Animal_CRTP: public Animal {
 public:
    void say() const override {
        static_cast<const T*>(this)->say();
    }
};

class Cat: public Animal_CRTP<Cat> {
 public:
    void say() const {
        std::cout << "Meow~ I'm a cat." << std::endl;
    }
};

class Dog: public Animal_CRTP<Dog> {
 public:
    void say() const {
       std::cout << "Wang~ I'm a dog." << std::endl;
    }
};

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

template<typename T>
class FT
{
   public:
      FT(T val):m_impl(new Impl(val)){}
      T  GetM(){return m_impl->Getm_count();}
   private:
      struct Impl;
      std::unique_ptr<Impl> m_impl;   
   
};

template<typename T>
struct FT<T>::Impl
{
   public:
      int m_count;
      Impl(int);
      ~Impl();   
      int Getm_count();
};

template<typename T>
FT<T>::Impl::Impl(int val):m_count(val)
{
}

template<typename T>
FT<T>::Impl::~Impl()
{
}

template<typename T>
int FT<T>::Impl::Getm_count()
{
   return m_count;
}

template<typename T>
class FO
{
   public:
      FO(T val):data(val){}
      void operator()(){++data;}
      virtual void get2(){std::cout << "get2\n";}
      virtual void get3(){std::cout << "get3\n";}
      virtual T get(){return data;}
      virtual ~FO(){};

   private:
      T data;
};

class OBJECT1
{
   public:
      typedef int data;
};
class OBJECT2
{
   public:
      typedef int val;
};
typedef const char RT1;
typedef struct
{
   char c[2];
}RT2;
template<typename T> RT1 test(typename T::data const &data){}
template<typename T> RT2 test(...){}
#define type_has_member_type_data(T) \
   (sizeof(test<T>(0)) == 1)

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

    /*template default parameter recover*/
    std::cout << g(30,40) << std::endl;
    std::cout << g(10) << std::endl;
    std::cout << g(true) << std::endl;   

   /*CRTP*/
    std::vector<Animal*> zoo;
    zoo.push_back(new Cat());
    zoo.push_back(new Dog());
    for (std::vector<Animal*>::const_iterator iter = zoo.begin(); iter != zoo.end(); ++iter) {
        (*iter)->say();
    }
    for (std::vector<Animal*>::iterator iter = zoo.begin(); iter != zoo.end(); ++iter) {
        delete (*iter);
    }     

   /*Metaprogrammming*/
   unsigned long val;
   std::cin >> val;
   std::cout << Fibonacci(val) << std::endl;
   std::cout << MetaFibonacci<19>::val << std::endl;    

   /*PIMPL*/
   FT<int> ft(16);
   std::cout << ft.GetM() << std::endl;

   /*Size of Objects*/
   std::cout << sizeof(unsigned int) << " " << sizeof(int) << std::endl;
   std::cout << sizeof(FO<unsigned int>) << " " << sizeof(FT<int>) << std::endl;

   /*SFINAE*/
   std::cout << std::endl << type_has_member_type_data(OBJECT1) << std::endl;  

   /*function template return and parameter*/
   std::cout << Compare<bool>(12,22) << std::endl;  

   /*TT*/
   TT<int,std::deque> tt;
   tt.Reserve(100);   
}

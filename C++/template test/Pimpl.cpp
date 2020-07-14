#include <iostream>
#include <vector>
#include <deque>

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

int main(int argc,char**argv)
{
   /*PIMPL*/
   FT<int> ft(16);
   std::cout << ft.GetM() << std::endl;

   /*Size of Objects*/
   std::cout << sizeof(unsigned int) << " " << sizeof(int) << std::endl;
   std::cout << sizeof(FO<unsigned int>) << " " << sizeof(FT<int>) << std::endl;
}

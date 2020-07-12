#include <iostream>

using namespace std;

// g++ -o2 -o -std=c++14

// stddef.h
/*
#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif
*/

template<typename Type1,typename ptrType>
void test(Type1 fun,ptrType ptr)
{
   /*do something*/
   fun(ptr);
   return;
}
void fun(int *val)
{
   cout<<"fun"<<endl;
}


void test(void *p)
{
   cout<<"p is pointer "<<p<<endl;
}
void test(int num)
{
   cout<<"num is int "<<num<<endl;
}

int main()
{
   // error: cannot initialize a variable of type 'int *' with an rvalue of type 'void *'
   /*
      char p[] = "12345";
      int *a = (void*)p;
      */

   // call to 'test' is ambiguous
   //test(NULL);


   // cannot initialize a parameter of type 'int *' with an lvalue of type 'long'
   //test(fun,NULL);


   test(nullptr);

   test(fun,nullptr);

   return 0;
}

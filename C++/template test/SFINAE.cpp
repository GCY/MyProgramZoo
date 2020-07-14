#include <iostream>
#include <vector>
#include <deque>

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



int main(int argc,char**argv)
{
   /*SFINAE*/
   std::cout << std::endl << type_has_member_type_data(OBJECT1) << std::endl;  
}

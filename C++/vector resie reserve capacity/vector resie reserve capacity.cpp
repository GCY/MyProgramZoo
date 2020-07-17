#include <iostream>
#include <vector>

class OBJ
{
   public:
      OBJ(){++value;std::cout << "construct:" << value << std::endl;}
      OBJ(const OBJ &obj){std::cout << "copy:" << value << std::endl;}
   private:
      static uint32_t value;
};

uint32_t OBJ::value = 0;

int main(void)
{
   std::vector<OBJ> v1;
   std::cout << "capacity:" << v1.capacity() << std::endl;
   v1.reserve(2);
   std::cout << "capacity:" << v1.capacity() << std::endl;
   std::cout << "==1==" << std::endl;
   v1.push_back(OBJ());
   v1.push_back(OBJ());
   v1.push_back(OBJ());
   std::cout << "capacity:" << v1.capacity() << std::endl;
   std::cout << "==2==" << std::endl;
   v1.resize(11);
   std::cout << "capacity:" << v1.capacity() << std::endl;
   std::cout << "==3==" << std::endl;
   return 0;
}

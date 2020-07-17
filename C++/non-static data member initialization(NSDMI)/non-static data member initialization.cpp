#include <iostream>
#include <vector>
#include <string>

// NSDMI and CTAD

class OtherType {
   public:
      static const int value = 10;
      static inline std::string className = "Hello Class"; // inline c++17
      static inline std::vector<int> s{ 1, 2, 3, 4, 5, 6, 7}; // inline c++17
      static inline auto theMeaningOfLife = 42; // inline c++17

      OtherType() { }
};

int initA() {
   std::cout << "initA() called\n";
   return 1;
}

std::string initB() {
   std::cout << "initB() called\n";
   return "Hello";
}

class SimpleType
{
   public:
      int a { initA() }; 
      std::string b { initB() }; 

      SimpleType() { }
      SimpleType(int x) : a(x) { }

      /*
	 SimpleType(const SimpleType& other) {
	 std::cout << "copy ctor\n";

	 a = other.a;
	 b = other.b;
	 }
	 */
      /* SimpleType(const SimpleType& other) = default; */

      SimpleType(const SimpleType& other) : a(other.a), b(other.b) {
	 std::cout << "copy ctor\n";
      }    
};

int main(void)
{
   //SimpleType obj1;
   OtherType obj2;

   std::cout << "SimpleType t10\n";    
   SimpleType t0;
   std::cout << "SimpleType t1(10)\n";    
   SimpleType t1(10);  // compiler extent SimpleType(int x):a(x),b(initB()){ } 

   std::cout << "SimpleType t2 = t1:\n";
   SimpleType t2 = t1;   
   return 0;
}

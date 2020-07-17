#include <iostream>
#include <vector>
#include <string>

// mutable example
class HashTable {
   public:

      std::string lookupInternal(const std::string& key) const
      {
	 return key + std::string("_value");
      }

      std::string lookup(const std::string& key) const
      {
	 if (key == last_key_) {
	    return last_value_;
	 }

	 std::string value{this->lookupInternal(key)};

	 last_key_   = key;
	 last_value_ = value;

	 return value;
      }

   private:
      mutable std::string last_key_;
      mutable std::string last_value_;
};

   template <typename Functor>  
void f(Functor functor)  
{  
   std::cout <<__PRETTY_FUNCTION__<< std::endl;  
}  
/* Or alternatively you can use this  
   void f(std::function functor)  
   {  
   std::cout <<__PRETTY_FUNCTION__<< std::endl;  
   }   
   */  
int g() { static int i = 0; return i++; }  

class Example  
{  
   public:  
      Example(){} 
      Example(int m_var):m_var(m_var){}
      void func()  
      {  
	 //[]() { std::cout << m_var << std::endl; }(); // error: 'this' cannot be implicitly captured in this context
	 //[=]() { std::cout << m_var << std::endl; }(); // IIFE 
	 [this]() { std::cout << m_var << std::endl; }(); // IIFE
	 //[&]() { std::cout << m_var << std::endl; }(); // IIFE
      }  
   private:  
      int m_var = { [](){return 100;}() };  
};

// tempalte pack param
   template<typename T>
T sum_pack(const T& first)
{
   return first;
}
   template<typename T, typename... Args>
T sum_pack(const T& first, const Args&... args)
{
   return first + sum_pack(args...);
}

// lambda tempalte pack param
void print_pack() {}
   template <typename First>
void print_pack(const First& first)
{
   std::cout << first << std::endl;
}
   template <typename First, typename... Rest>
void print_pack(const First& first, Rest&&... args)
{
   std::cout << first << ",";
   print_pack(args...);
}

int main(void)
{
   int x = 100, y = 200; 
   auto print = [&] { // Capturing object by reference  
      std::cout <<__PRETTY_FUNCTION__<< " : " << x << " , " << y << std::endl;  
   };  
   print();  

   auto Lambda_func = [i = 0]() mutable noexcept { return i++; };  
   f(Lambda_func); // Pass Lambda  
   f(g);           // Pass function  

   Example e1,e2(22);  
   e1.func();
   e2.func();

   // mutable example
   auto f1 = [=]() mutable {x = 42;};  // okay
   //auto f2 = [=]()         {x = 42;};  // error
   std::cout << "x: " << x << std::endl;

   // lambda function point, and type check
   // You can force the compiler to generate lambda as a function pointer rather than closure by adding + infront of it as above.
   auto funcPtr = +[]{};  
   static_assert(std::is_same<decltype(funcPtr), void (*)()>::value);  

   // return lambda function
   const auto less_than = [](auto x) {  
      return [x](auto y)->auto{return y < x;};  
   };
   auto less_than_five = less_than(5);  
   std::cout << less_than_five(3) << std::endl;  
   std::cout << less_than_five(10) << std::endl; 


   // constexpr lambda expression
   constexpr auto sum = [](const auto &a, const auto &b) { return a + b; };
   constexpr int answer = sum(10, 10); 
   std::cout << "ans: " << answer << std::endl;

   std::cout << sum_pack(1, 2, 3, 4, 5) << std::endl;

   // lambda tempalte pack param
   auto variadic_generic_lambda = [](auto... param) {
      print_pack(param...);
   };
   variadic_generic_lambda(1.2, "test", 1.1); 
   //print_pack("test",1.2, 1.1, 1.1);
   return 0;  
}

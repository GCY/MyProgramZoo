#include <iostream>
#include <vector>
#include <deque>

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

   // safe lock, example BigDog
   private:
      Animal_CRTP(){}
      friend T;    
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

// default constructor of 'BigDog' is implicitly deleted because base class 'Animal_CRTP<Dog>' has an inaccessible default
/*
class BigDog: public Animal_CRTP<Dog> {
   public:
      void say() const {
	 std::cout << "Wang~ I'm a BigDog." << std::endl;
      }
};
*/
class BigDog: public Animal_CRTP<BigDog> {
   public:
      void say() const {
	 std::cout << "Wang~ I'm a BigDog." << std::endl;
      }
};

int main(int argc,char**argv)
{

   /*CRTP*/
   std::vector<Animal*> zoo;
   zoo.push_back(new Cat());
   zoo.push_back(new Dog());
   zoo.push_back(new BigDog());
   for (std::vector<Animal*>::const_iterator iter = zoo.begin(); iter != zoo.end(); ++iter) {
      (*iter)->say();
   }
   for (std::vector<Animal*>::iterator iter = zoo.begin(); iter != zoo.end(); ++iter) {
      delete (*iter);
   }     
}

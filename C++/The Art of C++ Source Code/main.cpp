#include <iostream>   
#include <new>
#include "main.h"

int main() { 
   GCPtr<int> p; 

   try { 
      p = new int; 
   } catch(bad_alloc exc) { 
      cout << "Allocation failure!\n"; 
      return 1; 
   } 

   *p = 88; 

   cout << "Value at p is: " << *p << endl; 

   int k = *p; 

   cout << "k is " << k << endl; 

   return 0; 
}

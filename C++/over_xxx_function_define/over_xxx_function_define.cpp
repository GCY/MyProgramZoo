#include <stdio.h>
#include <iostream>
class Parent
{
   public:
      void F()
      {
	 printf("Parent.F()\n");
      }
      virtual void G()
      {
	 printf("Parent.G()\n");
      }
      int Add(int x, int y)
      {
	 return x + y;
      }
      //overload Add
      float Add(float x, float y)
      {
	 return x + y;
      }
};

class ChildOne:Parent
{
   //overwrite - replace
   void F()
   {
      printf("ChildOne.F()\n"); 
   }
   //override - virtual
   void G()
   {
      printf("ChildOne.G()\n");
   }
};


int main()
{
   ChildOne childOne;// = new ChildOne();
   Parent* p = (Parent*)&childOne;

   p->F();

   p->G();
   Parent* p2 = new Parent();

   printf("%d\n",p2->Add(1, 2));
   printf("%f\n",p2->Add(3.4f, 4.5f));

   delete p2;
   return 0;
}


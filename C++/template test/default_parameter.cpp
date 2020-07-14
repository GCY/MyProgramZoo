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

int main(int argc,char**argv)
{

    /*template default parameter recover*/
    std::cout << g(30,40) << std::endl;
    std::cout << g(10) << std::endl;
    std::cout << g(true) << std::endl;   

}

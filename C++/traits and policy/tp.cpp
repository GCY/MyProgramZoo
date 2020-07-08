#include <iostream>

#include "tp.hpp"

int main(int argc,char**argv)
{
   char str[] = "gcy";
   std::cout << algorithm<char>::cal(&str[0],&str[sizeof(str)]) << std::endl;

   int val_arr[5] = {5,11,3,3,22};
   std::cout << algorithm<int>::cal(&val_arr[0],&val_arr[(sizeof(val_arr)/sizeof(int))]) << std::endl;
   std::cout << algorithm<int,MulPolicy>::cal(&val_arr[0],&val_arr[(sizeof(val_arr)/sizeof(int))],1) << std::endl;
   

   float flo_arr[5] = {55.44,11111.131231,323123.414,13123123.111,23223.22};
   std::cout << algorithm<float>::cal(&flo_arr[0],&flo_arr[(sizeof(flo_arr)/sizeof(float))]) << std::endl;   


}

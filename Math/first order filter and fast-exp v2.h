float exp_approximation1(float x) {
   x = 1.0f + x / 256.0f;
   x *= x; x *= x; x *= x; x *= x;
   x *= x; x *= x; x *= x; x *= x;
   return x;
}

float exp_approximation2(float x) {
   x = 1.0f + x / 1024.0f;
   x *= x; x *= x; x *= x; x *= x;
   x *= x; x *= x; x *= x; x *= x;
   x *= x; x *= x;
   return x;
}

bool fast_sign(float f) {
   if(f < 0){return 1;}
   else{return 0;}
}

float coeff = -0.5f; // -10ms/20ms
float y_value_n_1 = 0;
float x_value_n_1 = 0;
int8_t y_flag_n_1 = 0;
int8_t x_flag_n_1 = 0;

float FirstOrderFilter(float angle,float *n_1,int8_t *flag)
{
   int8_t temp_flag = fast_sign(angle);
   if((int8_t)*flag - temp_flag){
      *n_1 = angle;
   }
   *flag = temp_flag;

   *n_1 = *n_1 + (1.0f-exp_approximation1(coeff))*(angle-*n_1);
   return *n_1;
}

float atan2_approximation1(float y, float x)
{
   const float ONEQTR_PI = M_PI / 4.0f;
   const float THRQTR_PI = 3.0f * M_PI / 4.0f;
   float r, angle;
   float abs_y = fabs(y) + 1e-10f;
   if ( x < 0.0f )
   {
      r = (x + abs_y) / (abs_y - x);
      angle = THRQTR_PI;
   }
   else
   {
      r = (x - abs_y) / (x + abs_y);
      angle = ONEQTR_PI;
   }
   angle += (0.1963f * r * r - 0.9817f) * r;
   if ( y < 0.0f )
      return -angle; 
   else
      return angle;
}



#define PI_FLOAT     3.14159265f
#define PIBY2_FLOAT  1.5707963f

float atan2_approximation2( float y, float x )
{
   if ( x == 0.0f )
   {
      if ( y > 0.0f ) return PIBY2_FLOAT;
      if ( y == 0.0f ) return 0.0f;
      return -PIBY2_FLOAT;
   }
   float atan;
   float z = y/x;
   if ( fabs( z ) < 1.0f )
   {
      atan = z/(1.0f + 0.28f*z*z);
      if ( x < 0.0f )
      {
	 if ( y < 0.0f ) return atan - PI_FLOAT;
	 return atan + PI_FLOAT;
      }
   }
   else
   {
      atan = PIBY2_FLOAT - z/(z*z + 0.28f);
      if ( y < 0.0f ) return atan - PI_FLOAT;
   }
   return atan;
}

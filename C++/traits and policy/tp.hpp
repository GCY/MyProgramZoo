#ifndef __TP__
#define __TP__

template <typename T>
class TypeTraits;

template <>
class TypeTraits<char>
{
   public:
      typedef int Type;
      static int Zero(){return 0;}
};

template <>
class TypeTraits<int>
{
   public:
      typedef long Type;
      static long Zero(){return 0;}
};

template <>
class TypeTraits<float>
{
   public:
      typedef float Type;
      static float Zero(){return 0;}
};

template<typename T1,typename T2>
class SumPolicy
{
   public:
      static void Algo(T1 &total,const T2 &value){
	 total += value;
      }
};

template<typename T1,typename T2>
class MulPolicy
{
   public:
      static void Algo(T1 &total,const T2 &value){
	 total *= value;
      }
};


template<typename T,template<typename,typename> class Policy = SumPolicy,typename Traits = TypeTraits<T> >
class algorithm
{
   public:
      typedef typename Traits::Type Type;
      static Type cal(const T *begin,const T *end,Type init_val = Traits::Zero()){
	 Type temp = init_val;
	 while(begin != end){
	    Policy<Type,T>::Algo(temp,*begin);
	    ++begin;
	 }
	 return temp;
      }
};

#endif

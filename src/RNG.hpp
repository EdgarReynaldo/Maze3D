



#ifndef RNG_HPP
#define RNG_HPP

#include <random>

/** uses mt19937 mersenne twister from C++ 11 */


class RNG {
   
///   MT* mt;
   
public :
   typedef uint_fast32_t MTYPE;
   typedef std::mt19937 MT;

protected :
   MT wtf;

public :
   RNG() : wtf() {Seed();}
   
   void Seed(unsigned int s = 0);
   
   MTYPE Generate();
   
   unsigned int URand();
   int IRand();
   float FRand();
   double DRand();
   
};

#endif // RNG_HPP

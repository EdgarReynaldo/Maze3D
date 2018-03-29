


#include "RNG.hpp"



void RNG::Seed(unsigned int s) {
   wtf.seed(s);
   (void)s;
}



RNG::MTYPE RNG::Generate() {
   return wtf();
}



unsigned int RNG::URand() {
   double d = DRand();
   return (unsigned int)(d*UINT_MAX);
///   return wtf() & 0xffffffff;
}



int RNG::IRand() {
   return (int)URand();
}



float RNG::FRand() {
   return (float)DRand();
}



double RNG::DRand() {
   return (double)wtf()/wtf.max();
}



unsigned int RNG::Rand0toNM1(unsigned int n) {
   return URand()%n;
}



unsigned int RNG::Rand1toN(unsigned int n) {
   return Rand0toNM1(n) + 1;
}



float RNG::Percent() {
   return (FRand()+1.0f)/2.0f;
}



double RNG::DPercent() {
   return (DRand()+1.0)/2.0;
}






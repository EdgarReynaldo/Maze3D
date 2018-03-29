


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


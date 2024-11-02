



#include "Wall.hpp"



Wall::Wall() :
      face_type(NUM_FACE_TYPES),
      face_pos(this),
      face_neg(this),
      room_pos(0),
      room_neg(0),
      open_pos(false),
      open_neg(false),
      kweight(-1)
{}   



void Wall::Reset() {
   face_type = NUM_FACE_TYPES;
   face_pos.Reset();
   face_neg.Reset();
   room_pos = 0;
   room_neg = 0;
   open_pos = false;
   open_neg = false;
   kweight = -1;
}



void Wall::Display() {
   if (Open()) {return;}
   if (!open_pos) {
      face_pos.Display();
   }
   if (!open_neg) {
      face_neg.Display();
   }
}



void Wall::Outline(EagleColor col) {
   if (Open()) {return;}
   if (!open_pos) {
      face_pos.Outline(col);
   }
   if (!open_neg) {
      face_neg.Outline(col);
   }
}



void Wall::SetOpen(bool open) {
   open_pos = open_neg = open;
}



void Wall::SetWeight(int w) {
   kweight = w;
}




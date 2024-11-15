



#ifndef MAZE_WALL_HPP
#define MAZE_WALL_HPP

#include "Face.hpp"

class Room;

class Wall {
   
   friend class Maze;
   
   FACE_TYPE face_type;/// u/d , n/s , e/w
   
   Face face_pos;/// n,u,e
   Face face_neg;/// s,d,w
   
   Room* room_pos;
   Room* room_neg;
   
public :
   bool open_pos;
   bool open_neg;

   int kweight;
   
   
   Wall();
   void Reset();

   
   void SetOpen(bool open);
   void SetWeight(int w);
   
   
   void Display();
   void Outline(EagleColor col);
   
   
   Face* GetFacePos() {return &face_pos;}
   Face* GetFaceNeg() {return &face_neg;}
   
   Room* GetRoomPos() {return room_pos;}
   Room* GetRoomNeg() {return room_neg;}

   bool OpenPos() {return open_pos;}
   bool OpenNeg() {return open_neg;}
   bool Open() {return open_pos && open_neg;}
   
   int Weight() {return kweight;}
   
   FACE_TYPE GetWallDirection() {return face_type;}
   
   
};




#endif // MAZE_WALL_HPP

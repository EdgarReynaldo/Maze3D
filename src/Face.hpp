



#ifndef Face_HPP
#define Face_HPP

#include "Eagle.hpp"
#include "Eagle/GraphicsContext.hpp"

#include "GL/gl.h"


enum ROOM_FACE {
   ROOM_ABOVE = 0,
   ROOM_BELOW = 1,
   ROOM_NORTH = 2,
   ROOM_SOUTH = 3,
   ROOM_EAST  = 4,
   ROOM_WEST  = 5,
   NUM_ROOM_FACES = 6
};

ROOM_FACE OppositeFace(ROOM_FACE f);

enum FACE_TYPE {
   FACE_UPDOWN = 0,
   FACE_NORTHSOUTH = 1,
   FACE_EASTWEST = 2,
   NUM_FACE_TYPES = 3
};

FACE_TYPE GetFaceType(ROOM_FACE rf);

enum ROOM_DIRECTION {
   ROOM_POSITIVE = 0,
   ROOM_NEGATIVE = 1,
   ROOM_NUM_DIRECTIONS = 2
};

ROOM_DIRECTION GetRoomDirection(ROOM_FACE rf);

enum FACE_CORNER {
   FC_UPPERLEFT = 0,
   FC_LOWERLEFT = 1,
   FC_LOWERRIGHT = 2,
   FC_UPPERRIGHT = 3,
   NUM_FACE_CORNERS = 4
};

struct Vec3;

class Wall;

class Face {
   friend class Maze;
   
   Wall* parent_wall;

   Vec3* v[NUM_FACE_CORNERS];

   Vec3 normal;
   
   GLuint texid;///< Texture id

public :
   Face(Wall* parent);
   
   void Reset();
   void ResetNormal();

   
   void SetVertex(FACE_CORNER corner , Vec3* vtx);
   

   void Display();
   void DisplayFill(EagleColor c);
   void Outline(EagleColor c);

};



#endif // Face_HPP

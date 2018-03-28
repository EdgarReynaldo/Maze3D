



#ifndef MazeRoom_HPP
#define MazeRoom_HPP


#include "GL/gl.h"

#include <vector>
#include <cstring>


#include "Vec3D.hpp"

enum ROOM_FACE {
   ROOM_ABOVE = 0,
   ROOM_BELOW = 1,
   ROOM_NORTH = 2,
   ROOM_SOUTH = 3,
   ROOM_EAST  = 4,
   ROOM_WEST  = 5,
   NUM_ROOM_FACES = 6
};

enum FACE_TYPE {
   FACE_UPDOWN = 0,
   FACE_NORTHSOUTH = 1,
   FACE_EASTWEST = 2,
   NUM_FACE_TYPES = 3
};

FACE_TYPE GetFaceType(ROOM_FACE rf);

class Room;

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

struct Vec3D;

class Face {
   Room* rooms[ROOM_NUM_DIRECTIONS];
   Vec3D* v[NUM_FACE_CORNERS];

   GLuint texidpos;/// Texture id for the POSITIVE direction
   GLuint texidneg;/// Texture id for the NEGATIVE direction

public :
   Face();

   void SetRoom(ROOM_DIRECTION dir , Room* room);
   void SetVertex(FACE_CORNER corner , Vec3D* vtx);
};



class Room {
   Face* faces[NUM_ROOM_FACES];

public :

///   Room();
   Room() :
         faces()
   {
      memset(faces , 0 , sizeof(Face*)*NUM_ROOM_FACES);
   }


   void SetFace(ROOM_FACE room_dir , Face* face);

};


class Maze {
protected :
   std::vector<Room> rooms;
   std::vector<Face> faces;
   std::vector<Vec3D> vertices;

   int nrooms_wide;/// x
   int nrooms_tall;/// y
   int nrooms_deep;/// z (horizontal z, not vertical)
   int nrooms_total;
   int nverts_total;

   int floor_area;/// width * depth
   int side_area;/// height * depth
   int front_area;/// width * height

   struct FaceInfo {
      int index;
      int size;
      FaceInfo() : index(-1) , size(0) {}
   };

   FaceInfo face_info[NUM_FACE_TYPES];

///   Cube unit_cube;

///   Maze();


   Room* GetRoom(int index);
   int GetRoomIndex(int floor , int row , int col);

   Face* GetFace(int index);
   int GetFaceIndex(int floor , int row , int col , ROOM_FACE face);

   Vec3D* GetVertex(int index);
   int GetVertexIndex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner);




public :
   Maze() :
         rooms(),
         faces(),
         vertices(),
         nrooms_wide(0),
         nrooms_tall(0),
         nrooms_deep(0),
         nrooms_total(0),
         nverts_total(0),
         floor_area(0),
         side_area(0),
         front_area(0),
         face_info()
   {}

   ~Maze();



   void ClearMaze();
   bool CreateMaze(int num_rooms_wide , int num_rooms_tall , int num_rooms_deep);



};



#endif // MazeRoom_HPP

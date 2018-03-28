



#ifndef Maze_HPP
#define Maze_HPP


#include <vector>

#include "Room.hpp"
#include "Face.hpp"
#include "Vec3D.hpp"

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



#endif // Maze_HPP

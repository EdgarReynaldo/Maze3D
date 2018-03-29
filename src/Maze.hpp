



#ifndef Maze_HPP
#define Maze_HPP


#include <vector>
#include <map>



#include "Room.hpp"
#include "Face.hpp"
#include "Vec3D.hpp"
#include "PathSet.hpp"
#include "RNG.hpp"




class Maze {
protected :
   std::vector<Room> rooms;
   std::vector<Face> faces;
   std::vector<Vec3D> vertices;
   std::vector<PathSet> path_sets;

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

   RNG rng;
   
   
   
///   Cube unit_cube;

   void ResetFaces(int weight);

   void AssignAboveBelowFaceWeightsOutside();
   void AssignEastWestFaceWeightsOutside();
   void AssignNorthSouthFaceWeightsOutside();

   void AssignFaceWeightsOutside();
   void AssignFaceWeigthsKeep();
   void AssignFaceWeightsExit();
   void AssignFaceWeightsRegular();
   void AssignFaceWeightsLater();

   Room* GetRoom(int index);
   int GetRoomIndex(int floor , int row , int col);
   Room* GetRoom(int floor , int row , int col) {
      return GetRoom(GetRoomIndex(floor,row,col));
   }
   
   Face* GetFace(int index);
   int GetFaceIndex(int floor , int row , int col , ROOM_FACE face);
   Face* GetFace(int floor , int row , int col , ROOM_FACE face) {
      return GetFace(GetFaceIndex(floor,row,col,face));
   }
   
   Vec3D* GetVertex(int index);
   int GetVertexIndex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner);
   Vec3D* GetVertex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner) {
      return GetVertex(GetVertexIndex(floor,row,col,face,corner));
   }

   std::map<int , std::vector<Face*> > CreateWeightMap();




public :
   Maze() :
         rooms(),
         faces(),
         vertices(),
         path_sets(),
         nrooms_wide(0),
         nrooms_tall(0),
         nrooms_deep(0),
         nrooms_total(0),
         nverts_total(0),
         floor_area(0),
         side_area(0),
         front_area(0),
         face_info(),
         rng()
   {}

   ~Maze();



   void ClearMaze();
   
   bool CreateMaze(int num_rooms_wide , int num_rooms_tall , int num_rooms_deep);

   void AssignFaceWeights();
   void KruskalRemoval();

   

};



#endif // Maze_HPP

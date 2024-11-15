



#ifndef Maze_HPP
#define Maze_HPP


#include <vector>
#include <map>



#include "Room.hpp"
#include "Eagle/Face.hpp"
#include "Eagle/Mesh.hpp"
#include "Eagle/Vec3.hpp"
#include "Eagle/Random.hpp"
#include "PathSet.hpp"


#include "GL/gl.h"

class Maze {
public :
   std::vector<Room> rooms;
   std::vector<Wall> walls;
   std::vector<Face*> faces;
//   std::vector<Face> faces_out;
   std::vector<Vec3> vertices;
   std::vector<PathSet> path_sets;

   GLuint TextureIDs[6];/// UDNSEW, corresponds to face directions, base texture for wall faces
   
   int nrooms_wide;/// x
   int nrooms_tall;/// y
   int nrooms_deep;/// z (horizontal z, not vertical)
   int nrooms_total;
   int nwalls_total;
   int nfaces_total;
   int nverts_total;

   int floor_area;/// width * depth
   int side_area;/// height * depth
   int front_area;/// width * height
   
//   struct FaceInfo {
//      int index;
//      int size;
//      FaceInfo() : index(-1) , size(0) {}
//   };

//   FaceInfo face_info[NUM_ROOM_FACES];
   
   MTRNG rng;
   
   
///   Cube unit_cube;

   void ResetWalls(int weight);

   void AssignAboveBelowFaceWeightsOutside();
   void AssignEastWestFaceWeightsOutside();
   void AssignNorthSouthFaceWeightsOutside();

   void AssignFaceWeightsOutside();
   void AssignFaceWeightsKeep();
   void AssignFaceWeightsExit();
   void AssignFaceWeightsRegular();
   void AssignFaceWeightsLater();

   void AssignFaceWeights();

   typedef std::vector<Wall*> WALLVEC;
   typedef std::map<int , WALLVEC> WEIGHTMAP;
   typedef WEIGHTMAP::iterator WMIT;
   typedef std::pair<int , WALLVEC> WMPAIR;
   
   WEIGHTMAP CreateWeightMap();

   void RandomizeWeightMap(WEIGHTMAP& wmap);

   void RandomizeWallVector(WALLVEC& wvec);


   Room* GetRoom(int index);/// 0 to nrooms_total-1
   Room* GetRoom(int floor , int row , int col);/// y z x
   int GetRoomIndex(int floor , int row , int col);/// y z x
   
   Wall* GetWall(int index);/// 0 to nwalls_total-1
   Wall* GetWall(int floor , int row , int col , ROOM_FACE face);/// y z x
   int GetWallIndex(int floor , int row , int col , ROOM_FACE face);/// y z x
   
//   Face* GetFace(int index);
//   Face* GetFace(ROOM_FACE face , int floor , int row , int col);
//   int GetFaceIndex(ROOM_FACE face , int floor , int row , int col);
   
//   Face* GetFaceOut(int index);
//   Face* GetFaceOut(ROOM_FACE face , int yindex , int xindex , int zindex);
//   ///< Pass the xyz position of the face, irrelevant index will be ignored
//   int GetFaceOutIndex(ROOM_FACE face , int yindex , int xindex , int zindex);

   Vec3* GetVertex(int index);
   Vec3* GetVertex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner);
   int GetVertexIndex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner);

public :
   Maze();
   ~Maze();

   void ClearMaze();
   
   bool CreateMaze(int num_rooms_wide , int num_rooms_tall , int num_rooms_deep);

   void KruskalRemoval();

   void SetFaceTexture(ROOM_FACE face , GLuint texidpos , GLuint texidneg);
   void Display();
};



#endif // Maze_HPP

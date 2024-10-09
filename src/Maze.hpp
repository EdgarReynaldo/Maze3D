



#ifndef Maze_HPP
#define Maze_HPP


#include <vector>
#include <map>



#include "Room.hpp"
#include "Eagle/Face.hpp"
#include "Eagle/Mesh.hpp"
#include "Eagle/Vec3.hpp"
#include "PathSet.hpp"
#include "RNG.hpp"


#include "GL/gl.h"

class Maze {
public :
   std::vector<Room> rooms;
   std::vector<Face> faces;
   std::vector<Face> faces_out;
   std::vector<Vec3> vertices;
   std::vector<PathSet> path_sets;

   GLuint TextureIDs[6];/// UDNSEW, corresponds to face directions
   
   int nrooms_wide;/// x
   int nrooms_tall;/// y
   int nrooms_deep;/// z (horizontal z, not vertical)
   int nrooms_total;
   int nverts_total;
   int nfaces_total;
   int nfaces_out_total;/// Number of faces on the outside of the maze

   int floor_area;/// width * depth
   int side_area;/// height * depth
   int front_area;/// width * height
   
   struct FaceInfo {
      int index;
      int size;
      FaceInfo() : index(-1) , size(0) {}
   };

   FaceInfo face_info[NUM_ROOM_FACES];
   FaceInfo face_out_info[NUM_ROOM_FACES];
   
   RNG rng;
   
   
   
///   Cube unit_cube;

   void ResetFaces(int weight);

   void AssignAboveBelowFaceWeightsOutside();
   void AssignEastWestFaceWeightsOutside();
   void AssignNorthSouthFaceWeightsOutside();

   void AssignFaceWeightsOutside();
   void AssignFaceWeightsKeep();
   void AssignFaceWeightsExit();
   void AssignFaceWeightsRegular();
   void AssignFaceWeightsLater();

   void AssignFaceWeights();

   typedef std::vector<Face*> FACEVEC;
   typedef std::map<int , FACEVEC> WEIGHTMAP;
   typedef WEIGHTMAP::iterator WMIT;
   typedef std::pair<int , FACEVEC> WMPAIR;
   
   WEIGHTMAP CreateWeightMap();

   void RandomizeWeightMap(WEIGHTMAP& wmap);

   void RandomizeFaceVector(FACEVEC& fvec);


   Room* GetRoom(int index);
   int GetRoomIndex(int floor , int row , int col);
   
   Face* GetFace(int index);
   int GetFaceIndex(int floor , int row , int col , ROOM_FACE face);
   Face* GetFaceOut(int index);
   int GetFaceOut(ROOM_FACE face , int yindex , int xindex , int zindex);
   ///< Pass the xyz position of the face, irrelevant index will be ignored
   int GetFaceOutIndex(ROOM_FACE face , int yindex , int xindex , int zindex);
   Vec3* GetVertex(int index);
   int GetVertexIndex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner);

public :
   Maze();
   ~Maze();



   void ClearMaze();
   
   bool CreateMaze(int num_rooms_wide , int num_rooms_tall , int num_rooms_deep);

   void KruskalRemoval();

   void SetFaceTexture(ROOM_FACE face , GLuint texid);
   void Display();
};



#endif // Maze_HPP

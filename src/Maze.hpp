



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
         path_sets(),
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

   void AssignFaceWeights();
   void AssignFaceWeights() {

      /** The basis of the Kruskal algorithm is to assign each edge in the graph (face in the maze)
          a weight. Edges are removed in order from least weight to most, and edges with equal weights
          are equally likely to be removed. An edge may only be removed if removing it does not create 
          a cycle in the graph. This means we end up with a spanning tree where in this case, the nodes
          are rooms and the connecting edge is a face (wall).
      */

      const int WEIGHT_OUTSIDE = -1;
      const int WEIGHT_EXITS = 0;
      const int WEIGHT_ROOMS = 1;
      const int WEIGHT_SHAFTS = 2;


      /// Assign all the outside faces of the maze to -1
      
      /// Create two exits with weight 0
      
      /// Assign NS and EW faces a weight of 1
      
      /// Assign UPDOWN faces a weight of 2
   }
   
   void KruskalRemoval();
   void KruskalRemoval() {
      
   }
   

};



#endif // Maze_HPP





#include "Maze.hpp"

#include <cassert>



/** These control how the maze will be generated */

const int WEIGHT_OUTSIDE = -2;/// This weight is for the outside of the maze, which we never want to remove
const int WEIGHT_KEEP = -1;/// We marked these faces as ones to keep in the maze
const int WEIGHT_EXITS = 0;/// Anything greater than zero will eventually be removed

/** For now, we want to keep xyz equal - this will make a super crazy 3D maze
    If you want to make hallways or shafts, increase the weight of one of these :
*/

const int WEIGHT_ROOMS_NORMAL = 1;

/** Not currently using these */
const int WEIGHT_ROOMS_EW = 1;
const int WEIGHT_ROOMS_NS = 1;
const int WEIGHT_ROOMS_UD = 1;

/** */
const int WEIGHT_ROOMS_LATER = 4;



/// -----------------------      Maze     ----------------------------------------



void Maze::ResetFaces(int weight) {
   const int stop = (int)faces.size();
   assert(stop);
   Face* pfacepalm = &faces[0];
   int i = 0;
   while (i++ < stop) {
      pfacepalm->SetWeight(weight);
      pfacepalm++->Reset();
   }
}



void Maze::AssignAboveBelowFaceWeightsOutside() {
   /// Do the floor and ceiling of the maze
   for (int z = 0 ; z < nrooms_deep ; ++z) {
      for (int x = 0 ; x < nrooms_wide ; ++x) {
         Face* floor = GetFace(0,z,x,ROOM_BELOW);
         Face* ceiling = GetFace(nrooms_tall - 1,z,x,ROOM_ABOVE);
         floor->SetWeight(WEIGHT_OUTSIDE);
         ceiling->SetWeight(WEIGHT_OUTSIDE);
      }
   }
}



void Maze::AssignEastWestFaceWeightsOutside() {
   /// Do the west and east outside of the maze
   for (int y = 0 ; y < nrooms_tall ; ++y) {
      for (int z = 0 ; z < nrooms_deep ; ++z) {
         Face* west = GetFace(y,z,0,ROOM_WEST);
         Face* east = GetFace(y,z,nrooms_wide - 1,ROOM_EAST);
         west->SetWeight(WEIGHT_OUTSIDE);
         east->SetWeight(WEIGHT_OUTSIDE);
      }
   }
}



void Maze::AssignNorthSouthFaceWeightsOutside() {
   /// Do the north and south outside of the maze
   for (int y = 0 ; y < nrooms_tall ; ++y) {
      for (int x = 0 ; x < nrooms_wide ; ++x) {
         Face* south = GetFace(y,0,x,ROOM_SOUTH);
         Face* north = GetFace(y,nrooms_deep-1,x,ROOM_NORTH);
         south->SetWeight(WEIGHT_OUTSIDE);
         north->SetWeight(WEIGHT_OUTSIDE);
      }
   }
}



void Maze::AssignFaceWeightsOutside() {
   AssignAboveBelowFaceWeightsOutside();
   AssignEastWestFaceWeightsOutside();
   AssignNorthSouthFaceWeightsOutside();
}



void Maze::AssignFaceWeigthsKeep() {
   /** Here we can reserve edges we want to keep, in case we want to define a certain set of walls */
   return; /// do nothing for now
}



void Maze::AssignFaceWeightsExit() {
   /// Find a random face on the front and back of the maze
   Face* front = GetFace(rng.Rand0toNM1(nrooms_tall) , 0 , rng.Rand0toNM1(nrooms_wide), ROOM_SOUTH);
   Face* back = GetFace(rng.Rand0toNM1(nrooms_tall) , nrooms_deep - 1 , rng.Rand0toNM1(nrooms_wide), ROOM_SOUTH);

   front->SetWeight(WEIGHT_EXITS);
   back->SetWeight(WEIGHT_EXITS);
/** we need to do this in ProcessFaces
   front->SetRoom(ROOM_NEGATIVE , 0);
   back->SetRoom(ROOM_POSITIVE , 0);
*/
}



void Maze::AssignFaceWeightsRegular() {
   /// Need to set all the internal faces now, like UpDown, NorthSouth, and EastWest
   /// There's got to be an easy way to get all of these faces
}



void Maze::AssignFaceWeightsLater() {
   /** This function is for assigning weights to edges you want to be less likely to be removed */
   return;/// Do nothing for now
}



Room* Maze::GetRoom(int index) {
   if (index == -1) {return 0;}
   return &(rooms[index]);
}



int Maze::GetRoomIndex(int floor , int row , int col) {
   int index = floor*floor_area + row*nrooms_deep + col;
   if (index >= 0 && index < (int)rooms.size()) {
      return index;
   }
   return -1;
}



Face* Maze::GetFace(int index) {
   if (index == -1) {return 0;}
   return &(faces[index]);
}



int Maze::GetFaceIndex(int floor , int row , int col , ROOM_FACE face) {

   /// We have 3 sections in our face array - first is up down second is forwards backwards third is right left

   ROOM_DIRECTION dir = GetRoomDirection(face);/// positive or negative
   FACE_TYPE type = GetFaceType(face);/// axis type

   int start_index = face_info[type].index;
   int stop_index = start_index + face_info[type].size;

   if (dir == ROOM_POSITIVE) {
      switch (type) {
      case FACE_UPDOWN :
         floor += 1;
         break;
      case FACE_NORTHSOUTH :
         row += 1;
         break;
      case FACE_EASTWEST :
         col += 1;
         break;
      default :
         assert(0);
         break;
      }
   }

   int index = start_index;
   switch (type) {
   case FACE_UPDOWN :
      index += floor*floor_area + row*nrooms_wide + col;
      break;
   case FACE_NORTHSOUTH :
      index += row*front_area + floor*nrooms_wide + col;
      break;
   case FACE_EASTWEST :
      index += col*side_area + floor*nrooms_deep + row;
      break;
   default :
      assert(0);
      break;
   }

   assert(index < stop_index);

   return index;
}



Vec3D* Maze::GetVertex(int index) {
   return &(vertices[index]);
}



int Maze::GetVertexIndex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner) {
   ROOM_DIRECTION dir = GetRoomDirection(face);
   if (dir == ROOM_POSITIVE) {
      switch (face) {
      case ROOM_ABOVE :
         floor += 1;
         break;
      case ROOM_NORTH :
         row += 1;
         break;
      case ROOM_EAST :
         col += 1;
         break;
      default : assert(0);
      };
   }
   int index = NUM_FACE_CORNERS*(floor*(nrooms_deep + 1)*(nrooms_wide + 1) + row*(nrooms_wide + 1) + col);
   index += (int)corner;
   return index;
}



std::map<int , std::vector<Face*> > Maze::CreateWeightMap() {
   std::map<int , std::vector<Face*> > wmap;
   return wmap;
}



Maze::~Maze() {
   ClearMaze();
}



void Maze::ClearMaze() {
   rooms.clear();
   faces.clear();
   vertices.clear();
   path_sets.clear();
   
   nrooms_wide = nrooms_tall = nrooms_deep = nrooms_total = 0;
   floor_area = side_area = front_area = 0;
   nverts_total = 0;
   
   memset(face_info , 0 , sizeof(FaceInfo)*NUM_FACE_TYPES);
}



bool Maze::CreateMaze(int num_rooms_wide , int num_rooms_tall , int num_rooms_deep) {
   ClearMaze();

   nrooms_wide = abs(num_rooms_wide);
   nrooms_tall = abs(num_rooms_tall);
   nrooms_deep = abs(num_rooms_deep);
   nrooms_total = nrooms_tall*nrooms_wide*nrooms_deep;

   assert(nrooms_total > 0);

   floor_area = nrooms_wide*nrooms_deep;
   side_area = nrooms_tall*nrooms_deep;
   front_area = nrooms_wide*nrooms_tall;

   /// Reserve the rooms
   
   rooms.resize(nrooms_total);
   
   
   /// Reserve the path sets
   
   path_sets.resize(nrooms_total);
   
   /// Reset the rooms and path sets
   
   for (int i = 0 ; i < nrooms_total ; ++i) {
      Room* r = &rooms[i];
      r->Reset();

      PathSet* p = &path_sets[i];
      p->Reset();
      
      p->AddRoom(r);
   }
   
   /// Count number of faces

   int face_count = 3*nrooms_total;/// Each room has a west, south, and down face
   face_count += side_area;/// Add in the faces on the east
   face_count += front_area;/// Add in the faces on the north
   face_count += floor_area;/// Add in the faces on the ceiling

   faces.resize(face_count);
   
   /// Reset the faces
   
   Face* f = &faces[0];
   for (int i = 0 ; i < face_count ; ++i ,++f) {
      f->Reset();
   }

   /** Setup our face indices */

   /// Bottom / Top faces are stored in the first third of the vector
   face_info[0].index = 0;

   face_info[0].size = (nrooms_tall + 1)*floor_area;/// There are n + 1 floors

   /// North / South faces are stored in the second third of the vector
   face_info[1].index = face_info[0].index + face_info[0].size;

   face_info[1].size = (nrooms_deep + 1)*front_area;

   /// West / East faces are stored in the third third of the vector
   face_info[2].index = face_info[1].index + face_info[1].size;

   face_info[2].size = (nrooms_wide + 1)*side_area;

   assert(face_info[2].index + face_info[2].size == (int)faces.size());/// Make sure our indices and sizes are correct

   /// Setup our vertice array
   nverts_total = NUM_FACE_CORNERS*(nrooms_tall + 1)*(nrooms_deep + 1)*(nrooms_wide + 1);

   vertices.resize(nverts_total);

   int index = 0;

   for (int y = 0 ; y < num_rooms_tall + 1 ; ++y) {
      for (int z = 0 ; z < num_rooms_deep + 1 ; ++z) {
         for (int x = 0 ; x < num_rooms_wide + 1 ; ++x) {
            /// Luckily, each room is 1.0 X 1.0 X 1.0 and matches the depth,row,col index exactly
            vertices[index].SetXYZ(x,y,z);
            ++index;
         }
      }
   }

   /// For each room, populate it's face pointers and the face's room pointers
   index = 0;
   for (int y = 0 ; y < num_rooms_tall ; ++y) {
      for (int z = 0 ; z < num_rooms_deep ; ++z) {
         for (int x = 0 ; x < num_rooms_wide ; ++x) {
            /// Each room has six face pointers
            /// Each face has two room pointers
            Room* r = &rooms[index];

            for (int i = 0 ; i < NUM_ROOM_FACES ; ++i) {
               Face* f = GetFace(GetFaceIndex(y,z,x , (ROOM_FACE)i));
               assert(f);
               r->SetFace((ROOM_FACE)i , f);
               /// get the direction to the face and reverse it
               ROOM_DIRECTION dir = (GetRoomDirection((ROOM_FACE)i) == ROOM_POSITIVE)?ROOM_NEGATIVE:ROOM_POSITIVE;
               f->SetRoom(dir , r);
               for (int j = 0 ; j < NUM_FACE_CORNERS ; ++j) {
                  Vec3D* vtx = GetVertex(GetVertexIndex(y,z,x , (ROOM_FACE)i , (FACE_CORNER)j));
                  f->SetVertex((FACE_CORNER)j , vtx);
               }
            }

            ++index;
         }
      }
   }
   
   return true;
}



void Maze::AssignFaceWeights() {

   /** The basis of the Kruskal algorithm is to assign each edge in the graph (face in the maze)
       a weight. Edges are removed in order from least weight to most, and edges with equal weights
       are equally likely to be removed. An edge may only be removed if removing it does not create 
       a cycle in the graph. This means we end up with a spanning tree where in this case, the nodes
       are rooms and the connecting edge is a face (wall).
   */

   ResetFaces(WEIGHT_ROOMS_NORMAL);

   /// Assign all the outside faces
   
   AssignFaceWeightsOutside();

   /// Create exits

   AssignFaceWeightsExit();
      
   /// Assign NS faces
   /// Assign EW faces
   /// Assign UD (UpDown) faces
   
   AssignFaceWeightsRegular();
   
   /// Assign later faces
   
   AssignFaceWeightsLater();
   
   /// Create a copy of the face list
   
   /// Process faces in order
   
   
   
}



void Maze::KruskalRemoval() {
   AssignFaceWeights();
   
   std::map<int , std::vector<Face*> > face_weight_map = CreateWeightMap();
}
   








#include "Maze.hpp"


#include <cassert>



/// -----------------------      Maze     ----------------------------------------



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
   
   for (int i = 0 ; i < face_count ; ++i) {
      Face* f = faces[i];
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




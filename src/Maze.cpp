



#include "Maze.hpp"

#include "Eagle/Exception.hpp"
#include "Wall.hpp"

/** These control how the maze will be generated */

const int WEIGHT_OUTSIDE = -2;/// This weight is for the outside of the maze, which we never want to remove
const int WEIGHT_KEEP = -1;/// We marked these faces as ones to keep in the maze

const int WEIGHT_MINIMUM = 0;/// Don't process anything below this value

const int WEIGHT_EXITS = 0;/// Anything greater than zero will eventually be removed

/** For now, we want to keep xyz weights equal - this will make a super crazy 3D maze
    If you want to make hallways or shafts, increase the weight of one of these :
*/

const int WEIGHT_NORMAL = 1;

/** Not currently using these */
const int WEIGHT_ROOMS_EW = WEIGHT_NORMAL;
const int WEIGHT_ROOMS_NS = WEIGHT_NORMAL;
const int WEIGHT_ROOMS_UD = WEIGHT_NORMAL;

/** For making rooms less likely to be removed */
const int WEIGHT_ROOMS_LATER = 4;



/// -----------------------      Maze     ----------------------------------------



void Maze::ResetWalls(int weight) {
   for (unsigned int i = 0 ; i < walls.size() ; ++i) {
      walls[i].SetWeight(weight);
   }
}



void Maze::AssignAboveBelowFaceWeightsOutside() {
   /// Do the floor and ceiling of the maze
   for (int z = 0 ; z < nrooms_deep ; ++z) {
      for (int x = 0 ; x < nrooms_wide ; ++x) {
         Room* bottom = GetRoom(0,z,x);
         Wall* floor = GetWall(0,z,x, ROOM_BELOW);
         
         Room* top = GetRoom(nrooms_tall - 1 , z , x);
         Wall* ceiling = GetWall(nrooms_tall - 1 , z , x , ROOM_ABOVE);

         floor->SetWeight(WEIGHT_OUTSIDE);
         ceiling->SetWeight(WEIGHT_OUTSIDE);
      }
   }
}



void Maze::AssignEastWestFaceWeightsOutside() {
   /// Do the west and east outside of the maze
   for (int y = 0 ; y < nrooms_tall ; ++y) {
      for (int z = 0 ; z < nrooms_deep ; ++z) {
         Room* left = GetRoom(y,z,0);
         Room* right = GetRoom(y,z,nrooms_wide - 1);
         Wall* west = GetWall(y,z,0 , ROOM_WEST);
         Wall* east = GetWall(y,z,nrooms_wide - 1 , ROOM_EAST);
         west->SetWeight(WEIGHT_OUTSIDE);
         east->SetWeight(WEIGHT_OUTSIDE);
      }
   }
}



void Maze::AssignNorthSouthFaceWeightsOutside() {
   /// Do the north and south outside of the maze
   for (int y = 0 ; y < nrooms_tall ; ++y) {
      for (int x = 0 ; x < nrooms_wide ; ++x) {
         Room* forward = GetRoom(y,nrooms_deep - 1 , x);
         Room* backward = GetRoom(y,0,x);
         Wall* north = GetWall(y,nrooms_deep - 1 , x , ROOM_NORTH);
         Wall* south = GetWall(y,0,x , ROOM_SOUTH);
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



void Maze::AssignFaceWeightsKeep() {
   /** Here we can reserve edges we want to keep, in case we want to define a certain set of walls */
   return; /// do nothing for now
}



void Maze::AssignFaceWeightsExit() {
   /// Find a random face on the front and back of the maze
   Room* entrance = GetRoom(0,0,0);
   Room* exit = GetRoom(nrooms_tall-1 , nrooms_deep-1, nrooms_wide-1);
   Wall* in = GetWall(0,0,0 , ROOM_SOUTH);
   Wall* out = GetWall(nrooms_tall-1,nrooms_deep-1,nrooms_wide-1 , ROOM_NORTH);
   in->SetWeight(WEIGHT_EXITS);
   out->SetWeight(WEIGHT_EXITS);
}



void Maze::AssignFaceWeightsRegular() {
   /// Need to set all the internal faces now, like UpDown, NorthSouth, and EastWest
   /// There's got to be an easy way to get all of these faces

   /** Right now, this is handled in ResetFaces(weight) inside AssignFaceWeights -
       this gives a default weight of WEIGHT_NORMAL. TODO : add code
   */

}



void Maze::AssignFaceWeightsLater() {
   /** This function is for assigning weights to edges you want to be less likely to be removed */
   return;/// Do nothing for now
}



void Maze::AssignFaceWeights() {

   /** The basis of the Kruskal algorithm is to assign each edge in the graph (face in the maze)
       a weight. Edges are removed in order from least weight to most, and edges with equal weights
       are equally likely to be removed. An edge may only be removed if removing it does not create
       a cycle in the graph. This means we end up with a spanning tree where in this case, the nodes
       are rooms and the connecting edge is a face (wall).
   */

   ResetWalls(WEIGHT_NORMAL);

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

}



Maze::WEIGHTMAP Maze::CreateWeightMap() {
   WEIGHTMAP wmap;
   WMIT it = wmap.end();
   Wall* w = &walls[0];
   for (unsigned int i = 0 ; i < walls.size() ; ++i) {
      it = wmap.find(w->Weight());
      if (it != wmap.end()) {
         it->second.push_back(w);
      }
      else {
         wmap.insert(WMPAIR(w->Weight() , std::vector<Wall*>{}));
      }
      ++w;
   }
   return wmap;
}



void Maze::RandomizeWeightMap(WEIGHTMAP& wmap) {
   for (WMIT it = wmap.begin() ; it != wmap.end() ; ++it) {
      RandomizeWallVector(it->second);
   }
}



void Maze::RandomizeWallVector(WALLVEC& wvec) {
   WALLVEC shuffle;
   while ( wvec.size() ) {
      int rnd = rng.Rand0toNM1(wvec.size());
      Wall* random_wall = wvec[rnd];
      shuffle.push_back(random_wall);
      wvec[rnd] = wvec.back();
      wvec.pop_back();
   }
   wvec = shuffle;
}



Room* Maze::GetRoom(int index) {
   if (index == -1) {return 0;}
   return &(rooms[index]);
}



int Maze::GetRoomIndex(int floor , int row , int col) {
   int index = floor*floor_area + row*nrooms_deep + col;
   if ((floor < 0) || (floor > nrooms_tall)) {
      return -1;
   }
   if ((row < 0 ) || (row > nrooms_deep)) {
      return -1;
   }
   if ((col < 0) || (col > nrooms_wide)) {
      return -1;
   }
   if (index >= 0 && index < (int)rooms.size()) {
      return index;
   }
   return -1;
}



Wall* Maze::GetWall(int index) {
   if ((index < 0) || (index >= nwalls_total)) {
      EAGLE_ASSERT(false);
      return 0;
   }
   return &walls[index];
}



Wall* Maze::GetWall(int floor , int row , int col , ROOM_FACE face) {
   return GetWall(GetWallIndex(floor,row,col,face));
}



int Maze::GetWallIndex(int floor , int row , int col , ROOM_FACE face) {
   if ((floor < 0) || (floor >= nrooms_tall)) {
      EAGLE_ASSERT(false);
      return 0;
   }
   if ((row < 0) || (row >= nrooms_deep)) {
      EAGLE_ASSERT(false);
      return 0;
   }
   if ((col < 0) || (col >= nrooms_wide)) {
      EAGLE_ASSERT(false);
      return 0;
   }
   if ((face < 0) || (face >= NUM_ROOM_FACES)) {
      EAGLE_ASSERT(false);
      return 0;
   }
   
   ROOM_DIRECTION rdir = GetRoomDirection(face);
   int index = -1;
   

   if (rdir == ROOM_NEGATIVE) {
      //check for outside faces
      if (floor == 0 && face == ROOM_BELOW) {
         //case ROOM_BELOW:
         return 3*nrooms_total + nrooms_wide*row + col;/// floor_area
      }
      if (row == 0 && face == ROOM_SOUTH) {
         //case ROOM_SOUTH:
         return 3*nrooms_total + floor_area + nrooms_wide*floor + col;/// front area
      }
      if (col == 0 && face == ROOM_WEST) {
         //case ROOM_WEST:
         return 3*nrooms_total + floor_area + front_area + row;/// side area
      }
   }
   switch (face) {
   case ROOM_ABOVE:
      index = 0;
      break;
   case ROOM_NORTH:
      index = 1;
      break;
   case ROOM_EAST:
      index = 2;
      break;
   case ROOM_BELOW:
      index = 0;
      EAGLE_ASSERT(floor > 0);
      floor = floor - 1;
      break;
   case ROOM_SOUTH:
      index = 1;
      EAGLE_ASSERT(row > 0);
      row = row - 1;
      break;
   case ROOM_WEST:
      index = 2;
      EAGLE_ASSERT(col > 0);
      col = col - 1;
      break;
   default :
      EAGLE_ASSERT(false);
   }
      
   return index*nrooms_total + floor*floor_area + nrooms_wide*row + col;


   EAGLE_ASSERT(false);
   return -1;
}








Vec3* Maze::GetVertex(int index) {
   return &(vertices[index]);
}


/**enum ROOM_FACE {
   ROOM_ABOVE = 0,ceiling, reverse of floor,modify z and x, same y but higher
   ROOM_BELOW = 1,floor,modify z and x, same y
   ROOM_NORTH = 2,ahead, modify y and x, same z but farther forward
   ROOM_SOUTH = 3,behind, modify y and x, same z
   ROOM_EAST  = 4,right, modify y and z, same x but farther right
   ROOM_WEST  = 5,left, modify y and z, same x

   enum FACE_CORNER
   FC_UPPERLEFT = 0,///
   FC_LOWERLEFT = 1,///
   FC_LOWERRIGHT = 2,
   FC_UPPERRIGHT = 3,
*/
int Maze::GetVertexIndex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner) {
//   ROOM_DIRECTION dir = GetRoomDirection(face);

   /// The room coordinates need to be offset
   int x = 0;// east west
   int y = 0;// up down
   int z = 0;// towards screen / towards you
   
   switch (face) {
   case ROOM_ABOVE :
      /// Ceiling
      y += 1;
      switch (corner) {
      case FC_UPPERLEFT :
         (void)0;
         break;
      case FC_LOWERLEFT :
         z += 1;
         break;
      case FC_LOWERRIGHT :
         x += 1;
         z += 1;
         break;
      case FC_UPPERRIGHT :
         x += 1;
         break;
      default : (void)0;break;
      }
   
      break;
   case ROOM_BELOW :
      /// Ground - up is to the north
      switch (corner) {
      case FC_UPPERLEFT :
         z += 1;
         break;
      case FC_LOWERLEFT :
         (void)0;
         break;
      case FC_LOWERRIGHT :
         x += 1;
         break;
      case FC_UPPERRIGHT :
         x += 1;
         z += 1;
         break;
      default : (void)0;break;
      }

      break;
   case ROOM_NORTH :
      /// North wall, positive z
      z += 1;
      switch (corner) {
      case FC_UPPERLEFT :
         y += 1;
         break;
      case FC_LOWERLEFT :
         (void)0;
         break;
      case FC_LOWERRIGHT :
         x += 1;
         break;
      case FC_UPPERRIGHT :
         x += 1;
         y += 1;
         break;
      default : (void)0;break;
      }

      break;
   case ROOM_SOUTH :
      /// South wall 
      switch (corner) {
      case FC_UPPERLEFT :
         y += 1;
         x += 1;
         break;
      case FC_LOWERLEFT :
         x += 1;
         break;
      case FC_LOWERRIGHT :
         (void)0;
         break;
      case FC_UPPERRIGHT :
         y += 1;
         break;
      default : (void)0;break;
      }

      break;
   case ROOM_EAST :
      /// East wall
      x += 1;
      switch (corner) {
      case FC_UPPERLEFT :
         // upper north east
         y += 1;
         z += 1;
         break;
      case FC_LOWERLEFT :
         // lower north east
         z += 1;
         break;
      case FC_LOWERRIGHT :
         // lower south east
         (void)0;
         break;
      case FC_UPPERRIGHT :
         // upper south east
         y += 1;
         break;
      default : (void)0;break;
      }

      break;
   case ROOM_WEST :
      switch (corner) {
      case FC_UPPERLEFT :
         // upper south west
         y += 1;
         break;
      case FC_LOWERLEFT :
         // lower south west
         (void)0;
         break;
      case FC_LOWERRIGHT :
         // lower north west
         z += 1;
         break;
      case FC_UPPERRIGHT :
         // upper north west
         y += 1;
         z += 1;
         break;
      default : (void)0;break;
      }

      break;
   default :
      assert(0);
      break;
   }

   floor += y;
   row += z;
   col += x;
   
   int index = floor*(nrooms_deep + 1)*(nrooms_wide + 1) + row*(nrooms_wide + 1) + col;
   return index;
}


Room* Maze::GetRoom(int floor , int row , int col) {
   return GetRoom(GetRoomIndex(floor,row,col));
}



//Face* Maze::GetFace(ROOM_FACE face , int floor , int row , int col) {
//   return GetFace(GetFaceIndex(face,floor,row,col));
//}



Vec3* Maze::GetVertex(int floor , int row , int col , ROOM_FACE face , FACE_CORNER corner) {
   return GetVertex(GetVertexIndex(floor,row,col,face,corner));
}



Maze::Maze() :
      rooms(),
      walls(),
//      faces(),
      vertices(),
      path_sets(),
      TextureIDs(),
      nrooms_wide(0),
      nrooms_tall(0),
      nrooms_deep(0),
      nrooms_total(0),
      nwalls_total(0),
//      nfaces_total(0),
      nverts_total(0),
      floor_area(0),
      side_area(0),
      front_area(0),
//      face_info(),
      rng()
{}



Maze::~Maze() {
   ClearMaze();
}



void Maze::ClearMaze() {
   rooms.clear();
   walls.clear();
//   faces.clear();
//   faces_out.clear();
   vertices.clear();
   path_sets.clear();

   nrooms_wide = nrooms_tall = nrooms_deep = nrooms_total = 0;
   floor_area = side_area = front_area = 0;
   nverts_total = 0;
//   nfaces_total = 0;
   nwalls_total = 0;
}



bool Maze::CreateMaze(int num_rooms_wide , int num_rooms_tall , int num_rooms_deep) {
   ClearMaze();

   nrooms_wide = abs(num_rooms_wide);
   nrooms_tall = abs(num_rooms_tall);
   nrooms_deep = abs(num_rooms_deep);
   nrooms_total = nrooms_tall*nrooms_wide*nrooms_deep;
   assert(nrooms_total > 0);

   /// Reserve the rooms
   rooms.resize(nrooms_total);

   floor_area = nrooms_wide*nrooms_deep;
   side_area = nrooms_tall*nrooms_deep;
   front_area = nrooms_wide*nrooms_tall;

   /// Reserve the walls
   
   nwalls_total = 3*nrooms_total + floor_area + side_area + front_area;
   walls.resize(nwalls_total);


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

   /// Setup our vertice array
   nverts_total = (nrooms_tall + 1)*(nrooms_deep + 1)*(nrooms_wide + 1);
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
   EAGLE_ASSERT(index == nverts_total);

   
   /// Populate neighbors
   for (int y = 0 ; y < num_rooms_tall ; ++y) {
      for (int z = 0 ; z < num_rooms_deep ; ++z) {
         for (int x = 0 ; x < num_rooms_wide ; ++x) {
            Room* r = GetRoom(y,z,x);
            if (r) {
               
               Wall* wa = r->walls[ROOM_ABOVE] = GetWall(y,z,x,ROOM_ABOVE);
               Wall* wb = r->walls[ROOM_BELOW] = GetWall(y,z,x,ROOM_BELOW);
               Wall* wn = r->walls[ROOM_NORTH] = GetWall(y,z,x,ROOM_NORTH);
               Wall* ws = r->walls[ROOM_SOUTH] = GetWall(y,z,x,ROOM_SOUTH);
               Wall* we = r->walls[ROOM_EAST]  = GetWall(y,z,x,ROOM_EAST);
               Wall* ww = r->walls[ROOM_WEST]  = GetWall(y,z,x,ROOM_WEST);
               Room* ra = r->neighbors[ROOM_ABOVE] = GetRoom(y+1,z,x);
               Room* rb = r->neighbors[ROOM_BELOW] = GetRoom(y-1,z,x);
               Room* rn = r->neighbors[ROOM_NORTH] = GetRoom(y,z+1,x);
               Room* rs = r->neighbors[ROOM_SOUTH] = GetRoom(y,z-1,x);
               Room* re = r->neighbors[ROOM_EAST]  = GetRoom(y,z,x+1);
               Room* rw = r->neighbors[ROOM_WEST]  = GetRoom(y,z,x-1);
               wa->room_pos = ra;
               wa->room_neg = r;
               wb->room_pos = r;
               wb->room_neg = rb;
               wn->room_pos = rn;
               wn->room_neg = r;
               ws->room_pos = r;
               ws->room_neg = rs;
               we->room_pos = re;
               we->room_neg = r;
               ww->room_pos = r;
               ww->room_neg = rw;
               
               Face* fa = &wa->face_neg;
               Face* fb = &wb->face_pos;
               Face* fn = &wn->face_neg;
               Face* fs = &ws->face_pos;
               Face* fe = &we->face_neg;
               Face* fw = &ww->face_pos;
               /// faces outside of room
               Face* fouta = &wa->face_pos;
               Face* foutb = &wb->face_neg;
               Face* foutn = &wn->face_pos;
               Face* fouts = &ws->face_neg;
               Face* foute = &we->face_pos;
               Face* foutw = &ww->face_neg;
               for (int j = 0 ; j < NUM_FACE_CORNERS ; ++j) {
                  Vec3* vtxa = GetVertex(GetVertexIndex(y,z,x , ROOM_ABOVE , (FACE_CORNER)j));
                  fa->SetVertex((FACE_CORNER)j , vtxa);
                  fouta->SetVertex((FACE_CORNER)(3-j),vtxa);
                  Vec3* vtxb = GetVertex(GetVertexIndex(y,z,x , ROOM_BELOW , (FACE_CORNER)j));
                  fb->SetVertex((FACE_CORNER)j , vtxb);
                  foutb->SetVertex((FACE_CORNER)(3-j),vtxb);
                  Vec3* vtxn = GetVertex(GetVertexIndex(y,z,x , ROOM_NORTH , (FACE_CORNER)j));
                  fn->SetVertex((FACE_CORNER)j , vtxn);
                  foutn->SetVertex((FACE_CORNER)(3-j),vtxn);
                  Vec3* vtxs = GetVertex(GetVertexIndex(y,z,x , ROOM_SOUTH , (FACE_CORNER)j));
                  fs->SetVertex((FACE_CORNER)j , vtxs);
                  fouts->SetVertex((FACE_CORNER)(3-j),vtxs);
                  Vec3* vtxe = GetVertex(GetVertexIndex(y,z,x , ROOM_EAST , (FACE_CORNER)j));
                  fe->SetVertex((FACE_CORNER)j , vtxe);
                  foute->SetVertex((FACE_CORNER)(3-j),vtxe);
                  Vec3* vtxw = GetVertex(GetVertexIndex(y,z,x , ROOM_WEST , (FACE_CORNER)j));
                  fw->SetVertex((FACE_CORNER)j , vtxw);
                  foutw->SetVertex((FACE_CORNER)(3-j),vtxw);
               }
            }
         }
      }
   }
   return true;
}



void Maze::KruskalRemoval() {
   AssignFaceWeights();

   WEIGHTMAP wmap = CreateWeightMap();

   RandomizeWeightMap(wmap);

   /// Put the map in a vector for processing now that it is sorted
   /// The order has already been randomized, so they will be in final order already

   WALLVEC sorted;
   WALLVEC* wvec = 0;

   WMIT it = wmap.begin();
   while (it != wmap.end()) {
      wvec = &(it->second);
      sorted.insert(sorted.end() , wvec->begin() , wvec->end());
      ++it;
   }
   assert(sorted.size());

   /// Process each face in order

   int start = 0;

   /// Skip all weights less than the minimum
   for ( ; start < (int)sorted.size() ; ++start) {
      Wall* w = sorted[start];
      if (w->Weight() >= WEIGHT_MINIMUM) {
         break;
      }
   }

   /// For every other edge check if removal would create a cycle if so keep it and keep the cycle broken
   /// This is what makes the spanning tree
   for (int i = start ; i < (int)sorted.size() ; ++i) {
      Wall* w = sorted[i];
      Room* rpos = w->GetRoomPos();
      Room* rneg = w->GetRoomNeg();
      Room** r1 = rpos?&rpos:rneg?&rneg:0;/// Set r1 to the address of the first non-null Room*
      Room** r2 = (r1 == &rpos)?&rneg:&rpos;/// Set r2 to the other pointer
      assert(*r1);/// There should be at least one room connected to this wall
      if (*r2) {/// Both Room*s are non-null
         PathSet* pset1 = (*r1)->GetPathSet();
         PathSet* pset2 = (*r2)->GetPathSet();

         /// If removing the face creates a Room* cycle, then skip it and keep, else remove

         if (pset1 == pset2) {
               /** If both PathSet*s are the same, then these two rooms are already connected somehow,
                   and removing the edge would create a cycle */
               w->SetOpen(false);
               continue;
         }
         else {
            w->SetOpen(true);
            pset1->AbsorbPathSet(pset2);
         }

      }
      else {
         /// Outside edge, can't remove it
         assert(0);/// These should all have been excluded
      }
   }
}



void Maze::SetFaceTexture(ROOM_FACE face , GLuint texid) {
   TextureIDs[face] = texid;
   for (int y = 0 ; y < nrooms_tall ; ++y) {
      for (int z = 0 ; z < nrooms_deep ; ++z) {
         for (int x = 0 ; x < nrooms_wide ; ++x) {
            Wall* w = GetWall(y,z,x,face);
            Face* fp = w->GetFacePos();
            Face* fn = w->GetFaceNeg();
            fp->texid = fn->texid = texid;
         }
      }
   }
}



void Maze::Display() {

   glEnable(GL_CULL_FACE);
   glFrontFace(GL_CCW);
   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);

   Wall* w = &walls[0];
   for (unsigned int i = 0 ; i < walls.size() ; ++i){
      w->Display();
      ++w;
   }

   glDisable(GL_CULL_FACE);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);
   for (unsigned int i = 0 ; i < walls.size() ; ++i) {
      Wall& w = walls[i];
      w.Outline(EagleColor(0,255,0,255));/// TODO : This results in black outlines investigate
   }
   
}

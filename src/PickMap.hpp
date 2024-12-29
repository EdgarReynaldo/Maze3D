



#ifndef PickMap_HPP
#define PickMap_HPP

#include "Eagle/Color.hpp"
#include "Maze.hpp"

#include <map>

class Face;

typedef std::map<EagleColor , Face*> PICKMAP;
typedef PICKMAP::iterator PMIT;

class EagleImage;
class EagleGraphicsContext;

class PickMap {
public :
   PICKMAP pickmap;
   EagleImage* pickimg;
   
   
   PickMap();
   
   
   
   
   void Display(Maze& m);
   bool Create(EagleGraphicsContext* win , int w , int h);
   void Destroy();
   
   EagleColor GetPixel(int x , int y);
   
   
};


EagleColor GetNextPickingColor();



#endif // PickMap_HPP


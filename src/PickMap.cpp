



#include "PickMap.hpp"


#include "Eagle/GraphicsContext.hpp"
#include "Face.hpp"
#include "Wall.hpp"





PickMap::PickMap() :
      pickmap(),
      pickimg(0)
{}



void PickMap::Display(Maze& m) {
   std::vector<Wall>& mwalls = m.walls;
   for (unsigned int i = 0 ; i < mwalls.size() ; ++i) {
      Wall* w = &mwalls[i];
      w->DisplayFill(w->PickColor());
   }
}



bool PickMap::Create(EagleGraphicsContext* win , int w , int h) {
   Destroy();
   if (win) {
      pickimg = win->CreateImage(w,h,VIDEO_IMAGE,"Pick Image");
   }
   return pickimg && pickimg->Valid();
}



void PickMap::Destroy() {
   if (pickimg) {
      pickimg->ParentContext()->FreeImage(pickimg);
      pickimg = 0;
   }
   pickmap.clear();
}



EagleColor GetNextPickingColor() {
   static EagleColor pick_color = EagleColor(1,1,1,1);
   return pick_color++;
}




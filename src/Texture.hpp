



#ifndef Texture_HPP
#define Texture_HPP

class Tex2D {
public :
   double u,v;
};


#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"

ALLEGRO_BITMAP* CreateTexture(ALLEGRO_FONT* f , int sz , ALLEGRO_COLOR c);


#endif // Texture_HPP




#include "Texture.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"


ALLEGRO_BITMAP* CreateTexture(ALLEGRO_FONT* f , int sz , ALLEGRO_COLOR c) {
   static char ch = 'A';
   ALLEGRO_BITMAP* texmap = al_create_bitmap(sz , sz);
   al_set_target_bitmap(texmap);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_filled_rectangle(0.0 , 0.0 , sz/2 , sz/2 , al_map_rgb(255,255,255));
   al_draw_filled_rectangle(sz  ,  sz , sz/2 , sz/2 , al_map_rgb(255,255,255));
   
   int fh = al_get_font_line_height(f);
   
   al_draw_textf(f , c ,   sz/4 ,   sz/4 - fh/2 , ALLEGRO_ALIGN_CENTER , "%c" , ch + 0);
   al_draw_textf(f , c , 3*sz/4 ,   sz/4 - fh/2 , ALLEGRO_ALIGN_CENTER , "%c" , ch + 1);
   al_draw_textf(f , c ,   sz/4 , 3*sz/4 - fh/2 , ALLEGRO_ALIGN_CENTER , "%c" , ch + 2);
   al_draw_textf(f , c , 3*sz/4 , 3*sz/4 - fh/2 , ALLEGRO_ALIGN_CENTER , "%c" , ch + 3);
   ch += 4;
   
   return texmap;
}


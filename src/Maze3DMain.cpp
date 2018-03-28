





#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_image.h"

#include "allegro5/allegro_opengl.h"

#include <cstdio>
#include <cmath>



#include "MazeRoom.hpp"
#include "Globals.hpp"


int testmain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   if (!al_init()) {return -1;}
   
   int height = 10;
   int depth = 10;
   int width = 10;
   for (int y = 1 ; y < height ; ++y) {
      for (int z = 1 ; z < depth ; ++z) {
         for (int x = 1 ; x < width ; ++x) {
            Maze m;
            double start = al_get_time();
            m.CreateMaze(x , y , z);
            double stop = al_get_time();
            printf("Created %d x %d x %d maze.\n" , y , z , x);
            printf("Creating a maze of size %d X %d X %d (%d) took %3.5lf seconds\n" , y , x , z , x*y*z , stop - start);
         }
      }
   }

   
   return 0;
}




int mazemain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;

   if (!al_init()) {return 1;}

   if (!al_init_primitives_addon()) {return 2;}
   if (!al_init_font_addon()) {return 3;}
   if (!al_init_ttf_addon()) {return 4;}
   if (!al_init_image_addon()) {return 5;}

   if (!al_install_keyboard()) {return -1;}
   if (!al_install_mouse()) {return -2;}
   

   al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);

   /// This is so our OpenGL context created by allegro has a depth buffer (necessary for drawing 3D graphics)
   al_set_new_display_option(ALLEGRO_DEPTH_SIZE , 24 , ALLEGRO_SUGGEST);
   
   ALLEGRO_DISPLAY* d = al_create_display(ww,wh);
   ALLEGRO_EVENT_QUEUE* q = al_create_event_queue();
   ALLEGRO_TIMER* t = al_create_timer(1.0/60.0);
   ALLEGRO_FONT* f = al_load_ttf_font("Verdana.ttf" , -64 , 0);
   
   if (!f || !d || !t || !q) {
      return -10;
   }
   
   /// Register our event sources, or we won't get any events
   al_register_event_source(q , al_get_display_event_source(d));
   al_register_event_source(q , al_get_timer_event_source(t));
   al_register_event_source(q , al_get_keyboard_event_source());
   al_register_event_source(q , al_get_mouse_event_source());
   
   /// For keeping track of which keys are held
   bool keys[ALLEGRO_KEY_MAX] = {0};
   
   bool quit = false;
   bool redraw = true;
   
   al_start_timer(t);
   
   while (!quit) {
      if (redraw) {
         al_set_target_backbuffer(d);
         al_clear_to_color(al_map_rgb(64,64,64));
         al_flip_display();
         redraw = false;
      }

      do {
         ALLEGRO_EVENT ev;
         al_wait_for_event(q , &ev);

         /// Monitor close events
         if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            quit = true;
         }

         /// Monitor key events
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            keys[ev.keyboard.keycode] = true;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            keys[ev.keyboard.keycode] = false;
         }
         if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
         }
      } while (!al_is_event_queue_empty(q));
      
   }
   
   al_uninstall_system();
   
   return 0;
}


  
int main(int argc , char** argv) {
///   return testmain(argc,argv);
   return mazemain(argc,argv);
}





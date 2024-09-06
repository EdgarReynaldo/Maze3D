





#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"
#include "Eagle/BinFileStream2.hpp"


#include "Maze.hpp"

#include "allegro5/allegro_opengl.h"
#include "GL/gl.h"


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Maze game" , 1024 , 768 , EAGLE_WINDOWED | EAGLE_OPENGL);
   
   
   EagleImage* faces[6] = {
      win->CreateImage(256,256),
      win->CreateImage(256,256),
      win->CreateImage(256,256),
      win->CreateImage(256,256),
      win->CreateImage(256,256),
      win->CreateImage(256,256)
   };
   GLuint texids[6];
   char dirs[6] = {'U' , 'D' , 'N' , 'S' , 'E' , 'W'};
   for (unsigned int i = 0 ; i < 6 ; ++i) {
      std::string s;
      s.push_back(dirs[i]);
      win->SetDrawingTarget(faces[i]);
      win->Clear();
      win->DrawTextString(win->DefaultFont() , s , 128.0f , 128.0f , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_CENTER);
      texids[i] = al_get_opengl_texture(GetAllegroBitmap(faces[i]));
   }
   
   
   int height = 1;
   int depth = 10;
   int width = 10;

/*   for (int y = 2 ; y < height ; ++y) {
      for (int z = 2 ; z < depth ; ++z) {
         for (int x = 2 ; x < width ; ++x) {
            Maze m;
            double start = al_get_time();
            m.CreateMaze(x , y , z);
            double stop = al_get_time();
            m.KruskalRemoval();
            double stop2 = al_get_time();
            printf("Created %d x %d x %d maze.\n" , y , z , x);
            printf("Creating a maze of size %d X %d X %d (%d) took %3.5lf seconds\n" , y , x , z , x*y*z , stop - start);
            printf("Kruskal Removal took %3.6lf seconds\n" , stop2 - stop);
         }
      }
//   }
*/
   
   Maze m;
   m.CreateMaze(width , height , depth);
   m.KruskalRemoval();
   m.SetFaceTexture(ROOM_ABOVE , texids[ROOM_ABOVE]);
   m.SetFaceTexture(ROOM_BELOW , texids[ROOM_BELOW]);
   m.SetFaceTexture(ROOM_NORTH , texids[ROOM_NORTH]);
   m.SetFaceTexture(ROOM_SOUTH , texids[ROOM_SOUTH]);
   m.SetFaceTexture(ROOM_EAST , texids[ROOM_EAST]);
   m.SetFaceTexture(ROOM_WEST , texids[ROOM_WEST]);
   
   Allegro5SpaceCamera cam;
   
   m.Display();
   win->FlipDisplay();
   
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   do {
      if (redraw) {
         win->Clear();
         cam.Setup3D(false);
         m.Display();
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         if (ev.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if ((ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) || (ev.type == EAGLE_EVENT_DISPLAY_CLOSE)) {
            quit = true;
         }
         if (ev.type == EAGLE_EVENT_TIMER) {
            if (input_key_held(EAGLE_KEY_LEFT)) {
               cam.StrafeRight(-0.1);
            }
            if (input_key_held(EAGLE_KEY_RIGHT)) {
               cam.StrafeRight(0.1);
            }
            if (input_key_held(EAGLE_KEY_UP)) {
               cam.ElevateUp(0.1);
            }
            if (input_key_held(EAGLE_KEY_DOWN)) {
               cam.ElevateUp(-0.1);
            }
            if (input_key_held(EAGLE_KEY_PAD_5)) {
               cam.AdvanceForward(0.1);
            }
            if (input_key_held(EAGLE_KEY_PAD_0)) {
               cam.AdvanceForward(-0.1);
            }
            if (input_key_held(EAGLE_KEY_PAD_2)) {
               cam.PitchUp(-M_PI/180.0);// 1 degree per update
            }
            if (input_key_held(EAGLE_KEY_PAD_8)) {
               cam.PitchUp(M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_6)) {
               cam.YawRight(-M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_4)) {
               cam.YawRight(M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_9)) {
               cam.SpinCCW(M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_7)) {
               cam.SpinCCW(-M_PI/180.0);
            }
         }
         
      } while (!sys->UpToDate());
      
   } while (!quit);


   return 0;
}




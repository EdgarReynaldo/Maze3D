





#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"
#include "Eagle/BinFileStream2.hpp"


#include "Maze.hpp"

#include "allegro5/allegro_opengl.h"
#include "GL/gl.h"



int main2(int argc , char** argv) {

   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Maze game" , 1024 , 768 , EAGLE_WINDOWED | EAGLE_OPENGL);
   
   EagleImage* img = win->CreateImage(512,512);
   
   win->SetDrawingTarget(img);
   win->Clear(EagleColor(0,255,0,255));
   win->DrawTextString(win->DefaultFont() , "ABC" , 256.0f , 256.0f , EagleColor(255,127,0) , HALIGN_CENTER , VALIGN_CENTER);

   win->DrawToBackBuffer();
   win->Clear();
   
//   win->Draw(img , 256.0 , 256.0);
//*

   SetupOpenGLDebug();

//   glEnable(GL_TEXTURE_2D);
//   glEnable(GL_BLEND);
//   glBindTexture(GL_TEXTURE_2D , al_get_opengl_texture(GetAllegroBitmap(img)));
   
//   Allegro5SpaceCamera cam;
//   cam.Setup2D(1024,768);
   
   Maze m;
   m.CreateMaze(3,3,3);
   m.Display();
/**
   glBegin(GL_LINE_LOOP);
      glColor3ub(0,255,0);
      
//      glTexCoord2f(0.0f , 1.0f);
      glVertex3d(256.0 , 256.0 , 0.0);

//      glTexCoord2f(0.0f , 0.0f);
      glVertex3d(256.0 , 512.0 , 0.0);

//      glTexCoord2f(1.0f , 0.0f);
      glVertex3d(768.0 , 512.0 , 0.0);

//      glTexCoord2f(1.0f , 1.0f);
      glVertex3d(768.0 , 256.0 , 0.0);

            
   glEnd();
*/
   
   DumpErrors();
   
//*/
   win->FlipDisplay();
   sys->Rest(3.0);
   
   
   return 0;
}



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Maze game" , 1024 , 768 , EAGLE_WINDOWED | EAGLE_OPENGL);

      
   EagleFont* font = win->GetFont("Data/Fonts/Verdana.ttf" , -20);
   
   int tw = 32;
   int th = 32;
   EagleImage* faces[6] = {
      win->CreateImage(tw,th),
      win->CreateImage(tw,th),
      win->CreateImage(tw,th),
      win->CreateImage(tw,th),
      win->CreateImage(tw,th),
      win->CreateImage(tw,th)
   };
   GLuint texids[6] = {0,0,0,0,0,0};
   const char* dirs[6] = {"U" , "D" , "N" , "S" , "E" , "W"};
   for (unsigned int i = 0 ; i < 6 ; ++i) {
      std::string s = dirs[i];
      win->SetDrawingTarget(faces[i]);
      win->Clear(EagleColor(255,255,255,255));
      win->DrawTextString(font , s , tw/2 , th/2 , EagleColor(0,0,0) , HALIGN_CENTER , VALIGN_CENTER);
      texids[i] = al_get_opengl_texture(GetAllegroBitmap(faces[i]));
   }
   
   
   int height = 1;
   int depth = 1;
   int width = 1;

   
   
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
//   m.KruskalRemoval();
   m.SetFaceTexture(ROOM_ABOVE , texids[ROOM_ABOVE] , texids[ROOM_BELOW]);
   m.SetFaceTexture(ROOM_NORTH , texids[ROOM_NORTH] , texids[ROOM_SOUTH]);
   m.SetFaceTexture(ROOM_EAST , texids[ROOM_EAST] , texids[ROOM_WEST]);
   
   Allegro5SpaceCamera cam;
   
   m.Display();
   win->FlipDisplay();
   
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   SetupOpenGLDebug();

   do {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear();
         glClear(GL_DEPTH_BUFFER_BIT);
         cam.Setup3D(false);
//         glEnable(GL_DEPTH_TEST);
//         glEnable(GL_BLEND);
//         glFrontFace(GL_CCW);
//         glEnable(GL_CULL_FACE);
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

   DumpErrors();

   return 0;
}




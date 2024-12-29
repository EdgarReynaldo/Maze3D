
/**
clamkzak66 christmas wish

It would be nice to see:

- dungeon crawler type of game
- rudolph the reindeer
- random generation
- chaos (like the one in the house the day before chrstmas)
*/




#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"
#include "Eagle/BinFileStream2.hpp"

#include "PickMap.hpp"

#include "Maze.hpp"
#include "Wall.hpp"
#include "allegro5/allegro_opengl.h"
#include "GL/gl.h"





int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   al_set_config_value(al_get_system_config() , "Trace" , "Level" , "DEBUG");
   
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   
   al_set_new_display_option(ALLEGRO_DEPTH_SIZE , 32 , ALLEGRO_SUGGEST);///Without this nothing looks right. We MUST create a depth buffer.
   

   int height = 10;
   int depth = 10;
   int width = 10;
   Maze m;
   m.CreateMaze(width , height , depth);
   m.KruskalRemoval();
   int sw = 1280;
   int sh = 720;
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Maze game" , sw , sh , EAGLE_WINDOWED | EAGLE_OPENGL);
   PickMap pickmap;
   pickmap.Create(win , sw , sh);
   
   EagleFont* font40 = win->GetFont("Data/Fonts/Verdana.ttf" , -40);
   EagleFont* font12 = win->GetFont("Data/Fonts/Verdana.ttf" , -20);
   
   int tw = 64;
   int th = 64;
   EagleImage* bg = win->LoadImageFromFile("Data/Krampus24/GrayBrick2.png");
   
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
      win->Draw(bg,0,0);
      win->DrawTextString(font40 , s , tw/2 , th/2 , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_CENTER);
      texids[i] = al_get_opengl_texture(GetAllegroBitmap(faces[i]));
   }
   
   m.SetFaceTexture(ROOM_ABOVE , texids[ROOM_ABOVE] , texids[ROOM_BELOW]);
   m.SetFaceTexture(ROOM_NORTH , texids[ROOM_NORTH] , texids[ROOM_SOUTH]);
   m.SetFaceTexture(ROOM_EAST , texids[ROOM_EAST] , texids[ROOM_WEST]);
   m.SetFaceTexture(ROOM_BELOW , texids[ROOM_ABOVE] , texids[ROOM_BELOW]);
   m.SetFaceTexture(ROOM_SOUTH , texids[ROOM_NORTH] , texids[ROOM_SOUTH]);
   m.SetFaceTexture(ROOM_WEST , texids[ROOM_EAST] , texids[ROOM_WEST]);

   
/**   
   for (int y = 1 ; y <= height ; ++y) {
      for (int z = 1 ; z <= depth ; ++z) {
         for (int x = 1 ; x <= width ; ++x) {
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
   }
//*/

   Allegro5SpaceCamera pcam;
   double py = 5;
   double pz = 35;
   double px = 5;
   pcam.SetPosition((Vec3(px,py,pz)));
   pcam.SetAspect(16/9.0);
   pcam.SetHFOV(120.0);
   pcam.LookAt(Vec3(5.0,5.0,5.0) , Vec3(0,1,0));
   ROOM_FACE pface = ROOM_NORTH;
   
   
   
   win->Clear();
   win->FlipDisplay();
   
   bool quit = false;
   bool redraw = true;
   bool redraw_picmap = true;
   
   sys->GetSystemTimer()->Start();
   
   SetupOpenGLDebug();
   
   GLfloat lpos[4] = {(GLfloat)px , (GLfloat)py , (GLfloat)pz , 0.0f};
   GLfloat lcol[4] = {0.75f , 0.75f , 0.75f , 1.0f};
   GLfloat acol[4] = {1.0f , 1.0f , 1.0f , 1.0f};
   GLfloat dir[3] = {0.0f , 0.0f , 3.0f};
   
   glLightfv(GL_LIGHT0 , GL_POSITION , lpos);
   glLightfv(GL_LIGHT0 , GL_DIFFUSE , lcol);
   glLightfv(GL_LIGHT0 , GL_AMBIENT , acol);
   glLightfv(GL_LIGHT0 , GL_SPOT_DIRECTION , dir);
   
//   glEnable(GL_LIGHTING);
//   glEnable(GL_LIGHT0);
   int mx = sw/2;
   int my = sh/2;
   int mdx = 0;
   int mdy = 0;
   
   
   do {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0,0,196));
         glClear(GL_DEPTH_BUFFER_BIT);
         pcam.Setup3D(false);
         m.Display();
         pcam.Setup2D(sw,sh);
         glFrontFace(GL_CW);
         win->DrawTextString(font12 , std::string("p=") + pcam.Pos().ToString() , 10 , 10 , EagleColor(255,255,255));
         win->FlipDisplay();
         redraw = false;
         if (redraw_picmap) {
            win->SetDrawingTarget(pickmap.pickimg);
            win->Clear(EagleColor(0,0,0,0));
            pickmap.Display(m);
            redraw_picmap = false;
         }
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         if (ev.type == EAGLE_EVENT_MOUSE_AXES) {
//            msbtn_held_duration;
            mdx += mx - ev.mouse.x;
            mdy += my - ev.mouse.y;
            mx = ev.mouse.x;
            my = ev.mouse.y;
         }
         if (ev.type == EAGLE_EVENT_TIMER) {
            if (ms_held(RMB)) {
               /// Look around
               if (mdx || mdy) {
                  pcam.Turn(Vec3(pcam.Yaw() - mdx*M_PI/180.0 , pcam.Pitch() + mdy*M_PI/180.0 , 0.0) , 0.0);
               }
               mdx = 0;
               mdy = 0;
            }
            redraw = true;
         }
         if ((ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) || (ev.type == EAGLE_EVENT_DISPLAY_CLOSE)) {
            quit = true;
         }
         if (ev.type == EAGLE_EVENT_TIMER) {
            if (input_key_held(EAGLE_KEY_W)) {
               pcam.AdvanceForward(0.1);
            }
            if (input_key_held(EAGLE_KEY_S)) {
               pcam.AdvanceForward(-0.1);
            }
            if (input_key_held(EAGLE_KEY_D)) {
               pcam.StrafeRight(0.1);
            }
            if (input_key_held(EAGLE_KEY_A)) {
               pcam.StrafeRight(-0.1);
            }
            if (input_key_held(EAGLE_KEY_Z)) {
               pcam.ElevateUp(-0.1);
            }
            if (input_key_held(EAGLE_KEY_X)) {
               pcam.ElevateUp(0.1);
            }
            if (input_key_held(EAGLE_KEY_PAD_4)) {
               // turn left
               pcam.YawRight(M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_6)) {
               // turn right
               pcam.YawRight(-M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_2)) {
               pcam.PitchUp(-M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_8)) {
               pcam.PitchUp(M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_7)) {
               // turn left
               pcam.SpinCCW(-M_PI/180.0);
            }
            if (input_key_held(EAGLE_KEY_PAD_9)) {
               // turn left
               pcam.SpinCCW(M_PI/180.0);
            }
            px = pcam.Pos().x;
            py = pcam.Pos().y;
            pz = pcam.Pos().z;
            GLfloat playpos[3] = {px,py,pz};
            GLfloat dir[3] = {pcam.Forward().x , pcam.Forward().y , pcam.Forward().z};
            glLightfv(GL_LIGHT0 , GL_POSITION , playpos);
            glLightfv(GL_LIGHT0 , GL_SPOT_DIRECTION , dir);
//            GLfloat lpos[4] = {(GLfloat)cam.Pos().x , (GLfloat)cam.Pos().y , (GLfloat)cam.Pos().z , 0.0f};
//            GLfloat ldir[4] = {(GLfloat)cam.Forward().x , (GLfloat)cam.Forward().y , (GLfloat)cam.Forward().z};
//            glLightfv(GL_LIGHT0 , GL_POSITION , lpos);
//            glLightfv(GL_LIGHT0 , GL_SPOT_DIRECTION , ldir);
         }
      } while (!sys->UpToDate());
      
   } while (!quit);

   DumpErrors();

   return 0;
}




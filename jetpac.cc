//Jetpac (ZX Spectrum) by @bypauu & 9-Bit Team 03-2025
//A representation to the classic jetpac game

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
/* ESAT LIBRARIES*/
#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/math.h"
#include "esat/time.h"
#include "esat/sprite.h"
//Sound ESAT libraries

#define WIN32 //This define is places bc sound doesn't work on other OS so comment it if played on other  

#ifdef WIN32
  #include <esat_extra/soloud/soloud.h>
  #include <esat_extra/soloud/soloud_wav.h>
  #undef DrawText 
#endif

// STRUCTS FOR THIS PROJECT
#include "Enums.cc"
#include "Structs.cc"// <-- INCLUDE AFTER ESAT

/****  CONSTANTS  & Globals*****/
// DECLARE YOUR CONSTANTS HERE.  <---
//MAKE SURE THIS IS AT THE TOP OF THE FILE ASIDE FROM LIBRARIES BEING IMPORTED
const unsigned int kWindowWidth = 1024;
const unsigned int kWindowHeight = 768;
// FOR MOVEMENT
const float kPlayerSpeed = 4.5f;
const float kPlayerGravity = 4.5f;

// TO ANIMATE THE PLAYER
const int kNumPlayerAnimations= 8;

// CONSTANT FOR THE RAY
const int KNumbersOfRays = 5;
const int KNumbersOfSubRays = 10;
const int KraySpeed = 12; // the speed every subray fired

bool reached_top = false;
// TO LIMITE THE AMOUNT OF X THINGS
const int KnumberOfPickups = 6;
int high_score; // THESE THREE SHOULD TO IN AN STRUCT
int g_score = 0;
int g_score2 = 0;
/****  END OF CONSTANTS *****/
/*                                                                              	*/

/****  STRUCTS*****/
// DEFINE YOUR STRUCTS HERE. <---
Terrain* terrain = NULL;// I'LL allow this but please no more unnecesary global variables
GameManager gm; // we will have to check it a lot so making it a global might be the best
SavedGame* save = NULL; // this should have size 2 
/****  END OF STRUCTS *****/
/*                                                                              	*/
// FOR THE PLATFORM AND ROCKET POSITIONS
esat::Vec2 KRocketOriginalPos = {kWindowWidth/2 + 125, kWindowHeight - 32 - 64};
esat::Vec2 KFirstPlatformPos;
esat::Vec2 KSecondPlatformPos;
esat::Vec2 KThirdPlatformPos;

/****  FUNCTIONS *****/
// DEFINE YOUR FUNCTIONS HERE <---
  // FOR THE TERRAIN
void InitTerrain();
void DrawTerrain();
  // FOR THE PLAYER
void InitPlayer(Player* player);
void DrawPlayer(Player* player, int frame_counter);
void PlayerMovement(Player* player);
void PlayerMovingUp(Player* player);
void PlayerMovingRight(Player* player);
void PlayerMovingLeft(Player* player);
bool IsCollidingUp(esat::SpriteTransform player, esat::Vec2 WandH, Terrain* platform);
bool IsCollidingRight(esat::SpriteTransform player, esat::Vec2 WandH, Terrain* platform);
bool IsCollidingLeft(esat::SpriteTransform player, esat::Vec2 WandH, Terrain* platform);
bool IsCollidingDown(esat::SpriteTransform player, esat::Vec2 WandH, Terrain* platform);

// FOR THE OBJECTS
void InitObject(Object** object);
void SpawnObject(Object* object,int frameCounter,Ship ship);
void DrawObject(Object* object);
void ObjectInteraction(Object* objects,Player* player);
void UpdateObjects(Object* objects);
 // FOR THE SHIP
 void InitShip(Ship* ship);
 void DrawShip(Ship ship, int frameCounter);
 void UpdateShip(Ship* ship, Player* player, int frameCounter,Object* objects);
 void ShipPosAtTop(Ship* ship);
 void ShipWithFuelColor(Ship* ship, int component_part_painted, int component_piece);
 void PickComponents(Player* player, Ship* ship);
 void UpdateComponents(Ship* ship, Player* player);
 void LandingShip(Ship* ship, int frameCounter, Enemy* enemies, int kNenemys, int &actual_enemy_type);
 void ShipTakeOff(Ship* ship,int  frameCounter);
 void ShipNewLevel(Ship* ship, Player player);
 void ShipNewStage(Ship* ship);
 void loadFuel(Object* object,Ship* shp,Player* player);
 void ShipSkinChanger(Ship* ship);
 void ResetShipColor(Ship* ship);
//HUD
void GameOverScreen(int number);
void ShowScore(Player player);
void SaveHighScore();
void LoadHighScore();
void UpdateHighScore(int score);
void ShowLivesPlayer(Player player, esat::SpriteHandle sprite);
// FOR THE RAY
void AddRay(Ray** first,Player player);
RayComponent* AddSubRays(Player player);
void DrawRays(Ray** first_ray);
void FreeRays(Ray** first);
void FireRay(Ray** head,Player player);
void UpdateRay(Ray** head,Player* player,Enemy* enemies);
Ray* deleteRay(Ray** head, Ray* current_ray);
bool ColisionRightRay(RayComponent* sub_rays,Enemy* enemies);
bool ColisionLeftRay(RayComponent* sub_rays,Enemy* enemies);
void PlayerReflect(Player* player, Player* reflect);
// FOR THE ENEMIES
void InitEnemy(Enemy** enemies, int kNenemys, int &actual_enemy_type);
void DrawEnemy(Enemy* enemies, int kNenemys);
void EnemyMovement(Enemy* enemies, int kNenemys, Player* player);
void EnemySpawnRates(Enemy* enemies, int kNenemys);
void EnemyGeneration(Enemy* enemies, int kNenemys);
void EnemyAnimation(Enemy* enemies, int kNenemys);
void SwapEnemy(Enemy* enemies, int kNenemys, int &actual_enemy_type);
void TypeColors(Enemy* enemies, int kNenemys);
void KillEnemies(Enemy* enemies, int kNenemys);
//Enemies collision
void EnemyCollides(Enemy* enemies, int kNenemys, Player* player,bool iscollidingright,bool iscollidingleft,bool iscollidingdown,bool iscollidingup,Ship* ship);
bool EnemyIsCollidingUp(esat::Vec2 enemies, esat::Vec2 WandH, Terrain* platform);
bool EnemyIsCollidingRight(esat::Vec2 enemies,esat::Vec2 WandH, Terrain* platform);
bool EnemyIsCollidingLeft(esat::Vec2 enemies,esat::Vec2 WandH, Terrain* platform);
bool EnemyIsCollidingDown(esat::Vec2 enemies,esat::Vec2 WandH, Terrain* platform);
//GameManager and saving progress
void readSavedFile();
void InitGameManager();
void readLevelFromFile();
void nextLevel();
void initSavedGame();
void swapPlayer(Player* player,Ship* ship);
void saveProgress(Player player);
float AbsoluteValue(float value);// more than one libreary include had a abs() function and it was bothering me so...
bool areTheseColliding(esat::SpriteTransform tr_1,esat::Vec2 dimensions_1,esat::SpriteTransform tr_2,esat::Vec2 dimensions_2 );
// SOUND
void InitSound();
void FreeSound();
#ifdef WIN32
SoLoud::Soloud soloud;
SoLoud::Wav audio[14]; // esto es ilegal pero no pasa naranja
SoLoud::handle g_rocketLoopHandle = 0;
bool LoopingSound = false;
#endif
// MENU
void Menu(int frame_counter);
void MenuLogic(int frame_counter);
void MenuHover(float x, float y, float width, float height);


// NEED TO IMPLEMENT. USED AFTER FINNISHING A LEVEL

// void resetPlayer();
// void resetObjects();// These two related to the pick up mechanic
// void resetComponents();// so , reset timers and whatever that might be needed

/****  END OF FUNCTIONS*****/
/*                                                                              	*/   	 
/*__  __          _____ _   _       _
 |  \/  |   /\   |_   _| \ | |    _| | _
 | \  / |  /  \    | | |  \| |   \     /
 | |\/| | / /\ \   | | | . ` |    \   /
 | |  | |/ ____ \ _| |_| |\  |     \ /
 |_|  |_/_/    \_\_____|_| \_|      v
*/

int esat::main(int argc, char** argv) {
  /****  FPS MANAGEMENT *****/
  srand(time(nullptr));
  double current_time = 0.0;
  double last_time = 0.0;
  double deltaTime;
  double fps = 60.0;
  int frameCounter = 0;
  int kNenemys = 6;

  // THINGS THAT MIGHT NEED SOME REVISION 
  int actual_enemy_type = 0; // THIS IS PROBABLY : " CURRENT_ENEMY_TYPE" <--------
  bool iscollidingup = false;
  bool iscollidingdown = false;
  bool iscollidingleft = false;
  bool iscollidingright = false;// ALL THIS BOOLEANS PROBABLY SHOULDNT BE HERE
  /****  ENF OF FPS MANAGEMENT *****/
  /*                                                                              	*/
  /****  VARIABLE DECLARATION *****/
  Player player, reflect;
  Ray* first_ray= NULL;// this is a list
  Object* object;
  Ship ship;
  Enemy *enemies;
  /****  END OF VARIABLE DECLARATION *****/
  /*                                                                              	*/
  /**** "SCREEN SETTINGS" *****/
  esat::WindowInit(kWindowWidth, kWindowHeight);
  esat::WindowSetMouseVisibility(true);
  /****  END OF SCREEN SETTINGS *****/
  /*                                                                                */
  esat::SpriteHandle live_icon_ = esat::SpriteFromFile("sprites/general/live_icon.png");
  /****  INIT VARIABLES *****/
  terrain= (Terrain*)malloc( 4 * sizeof(Terrain)); // PUT THIS INSIDE INTI TERRAIN LATER!
  if(terrain!=NULL) InitTerrain();

  initSavedGame();
  InitGameManager();
  InitShip(&ship);
  ShipPosAtTop(&ship);
  //ShipNewStage(&ship);
  InitPlayer(&player);
  InitPlayer(&reflect);
  InitObject(&object); // empty pointer at this stage. malloc inside the function
  InitEnemy(&enemies, kNenemys, actual_enemy_type);
  InitSound();
  /****  END OF INIT VARIABLES ****/
  /*                                                                              	*/
  esat::DrawSetTextFont("fonts/zx-spectrum.ttf");
  LoadHighScore();
  int atLeastOneColling = 0;
  while (!esat::IsSpecialKeyDown(esat::kSpecialKey_Escape) && esat::WindowIsOpened()){

    /****  REGULAR CODE*****/
    // RENEMBER TO DO ALL THE CODE THAT ISN'T FOR DRAWING OUTSIDE THE LIMITS OF DrawBegin() AND  DrawEnd()
    last_time = esat::Time();
    /****  END OF REGULAR CODE*****/
    /*                                                                              	*/
    /**** DRAWING CODE *****/
    // PUT THE DRAWING RELATED CODE HERE <---
    
    // lets see if putting the state machine between the DrawBeingand DrawEnd causes problems or not. i think it shouldnt.
    // and its easier than making a DrawBegin() call in every case individually
    switch (gm.state){

      case KMENU:{
        #ifdef WIN32
        LoopingSound = false; //Just in case.
        #endif
        MenuLogic(frameCounter);
        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);
        Menu(frameCounter);
        esat::DrawEnd();
        esat::WindowFrame();
        break;
      }
      case KLANDING:{

        //  FUNCTIONS THAT DRAWS THE LANDING AND THEN CHANGE THE STATE TO MAIN_LOOP
        //  FALTAN LAS FUNCIONES ADECUADAS PARA ESTA PARTE
        // UpdateShip(&ship, frameCounter);
        // DrawShip(ship, frameCounter);
        // state = landing => MAIN_LOOP
        // ship = initShip(0,1,asdasdasd);
        #ifdef WIN32
        LoopingSound = true;
        #endif
        UpdateShip(&ship, &player, frameCounter,object);
        LandingShip(&ship,frameCounter,enemies,kNenemys,actual_enemy_type);
        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);

        DrawTerrain();
        DrawShip(ship, frameCounter);
      
        esat::DrawEnd();
        esat::WindowFrame();
        break;
      }

      case KMAIN_LOOP:{
        // HUD
        ShowLivesPlayer(player, live_icon_);
        ShowScore(player);
        UpdateHighScore(player.score);
        SaveHighScore();
        // PLAYER - RAY
        PlayerMovement(&player);
        FireRay(&first_ray,player); // Añadir nuevos rayos
        UpdateRay(&first_ray,&player,enemies); // Actualizar todos los rayos
        PlayerReflect(&player, &reflect);
        //SHIP
        UpdateShip(&ship, &player, frameCounter,object);
        loadFuel(object,&ship,&player);
        #ifdef WIN32
        LoopingSound = false;
        #endif
        //<---
        // OBJECTS
        SpawnObject(object,frameCounter,ship);
        UpdateObjects(object);
        ObjectInteraction(object,&player);
        PickComponents(&player,&ship);
        UpdateComponents(&ship,&player);
        // ENEMIES
        EnemySpawnRates(enemies, kNenemys);
        EnemyAnimation(enemies, kNenemys);
        EnemyCollides(enemies, kNenemys, &player,iscollidingright,iscollidingleft,iscollidingdown,iscollidingup,&ship);
        EnemyMovement(enemies, kNenemys, &player);
        //LoadFuel(); // hasta que esta funcion no exista el tanke actua no desaparecerá y a los 10 segundos de llegar al suelo se va a resetear su posicion en x( por como funciona el spawn)
        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);
        DrawTerrain();
        DrawPlayer(&player, frameCounter);
        DrawPlayer(&reflect, frameCounter);
        DrawRays(&first_ray); // Dibujar todos los rayos
        DrawObject(object); // debería dibujar el primero
        DrawShip(ship, frameCounter);
        DrawEnemy(enemies, kNenemys);
       
        esat::DrawEnd();
        esat::WindowFrame();

      break;
    }
    case KTAKEOFF:{
      // DRAW SHIP TAKING OFF
      #ifdef WIN32
      LoopingSound = true;
      #endif
      UpdateShip(&ship, &player, frameCounter,object);
      ShipTakeOff(&ship, frameCounter);
      esat::DrawBegin();
      esat::DrawClear(0, 0, 0);
      DrawTerrain();
      DrawShip(ship, frameCounter);
      
      esat::DrawEnd();
      esat::WindowFrame();
      break;
    }
    case KWON:{ // there is probably no reason to have 2 cases since its the same message but there might be.... again, probably not.
      #ifdef WIN32
      LoopingSound = false;
      #endif
      esat::DrawBegin();
      esat::DrawClear(0, 0, 0);

      ShowScore(player);
      GameOverScreen(gm.current_player+1);// curent player is either 0 or 1 ( for convenience) hence the plus 1

      esat::DrawEnd();
      esat::WindowFrame();
      break;
    }
    case KLOST:{
      #ifdef WIN32
      LoopingSound = false;
      #endif
      esat::DrawBegin();
      esat::DrawClear(0, 0, 0);

      ShowScore(player);
      GameOverScreen(gm.current_player+1);// curent player is either 0 or 1 ( for convenience) hence the plus 1

      esat::DrawEnd();
      esat::WindowFrame();
      break;
    }
      
    default:{
      #ifdef WIN32
      LoopingSound = false;
      #endif
      printf("ERROR INEXPLICABLE DE OTRA DIMENSION. EG: NO SE QUE PASÓ");

      esat::DrawEnd(); // IN CASE SOME DRAW BEGIN ISNT CALLED
      esat::WindowFrame();
      break;
    }
  }


    // after every case this will count the fps regardless of the state of the game
    do{
      current_time = esat::Time();
      deltaTime = current_time - last_time;
    } while((deltaTime) <= 1000.0 / fps);
    frameCounter = (frameCounter>=60)?0:frameCounter +1;




    /****  END OF DRAWING CODE*****/
    /*                                                                              	*/

    
  }
  esat::WindowDestroy();
  /****  FREE MEMORY *****/
  // FREE ANY POINTER YOU HAVE MEMORY ASSIGNED TO HERE <---
  // DONT FORGET TO FREE IT PLEASE I'LL GET REALLY SAD IF YOU DO.
  // FREE THE PLAYER
  
  
  
  /**** END OF FREE MEMORY*****/

  return 0;
}                               
/*__  __          _____ _   _        ^
 |  \/  |   /\   |_   _| \ | |      / \
 | \  / |  /  \    | | |  \| |     /   \
 | |\/| | / /\ \   | | | . ` |    /_____\
 | |  | |/ ____ \ _| |_| |\  |      | |
 |_|  |_/_/    \_\_____|_| \_|      |_|

*/
void initSavedGame(){
  save = (SavedGame*)malloc( 2 * sizeof(SavedGame));// space for two players

  if(save == NULL){
    printf("error al asignar memoria a save");
  }
  else {
  save->current_level = 1;
  save->level_stage = 1;
  save->lives = 5;
  save->score = 0;

  (save +1)->current_level = 1;
  (save +1)->level_stage = 1;
  (save +1)->lives = 5;
  (save +1)->score = 0;
  }
}

void readSavedFile(){
  size_t file_size;
  FILE* file;

  if(save !=NULL){
    file = fopen("save.dat","rb");
    if(!file){
        printf("archivo no existe");
        return;
    } 
  
    // we check if the file is empty 
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    if( file_size <sizeof(SavedGame)){
        fclose(file);
        return;
    }
    // 
    gm.state = KLANDING;  // if the file have data in it go straight to the game
    rewind(file);
    printf("File size: %lu, Expected size: %lu\n", file_size, gm.num_players * sizeof(SavedGame));
    for(int i = 0;i<gm.num_players;i++){
      if (fread((save+i),  sizeof(SavedGame), 1, file) != 1) { // save +0 is the first player and save +1 the second
        printf("Error al leer el nivel saved file.\n");
        gm.level.current_level = 1;
        gm.level.level_stage = 1;
        fclose(file);
        return;
      }
  
    }
  
  
    fclose(file);

  }
  // after this we should have the games for both players. in case only one has a save. the other player should have only 0s in file
}

void InitGameManager(){
  gm.num_players = 1; // for the moment
  gm.current_player = 0; // it is easier for me to take player 1 as 0 for reasons yoy see at the end of this function
  gm.is_newGame = true;// to check if its needed to load a game savefile or not
  gm.is_game_paused;
  gm.no_objects_displayed = true; // can be misleading. if this is true means there is no objects hence we spawn one 
  gm.frames_Since_Object_Spawn = 0;
  gm.frames_Since_Fuel_Spawn = 0;
  gm.state = KMENU; // by default LANDING is the first state of the game but i dont have the code yet
  readSavedFile();
  gm.level.current_level = ((save + gm.current_player)->current_level == 0)?1:(save +  gm.current_player)->current_level;// if the save file has nothing just put 1, in any other case use the value os the save file
  gm.level.level_stage = ((save + gm.current_player)->level_stage == 0)?1:(save + gm.current_player)->level_stage;// if the save file has nothing just put 1, in any other case use the value os the save file
  readLevelFromFile();

}

void readLevelFromFile(){
  // si está vacio el struct tengo que cargar nivel1 , sino tengo que leer el nivel actualy sumarle uno
  int level = (gm.level .current_level -1) *4;// level one need to make no difference when reading the file so i put a -1 and put it to zero
  int stage = gm.level.level_stage -1;

  size_t file_size;
  FILE* file;
  
  file = fopen("levels.dat","rb");
  if(!file){
      printf("archivo no existe");
      return;
  } 

  // we check if the file is empty 
  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  if( file_size <sizeof(Level)){
      fclose(file);
      return;
  }
  // go back to the point we want to read
  fseek(file, (level + stage) * sizeof(Level), SEEK_SET);

  if (fread(&(gm.level), sizeof(Level), 1, file) != 1) {
    printf("Error al leer el nivel.\n");
    gm.level.current_level = 1;
    gm.level.level_stage = 1;
    fclose(file);
    return;
  }
  // Mostrar los datos leídos
  printf("\nCurrent_Level: %d\n", gm.level.current_level);
  printf("Level_stage: %d\n", gm.level.level_stage);
  printf("Enemy_to_spawn: %d\n", gm.level.enemy_to_spawn);
  printf("Is_Ship_Built: %d\n", gm.level.is_ship_built);
  printf("*************************************************\n");

  fclose(file);

}

void nextLevel(){
  if(gm.level.level_stage< 4){
    gm.level.level_stage++;
    (save +gm.current_player)->current_level = gm.level.level_stage;
    readLevelFromFile();
  }
  else{
    if(gm.level.current_level == 4 && gm.level.level_stage == 4){
      return;
    }
    gm.level.current_level++;
    gm.level.level_stage = 1;

    (save +gm.current_player)->current_level;
    (save +gm.current_player)->level_stage =  gm.level.level_stage;
    readLevelFromFile();
  }
  
}

void swapPlayer(Player* player,Ship* ship){
  // CALL THIS FUNCTION AFTER THE PLAYER IS HIT
  // to load the game for the next player it would look like this:
  /*
    SWAP the player if needed. if not, we have just 1 player; reset the position of the player
  */
  if(gm.num_players == 2){
    printf(" hay dos players ? ");
    // change player
    // Make some kind of transition
    gm.current_player = !gm.current_player; // change player from 1 to 0 or 0 to 1
    gm.level.current_level = ((save + gm.current_player)->current_level == 0)?1:(save +  gm.current_player)->current_level;// if the save file has nothing just put 1, in any other case use the value os the save file
    gm.level.level_stage = ((save + gm.current_player)->level_stage == 0)?1:(save + gm.current_player)->level_stage;// if the save file has nothing just put 1, in any other case use the value os the save file
    readLevelFromFile();
    gm.state = KLANDING;
    reached_top = false; // sin esto creo que no fonfona
  }
  else{
    // only one player so 
    //Player respawns after a brief inmunity in the starting position
    player->transform.x = kWindowWidth / 2;
    player->transform.y = kWindowHeight - ((terrain)->height) - player->height ; // one pixel over the floor just in case
  
  }
  
}

void saveProgress(Player player){
  FILE  *datFile;

  // Abrir el archivo .dat en modo escritura binaria
  
  datFile = fopen("save.dat", "wb");
  if (datFile == NULL) {
      printf("Error abriendo el .Dat\n");
      return ;
  }
  (save +gm.current_player)->current_level  = gm.level.current_level;
  (save +gm.current_player)->level_stage  = gm.level.level_stage;
  (save +gm.current_player)->lives = player.lives;
  (save +gm.current_player)->score = player.score;
  // falta actualizar puntaje y vidas. pasar player a esta funci
  fseek(datFile,0, SEEK_SET);
  fwrite(save, sizeof(SavedGame), 1, datFile);
  fclose(datFile);
}
// MENU
void Menu(int frame_counter){
  #ifdef WIN32
  static bool reproduced_already = false; //Static bool to reproduce the sound only once despite Menu function being called multiple times
  if (!reproduced_already) {
    soloud.play(audio[1]); //Startup menu sound 
    reproduced_already = true; //Maked true to avoid reproducing the sound again
  }
  #endif
  esat::DrawSetTextSize(25);
  esat::DrawSetFillColor(255,255,255);
  esat::DrawText(200, 300, "1   1 PLAYER GAME"); 
  esat::DrawText(200, 350, "2   2 PLAYER GAME");
  esat::DrawText(200, 400, "3   KEYBOARD");
  esat::DrawText(200, 450, "4   KEMPSTON JOYSTICK");
  esat::DrawText(200, 600, "5   START GAME");
  esat::DrawText(50, 725, " ©1983 A.C.G ALL RIGHTS RESERVED");
  esat::DrawText(200, 200, "JETPAC GAME SELECTION");
  //Select 1 or 2 players
  if(gm.num_players==1){
    if(frame_counter>30){
      esat::DrawSetFillColor(0,0,0); //Color text set to black
      MenuHover(200.0f, 270.0f, 600.0f, 40.0f); //Hover on the 1 player option
      esat::DrawText(200, 300, "1   1 PLAYER GAME"); //Draw black text
    }else{
      esat::DrawSetFillColor(255,255,255); //Color text set to white 
      esat::DrawText(200, 300, "1   1 PLAYER GAME"); //Draw white text when hover is not active
    }
  }
  if(gm.num_players==2){
    if(frame_counter>30){
      esat::DrawSetFillColor(0,0,0); //Color text set to black
      MenuHover(200.0f, 320.0f, 600.0f, 40.0f); //Hover on the 2 player game option
      esat::DrawText(200, 350, "2   2 PLAYER GAME");
    }else{
      esat::DrawSetFillColor(255,255,255); //Color text set to white 
      esat::DrawText(200, 350, "2   2 PLAYER GAME");
    }
  }
  //Select Keyboard or Joystick
  if(gm.keyboard_or_joystick==0){
    if(frame_counter>30){
      esat::DrawSetFillColor(0,0,0); //Color text set to black
      MenuHover(200.0f, 370.0f, 600.0f, 40.0f); //Hover on the Keyboard option
      esat::DrawText(200, 400, "3   KEYBOARD");
    }else{
      esat::DrawSetFillColor(255,255,255); //Color text set to white 
      esat::DrawText(200, 400, "3   KEYBOARD");
    }
  }
  if(gm.keyboard_or_joystick==1){
    if(frame_counter>30){
      esat::DrawSetFillColor(0,0,0); //Color text set to black
      MenuHover(200.0f, 420.0f, 610.0f, 40.0f); //Hover on the Joystick option
      esat::DrawText(200, 450, "4   KEMPSTON JOYSTICK");
      MenuHover(100.0f, 500.0f, 840.0f, 40.0f); //Hover on the No Kempston Joystick detected
      esat::DrawText(100, 530, "NO KEMPSTON JOYSTICK DETECTED");
    } else {
      esat::DrawSetFillColor(255,255,255); //Color text set to white 
      esat::DrawText(200, 450, "4   KEMPSTON JOYSTICK");
      esat::DrawText(100, 530, "NO KEMPSTON JOYSTICK DETECTED");
    }
  }
}

void MenuLogic(int frame_counter){
  gm.level.current_level = 1;
  gm.level.level_stage = 1;
  readLevelFromFile();
  //Determinate the number of the player
  if(esat::IsKeyDown('1')){
    gm.num_players = 1;    
  }else if(esat::IsKeyDown('2')){
    gm.num_players = 2;
  }

  if(esat::IsKeyDown('3')){
    gm.keyboard_or_joystick = 0;
  }else if(esat::IsKeyDown('4')){
    gm.keyboard_or_joystick = 1;
  }

  if(esat::IsKeyDown('5') && gm.num_players!=0 && gm.keyboard_or_joystick!=1){
    gm.state = KLANDING;
    
  }
}

void MenuHover(float x, float y, float width, float height){
  float points[10]={
    x, y,
    x, y+height,
    x+width, y+height,
    x+width, y,
    x,y
  };
  esat::DrawSetFillColor(255,255,255);
  esat::DrawSolidPath(points, 5);
  esat::DrawSetFillColor(0,0,0);
}
/*.___________. _______ .______      .______          ___       __  .__   __. 
  |           ||   ____||   _  \     |   _  \        /   \     |  | |  \ |  | 
  `---|  |----`|  |__   |  |_)  |    |  |_)  |      /  ^  \    |  | |   \|  | 
      |  |     |   __|  |      /     |      /      /  /_\  \   |  | |  . `  | 
      |  |     |  |____ |  |\  \----.|  |\  \----./  _____  \  |  | |  |\   | 
      |__|     |_______|| _| `._____|| _| `._____/__/     \__\ |__| |__| \__|                                                                         
*/
void InitTerrain(){
  if(terrain != NULL){
    // LATER ON we will need to change how we assign the initial position. hard coding it now is ok
    // but in the future we will want to do it taking the screen size into consideration
    // FLOOR
    (terrain + 0)->sprite = esat::SpriteFromFile("sprites/terrain/floor.png");
    esat::SpriteTransformInit(  &((terrain+0)->transform) );
    (terrain+0)->transform.x = 0;
    (terrain+0)->transform.y = 736;
    (terrain+0)->transform.scale_x *= 1.0f;
    (terrain+0)->transform.scale_y *= 1.0f;
    (terrain+0)->width = 1124;
    (terrain+0)->height = 32;
    // PLATFORM 1 AND 2 AKA BIG PLATFORMS
    (terrain + 1)->sprite = esat::SpriteFromFile("sprites/terrain/grass2.png");
    esat::SpriteTransformInit(  &((terrain+1)->transform) );
    (terrain+1)->transform.x = 132;
    (terrain+1)->transform.y = 292;
    (terrain+1)->transform.scale_x *= 1.0f;
    (terrain+1)->transform.scale_y *= 1.0f;
    (terrain+1)->width = 192;
    (terrain+1)->height = 32;
    /*                                      */
    (terrain + 2)->sprite = esat::SpriteFromFile("sprites/terrain/grass2.png");
    esat::SpriteTransformInit(  &((terrain+2)->transform) );
    (terrain+2)->transform.x = 750;
    (terrain+2)->transform.y = 192;
    (terrain+2)->transform.scale_x *= 1.0f;
    (terrain+2)->transform.scale_y *= 1.0f;
    (terrain+2)->width = 192;
    (terrain+2)->height = 32;
    // PLATFORM 3 AKA SMALL PLATFORM
    (terrain + 3)->sprite = esat::SpriteFromFile("sprites/terrain/grass1.png");
    esat::SpriteTransformInit(  &((terrain+3)->transform) );
    (terrain+3)->transform.x = 450;
    (terrain+3)->transform.y = 400;
    (terrain+3)->transform.scale_x *= 1.0f;
    (terrain+3)->transform.scale_y *= 1.0f;
    (terrain+3)->width = 128;
    (terrain+3)->height = 32;
  }else{ printf("Terrain NULL");}
}

void DrawTerrain(){
  if(terrain != NULL){
    esat::DrawSprite((terrain + 0)->sprite, (terrain + 0)->transform);  //FLOOR
    esat::DrawSprite((terrain + 1)->sprite, (terrain + 1)->transform);  //Big Platfm 1
    esat::DrawSprite((terrain + 2)->sprite, (terrain + 2)->transform);  //Big Platfm 2
    esat::DrawSprite((terrain + 3)->sprite, (terrain + 3)->transform);  //Small Platfm
  }
}

/*.______    __          ___   ____    ____  _______ .______      
  |   _  \  |  |        /   \  \   \  /   / |   ____||   _  \     
  |  |_)  | |  |       /  ^  \  \   \/   /  |  |__   |  |_)  |    
  |   ___/  |  |      /  /_\  \  \_    _/   |   __|  |      /     
  |  |      |  `----./  _____  \   |  |     |  |____ |  |\  \----.
  | _|      |_______/__/     \__\  |__|     |_______|| _| `._____|
*/
void InitPlayer(Player* player){
  // I don't know if it's going to be one or two sprites.
  player->sprite = (esat::SpriteHandle*) malloc (sizeof(esat::SpriteHandle)*kNumPlayerAnimations);

  if(player->sprite!=NULL){
    *(player->sprite+0) = esat::SpriteFromFile("sprites/player/player1.png");
    *(player->sprite+1) = esat::SpriteFromFile("sprites/player/player4.png");
    *(player->sprite+2) = esat::SpriteFromFile("sprites/player/player3.png");
    *(player->sprite+3) = esat::SpriteFromFile("sprites/player/player2.png");
    *(player->sprite+4) = esat::SpriteFromFile("sprites/player/flyplayer1.png");
    *(player->sprite+5) = esat::SpriteFromFile("sprites/player/flyplayer2.png");
    *(player->sprite+6) = esat::SpriteFromFile("sprites/player/flyplayer3.png");
    *(player->sprite+7) = esat::SpriteFromFile("sprites/player/flyplayer4.png");
    
    
    
    esat::SpriteTransformInit(&(player->transform));
    player->width = esat::SpriteWidth(*(player->sprite));
    player->height = esat::SpriteHeight(*(player->sprite));  
    
    //printf("%f",player->height);
    player->transform.x = kWindowWidth / 2;
    player->transform.y = kWindowHeight - ((terrain)->height) - player->height ; // one pixel over the floor just in case
    player->component = NULL;
    player->object = NULL;
    player->is_holding_object = false;
    player->is_holding_component = false;
    player->is_on_ground = false; // for the moment lets init this as false since the player doest spawn on the ground
    player->animation_state = 0;
    
    player->score = (save->score != NULL)?save->score:0;
    player->lives = (save->lives)?save->lives:5;
  }
}

void DrawPlayer(Player* player, int frame_counter){
  
  if(player->is_on_ground){
    if(esat::IsKeyPressed('A') || esat::IsKeyPressed('D')){
      
      if(frame_counter%3==0){
        player->animation_state++;
      }
      if(player->animation_state>3){
        player->animation_state=0;
      }
    }else{
      player->animation_state=0; // player1 (not moving)
    }
  }else{
      if(frame_counter%3==0){
        player->animation_state++;
      }
      if(player->animation_state>7){
        player->animation_state=4;
      }
  }
  esat::DrawSprite(*(player->sprite+player->animation_state), player->transform);
  
}

void PlayerMovement(Player* player){
  // PLAYER MOVEMENT IN EVERY DIRECTION
  PlayerMovingUp(player);
  PlayerMovingRight(player);
  PlayerMovingLeft(player);
  // FALLS
  bool collidingdown = false;
  if(!(esat::IsKeyPressed('W'))){
    for (int i = 1; i < 4; ++i){
      if (IsCollidingDown(player->transform, {player->width, player->height}, terrain+i) == true){
        collidingdown = true;
        player->is_on_ground = true;
      }
    }
    if ((player->transform.y + player->height) >= 736) {
      collidingdown = true;
      player->is_on_ground = true;
    }
    if (collidingdown == false){
      player->transform.y += kPlayerGravity;
      player->is_on_ground = false;
    }
  }
  // either drops the objects or makes it follow the player
  // COED TO DROP THE FUEL TANK
  if( player->is_holding_object == true){
    if( player->object->transform.x >= (KRocketOriginalPos.x)  &&  player->object->transform.x <= (KRocketOriginalPos.x + 16) ){
      // let go off the object is you pass over the ship
      player->is_holding_object = false;
      player->object->is_being_held = false; 
      player->object->was_dropped = true;
      //player->object->transform.y = player->transform.y + (player->height) +1;
      player->object = NULL;
    }
    else{
      player->object->transform.x = player->transform.x + (player->width/2) - (player->object->width/2);// position the object in the middle of the player
      player->object->transform.y = player->transform.y + (player->height) - (player->object->height);
    }
  }
  // CODE TO DROP THE SHIP COMPONENT. PRACTICALLY THE SAME. they could be the same if we had the time to fix it.
  if( player->is_holding_component == true){
    if( player->component->transform.x>=(KRocketOriginalPos.x)  &&  player->component->transform.x<=(KRocketOriginalPos.x + 64) ){// >ship pos , <pos + width
      // let go off the object is you pass over the ship
      player->is_holding_component = false;
      player->component->is_being_held = false; 
      player->component->was_dropped = true;
      //printf("\ndroppea aquí o no?%d",player->component->was_dropped);
      //player->object->transform.y = player->transform.y + (player->height) +1;
      //player->component = NULL;
    }
    else{
      player->component->transform.x = player->transform.x + (player->width/2) - (player->component->width/2);// position the object in the middle of the player
      player->component->transform.y = player->transform.y + (player->height) - (player->component->height);
    }
  }
}

bool IsCollidingUp(esat::SpriteTransform player, esat::Vec2 WandH, Terrain* platform){
  // Vector colider from the player
  esat::Vec2 P1 = {player.x + (WandH.x/3), player.y}; // first point from the sprite of the player
  esat::Vec2 P2 = {player.x + WandH.x - (WandH.x/3), player.y}; // second point from the sprite of the player
  
  // Vector colider from the platform 
  esat::Vec2 Q1 = {platform->transform.x, platform->transform.y}; // first point from the platform
  esat::Vec2 Q2 = {platform->transform.x + platform->width, platform->transform.y}; // second point from the platform 
  esat::Vec2 Q3 = {platform->transform.x, platform->transform.y + platform->height}; // third point from the platfrom

  //esat::DrawSetStrokeColor(255,0,0);
  //esat::DrawLine(P1.x,P1.y,P2.x,P2.y);
  //esat::DrawLine(Q1.x,Q1.y,Q2.x,Q2.y);
  
  // First checks if the player is in the position Y of the platform
  // Then checks if the player position X1 and X2 that are the points from up
  // if the player have one point inside the width of the platform it is colliding
  if (player.scale_x == 1){
    if ((P1.y < Q3.y && P1.y > Q1.y) && (P2.x > Q1.x && P1.x < Q2.x)) { 
      //printf("colisiona por arriba\n");
      return true;
    } else{
      return false;
    }
  } else {
    if ((P1.y < Q3.y && P1.y > Q1.y) && (P1.x > Q1.x && P2.x < Q2.x)) { 
      ///printf("colisiona por arriba\n");
      return true;
    } else{
      return false;
    }
  }
}

bool IsCollidingRight(esat::SpriteTransform player, esat::Vec2 WandH, Terrain* platform){
  // Vector colider from the player
  esat::Vec2 P2 = {player.x + WandH.x, player.y}; // second point from the player
  esat::Vec2 P4 = {player.x + WandH.x, player.y + WandH.y - (WandH.y/10)}; // fourth point from the player
  
  // Vector colider from the platform
  esat::Vec2 Q1 = {platform->transform.x, platform->transform.y}; // first point from the platform
  esat::Vec2 Q2 = {platform->transform.x + platform->width, platform->transform.y}; // second point from the platform
  esat::Vec2 Q3 = {platform->transform.x, platform->transform.y + platform->height}; // third point from the platform
  
  //esat::DrawSetStrokeColor(255,0,0);
  //esat::DrawLine(P2.x,P2.y,P4.x,P4.y);
  //esat::DrawLine(Q1.x,Q1.y,Q3.x,Q3.y);

  // First checks the platform height is inside the
  // box colider from the player
  // then it checks if the player is not colliding with the platform in X
  if ((P2.x > Q1.x && P2.x < Q2.x) && (Q1.y < P4.y && Q3.y > P2.y)){
    // If it is colliding then return true
    //printf("colisiona por der\n");
    return true;
  } else {
    // If it is not colliding the return false
    return false;
  }
}

bool IsCollidingLeft(esat::SpriteTransform player, esat::Vec2 WandH, Terrain* platform){
  // Vector colider from the player
  esat::Vec2 P1 = {player.x + WandH.x, player.y}; // first point of the player
  esat::Vec2 P3 = {player.x + WandH.x, player.y + WandH.y - (WandH.y/10)}; // third point of the player
  
  // Vector colider from the platform
  esat::Vec2 Q1 = {platform->transform.x, platform->transform.y}; // first point from the platform
  esat::Vec2 Q2 = {platform->transform.x + platform->width, platform->transform.y}; // second point from the platform
  esat::Vec2 Q4 = {platform->transform.x + platform->width, platform->transform.y + platform->height}; // fourth point from the platform 
  
  ////esat::DrawSetStrokeColor(255,0,0);
  ////esat::DrawLine(P1.x,P1.y,P3.x,P3.y);
  ////esat::DrawLine(Q2.x,Q2.y,Q4.x,Q4.y);

  // First checks the platform height is inside the
  // box colider from the player
  // then it checks if the player is not colliding with the platform in X
  if ((P1.x > Q1.x && P1.x < Q2.x ) && (P3.y > Q2.y && P1.y < Q4.y)){
    // If it is colliding then return true
    //printf("colisiona por izq\n");
    return true;
  } else {
    // If it is not colliding the return false
    return false;
  }
}

bool IsCollidingDown(esat::SpriteTransform player, esat::Vec2 WandH, Terrain* platform){
  // Vector colider from the player
  esat::Vec2 P3 = {player.x + WandH.x/2, player.y + WandH.y}; // Third point of the player
  esat::Vec2 P4 = {player.x + WandH.x/3, player.y + WandH.y}; // Fourth point of the player 
  
  // Vector colider from the platform 
  esat::Vec2 Q1 = {platform->transform.x, platform->transform.y}; // first point from the platform
  esat::Vec2 Q2 = {platform->transform.x + platform->width, platform->transform.y}; // second point from the platform 
  esat::Vec2 Q3 = {platform->transform.x, platform->transform.y + platform->height}; // third point from the player
  
  //esat::DrawSetStrokeColor(255,0,0);
  //esat::DrawLine(P3.x,P3.y,P4.x,P4.y);
  //esat::DrawLine(Q1.x,Q1.y,Q2.x,Q2.y);

  // First checks if the player is in the position Y of the platform
  // Then checks if the player position X1 and X2 that are the points from up
  // if the player have one point inside the width of the platform it is colliding
  if ((P4.y >= Q1.y && P4.y <= Q3.y) && (P4.x > Q1.x && P3.x < Q2.x)) { 
    //printf("colisiona por abajo\n");
    return true;
  } else{
    return false;
  }
}

void PlayerMovingUp(Player* player){
  esat::SpriteTransform temp_tr1;
  // Creates a temporaly transform for check if where it is
  // moving it will colide or not
  temp_tr1.x = player->transform.x;
  temp_tr1.y = player->transform.y - kPlayerSpeed;
  bool collidingup = false;
  
  if(esat::IsKeyPressed('W')){
    player->is_on_ground = false;
    for (int i = 1; i < 4; ++i){
      if ((IsCollidingUp(temp_tr1, {player->width, player->height}, terrain+i) == true)){
        // For making sure that the player is colliding
        collidingup = true;
      }
    }
    if (collidingup == false){
      // here the player starts moving up
      player->transform.y -= kPlayerSpeed;
    }
  }
}

void PlayerMovingRight(Player* player){
  esat::SpriteTransform temp_tr2;
  // Creates a temporaly transform for check if where it is
  // moving it will colide or not
  temp_tr2.x = player->transform.x - kPlayerSpeed;
  temp_tr2.y = player->transform.y;
  bool collidingright = false;
  
  if(esat::IsKeyPressed('D')){
    for (int i = 1; i < 4; ++i){
      if ((IsCollidingRight(temp_tr2, {player->width, player->height}, terrain+i) == true)){
        // For making sure that the player is colliding
        collidingright = true;
      }
    }
    if (collidingright == false){
      // here the player starts moving up
      player->transform.x += kPlayerSpeed;
      if (player->transform.scale_x != 1){
        player->transform.scale_x = 1;
        player->width *= -1;
        player->transform.x -= player->width;
      }
      if (player->transform.x > kWindowWidth){
        player->transform.x = 0;
      }
    }
  }
}

void PlayerMovingLeft(Player* player){
  esat::SpriteTransform temp_tr3;
  // Creates a temporaly transform for check if where it is
  // moving it will colide or not
  temp_tr3.x = player->transform.x - kPlayerSpeed;
  temp_tr3.y = player->transform.y;
  bool collidingleft = false;
  
  if(esat::IsKeyPressed('A')){
    for (int i = 1; i < 4; ++i){
      if ((IsCollidingLeft(temp_tr3, {player->width, player->height}, terrain+i) == true)){
        // For making sure that the player is colliding
        collidingleft = true;
      }
    }
    if (collidingleft == false){
      // here the player starts moving up
      player->transform.x -= kPlayerSpeed;
      if(player->transform.scale_x != -1){
        player->transform.scale_x = -1;
        player->width *= -1;
        player->transform.x -= player->width;
      }
      if (player->transform.x < 20){
        player->transform.x = kWindowWidth;
      }
    }
  }
}

void PlayerReflect(Player* player, Player* reflect) {
  reflect->transform.y = player->transform.y;
  reflect->transform.scale_x = player->transform.scale_x;
  if (player->transform.scale_x == 1) {
    reflect->transform.x = player->transform.x - kWindowWidth;
  } else if (player->transform.scale_x == -1) {
    reflect->transform.x = player->transform.x + kWindowWidth;
  }
  reflect->animation_state = player->animation_state;
}

bool areTheseColliding(esat::SpriteTransform tr_1,esat::Vec2 dimensions_1,esat::SpriteTransform tr_2,esat::Vec2 dimensions_2 ){
  // with the positions and dimensions of two objects lets see if they collide, return true if so. Why pass this instead o PLayer and Enemy?
  // this way this function is more generic and anything with a transform can be checked. dimensions is just width and height. this ways is a third less parameters

  float Ax,Ay,Aw,Ah; 
  float Bx,By,Bw,Bh;

  Ax= tr_1.x;
  Ay= tr_1.y;
  Aw = dimensions_1.x;
  Ah = dimensions_1.y;

  Bx= tr_2.x;
  By= tr_2.y;
  Bw = dimensions_2.x;
  Bh = dimensions_2.y;
  //This part is not usually necesary but since the player scales is multiplied by -1 and this changes the coordinates a little
  // we need to "normalize" it before checking the collision. basically just making it positive.
  if (Aw < 0) { Ax += Aw; Aw = -Aw; }  // 
  if (Bw < 0) { Bx += Bw; Bw = -Bw; }
  // Detect collision AABB. return true if A is "inside" B. no homo.
  // imagine this as the superior version of checking if player.x <enemy.x ........
  return (Ax < Bx + Bw && Ax + Aw > Bx &&
          Ay < By + Bh && Ay + Ah > By);

  // well known algorith. " Bounding Box Collision Detection". Works just fine if the boxes dont rotate
  // if the left side of A is before the right side of B and the right side of A is after the left side of B &&
  // if the top of A is over the bottom of B and the bottom of A is under the top of A then return return
}

/*.  _    _  _    _  _____  
  | | |  | || |  | ||  __ \ 
  | | |__| || |  | || |  | |
  | |  __  || |  | || |  | |
  | | |  | || `--' || |__| |
  |_|_|  |_(_)____/ |_____/  
*/

void GameOverScreen(int number) {
  char *game_over_text = (char*) malloc(50 * (sizeof(char)));
  snprintf(game_over_text, (50*sizeof(char)), "GAME OVER PLAYER %d", number);
  esat::DrawSetFillColor(255,255,255);
  esat::DrawSetTextSize(25);
  esat::DrawText((kWindowWidth/4),(kWindowHeight/2),game_over_text);
}

void ShowScore(Player player) {
	char *score_text = (char*) malloc(50 * (sizeof(char))); // size of the char[]
  char *score_text_ = (char*) malloc(50 * (sizeof(char)));
  char *score_text2 = (char*) malloc(50 * (sizeof(char))); // size of the char[]
  char *score_text_2 = (char*) malloc(50 * (sizeof(char)));
	char *high_score_text = (char*) malloc(50 * (sizeof(char)));
  char *high_score_text_ = (char*) malloc(50 * (sizeof(char)));
	snprintf(score_text, sizeof(score_text), "1UP"); // formatted text to a character buffer
  snprintf(score_text_, (6*sizeof(int)), "%06d", player.score);
  snprintf(score_text2, sizeof(score_text2), "2UP"); // formatted text to a character buffer
  snprintf(score_text_2, (6*sizeof(int)), "%06d", g_score2);
	snprintf(high_score_text, sizeof(high_score_text), "HI");
  snprintf(high_score_text_, (6*sizeof(int)), "%06d", high_score);
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(25);
	esat::DrawText(60.0f, 30.0f, score_text);
  esat::DrawText(870.0f, 30.0f, score_text2);
  esat::DrawSetFillColor(4, 252, 252);
	esat::DrawText(480.0f, 30.0f, high_score_text);
  esat::DrawSetFillColor(252, 252, 4);
  esat::DrawText(20.0f, 70.0f, score_text_);
  esat::DrawText(830.0f, 70.0f, score_text_2);
  esat::DrawText(440.0f, 70.0f, high_score_text_);
}

void ShowLivesPlayer(Player player, esat::SpriteHandle sprite){
  char *lives = (char*) malloc(sizeof(char));
  snprintf(lives, sizeof(int), "%d", player.lives);
  esat::DrawSetFillColor(255, 255, 255);
  esat::DrawText(220.0f,50.0f,lives);
  esat::DrawSprite(sprite,252, 20);
}

// Save the higher score of the player
void SaveHighScore() {
  FILE *file = fopen("highscore.txt", "w"); // Open a write file
  // check if the file opened great
  if (file != NULL) {
    fprintf(file, "%d", high_score); // writes the higher score in the file
    fclose(file); // close the file
  }
}

// Loads the higher score of the file previously saved
void LoadHighScore() {
  FILE *file = fopen("./highscore.txt", "r"); // opens the file in only reading mode, with no writing
  // check if the file opened great
  if (file != NULL) {
    fscanf(file, "%d", &high_score); // reads the higher score in the file
    fclose(file); // close the file
  } 
}

// Checks if the current score is higher than the last score
void UpdateHighScore(int score) {
  // if is higher then it changes the higher score
  if (score >= high_score) {
    high_score = score; // changes the high score
  }
}

/*
   _____  _____  _____ _  __ _    _ _____    _____  
  | |  __ \|  __ \|_   _| |/ /| |  | |  __ \  | ____| 
  | | |__) | |__) | | | | ' / | |  | | |__) | | |__   
  | |  ___/|  _  /  | | |  <  | |  | |  ___/  |___ \  
  | | |    | | \ \ _| |_| . \ | |__| | |      ___) | 
  |_|_|    |_|  \_\_____|_|\_\ \____/|_|     |____/  
*/
void InitObject(Object** object){
  const char *path[KnumberOfPickups] = {
    "sprites/objects/fuel.png",
    "sprites/objects/atom.png",
    "sprites/objects/diamond.png",
    "sprites/objects/lingot.png",
    "sprites/objects/nuke.png",
    "sprites/objects/slime.png",
  };
  *object = (Object*)malloc( KnumberOfPickups * sizeof(Object));
  if(*object != NULL){
    for(int i=0;i<KnumberOfPickups;i++){
      // do until size of the enum -1
      
      (*(*object + i)).sprite = esat::SpriteFromFile((path[i]));
      if((*(*object + i)).sprite != NULL){
        (*(*object + i)).width = esat::SpriteWidth((*(*object + i)).sprite);
        (*(*object + i)).height = esat::SpriteHeight((*(*object + i)).sprite);  
        
        esat::SpriteTransformInit(&(*(*object + i)).transform);
        (*(*object + i)).transform.x = 0;
        (*(*object + i)).transform.y = 0;

        (*(*object + i)).type = (PickUps)i;
        //printf("tipo 1: %d",(*(*object + i)).type);
        (*(*object + i)).points = 50*i;
        (*(*object + i)).is_active = false; // not active by default;
        (*(*object + i)).is_being_held = false; // not active by default;
        (*(*object + i)).was_dropped = false; // not active by default;

      }
      else{
        printf("sprite null");
      }
    }
  }
  else{
    printf("Malloc Object");
  }
}

void DrawObject(Object* object){
  for(int i=0;i<KnumberOfPickups;i++){
    if((object+i)->is_active == true && ((object+i)->sprite != NULL) ) {
      esat::DrawSprite((object+i)->sprite,(object+i)->transform);
    }
  }
}

void SpawnObject(Object* object, int frameCounter, Ship ship){
  int random;
  int randomXposition;
  int minX = 64; // Posición mínima donde puede aparecer el objeto
  int maxX = kWindowWidth - 64; // Posición máxima antes de salirse
  // count the frames when the fuel there hasnt been objects on display
  if(gm.no_objects_displayed && gm.frames_Since_Object_Spawn <= 60 * 10  ){ // this amount of frames are 10 seconds
    // if less than then seconds have passed we keep counting
    gm.frames_Since_Object_Spawn ++;
  }
  if(gm.no_objects_displayed && gm.frames_Since_Object_Spawn >= 60 * 5  ){

    // reset conditions
    gm.frames_Since_Object_Spawn = 0;
    gm.no_objects_displayed = false;
    //  assign random object and random position to spawn said object
    random =  1 + rand() % 5;;
    randomXposition = minX + rand() % (maxX - minX + 1);
    (object + random)->transform.x = randomXposition;
    (object + random)->transform.y = 0 - object->height;// this technically uses the height of the fuel but its the same for every object

    (object + random)->is_active = true;
    //printf("spawneo pero quizá no se ve\n");
  }
  
  
  
  // the specific spawn of the FUEL. the thing above if for every other object
  if( ship.components_attached == 3){
    // only spawn fuel after the ship is built
    if(!(object->is_active)){
      // check for the first object, just the Fuel
      gm.frames_Since_Fuel_Spawn++;
      
    }
    if(gm.frames_Since_Fuel_Spawn >= 60 * 10){
      // if more than ten seconds have passed we reset the count and spawn a fuel tank
      object->is_active = true;
      gm.frames_Since_Fuel_Spawn = 0;
      
      randomXposition = minX + rand() % (maxX - minX + 1);
      object->transform.x = randomXposition;
      object->transform.y = 0 - object->height;
    }
    
    // if more than ten seconds have passed we reset the count and spawn an object
  }

}

void UpdateObjects(Object* object){
  int offset= 2;
  int atLeatOneColliding = 0;
  esat::SpriteTransform temp_tr;
  for(int i = 0;i<KnumberOfPickups;i++){

    if( i == 0 && (object->is_being_held) == true) i++; // to avoid the fuel from falling when being held
    if( (object +i)->is_active ){
      // an active object that is not being held by the player should fall down until hitting a terrain
      temp_tr.x = (object +i)->transform.x;
      temp_tr.y = (object +i)->transform.y + offset;
      //printf("Spawnea pero donde? : %f %f",(object +i)->transform.x,(object +i)->transform.y);
      for(int i =0;i<4;i++){
        // since the objects spawn in specific locations and only collide from the top this functions works just fine
        if( areTheseColliding((terrain +i)->transform,{(terrain +i)->width,(terrain +i)->height},temp_tr,{(object +i)->width,(object +i)->height} ) == true  ){
          atLeatOneColliding++;
        }
      }
      if(atLeatOneColliding == 0){
        (object +i)->transform.y += offset;
      }
      atLeatOneColliding = 0;

    }
  }
}

void ObjectInteraction(Object* object,Player* player){
  // objects that are not the fuel tank disapear an add to the players score
  // the fuels center is being attached to the player and moves with them afterwards
  esat::SpriteTransform object_center;

  for(int i = 0; i<KnumberOfPickups;i++){

    if((object+i)->is_active && !((object+i)->is_being_held) && !(object+i)->was_dropped){
      // an active object that isnt being held can and will drop, unless is was dropped by the player. in that case we dont want the player to regrab it mid air
      object_center.x = (object +i)->transform.x + ((object +i)->width/2);
      object_center.y = (object +i)->transform.y + ((object +i)->height/2);
      // at the moment is checking a collision between the center of the object, yes just the center,a point and the player
      if(areTheseColliding(player->transform,{player->width,player->height},object_center,{0,0}  )){
        // Tengo que cambiar esto para que revise la colision con el centro de la caja y no con el contorno
        if((object+i)->type == KFUEL ){ 
          if( (player->is_holding_object == false) && (player->is_holding_component == false)  ) {

            // if a player that isnt already holding an object passes over the fuel tank it grabs it
            // if we dont make the is_holding_object false untill the tank dissapears we avoid picking it up again after droping it
  
            (object+i)->is_being_held = true; // so it doesnt just keep falling in the updateObject function
            player->is_holding_object = true;
            #ifdef WIN32
            soloud.play(audio[4]);
            #endif
            player->is_holding_component = false; // Just in case;
            (player->object) = object;// atach
            object->transform.x = player->transform.x + (player->width/2) - (object->width/2);// position the object in the middle of the player
            object->transform.y = player->transform.y + (player->height) - (object->height);
          }
        }
        else{
          player->score += (object +i)->points;
          (object+i)->is_active = false;
          #ifdef WIN32
          soloud.play(audio[5]);
          #endif
          gm.no_objects_displayed = true;
          gm.frames_Since_Object_Spawn= 0;// i believe this is unnecesary here but i will keep it just in case
        }
        
        }
    }
  }
}

/*
   ____  _   _ ___ ____  
/ ___|| | | |_ _|  _ \ 
\___ \| |_| || || |_) |
 ___) |  _  || ||  __/ 
|____/|_| |_|___|_|    
*/
void InitShip(Ship* ship){
  if(ship != NULL){ //Only execute if ship exists
    //We do a malloc of the size for every component of the ship (fire animation included).
    ship->component = (ShipComponent*) malloc (sizeof(ShipComponent)*5); //3 for total components.
    
    // The size of this malloc is for:
    // 1. The quantity of components we are handling to change its color
    // 2-3 its the size of the sprite.
    ship->save_pixel = (unsigned char*) malloc (sizeof(unsigned char)*(3*64*64*4));

    // Another malloc for the ship RGBA
    ship->rocket_px_RGBA = (unsigned char*) malloc (sizeof(unsigned char)*(4));
    //ship->fuel_counter = 0; // esto nunca se iniciaba

    if(ship->component!=NULL){
      // Depending on the current level stage we change the type of rocket.
      switch(gm.level.current_level){
        case 1:
          //Rocket 1 sprite component loading from file a_rocket[1-3].png
          (ship->component)->sprite = esat::SpriteFromFile("Sprites/rocket/a_rocket1.png");
          (ship->component+1)->sprite = esat::SpriteFromFile("Sprites/rocket/a_rocket2.png");
          (ship->component+2)->sprite = esat::SpriteFromFile("Sprites/rocket/a_rocket3.png");
        break;
        case 2:
          //Rocket 2 sprite component loading from file b_rocket[1-3].png
          (ship->component)->sprite = esat::SpriteFromFile("Sprites/rocket/b_rocket1.png");
          (ship->component+1)->sprite = esat::SpriteFromFile("Sprites/rocket/b_rocket2.png");
          (ship->component+2)->sprite = esat::SpriteFromFile("Sprites/rocket/b_rocket3.png");
        break;
        case 3:
          //Rocket 3 sprite component loading from file c_rocket[1-3].png
          (ship->component)->sprite = esat::SpriteFromFile("Sprites/rocket/c_rocket1.png");
          (ship->component+1)->sprite = esat::SpriteFromFile("Sprites/rocket/c_rocket2.png");
          (ship->component+2)->sprite = esat::SpriteFromFile("Sprites/rocket/c_rocket3.png");
        break;
        case 4:
          //Rocket 3 sprite component loading from file d_rocket[1-3].png
          (ship->component)->sprite = esat::SpriteFromFile("Sprites/rocket/d_rocket1.png");
          (ship->component+1)->sprite = esat::SpriteFromFile("Sprites/rocket/d_rocket2.png");
          (ship->component+2)->sprite = esat::SpriteFromFile("Sprites/rocket/d_rocket3.png");
        break;
      }
      //Rocket fire sprites (2). 
      (ship->component+3)->sprite = esat::SpriteFromFile("Sprites/rocket/fire1.png");
      (ship->component+4)->sprite = esat::SpriteFromFile("Sprites/rocket/fire2.png");

    }
    // To get all the pixels from the original sprite
    for(int w = 0; w < 3; w++){
      for(int i = 0; i < 64; i++){
          for(int j = 0; j < 64; j++){
            // This down there is for the pixel colors, we save the original RGBA from the rocket
              esat::SpriteGetPixel((ship->component+w)->sprite,i,j,ship->rocket_px_RGBA);
              // Then we use that saved RGBA to the ship's save pixel variable.
              *(ship->save_pixel+w*64*64*4 + j*64*4 + i*4 + 0) = *(ship->rocket_px_RGBA+0);
              *(ship->save_pixel+w*64*64*4 + j*64*4 + i*4 + 1) = *(ship->rocket_px_RGBA+1);
              *(ship->save_pixel+w*64*64*4 + j*64*4 + i*4 + 2) = *(ship->rocket_px_RGBA+2);
              *(ship->save_pixel+w*64*64*4 + j*64*4 + i*4 + 3) = *(ship->rocket_px_RGBA+3);
          }
      }
    }
  //Added else with return in case of errors
  } else {
    //Print error in case of failure
    printf("\n\n-ERROR! trying to asign memory for every ship component. ERROR!\n");
    return;
  }
}

// Once we beat one level the ship starts at the top simulating that it went to another planet ig
// This function its practically for that, so the ship starts at the top
void ShipNewStage(Ship* ship){
  if(ship != NULL){ //Only execute if ship exists
    //We initialize the fuel and the components to the default amount (0 and 1)
    //The components_attached its always 1 since the bottom part of the rocket its always on the floor
    ship->fuel_counter = 0;
    ship->components_attached = 3;
    for(int i = 0; i < 3; i++){
      // We need to save the component dimensions for later uses.
      (ship->component+i)->width = 64;
      (ship->component+i)->height =64 ;
      (ship->component+i)->is_being_held = false; // to check when to attach it to the player
      (ship->component+i)->was_dropped = false; // to check is dropped and cant be picked up again
      (ship->component+i)->is_conected = true; // to check if it is attached to the rocket
    }
  //Added else with return in case of errors
  } else {
    //Print error in case of failure
    printf("\n\n-ERROR! trying to asign memory for every ship component. ERROR!\n");
    return;
  }
}

// Vas a tener que jugar entre 
// Function for reseting all the values when we change the ship.
void ShipNewLevel(Ship* ship){
  //Initialization of transforms
  ship->fuel_counter = 0;
  ship->components_attached = 1;

  for(int i = 0; i < 3; i++){
    esat::SpriteTransformInit(&(ship->component+i)->transform); //Init transform component

    // We need to save the component dimensions for later uses.
    (ship->component+i)->width = 64;
    (ship->component+i)->height =64 ;
    (ship->component+i)->is_being_held = false; // to check when to attach it to the player
    (ship->component+i)->was_dropped = false ; // to check is dropped and cant be picked up again
    (ship->component+i)->is_conected = false; // to check if it is attached to the rocket
  }

  // We initialize the default positions of the rocket's components.
  // Bottom part (always on the floor)
  (ship->component + 0)->transform.x = kWindowWidth / 2 + 125;
  (ship->component + 0)->transform.y = kWindowHeight - terrain->height - 64;
  // Middle part of the rocket (on one of the platforms)
  (ship->component + 1)->transform.x = 450 + 30;
  (ship->component + 1)->transform.y = 400 - 64;
  // Top part of the rocket (on a different platform)
  (ship->component + 2)->transform.x = 132 + 70;
  (ship->component + 2)->transform.y = 292 - 64;

  for(int i = 3; i < 5; i++){
    esat::SpriteTransformInit(&(ship->component+i)->transform); //Init transform component
    (ship->component+i)->transform.x = kWindowWidth/2 + 125; //X = Half-screen + 125px
    //20 to separate fire from rocket (because of the original design of the game)
    (ship->component+i)->transform.y = 20 + kWindowHeight - ((terrain)->height);
  }

  
}

void ShipPosAtTop(Ship* ship){
  int distancebetweenpieces = 64*2;
  for(int component_pos = 0; component_pos < 3; component_pos++){
    esat::SpriteTransformInit(&(ship->component+component_pos)->transform); //Init transform component
    (ship->component + component_pos)->transform.x = kWindowWidth / 2 + 125;
    (ship->component + component_pos)->transform.y = 0 + distancebetweenpieces;
    distancebetweenpieces -= 64;
  }
  for(int i = 3; i < 5; i++){
    esat::SpriteTransformInit(&(ship->component+i)->transform); //Init transform component
    (ship->component+i)->transform.x = kWindowWidth/2 + 125; //X = Half-screen + 125px
    //20 to separate fire from rocket (because of the original design of the game)
    (ship->component+i)->transform.y = (ship->component+0)->transform.y+64+20;
  }
}

// We need a function to 'fill' the rocket, in this case is painting it the parts we want
void ShipWithFuelColor(Ship* ship, int component_part_painted, int component_piece){
  for(int i = component_part_painted; i < 64; i++){
      for(int j = 0; j < 64; j++){
          esat::SpriteGetPixel((ship->component+component_piece)->sprite,j,i,ship->rocket_px_RGBA);
          // This down there is for the pixel colors, we save on the variable save_pixel the RGBA from the original sprite
          *(ship->save_pixel+1*component_piece*64*64*4 + i*64*4 + j*4 + 0) = 198;
          *(ship->save_pixel+1*component_piece*64*64*4 + i*64*4 + j*4 + 1) = 55;
          *(ship->save_pixel+1*component_piece*64*64*4 + i*64*4 + j*4 + 2) = 183;
          *(ship->save_pixel+1*component_piece*64*64*4 + i*64*4 + j*4 + 3) = *(ship->rocket_px_RGBA+3);
      }
  }
}

void DrawShip(Ship ship, int frameCounter){
  //Fire animation iterating if Y of ship component 3 (first fire sprite) is even or not
  //Added check for height to stop showing fire if there's not enough space to the floor
  if(((ship.component+3)->transform.y) < 670) {
    /*Show fire animation only if there's enough fuel. 
    Fire disapears when ship is landed (because fuelCounter is set to 0)*/
    if((int)((ship.component)->transform.y) % 2 == 0){
      esat::DrawSprite((ship.component+3)->sprite, (ship.component+3)->transform);
    } else {
      esat::DrawSprite((ship.component+4)->sprite, (ship.component+4)->transform);
    }
  }
  for(int i=0; i<3; i++){
    esat::SpriteUpdateFromMemory((ship.component+i)->sprite, (ship.save_pixel+i*64*64*4 + 0*64*4 + 0*4 + 0));
    esat::DrawSprite((ship.component+i)->sprite, (ship.component+i)->transform);
  }
}

void UpdateShip(Ship* ship, Player* player, int frameCounter, Object* object){

  if((ship->component+2)->transform.y <= 0){
    reached_top = true; // <--- esta linea
  }
  // Este if es básicamente cuando la nave llega al suelo, creo que si va a ser necesario para saber si se separa o no, es decir
  // el momento en el que llamamos la funcion ShipNewLevel o ShipNewStage
  if((ship->component+0)->transform.y >= kWindowHeight - ((terrain)->height)-64 && reached_top == true){
    reached_top = false;
  }
  
  // Esta funcion es para ir cambiando las posiciones de las piezas y que se acomoden solas
  // En teoria solo se llamaria cuando pasamos de "Stage" ya que ahí es cuando estan separados
  if(player->component !=NULL){
    if(player->component->was_dropped && ship->fuel_counter < 6){
      // While the components attached are equal to 1 then the lower part stays there, then we put the component
      // as connected to the rocket
      if(ship->components_attached == 1){
        //Bottom part of the rocket
        (ship->component + 0)->transform.x = kWindowWidth / 2 + 125;
        (ship->component + 0)->transform.y = kWindowHeight - terrain->height - 64;
        (ship->component + 0)->is_conected = true;
        // When the 2nd component hits the lower part of the rocket then we add 1 to the components that are attached to the rocket
        if((ship->component + 1)->transform.y >= (ship->component +0)->transform.y-64){
          ship->components_attached += 1;
        }
      }
      // Once there are 2 components then we place the component so its perfectly fit with the lower part
      // We also put this component as true
      if((ship->components_attached == 2)){
        (ship->component + 1)->transform.x = kWindowWidth / 2 + 125;
        (ship->component + 1)->transform.y = kWindowHeight - terrain->height - 128;
        (ship->component + 1)->is_conected = true;
        // Same as the 1st if, we add 1 to the components attached and go to the next if.
        if((ship->component + 2)->transform.y >= (ship->component + 1)->transform.y-64){

          ship->components_attached += 1;
        }
      }
      // Same as the 2nd if, but we set the was_dropped variable to false so we don't enter to this if again
      if(ship->components_attached == 3){
        (ship->component + 2)->transform.x = kWindowWidth / 2 + 125;
        (ship->component + 2)->transform.y = kWindowHeight - terrain->height - 192;
        (ship->component + 2)->is_conected = true;
        player->component->was_dropped = false;
      }
    }
  }

  if(ship->components_attached >= 3){
    // Different paint of the sprite depending on the amount of fuel.
    if(esat::IsKeyDown('I')){
      ship->fuel_counter += 1;
      #ifdef WIN32
      soloud.play(audio[10]);
      #endif
    }
    switch(ship->fuel_counter){
      case 1:
      ShipWithFuelColor(ship,32,0);
      break;
      case 2:
      ShipWithFuelColor(ship,0,0);
      break;
      case 3:
      ShipWithFuelColor(ship,32,1);
      break;
      case 4:
      ShipWithFuelColor(ship,0,1);
      break;
      case 5:
      ShipWithFuelColor(ship,32,2);
      break;
      case 6:
      ShipWithFuelColor(ship,0,2);
      break;
    }
  }
  if(gm.state == KMAIN_LOOP && ship->fuel_counter >= 6 && (player->transform.x > ship->component->transform.x-32 && player->transform.x < ship->component->transform.x+64)){
    //ShipTakeOff(ship,frameCounter);
    printf("lo llama siempre ? ");
    nextLevel();
    player->lives +=1;
    saveProgress(*player);
    gm.state = KTAKEOFF;
    // limpiar objetos
    for(int i =0;i<KnumberOfPickups;i++){
      if((object+i)->is_active){
        (object+i)->transform.x = 0;
        (object+i)->transform.y = 0;
        (object+i)->is_active = false; // not active by default;
        (object+i)->is_being_held = false; // not active by default;
        (object+i)->was_dropped = false; // not active by default;
      }
    }
  }
  #ifdef WIN32
  if(LoopingSound){
    if (!soloud.isValidVoiceHandle(g_rocketLoopHandle)){
        g_rocketLoopHandle = soloud.play(audio[9], 1.0f /* volumen */, 0.0f /* pan */);
        // Al usar setLooping(true), el sonido se repetirá indefinidamente
    }
  } else {
    soloud.stop(g_rocketLoopHandle);
    g_rocketLoopHandle = 0;
  }
  #endif
}

// Para cuando despegue llamamos a la funcion
// Function for when the ship starts takingoff
void ShipTakeOff(Ship* ship, int frameCounter){
  //Every 10fps Y +/-= 5px (up is -) if fuel_counter >= 6.
  if( !reached_top){
    //For every shipComponent (3 total for each rocket model)
    if(frameCounter % 5 == 0){
      for(int i=0; i<5; i++){
        (ship->component+i)->transform.y -= 5;
      }

    }
  }
  else{
    if(gm.level.current_level ==4 && gm.level.level_stage == 4) {
      // winning condition
      printf("player won but there is no game over screen");
      gm.state = KWON;
    }
    else{

      ShipSkinChanger(ship);
      ResetShipColor(ship);
      ship->fuel_counter = 0;
      gm.state = KLANDING;
    }
  }

}

// We need another function for when the ship is at the top and starts landing
void LandingShip(Ship* ship, int frameCounter, Enemy* enemies, int kNenemys, int &actual_enemy_type){
  if(reached_top){
    if(frameCounter % 5 == 0){
      for(int i=4; i>=0; i--){
        /*We set all the components y position to a range so we don't enter
        infinitely on the if. */
        (ship->component+i)->transform.y += 5;
      }
    }
    if(gm.level.level_stage == 1 && (ship->component+0)->transform.y >= kWindowHeight - ((terrain)->height)-64){
      ShipNewLevel(ship);
    }else{
      ShipNewStage(ship);
    }
  }
  else{
    gm.no_objects_displayed = true;
    gm.frames_Since_Object_Spawn = 0;
    gm.frames_Since_Fuel_Spawn =0;
    gm.state= KMAIN_LOOP; //<---- Nave tendría que desarmarse
    SwapEnemy(enemies, kNenemys, actual_enemy_type);
    KillEnemies(enemies, kNenemys);
  }
}

void loadFuel(Object* object,Ship* ship,Player* player){
  if(object->is_active){
    // the fisrt object is FUEL
    if( (object->transform.x >= KRocketOriginalPos.x && object->transform.x  <= KRocketOriginalPos.x +64 )
      && (object->transform.y + object->height >= kWindowHeight - ((terrain)->height) - 10) ){
      // if the lower end of the objects reaches the top part
      //printf("\nCombustible tocó el suelo3 ");
      player->is_holding_object = false;
      player->is_holding_component = false;
      object->was_dropped = false;
      ship->fuel_counter += 1;
      object->is_active = false;
      gm.no_objects_displayed = true;
      gm.frames_Since_Object_Spawn= 0;
      #ifdef WIN32
      soloud.play(audio[10]);
      #endif
    }
  }
}

void ShipSkinChanger(Ship* ship){
  if(ship != NULL){ //Only execute if ship exists
    if(ship->component!=NULL){
      // Depending on the current level stage we change the type of rocket.
      switch(gm.level.current_level){
        case 1:
          //Rocket 1 sprite component loading from file a_rocket[1-3].png
          (ship->component)->sprite = esat::SpriteFromFile("Sprites/rocket/a_rocket1.png");
          (ship->component+1)->sprite = esat::SpriteFromFile("Sprites/rocket/a_rocket2.png");
          (ship->component+2)->sprite = esat::SpriteFromFile("Sprites/rocket/a_rocket3.png");
        break;
        case 2:
          //Rocket 2 sprite component loading from file b_rocket[1-3].png
          (ship->component)->sprite = esat::SpriteFromFile("Sprites/rocket/b_rocket1.png");
          (ship->component+1)->sprite = esat::SpriteFromFile("Sprites/rocket/b_rocket2.png");
          (ship->component+2)->sprite = esat::SpriteFromFile("Sprites/rocket/b_rocket3.png");
        break;
        case 3:
          //Rocket 3 sprite component loading from file c_rocket[1-3].png
          (ship->component)->sprite = esat::SpriteFromFile("Sprites/rocket/c_rocket1.png");
          (ship->component+1)->sprite = esat::SpriteFromFile("Sprites/rocket/c_rocket2.png");
          (ship->component+2)->sprite = esat::SpriteFromFile("Sprites/rocket/c_rocket3.png");
        break;
        case 4:
          //Rocket 4 sprite component loading from file d_rocket[1-3].png
          (ship->component)->sprite = esat::SpriteFromFile("Sprites/rocket/d_rocket1.png");
          (ship->component+1)->sprite = esat::SpriteFromFile("Sprites/rocket/d_rocket2.png");
          (ship->component+2)->sprite = esat::SpriteFromFile("Sprites/rocket/d_rocket3.png");
        break;
      }
      //Rocket fire sprites (2). 
      (ship->component+3)->sprite = esat::SpriteFromFile("Sprites/rocket/fire1.png");
      (ship->component+4)->sprite = esat::SpriteFromFile("Sprites/rocket/fire2.png");

    }
    // To get all the pixels from the original sprite
    for(int w = 0; w < 3; w++){
      for(int i = 0; i < 64; i++){
          for(int j = 0; j < 64; j++){
            // This down there is for the pixel colors, we save the original RGBA from the rocket
              esat::SpriteGetPixel((ship->component+w)->sprite,i,j,ship->rocket_px_RGBA);
              // Then we use that saved RGBA to the ship's save pixel variable.
              *(ship->save_pixel+w*64*64*4 + j*64*4 + i*4 + 0) = *(ship->rocket_px_RGBA+0);
              *(ship->save_pixel+w*64*64*4 + j*64*4 + i*4 + 1) = *(ship->rocket_px_RGBA+1);
              *(ship->save_pixel+w*64*64*4 + j*64*4 + i*4 + 2) = *(ship->rocket_px_RGBA+2);
              *(ship->save_pixel+w*64*64*4 + j*64*4 + i*4 + 3) = *(ship->rocket_px_RGBA+3);
          }
      }
    }
  //Added else with return in case of errors
  } else {
    //Print error in case of failure
    return;
  }
}

void ResetShipColor(Ship* ship){
  for(int w = 0; w < 3; w++){
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 64; j++){
            esat::SpriteGetPixel((ship->component+w)->sprite,j,i,ship->rocket_px_RGBA);
            // This down there is for the pixel colors, we save on the variable save_pixel the RGBA from the original sprite
            *(ship->save_pixel+1*w*64*64*4 + i*64*4 + j*4 + 0) = 255;
            *(ship->save_pixel+1*w*64*64*4 + i*64*4 + j*4 + 1) = 255;
            *(ship->save_pixel+1*w*64*64*4 + i*64*4 + j*4 + 2) = 255;
            *(ship->save_pixel+1*w*64*64*4 + i*64*4 + j*4 + 3) = *(ship->rocket_px_RGBA+3);
        }
    }
  }
}

void PickComponents(Player* player, Ship* ship){
  // THIS SHOULD CHECK ONLY THE AVAILABLE COMPONENT. at the beginning it would be the second or index 1 of the ship components and then move to the next when that is dropped
  // I receive the whole ship to be able to loop the ship components
  esat::SpriteTransform component_center;
  for(int i = 1; i<3;i++){// avoi the first component since that one is bound to the ground since the beginning
      if(player->is_holding_object == false && player->is_holding_component == false){
      // make sure the player isnt holding anything before trying to put another thing in his hands
      // loop the three ship components
        //printf("dropea o no parte 2: %d",(ship->component +i)->was_dropped ) ;
        if(ship->components_attached == i){
          if((ship->component +i)->was_dropped == false){
          // to avoid picking and object that was already dropped on the ship. this works
            component_center.x = (ship->component +i)->transform.x + ((ship->component +i)->width/2);
            component_center.y = (ship->component +i)->transform.y + ((ship->component +i)->height/2) ;
            if(areTheseColliding(player->transform,{player->width,player->height},component_center,{0,0}  )){
              //printf("\nhay colisión");
              player->is_holding_component = true;
              player->is_holding_object = false; // Just in case;
              player->component = (ship->component +i);
              (ship->component +i)->is_being_held= true;
              (ship->component +i)->transform.x = player->transform.x + (player->width/2) - ((ship->component +i)->width/2);// position the object in the middle of the player
              (ship->component +i)->transform.y = player->transform.y + (player->height) - ((ship->component +i)->height);
              #ifdef WIN32
              soloud.play(audio[6]);
              #endif
              //printf("\ntermina de asignar");
            }
          }
        }
    }
  }
}

void UpdateComponents(Ship* ship, Player* player){
  int atLeatOneColliding = 0;
  int offset = 2;
  esat::SpriteTransform temp_tr;
  if(player->component == NULL) return; // to avoid checking to null when the player doest have a component in hand
  if( player->component->was_dropped == true){
    // when the objects needs to fall after being dropped
    temp_tr.x = (player->component)->transform.x;
    temp_tr.y = (player->component)->transform.y + offset; 

    for(int i =0;i<4;i++){
      // since the objects spawn in specific locations and only collide from the top this functions works just fine
      if( areTheseColliding((terrain +i)->transform,{(terrain +i)->width,(terrain +i)->height},temp_tr,{(player->component)->width,(player->component)->height} ) == true  ){
        atLeatOneColliding++;
      }
    }

    if(atLeatOneColliding == 0){
      (player->component)->transform.y += offset;
    }
    //printf("%d",atLeatOneColliding );
    atLeatOneColliding = 0;

  }


  
}
// END OF SHIP RELATED CODE

/*
 ____      _ __   __
|  _ \    / \\ \ / /
| |_) |  / _ \\ V / 
|  _ <  / ___ \| |  
|_| \_\/_/   \_\_|  
*/
void AddRay(Ray** head,Player player){
  // this adds the new ray to the end  of the list
  printf("\naddray");
  Ray* new_ray = (Ray*)malloc (1* sizeof(Ray));
    if (!new_ray) {
        printf("Error al asignar memoria.\n");
        return;
    }
    new_ray->distance = 0;
    new_ray->sub_rays =  AddSubRays(player);
    new_ray->front_component = 0;// in the beginning the front sub_ray will be the first and only
    new_ray->speed = (player.transform.scale_x == -1)?(-KraySpeed):(KraySpeed);
    if (!new_ray->sub_rays) {
      printf("Error: no se pudieron asignar sub_rays.\n");
      free(new_ray);
      return;
  }
    if (!(*head)) {  // Si la lista está vacía
      new_ray->next = new_ray;
      new_ray->prev = new_ray;
      *head = new_ray;
    } else {
      Ray* tail = (*head)->prev; // al anterior al primero o últmo
      tail->next = new_ray;// el nuevo ultimo elemento
      new_ray->prev = tail;// el previo es quien solía ser el último
      new_ray->next = *head;// y su next es el primero
      (*head)->prev = new_ray;// damos la vuelta completa y el previo al primero es el nuevo elemento/último elemento
    }

    printf("\nfin addray");
  return;

}

RayComponent* AddSubRays(Player player) {
  printf("\nentra a addSubrays");
  RayComponent* sub_rays  = (RayComponent*)malloc( KNumbersOfSubRays  * sizeof(RayComponent));
  if( (sub_rays) != NULL ){
    for (int i = 0; i < KNumbersOfSubRays ; i++) {
        (sub_rays+i)->sprite = esat::SpriteFromFile("sprites/objects/thinRay.png");
        if((sub_rays+i)->sprite !=NULL){
          (sub_rays+i)->height= 4;
          (sub_rays+i)->width = 12;
          (sub_rays+i)->distance= 0;// to check how far has this specific sub_ray traveled before start drawing the next one
          esat::SpriteTransformInit(&(sub_rays+i)->transform);
          //(sub_rays+i)->transform.x = player.transform.x;
          if (i < KNumbersOfSubRays / 4) {
            (sub_rays+i)->transform.scale_x = 4.0f; // Primer cuarto de los segmentos
            (sub_rays+i)->width *=4;
          } else if (i < KNumbersOfSubRays / 2) {
              (sub_rays+i)->transform.scale_x = 3.0f; // Segundo cuarto de los segmentos
              (sub_rays+i)->width *=3;
          } else if (i < (3 * KNumbersOfSubRays) / 4) {
              (sub_rays+i)->transform.scale_x = 2.0f; // Tercer cuarto de los segmentos
              (sub_rays+i)->width *=2;
          } else {
              (sub_rays+i)->transform.scale_x = 1.0f; // Último cuarto de los segmentos
          }

          (sub_rays+i)->transform.y = player.transform.y + (player.height/2);
          (sub_rays+i)->transform.x = player.transform.x +  (player.width +1);
  
          (sub_rays+i)->is_active = false;
          
        }
        
      }
      sub_rays->is_active= true; // active only the first item

  }
  else{
    printf("\nerror en el malloc");
  }
  printf("\nimprimamos algo a ver :%f",(sub_rays+0)->width );
  return sub_rays;

 
}

void DrawRays(Ray** head) {
  if (*head == NULL) return; // Si no hay rayos, no hacer nada

  Ray* current = *head;
  do {
      for (int i = 0; i < KNumbersOfSubRays; i++) {
          if ((current->sub_rays + i)->is_active) {
              esat::DrawSprite((current->sub_rays + i)->sprite, (current->sub_rays + i)->transform);
          }
      }
      current = current->next;
  } while (current != *head); // Recorrer toda la lista circular
}

void FireRay(Ray** head,Player player){
  if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
    #ifdef WIN32
    soloud.play(audio[2]);
    #endif
    AddRay(head,player);
  }
}

bool ColisionRightRay(RayComponent* sub_rays,Enemy* enemies) {
  // Vector from the ray
  esat::Vec2 Point1 = {sub_rays->transform.x + sub_rays->width, sub_rays->transform.y};
  esat::Vec2 Point2 = {sub_rays->transform.x + sub_rays->width, sub_rays->transform.y + sub_rays->height};

  // Vector from the enemy
  esat::Vec2 Qoint1 = {enemies->transform.x, enemies->transform.y};
  esat::Vec2 Qoint2 = {enemies->transform.x, enemies->transform.y + enemies->height};

  if ((Point1.x > Qoint1.x && Point2.x < Qoint2.x + 10) && 
          (Point1.y < Qoint2.y && Point2.y > Qoint1.y) && enemies->is_alive != 0 && enemies->is_alive != 2){
    return true;
  } else {
    return false;
  }
}
  
bool ColisionLeftRay(RayComponent* sub_rays,Enemy* enemies) {
  // Vector from the ray
  esat::Vec2 Point1 = {sub_rays->transform.x, sub_rays->transform.y};
  esat::Vec2 Point2 = {sub_rays->transform.x, sub_rays->transform.y + sub_rays->height};

  // Vector from the enemy
  esat::Vec2 Qoint1 = {enemies->transform.x + enemies->width, enemies->transform.y};
  esat::Vec2 Qoint2 = {enemies->transform.x + enemies->width, enemies->transform.y + enemies->height};
  if ((Point1.x > Qoint1.x && Point2.x < Qoint2.x + 10) && 
          (Point1.y < Qoint2.y && Point2.y > Qoint1.y) && enemies->is_alive != 0 && enemies->is_alive != 2){
    return true;
  } else {
    return false;
  }
}

void UpdateRay(Ray** head,Player* player,Enemy* enemies) {
  bool hit = false;
  if (*head == NULL) return; // Si no hay rayos

  Ray* current = *head;
  do {
      if (current->distance < KraySpeed * KNumbersOfSubRays) {
        // if not every subray has finnished then keedp drawing
          for (int i = 0; i < KNumbersOfSubRays; i++) {
              if ((current->sub_rays + i)->is_active) {
                // move the active sub rays
                (current->sub_rays + i)->distance += (current)->speed;
                
                if( (current->sub_rays + i)->transform.x > kWindowWidth || (current->sub_rays + i)->transform.x < 0){
                  (current->sub_rays + i)->transform.x = ((current)->speed == KraySpeed)? 0:kWindowWidth -1;
                }
                (current->sub_rays + i)->transform.x += (current)->speed;

                if (i == current->front_component){
                  for (int j = 0; j < 6; ++j){
                    if ((ColisionLeftRay(current->sub_rays + i, enemies + j)) || 
                            (ColisionRightRay(current->sub_rays + i, enemies + j))){
                      //(current->sub_rays + i)->is_active = false;
                      (current->sub_rays + i)->distance = 0;
                      player->score += (enemies + j)->points;
                      current->distance = KraySpeed * KNumbersOfSubRays;
                      (enemies + j)->is_alive = 2;
                      (enemies + j)->animation = 0;
                      break;
                    }
                  }
                }
              }
              if (i < KNumbersOfSubRays - 1 && AbsoluteValue((current->sub_rays + i)->distance ) >= 12 * 4) {
                //(current->sub_rays + i)->width * 4
                // when an specific subray travells further than 4 times its width start drawing the next one too
                  if ((current->sub_rays + i + 1)->is_active == false) {
                      (current->sub_rays + i + 1)->is_active = true;
                  }
              }

              if (AbsoluteValue((current->sub_rays + i)->distance ) >= kWindowWidth / 2) {
                //when a single subray travells the intended distance of the ray deactivate it so it isnt drawn anymore
                // and add to the distance travelled by the entire ray.
                  current->front_component++;// the front sub_ray will be the next one;
                  (current->sub_rays + i)->is_active = false;
                  (current->sub_rays + i)->distance = 0;
                  current->distance += KraySpeed;
                  current->front_component++;// when the current front component is deactivated make the next one the front component
              }
          }
      } else {
          // if the ray has moved the intended distance remove from the list
          Ray* next_ray = current->next;
          current = deleteRay(head, current);
          if (current == NULL) break; // get out of the scope if the list is empty after removing
          current = next_ray;
          continue;// without this you do ->next twice.
      }
      current = current->next;
  } while (current != *head); //parte en head por lo que si vuelve a serlo dio una vuelta  completa
}

Ray* deleteRay(Ray** head, Ray* current_ray) {
  #ifdef WIN32
  soloud.play(audio[0]);
  #endif
  Ray* next_ray = NULL;

  if (*head == NULL || current_ray == NULL) return NULL;

  // Caso 2: Único nodo en la lista
  if (current_ray->next == current_ray) {
      *head = NULL;
  } else {
      // Conectar el anterior con el siguiente
      next_ray = current_ray->next;

      current_ray->prev->next = current_ray->next;
      current_ray->next->prev = current_ray->prev;

      // Si estamos eliminando el nodo `head`, mover `head` al siguiente
      if (*head == current_ray) {
          *head = next_ray;
      }
  }

  free(current_ray->sub_rays);
  free(current_ray);

  return next_ray; // Devolvemos el nuevo head
}

float AbsoluteValue(float value){
  if(value<0){
    return value*-1;
  }
  else{
    return value;
  }
}

/* _______ .__   __.  _______ .___  ___.  __   _______     _______.
  |   ____||  \ |  | |   ____||   \/   | |  | |   ____|   /       |
  |  |__   |   \|  | |  |__   |  \  /  | |  | |  |__     |   (----`
  |   __|  |  . `  | |   __|  |  |\/|  | |  | |   __|     \   \    
  |  |____ |  |\   | |  |____ |  |  |  | |  | |  |____.----)   |   
  |_______||__| \__| |_______||__|  |__| |__| |_______|_______/                                                                  
 */

/************************************** ENEMY RELATED CODE *****************************************/
void InitEnemy(Enemy** enemies, int kNenemys, int &actual_enemy_type){//INITIAlICE SOME ENEMY VALUES
  *enemies = (Enemy*)malloc(kNenemys * sizeof(Enemy));
  actual_enemy_type--;
  if(*enemies != NULL){
    for(int i=0; i < kNenemys; i++){
      (*enemies + i)->is_alive = 0;  //Setting all of them as deads
      //Opening the sprite malloc for 5 spaces (Two enemys and 3 explosions sprites)
      (*enemies + i)->sprite = (esat::SpriteHandle*) malloc(5 * sizeof(esat::SpriteHandle));
      (*enemies + i)->type = EnemyType::kType_Asteroid; //Initing them as asterois
      (*enemies + i)->animation = 0; //Settings their animation counter to 0
  
      //Setting explosion sprites
      *((*enemies + i)->sprite + 2) = esat::SpriteFromFile("sprites/general/cloud1.png");
      *((*enemies + i)->sprite + 3) = esat::SpriteFromFile("sprites/general/cloud2.png");
      *((*enemies + i)->sprite + 4) = esat::SpriteFromFile("sprites/general/cloud3.png");
    }
  }
}
void KillEnemies(Enemy* enemies, int kNenemys){
  for(int e = 0; e < kNenemys; e++){
    (enemies + e)->is_alive = 0;
    (enemies + e)->animation = 0;
  }
}
void EnemySpawnRates(Enemy* enemies, int kNenemys){ //Switch acting as a random enemy generator depending on type
  switch((enemies)->type){
    case EnemyType::kType_Asteroid:{
      if(rand() % 60 == 0){
        EnemyGeneration(enemies, kNenemys); //After the rand equals 0 it goes into generation
      }
      break;
    }
    case EnemyType::kType_Spikes :{
      if(rand() % 60 == 0){
        EnemyGeneration(enemies, kNenemys); //After the rand equals 0 it goes into generation
      }
      break;
    }
    case EnemyType::kType_Bubble :{
      if(rand() % 60 == 0){
        EnemyGeneration(enemies, kNenemys); //After the rand equals 0 it goes into generation
      }
      break;
    }
    case EnemyType::kType_Ship :{
      if(rand() % 60 == 0){
        EnemyGeneration(enemies, kNenemys); //After the rand equals 0 it goes into generation
      }
      break;
    }
    case EnemyType::kType_UFO :{
      if(rand() % 60 == 0){
        EnemyGeneration(enemies, kNenemys); //After the rand equals 0 it goes into generation
      }
      break;
    }
    case EnemyType::kType_Frog :{
      if(rand() % 60 == 0){
        EnemyGeneration(enemies, kNenemys); //After the rand equals 0 it goes into generation
      }
      break;
    }
    case EnemyType::kType_Cross :{
      if(rand() % 60 == 0){
        EnemyGeneration(enemies, kNenemys); //After the rand equals 0 it goes into generation
      }
      break;
    }
    case EnemyType::kType_Starship :{
      if(rand() % 60 == 0){
        EnemyGeneration(enemies, kNenemys); //After the rand equals 0 it goes into generation
      }
      break;
    }
  }
}

void EnemyGeneration(Enemy* enemies, int kNenemys) {  //Search for the first dead enemy and init him
  int count = 0;
  while((enemies + count)->is_alive != 0 && count < kNenemys - 1){
    count++;
  }
  //(kNenemys - 1) bc if we go until 6 it will then ++ to count and we will get out of the array
  //If count == 7 program will explode

  //Check that enemy with offset setted ont the while
  if((enemies + count)->is_alive == 0){
    //SET EVERY ENEMY VARIABLE
    (enemies + count)->is_alive = 1;//Making im alive
    esat::SpriteTransformInit(  &(enemies + count)->transform );//Initing transform
    (enemies + count)->transform.y = rand() % (kWindowHeight - 32 - 64);
    (enemies + count)->transform.scale_y *= 1.0f;
    (enemies + count)->color = rand()%(4) + 1;
    TypeColors((enemies + count), kNenemys);

    switch( (enemies + count)->type ){//SETTING EVERY DATA TO EACH TYPE OF ENEMYw
      /******************************  ASTEROID  ******************************/
      case EnemyType::kType_Asteroid: {
        //To decide which side they spawn
        if ((rand() % 2) == 0){
          //Right Side Enemies
          (enemies + count)->transform.x = 1024+64; //Right side 'x'
          (enemies + count)->transform.scale_x *= -1.0f;  //-1 * Scale to flip them
          (enemies + count)->speed_x = -3;  //-1 * Speed bc they are flipped
          (enemies + count)->width = -64; //-1 * Width bc flipped
        }else{
          //Left Side Enemies
          (enemies + count)->transform.x = 0-64; //Left side 'x'
          (enemies + count)->transform.scale_x *= 1.0f; //1 * Scale
          (enemies + count)->speed_x = +3;  //1 * Speed
          (enemies + count)->width = +64; //1 * Width 
        }
        (enemies + count)->points = 25; //The points you get when you kill them
        (enemies + count)->speed_y = rand() % 2;  
        
        //Their 'y' speed, if they go up, down or anywhere on 'y' coord
        break;
      }
      /******************************  SPIKES ******************************/
      case EnemyType::kType_Spikes :{

        if ((rand() % 2) == 0){
          (enemies + count)->transform.x = 1024+64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = -3;
          (enemies + count)->width = 64;
        }else{
          (enemies + count)->transform.x = 0-64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = +3;
          (enemies + count)->width = +64;
        }
        (enemies + count)->points = 80;
        (enemies + count)->speed_y = (((rand() % 2)*2)-1) * 3;
        break;
      }
      /******************************  BUBBLE ******************************/
      case EnemyType::kType_Bubble :{
      
        if ((rand() % 2) == 0){
          (enemies + count)->transform.x = 1024+64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = -3;
          (enemies + count)->width = 64;
        }else{
          (enemies + count)->transform.x = 0-64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = +3;
          (enemies + count)->width = +64;
        }
        (enemies + count)->points = 40;
        (enemies + count)->speed_y = (((rand() % 2)*2)-1) * 3;
        break;
      }
      /******************************  SHIP ******************************/
      case EnemyType::kType_Ship :{
        if ((rand() % 2) == 0){
          (enemies + count)->transform.x = 64;
          (enemies + count)->transform.scale_x *= -1.0f;
          (enemies + count)->speed_x = -3;
          (enemies + count)->width = -64;
        }else{
          (enemies + count)->transform.x = 0;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = +3;
          (enemies + count)->width = +64;
        }
        (enemies + count)->points = 55;
        (enemies + count)->speed_y = (((rand() % 2)*2)-1) * 1.5;


        //STATE WHEN THEY ARE WAITING TO TRACK PLAYER
        (enemies + count)->is_alive = 3;
        break;
      }
      /******************************  UFO ******************************/
      case EnemyType::kType_UFO :{
        if ((rand() % 2) == 0){
          (enemies + count)->transform.x = 1024;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = 3;
          (enemies + count)->width = 64;
        }else{
          (enemies + count)->transform.x = 0-64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = +3;
          (enemies + count)->width = +64;
        }
        (enemies + count)->points = 50;
        (enemies + count)->speed_y = (((rand() % 2)*2)-1) * 2;
        (enemies + count)->direction = 1;
      
        break;
      }
      /******************************  FROGG ******************************/
      case EnemyType::kType_Frog :{
        if ((rand() % 2) == 0){
          (enemies + count)->transform.x = 1024;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = 3;
          (enemies + count)->width = 64;
        }else{
          (enemies + count)->transform.x = 0-64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = 3;
          (enemies + count)->width = 64;
        }
        (enemies + count)->points = 50;
        (enemies + count)->speed_y = (((rand() % 2)*2)-1) * 2;
        (enemies + count)->direction = 1;
        break;
      }
      /******************************  CROSS  ******************************/
      case EnemyType::kType_Cross :{
        if ((rand() % 2) == 0){
          (enemies + count)->transform.x = 1024+64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = -3;
          (enemies + count)->width = 64;
        }else{
          (enemies + count)->transform.x = 0-64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = 3;
          (enemies + count)->width = 64;
        }
        (enemies + count)->points = 60;
        (enemies + count)->speed_y = ((rand() % 3)-1) * 3;
        break;
      }
      /******************************  STARSHIP  ******************************/
      case EnemyType::kType_Starship :{
        if ((rand() % 2) == 0){
          (enemies + count)->transform.x = 1024+64;
          (enemies + count)->transform.scale_x *= -1.0f;
          (enemies + count)->speed_x = -3;
          (enemies + count)->width = -64;
        }else{
          (enemies + count)->transform.x = 0-64;
          (enemies + count)->transform.scale_x *= 1.0f;
          (enemies + count)->speed_x = +3;
          (enemies + count)->width = +64;
        }
        (enemies + count)->points = 25;
        (enemies + count)->speed_y = rand() % 2;
        break;
      }
    }
  }
}

void EnemyMovement(Enemy* enemies, int kNenemys, Player* player){//To move enemys back and foward
  if(enemies != NULL){
    for(int i=0; i < kNenemys; i++) {
      if((enemies + i)->is_alive == 1 || (enemies + i)->is_alive == 3){
        switch( (enemies + i)->type ){//WICTH TO CHOOSE BETWEEN EVERY ENEMY TYPE
          /*    _    ____ _____ _____ ____   ___ ___ ____  ____  
               / \  / ___|_   _| ____|  _ \ / _ \_ _|  _ \/ ___| 
              / _ \ \___ \ | | |  _| | |_) | | | | || | | \___ \ 
             / ___ \ ___) || | | |___|  _ <| |_| | || |_| |___) |
            /_/   \_\____/ |_| |_____|_| \_\\___/___|____/|____/ 
             ____  ____ ___ _  _______ ____  
            / ___||  _ \_ _| |/ / ____/ ___| 
            \___ \| |_) | || ' /|  _| \___ \ 
             ___) |  __/| || . \| |___ ___) |
            |____/|_|  |___|_|\_\_____|____/  
             ____ _____  _    ____  ____  _   _ ___ ____  
            / ___|_   _|/ \  |  _ \/ ___|| | | |_ _|  _ \ 
            \___ \ | | / _ \ | |_) \___ \| |_| || || |_) |
             ___) || |/ ___ \|  _ < ___) |  _  || ||  __/ 
            |____/ |_/_/   \_\_| \_\____/|_| |_|___|_|                                                                                
          */
          //Asteroid Spikes and Starship had the same kind of 'x' movement,
          //They go side to side, depending on which, up down or just left or right
          /******************************  ASTEROID  ******************************/
          case EnemyType::kType_Asteroid:
          /******************************  SPIKES ******************************/
          case EnemyType::kType_Spikes :
          /******************************  STARSHIP  ******************************/
          case EnemyType::kType_Starship:{
            (enemies + i)->transform.x += (enemies + i)->speed_x;
            (enemies + i)->transform.y += (enemies + i)->speed_y;
            break;
          }
          /*   ____  _   _ ____  ____  _     _____ ____  
              | __ )| | | | __ )| __ )| |   | ____/ ___| 
              |  _ \| | | |  _ \|  _ \| |   |  _| \___ \ 
              | |_) | |_| | |_) | |_) | |___| |___ ___) |
              |____/ \___/|____/|____/|_____|_____|____/                            
          */
          //The bubbles swap randombly their y speed, they can go from up to dawn randomly
          /******************************  BUBBLE ******************************/
          case EnemyType::kType_Bubble :{
            (enemies + i)->transform.x += (enemies + i)->speed_x;
            (enemies + i)->transform.y += (enemies + i)->speed_y;


            if(rand()%180 == 0){//When rand() == 0 it changes from -1 to 1 || 1 to -1
              (enemies + i)->speed_y = -(enemies + i)->speed_y;
            }
            break;
          }
          /*   ____  _   _ ___ ____  
              / ___|| | | |_ _|  _ \ 
              \___ \| |_| || || |_) |
               ___) |  _  || ||  __/ 
              |____/|_| |_|___|_|    
          */
          //Ship define their side at init and swap their 'y' coordinate following player
          //First they stay on left side of the screen for 3 secs
          /******************************  SHIP ******************************/
          case EnemyType::kType_Ship :{
            //Little patch to make sure they dont go out the screen
            if((enemies + i)->transform.y < kWindowHeight - 64 && (enemies + i)->transform.y > 0){
              (enemies + i)->transform.y += (enemies + i)->speed_y;
            }
            //When they are ready ( Is_Alive == 1) they start following player
            //If they are (Is_Alive == 3) they stay at the left of screen
            if((enemies + i)->is_alive == 1){
              (enemies + i)->transform.x += (enemies + i)->speed_x;
            }

            //To track player on those who go left
            if(player->transform.x > (enemies + i)->transform.x && (enemies + i)->speed_x > 0){
              //Both if swap their 'y' speed if player is higher or lower than them
              if(player->transform.y < (enemies + i)->transform.y && (enemies + i)->speed_y > 0){
                (enemies + i)->speed_y = -(enemies + i)->speed_y;
              }
              if(player->transform.y > (enemies + i)->transform.y && (enemies + i)->speed_y < 0){
                (enemies + i)->speed_y = -(enemies + i)->speed_y;
              }
            }
            //To track player on those who go right
            if(player->transform.x < (enemies + i)->transform.x && (enemies + i)->speed_x < 0){
              //Both if swap their 'y' speed if player is higher or lower than them
              if(player->transform.y < (enemies + i)->transform.y && (enemies + i)->speed_y > 0){
                (enemies + i)->speed_y = -(enemies + i)->speed_y;
              }
              if(player->transform.y > (enemies + i)->transform.y && (enemies + i)->speed_y < 0){
                (enemies + i)->speed_y = -(enemies + i)->speed_y;
              }
            }
            break;
          }
          /* _____ ____   ___   ____ 
            |  ___|  _ \ / _ \ / ___|
            | |_  | |_) | | | | |  _ 
            |  _| |  _ <| |_| | |_| |
            |_|   |_| \_\\___/ \____|
          */
          /* _   _ _____ ___  
            | | | |  ___/ _ \ 
            | | | | |_ | | | |
            | |_| |  _|| |_| |
             \___/|_|   \___/ 
          */
          //Frog follow player though the map, if they colide with the terrain they bounce
          /******************************  FROG  ******************************/
          /******************************  UFO ******************************/
          case EnemyType::kType_UFO:
          case EnemyType::kType_Frog :{
            if((enemies + i)->is_alive == 1){
              //First we get a filter to not enter if something is 0 to not crash
              if( player->transform.x != 0 && player->transform.y != 0 && (enemies + i)->transform.x != 0 && (enemies + i)->transform.y != 0){
                //Direction is set to target diferent player parts
                //If its -1 it tracks a corner and if its 1 it tracks another corner
                if((enemies + i)->direction > 0){
                  float vector_x = 0;
                  //To track if player is upside down or not
                  if(player->transform.scale_x < 0){
                    //To track player either if player is upscale or not
                    vector_x = player->transform.x - 72 - (enemies + i)->transform.x;
                  }else{
                    vector_x = player->transform.x - (enemies + i)->transform.x;
                  }
                  //Creating vector that will follow player pos
                  float vector_y = player->transform.y - (enemies + i)->transform.y;
                  float aux = sqrtf(  vector_x * vector_x + vector_y * vector_y );
                  //Setting it into speed
                  (enemies + i)->speed_x = (vector_x / aux) * 3;
                  (enemies + i)->speed_y = (vector_y / aux) * 3;
                }else{
                  //This is the same but with direction -1
                  float vector_x = 0;
                  if(player->transform.scale_x < 0){
                    vector_x = player->transform.x + 8 - 72 - (enemies + i)->transform.x;
                  }else{
                    vector_x = player->transform.x + 8 - (enemies + i)->transform.x;
                  }
                  float vector_y = player->transform.y + 28 - (enemies + i)->transform.y;
                  float aux = sqrtf(  vector_x * vector_x + vector_y * vector_y );

                  (enemies + i)->speed_x = (vector_x / aux) * 3;
                  (enemies + i)->speed_y = (vector_y / aux) * 3;
                }
              }
            }
            //To move that enemy
            (enemies + i)->transform.x += (enemies + i)->speed_x;
            (enemies + i)->transform.y += (enemies + i)->speed_y;
            //If it enters, it will change direcctiom
            if(rand()%180 == 0){
              (enemies + i)->direction = -(enemies + i)->direction;
            }
          
            break;
          }
          /*
             ____ ____   ___  ____ ____  
            / ___|  _ \ / _ \/ ___/ ___| 
           | |   | |_) | | | \___ \___ \ 
           | |___|  _ <| |_| |___) |__) |
            \____|_| \_\\___/|____/____/ 
          */
          //Cross will follow similar movement as Bubbles
          //It will move up, foward or down
          /******************************  CROSS ******************************/
          case EnemyType::kType_Cross :{

            (enemies + i)->transform.x += (enemies + i)->speed_x;
            (enemies + i)->transform.y += (enemies + i)->speed_y;

            //To calc up, foward or down 'y' movement, on x will go left or right
            if(rand()%300 == 0){
              int aux = (rand()%5) - 2;
              switch(aux){
                case -2:
                case -1:{
                  (enemies + i)->speed_y = -3;
                  break;
                }
                case 0:{
                  (enemies + i)->speed_y = 0;
                  break;
                }
                case 2:
                case 1:{
                  (enemies + i)->speed_y = +3;
                  break;
                }
              }
            }
            break;
          }
        }
      }
    }
  }
}

void SwapEnemy(Enemy* enemies, int kNenemys, int &actual_enemy_type){
  actual_enemy_type++;
  if(actual_enemy_type >= 8){
    actual_enemy_type = 0;
  }
  for(int i=0; i < kNenemys; i++){
    (enemies + i)->is_alive = 0;
    (enemies + i)->type = EnemyType(actual_enemy_type);
  }
}

void DrawEnemy(Enemy* enemies, int kNenemys){ //To draw every alive enemy on screen
  if(enemies != NULL){
    //To draw every enemy
    for(int i=0; i < kNenemys; i++){
      //NORMAL DRAWINGS, WHEN ITS ALIVE
      if((enemies + i)->is_alive == 1 || (enemies + i)->is_alive == 3){//If its alive it is drew
        if((enemies + i)->animation > 15){ //Animation is a counter to draw every sprite
          esat::DrawSprite(*((enemies + i)->sprite), (enemies + i)->transform);
        }else{
          esat::DrawSprite(*((enemies + i)->sprite + 1), (enemies + i)->transform);
        }
      }
      
      //EXPLOSIONS DRAWINGS
      if((enemies + i)->is_alive == 2){//If its alive it is drew
        if((enemies + i)->animation < 10){  //DRAW FIRST CLOUD
          esat::DrawSprite(*((enemies + i)->sprite + 2), (enemies + i)->transform);


        }else if((enemies + i)->animation < 20){  //DRAW SECOND CLOUD
          esat::DrawSprite(*((enemies + i)->sprite + 3), (enemies + i)->transform);


        }else if((enemies + i)->animation < 30){  //DRAW THIRD CLOUD
          esat::DrawSprite(*((enemies + i)->sprite + 4), (enemies + i)->transform);
        }
      }
    }
  }
}

void EnemyAnimation(Enemy* enemies, int kNenemys){
  //This function resets animations values as other variables necesaries to enemies
  for(int i=0; i < kNenemys; i++){
    //ANIMATIONS WHEN ITS ALIVE
    if((enemies + i)->is_alive == 1){
      //To reset to the first sprite
      //If it reaches counter numb 30 it resets
      if((enemies + i)->animation >= 30){
        (enemies + i)->animation = 0;
      }
      (enemies + i)->animation++;
    }


    //ANIMATIONS WHEN ITS EXPLODING
    if((enemies + i)->is_alive == 2){
      //Its swaps between every sprite every 10 frames
      if((enemies + i)->animation < 40){
        (enemies + i)->animation++;
      }else{
        (enemies + i)->is_alive = 0;
      }
    }

    //ANIMATION WHEN ITS PREPARING (SHIP, UFO OR FROG)
    if((enemies + i)->is_alive == 3){ //Is alive 3 is a custom state on each enemy
      switch((enemies + i)->type){
        //On Frog and UFO alive 3 is when they are bouncing and they dont track player
        //This state only lasts half of a second, 30 frames
        case EnemyType::kType_Frog:
        case EnemyType::kType_UFO:{
          if((enemies + i)->animation < 30){
            (enemies + i)->animation++;
          }else{
            (enemies + i)->animation = 0;
            (enemies + i)->is_alive = 1;
          }
          break;
        }
        //On ship this state is used at the begining when they stand at
        //Left side of the screen
        //It lasts 180 frames (3 seconds), and also they swap to white
        case EnemyType::kType_Ship:{
          //First part when its on the side
          if((enemies + i)->animation < 180){
            (enemies + i)->animation++;
          }else{
            //Second part it swaps color and changes
            (enemies + i)->color = 0;
            TypeColors((enemies + i), kNenemys);
            (enemies + i)->animation = 0;
            (enemies + i)->is_alive = 1;
          }
          break;
        }
      }
    }
  }
}

void TypeColors(Enemy* enemies, int kNenemys){
  switch( (enemies)->type ){//SETTING EVERY COLOR FOR EVERY ENEMY
  //1 IS RED, 2 IS BLUE, 3 IS GREEN, 4 IS VIOLET AND 0 IS WHITE
    /*   _    ____ _____ _____ ____   ___ ___ ____  ____  
        / \  / ___|_   _| ____|  _ \ / _ \_ _|  _ \/ ___| 
       / _ \ \___ \ | | |  _| | |_) | | | | || | | \___ \ 
      / ___ \ ___) || | | |___|  _ <| |_| | || |_| |___) |
     /_/   \_\____/ |_| |_____|_| \_\\___/___|____/|____/ 
    */
    /******************************  ASTEROID  ******************************/
    case EnemyType::kType_Asteroid: {
      //Sprites
      switch((enemies)->color){
        case 1:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/red_asteroid1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/red_asteroid2.png");
          break;
        }
        case 2:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/blu_asteroid1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/blu_asteroid2.png");
          break;
        }
        case 3:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/grn_asteroid1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/grn_asteroid2.png");
          break;
        }
        case 4:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/vio_asteroid1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/vio_asteroid2.png");
          break;
        }
      }
      break;
    }
    /*  ____  ____ ___ _  _______ ____  
       / ___||  _ \_ _| |/ / ____/ ___| 
       \___ \| |_) | || ' /|  _| \___ \ 
        ___) |  __/| || . \| |___ ___) |
       |____/|_|  |___|_|\_\_____|____/ 
    */
    /******************************  SPIKES ******************************/
    case EnemyType::kType_Spikes :{
      switch((enemies)->color){
        case 1:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/red_spikes1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/red_spikes2.png");
          break;
        }
        case 2:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/blu_spikes1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/blu_spikes2.png");
          break;
        }
        case 3:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/grn_spikes1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/grn_spikes2.png");
          break;
        }
        case 4:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/vio_spikes1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/vio_spikes2.png");
          break;
        }
      }
      break;
    }
    /*   ____  _   _ ____  ____  _     _____ ____  
        | __ )| | | | __ )| __ )| |   | ____/ ___| 
        |  _ \| | | |  _ \|  _ \| |   |  _| \___ \ 
        | |_) | |_| | |_) | |_) | |___| |___ ___) |
        |____/ \___/|____/|____/|_____|_____|____/                            
    */
    /******************************  BUBBLE ******************************/
    case EnemyType::kType_Bubble :{
      switch((enemies)->color){
        case 1:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/red_bubble1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/red_bubble2.png");
          break;
        }
        case 2:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/blu_bubble1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/blu_bubble2.png");
          break;
        }
        case 3:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/grn_bubble1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/grn_bubble2.png");
          break;
        }
        case 4:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/vio_bubble1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/vio_bubble2.png");
          break;
        }
      }
      break;
    }
    /*   ____  _   _ ___ ____  
        / ___|| | | |_ _|  _ \ 
        \___ \| |_| || || |_) |
         ___) |  _  || ||  __/ 
        |____/|_| |_|___|_|    
    */
    /******************************  SHIP ******************************/
    case EnemyType::kType_Ship :{
      switch((enemies)->color){
        case 0:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/whi_ship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/whi_ship2.png");
          break;
        }
        case 1:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/red_ship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/red_ship2.png");
          break;
        }
        case 2:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/blu_ship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/blu_ship2.png");
          break;
        }
        case 3:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/grn_ship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/grn_ship2.png");
          break;
        }
        case 4:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/vio_ship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/vio_ship2.png");
          break;
        }
      }
      break;
    }
    /* _   _ _____ ___  
      | | | |  ___/ _ \ 
      | | | | |_ | | | |
      | |_| |  _|| |_| |
       \___/|_|   \___/ 
    */
    /******************************  UFO ******************************/
    case EnemyType::kType_UFO :{
      switch((enemies)->color){
        case 1:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/red_ufo1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/red_ufo2.png");
          break;
        }
        case 2:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/blu_ufo1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/blu_ufo2.png");
          break;
        }
        case 3:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/grn_ufo1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/grn_ufo2.png");
          break;
        }
        case 4:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/vio_ufo1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/vio_ufo2.png");
          break;
        }
      }
      break;
    }
    /* _____ ____   ___   ____ 
      |  ___|  _ \ / _ \ / ___|
      | |_  | |_) | | | | |  _ 
      |  _| |  _ <| |_| | |_| |
      |_|   |_| \_\\___/ \____|
    */
    /******************************  FROGG ******************************/
    case EnemyType::kType_Frog :{
      switch((enemies)->color){
        case 1:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/red_frog1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/red_frog2.png");
          break;
        }
        case 2:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/blu_frog1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/blu_frog2.png");
          break;
        }
        case 3:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/grn_frog1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/grn_frog2.png");
          break;
        }
        case 4:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/vio_frog1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/vio_frog2.png");
          break;
        }
      }
      (enemies)->direction = 1;
      break;
    }
    /*
       ____ ____   ___  ____ ____  
      / ___|  _ \ / _ \/ ___/ ___| 
     | |   | |_) | | | \___ \___ \ 
     | |___|  _ <| |_| |___) |__) |
      \____|_| \_\\___/|____/____/ 
    */
    /******************************  CROSS  ******************************/
    case EnemyType::kType_Cross :{
      switch((enemies)->color){
        case 1:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/red_cross1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/red_cross2.png");
          break;
        }
        case 2:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/blu_cross1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/blu_cross2.png");
          break;
        }
        case 3:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/grn_cross1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/grn_cross2.png");
          break;
        }
        case 4:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/vio_cross1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/vio_cross2.png");
          break;
        }
      }
      break;
    }
    /* ____ _____  _    ____  ____  _   _ ___ ____  
      / ___|_   _|/ \  |  _ \/ ___|| | | |_ _|  _ \ 
      \___ \ | | / _ \ | |_) \___ \| |_| || || |_) |
       ___) || |/ ___ \|  _ < ___) |  _  || ||  __/ 
      |____/ |_/_/   \_\_| \_\____/|_| |_|___|_|   
    */
    /******************************  STARSHIP  ******************************/
    case EnemyType::kType_Starship :{
      switch((enemies)->color){
        case 1:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/red_starship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/red_starship2.png");
          break;
        }
        case 2:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/blu_starship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/blu_starship2.png");
          break;
        }
        case 3:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/grn_starship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/grn_starship2.png");
          break;
        }
        case 4:{
          *((enemies)->sprite) = esat::SpriteFromFile("sprites/enemy/vio_starship1.png");
          *((enemies)->sprite + 1) = esat::SpriteFromFile("sprites/enemy/vio_starship2.png");
          break;
        }
      }
      break;
    }
  }
}

bool EnemyIsCollidingUp(esat::Vec2 enemies,esat::Vec2 WandH, Terrain* platform){
  // Box colider from the enemy
  esat::Vec2 P1 = {enemies.x , enemies.y}; // first point from the sprite of the enemy
  esat::Vec2 P2 = {enemies.x + WandH.x , enemies.y}; // second point from the sprite of the enemy


  // Box colider from the platform
  esat::Vec2 Q1 = {platform->transform.x, platform->transform.y}; // first point from the platform
  esat::Vec2 Q2 = {platform->transform.x + platform->width, platform->transform.y}; // second point from the platform
  esat::Vec2 Q3 = {platform->transform.x, platform->transform.y + platform->height}; // third point from the player


  // esat::DrawSetStrokeColor(255,0,0);
  // esat::DrawLine(P1.x,P1.y,P2.x,P2.y);
  // esat::DrawLine(Q1.x,Q1.y,Q2.x,Q2.y);


  // First checks if the enemy is in the position Y of the platform
  // Then checks if the enemy position X1 and X2 that are the points from up
  // if the enemy have one point inside the width of the platform it is colliding
  if ((P1.y < Q3.y && P1.y > Q1.y) && (P2.x > Q1.x && P1.x < Q2.x)) {
    return true;
  } else{
    return false;
  }
}

bool EnemyIsCollidingRight(esat::Vec2 enemies,esat::Vec2 WandH, Terrain* platform){
  esat::Vec2 P2 = {enemies.x + WandH.x, enemies.y}; // second point from the enemy
  esat::Vec2 P4 = {enemies.x + WandH.x, enemies.y + WandH.y}; // fourth point from the enemy


  esat::Vec2 Q1 = {platform->transform.x, platform->transform.y}; // first point from the platform
  esat::Vec2 Q2 = {platform->transform.x + platform->width, platform->transform.y};
  esat::Vec2 Q3 = {platform->transform.x, platform->transform.y + platform->height}; // third point from the platform


  // esat::DrawSetStrokeColor(255,0,0);
  // esat::DrawLine(P2.x,P2.y,P4.x,P4.y);
  // esat::DrawLine(Q1.x,Q1.y,Q3.x,Q3.y);


  // First checks the platform height is inside the
  // box colider from the enemy
  // then it checks if the enemy is not colliding with the platform in X
  if ((P2.x > Q1.x && P2.x < Q2.x) && (Q1.y < P4.y && Q3.y > P2.y)){
    // If it is colliding then return true
    return true;
  } else {
    // If it is not colliding the return false
    return false;
  }
}

bool EnemyIsCollidingLeft(esat::Vec2 enemies,esat::Vec2 WandH, Terrain* platform){
  esat::Vec2 P1 = {enemies.x, enemies.y};
  esat::Vec2 P3 = {enemies.x, enemies.y + WandH.y};


  esat::Vec2 Q1 = {platform->transform.x, platform->transform.y}; // first point from the platform
  esat::Vec2 Q2 = {platform->transform.x + platform->width, platform->transform.y}; // second point from the platform
  esat::Vec2 Q4 = {platform->transform.x + platform->width, platform->transform.y + platform->height}; // fourth point from the platform


  // esat::DrawSetStrokeColor(255,0,0);
  // esat::DrawLine(P1.x,P1.y,P3.x,P3.y);
  // esat::DrawLine(Q2.x,Q2.y,Q4.x,Q4.y);


  // First checks the platform height is inside the
  // box colider from the enemy
  // then it checks if the enemy is not colliding with the platform in X
  if ((P1.x > Q1.x && P1.x < Q2.x ) && (P3.y >= Q2.y && P1.y <= Q4.y)){
    // If it is colliding then return true
    return true;
  } else {
    // If it is not colliding the return false
    return false;
  }
}

void TypeCollides(Enemy* enemigos, bool iscollidingright,bool iscollidingleft,bool iscollidingdown,bool iscollidingup){
  switch((enemigos)->type){
    case kType_Ship:
    case kType_Asteroid:
    case kType_Starship:{
        (enemigos)->is_alive = 2;  //EXPLODE ENEMIES THAT GO DOWN
        (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
      break;
    }
    case kType_Cross:
    case kType_Spikes:
    case kType_Bubble:{
      //TO BOUNCE ENEMIES
      if(iscollidingup == true){
        (enemigos)->speed_y = -(enemigos)->speed_y; //MAKE ENEMIES BOUNCE
        iscollidingup = false;
      }
      else if(iscollidingdown == true){
        (enemigos)->speed_y = -(enemigos)->speed_y; //MAKE ENEMIES BOUNCE
        iscollidingdown = false;
      }
      else if(iscollidingright == true){
          (enemigos)->speed_x = -(enemigos)->speed_x; //CHANGES THEIR DIRECTION
          iscollidingright = false;
      }
      else if(iscollidingleft == true){
          (enemigos)->speed_x = -(enemigos)->speed_x; //CHANGES THEIR DIRECTION
          iscollidingleft = false;
      }
      break;
    }
    case kType_Frog:{
      if(iscollidingup == true){
        (enemigos)->speed_y = -(enemigos)->speed_y;
        (enemigos)->direction = -(enemigos)->direction;
        (enemigos)->is_alive = 3;
        (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
        iscollidingup = false;
      }
      else if(iscollidingdown == true){
        (enemigos)->speed_y = -(enemigos)->speed_y;
        (enemigos)->direction = -(enemigos)->direction;
        (enemigos)->is_alive = 3;
        (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
        iscollidingdown = false;
      }
      else if(iscollidingright == true){
          (enemigos)->speed_x = -(enemigos)->speed_x;
          (enemigos)->direction = -(enemigos)->direction;
          (enemigos)->is_alive = 3;
          (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
          iscollidingright = false;
      }
      else if(iscollidingleft == true){
          (enemigos)->speed_x = -(enemigos)->speed_x;
          (enemigos)->direction = -(enemigos)->direction;
          (enemigos)->is_alive = 3;
          (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
          iscollidingleft = false;
      }
      break;
    }
    case kType_UFO:{
      if(iscollidingup == true){
        (enemigos)->speed_y = -(enemigos)->speed_y;
        (enemigos)->is_alive = 3;
        (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
        iscollidingup = false;
      }
      else if(iscollidingdown == true){
        (enemigos)->speed_y = -(enemigos)->speed_y;
        (enemigos)->is_alive = 3;
        (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
        iscollidingdown = false;
      }
      else if(iscollidingright == true){
          (enemigos)->speed_x = -(enemigos)->speed_x;
          (enemigos)->is_alive = 3;
          (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
          iscollidingright = false;
      }
      else if(iscollidingleft == true){
          (enemigos)->speed_x = -(enemigos)->speed_x;
          (enemigos)->is_alive = 3;
          (enemigos)->animation = 0; //RESTART ANIMATION TO CHANGE TO EXPLODING
          iscollidingleft = false;
      }
      break;
    }
  }
}

void EnemyCollides(Enemy* enemies, int kNenemys, Player* player, bool iscollidingright,bool iscollidingleft,bool iscollidingdown,bool iscollidingup,Ship* ship){
  esat::SpriteTransform EnemyMovementPrediction_tr;
  if(enemies != NULL){
    for (int i=0;i< kNenemys; i++){
      EnemyMovementPrediction_tr.x = (enemies + i)->transform.x + (enemies + i)->speed_x; //Predicts enemy movement
      EnemyMovementPrediction_tr.y = (enemies + i)->transform.y + (enemies + i)->speed_y; //Predicts enemy movement
      if((enemies + i)->is_alive == 1){
        if((enemies + i)->transform.x >= 1024 && (enemies + i)->speed_x > 0){
          (enemies + i)->transform.x = 0;  //TO TP ENEMIES THAT GO RIGHT
        }
        if((enemies + i)->transform.x <= 0 && (enemies + i)->speed_x < 0){
          (enemies + i)->transform.x = 1024;  //TO TP ENEMIES THAT GO LEFT
        }
        if((enemies + i)->transform.y <= kWindowHeight/10 && (enemies + i)->speed_y <= 0){ //checks if colliding with the hud
          iscollidingup = true;
          TypeCollides((enemies + i),iscollidingright,iscollidingleft,iscollidingdown,iscollidingup);
          #ifdef WIN32
          soloud.play(audio[0]);
          #endif
        }
        (enemies + i)->height = 64;
        for(int j=0; j < 4; j++){ //Checks if the enemies are colliding with any of the 3 platforms or the floor
          if(EnemyIsCollidingUp({(enemies + i)->transform.x,EnemyMovementPrediction_tr.y},{(enemies + i)->width,(enemies + i)->height},terrain+j) == true){
            iscollidingup = true;
            TypeCollides((enemies + i),false,false,false,iscollidingup);
            #ifdef WIN32
            soloud.play(audio[0]);
            #endif
          }
          else if(EnemyIsCollidingDown({(enemies + i)->transform.x,EnemyMovementPrediction_tr.y},{(enemies + i)->width,(enemies + i)->height},terrain+j) == true){
            #ifdef WIN32
            soloud.play(audio[0]);
            #endif
            iscollidingdown = true;
            TypeCollides((enemies + i),false,false,iscollidingdown,false);
          }
          else if(EnemyIsCollidingRight({EnemyMovementPrediction_tr.x,(enemies + i)->transform.y},{(enemies + i)->width,(enemies + i)->height},terrain+j) == true){
            iscollidingright = true;
            TypeCollides((enemies + i),iscollidingright,false,false,false);
            #ifdef WIN32
            soloud.play(audio[0]);
            #endif
          }
          else if(EnemyIsCollidingLeft({EnemyMovementPrediction_tr.x,(enemies + i)->transform.y},{(enemies + i)->width,(enemies + i)->height},terrain+j) == true){
            iscollidingleft = true;
            TypeCollides((enemies + i),false,iscollidingleft,false,false);
            #ifdef WIN32
            soloud.play(audio[0]);
            #endif
          }
        }
        if (areTheseColliding(player->transform,{player->width,player->height}, EnemyMovementPrediction_tr,{(enemies + i)->width,(enemies + i)->height} ) == true  ){
          (enemies + i)->is_alive = 2;
          (enemies + i)->animation = 0;

          KillEnemies(enemies, kNenemys);
          // PLAYER DIES
          if(player->lives > 1){
            player->lives = player->lives - 1;
            #ifdef WIN32
            soloud.play(audio[8]);
            #endif
          }
          else{
            // CHECK IF THERE IS ANOTHER PLAYER ALIVE
            #ifdef WIN32
            soloud.play(audio[8]);
            #endif
            printf("player died but there is no game over screen");
            gm.state = KLOST;
          }
          swapPlayer(player,ship);
        }
      }
    }
  }
}

bool EnemyIsCollidingDown(esat::Vec2 enemies,esat::Vec2 WandH, Terrain* platform){
  // Box colider from the enemy
  esat::Vec2 P3 = {enemies.x, enemies.y + WandH.y};
  esat::Vec2 P4 = {enemies.x + WandH.x, enemies.y + WandH.y};


  // Box colider from the platform
  esat::Vec2 Q1 = {platform->transform.x, platform->transform.y}; // first point from the platform
  esat::Vec2 Q2 = {platform->transform.x + platform->width, platform->transform.y}; // second point from the platform
  esat::Vec2 Q3 = {platform->transform.x, platform->transform.y + platform->height}; // third point from the player


  // esat::DrawSetStrokeColor(255,0,0);
  // esat::DrawLine(P3.x,P3.y,P4.x,P4.y);
  // esat::DrawLine(Q1.x,Q1.y,Q2.x,Q2.y);


  // First checks if the enemy is in the position Y of the platform
  // Then checks if the enemy position X1 and X2 that are the points from up
  // if the enemy have one point inside the width of the platform it is colliding
  if ((P4.y >= Q1.y && P4.y <= Q3.y) && (P4.x > Q1.x && P3.x < Q2.x)) {
    return true;
  } else{
    return false;
  }
}

void InitSound(){
  #ifdef WIN32
  //Load audio
  soloud.init();
    //Enemy explosion
    audio[0].load("./Audio/Enemy-CollisionWithFloorOrPlatforms.ogg");
    //Sound when menu is showed and it's done loading
    audio[1].load("./Audio/Program-MenuFirstTimeOpened.ogg"); 
    //Player fire sounds (First is single shot, second if key maintained)
    audio[2].load("./Audio/Fire1-PlayerSingleShotKeyOnceNotMantained.ogg");
    audio[3].load("./Audio/Fire2-ShotPressedKey(FireKeyPressed-2d-FireSoundIfKeyStillPressed).ogg"); 
    //Pickups and power sounds
    audio[4].load("./Audio/Pickup-PickFuel.ogg"); 
    audio[5].load("./Audio/Pickups-PickDiamond.ogg");
    audio[6].load("./Audio/Pickups-PickShipComponent.ogg");
    //Player respawn and collision
    audio[7].load("./Audio/Player-SpawnAndRespawn.ogg");
    audio[8].load("./Audio/Player-TouchesEnemyAndDies.ogg");
    //Ship sounds and fuel
    audio[9].load("./Audio/Ship-FlyingSoundLoop.ogg");
    audio[10].load("./Audio/Ship-AddedFuel.ogg");
    //As a reference to how it should sounds when Ship-FlyingSoundLoop is looped
    audio[11].load("./Audio/Example-ShipFlyingFullSoundSample.ogg");
    //Just for lols
    audio[12].load("./Audio/grunt1-84540.mp3");
    //For epic ship launch start
    audio[13].load("./Audio/halo_theme.mp3");
    //For looping the ship audio
    audio[9].setLooping(true);
    #endif
}

void FreeSound(){
  #ifdef WIN32
  soloud.deinit();
  #endif
}
/* This part should be localized after the libraries include */
#ifndef Structs
#define Structs 1


typedef struct{
  esat::SpriteHandle sprite;
  esat::SpriteTransform transform;
  PickUps type;
  bool is_active;
  bool is_being_held;
  bool was_dropped;
  int points; // for objects that give points, in the case of fuel just set it to 0 
  float width,height;
}Object; // any powerUp/ pick up or item that gives points

struct RayComponent{
  esat::SpriteHandle sprite; // Sprite and position of  every subcomponent of the player atack (ray, beam...) 
  esat::SpriteTransform transform;
  float distance;
  float width, height; // For creating the collide box's
  bool is_active; 
};

//Ray represents the list ("array") of bullets/rays shoot by the player. While pressing the shot button more elements are atached to the list
struct Ray{
  RayComponent* sub_rays; //
  float distance;
  int speed;
  int front_component; // to know which sub_ray to check for collisions;
  Ray* next;// without the struct it gives me errors
  Ray* prev;
};

typedef struct {
  // We will need to initialize every time this for *enemy_sprite = (esat::SpriteHandle*) malloc(2 * sizeof(esat::SpriteHandle))
  esat::SpriteHandle* sprite; // In the meanwhile for sprites. Maybe move using 2 sprites like in Space Invaders.
  esat::SpriteTransform transform;
  int points;
  float speed_x, speed_y;
  int direction;
  int movement;
  int animation;
  int is_alive; //0 -> DEAD || 1-> ALIVE || 2-> EXPLODING //To not interface whith other states
  float width, height;
  EnemyType type;
  int color;
} Enemy;


typedef struct{
  esat::SpriteHandle sprite; 
  esat::SpriteTransform transform;
  bool is_conected; // when not atached to the ship this is a pickable object, 
  bool was_dropped;
  bool is_being_held;
  float width, height;
  // connected to the ship, right?
} ShipComponent;

typedef struct{
  ShipComponent* component; // pointer to the components, we init this 
  esat::SpriteTransform transform;
  int fuel_counter; // to check the fuel.
  int components_attached; // counter of all the pieces already attached to 
  unsigned char* save_pixel; // to save each pixel from the original sprite so we can change it later :)
  unsigned char* rocket_px_RGBA;
  unsigned char* rocket_fuel_px_RGBA;
  // the ship
}Ship;

typedef struct {
  esat::SpriteHandle* sprite;// pointer to how many sprites we use for the player
  esat::SpriteTransform transform;
  Ray ray; // just ray...
  Object* object; // pointer to the object being held. we might be able to check with this value !=NULL instead of is_holding_object == true. but the boolean is easier to understand...
  ShipComponent* component; // this is easier, trust me.
  bool is_holding_object;// True when holding something else false. Check if player's holding something. Posibly also check if it's holding any rocket piece.
  bool is_holding_component;
  bool is_shooting ; // True if the player is shooting.
  bool is_on_ground;
  int lives, score,animation_state;
  float width, height;
} Player;

typedef struct {
  esat::SpriteHandle sprite; // pointer to how many sprites we use for the terrain, if we end up using only one sprite remove the pointer
  esat::SpriteTransform transform;
  float width, height; //
  // THE SAME STRUCT FOR THE PLATFOMRS AND THE FLOOR OF THE LEVEL. HENCE THE WORD "TERRAIN"
} Terrain;

typedef struct{
  int current_level; // just the number for the level
  int level_stage;// 4 levels ( one for each ship) and 4 stages when we reach stage 4 we change level
  EnemyType enemy_to_spawn;
  bool is_ship_built; // to differenciate the levels in wich putting the ship togethers is needed and the ones that is not

}Level; 
// before drawing the level we need to know which enemy spawns in this level and if the ship is built or not.
typedef struct{
  int num_players;
  bool current_player; // 1 or 2. could be boolean. yeah, boolean.
  int keyboard_or_joystick; // 0 for keyboard, 1 for joystick
  bool is_newGame;// to check if its needed to load a game savefile or not
  bool is_game_paused;
  bool no_objects_displayed; // if there are no objects on the screen we spawn one
  int frames_Since_Object_Spawn;// long...
  int frames_Since_Fuel_Spawn; // fue is just one object more and i need this to make sure it can spawn one regular object and one fuel at the same time
  GameState state;
  Level level;
}GameManager;// to check game states 

typedef struct{
  int lives, score;
  int current_level,level_stage;
}SavedGame;
#endif


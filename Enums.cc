#ifndef Enums
#define Enums 1
// INCLUDE THIS BEFORE THE STRUCTS.CC FILE!
enum EnemyType {
  kType_Asteroid = 0,
  kType_Spikes,
  kType_Bubble,
  kType_Ship,
  kType_UFO,
  kType_Cross,
  kType_Starship,
  kType_Frog
}; 
enum PickUps{
  KFUEL = 0,
  KATOM , // we can make a base value for the points this objects gives and then multiply it for the index ( example: base = 50 so ATOM( 1) gives 50 pointts and diamond(2) gives 100)
  KDIAMOND,
  KLINGOT,
  KNUKE,
  KSLIME //?????? im basing this names on the sprite names that Pau made btw
};
enum ShipType{
  KShip1 = 1,
  KShip2,
  KShip3,
  KShip4
};
enum ColoringMode{
  KFULL,// paints the entire block that is the sprite
  KSHAPE,// PAINTS ONLY THE SHAPE OF THE SPRITE; THE NON TRANSPARENT PIXELS
  KTOP_HALF,
  KBOTTOM_HALF

};
enum Color{
  KRED,
  KBLUE,
  KGREEN,
  KYELLOW
};
enum GameState{
  KMENU,
  KLANDING,
  KMAIN_LOOP,
  KNEXT_LEVEL,
  KTAKEOFF,
  KWON,
  KLOST,
  KNEXTPLAYER
};

// My objective is a function that looks like this  colorSprite(SpriteHandle,ColoreingMode,Color);
#endif
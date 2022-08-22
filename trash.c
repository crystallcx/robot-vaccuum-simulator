#include "trash.h"
#include "robot.h"
#include "helpers.h"
#include "collision.h"
#include <stdlib.h>
#include <stdbool.h>
#include <cab202_timers.h>

//TRASH STATE
#define DUST_WIDTH 1
#define DUST_HEIGHT 1
#define TRASH_WIDTH 11
#define TRASH_HEIGHT 6
#define SLIME_WIDTH 5
#define SLIME_HEIGHT 5

#define MAX_DUST 1000
#define MAX_SLIME 10
#define MAX_TRASH 5
#define DUST_MASS 1
#define SLIME_MASS 5
#define TRASH_MASS 20

// character pointer arrays for trash, slime & dust.
char * trash = "     &         &&&       &&&&&     &&&&&&&   &&&&&&&&& &&&&&&&&&&&";
char * slime = " ~~~ ~~~~~~~~~~~~~~~ ~~~ ";
char * dust = ".";

//Global variables
static double dx[MAX_DUST], dy[MAX_DUST];
static double sx[MAX_SLIME], sy[MAX_SLIME];
static double tx[MAX_TRASH], ty[MAX_TRASH];
static int num_dust,num_slime,num_trash = 1;
static int dust_left,slime_left,trash_left = 1;


/** setup_trash(int i), setup_slime(int i), setup_dust(int i)
 *  Set up a single rubbish at a random location.
 *
 *  Parameters:
 *      i: An integer satisfying 0 <= i < num_dust/num_slime/num_trash which indicates the
 *          object to be placed.
 */
void setup_trash(int i){
  int w, h;
  get_screen_size( &w, &h );
  tx[i] = rand() % w;
  ty[i] = rand() % h;
  while (tx[i] < 1 || tx[i]> (w-TRASH_WIDTH-1))        tx[i] = rand() % w;
  while (ty[i] < 5 || ty[i]> (h- TRASH_HEIGHT-3))       ty[i] = rand() % h;
}

void setup_slime(int i){
  int w, h;
  get_screen_size( &w, &h );
  sx[i] =  rand() % w;
  sy[i]= rand() % h;
  while (sx[i] < 1 || sx[i]> (w-SLIME_WIDTH-1))      sx[i] = rand() % w;
  while (sy[i] < 5 || sy[i]> (h- SLIME_HEIGHT-3))    sy[i] =  rand() % h;
}

void setup_dust(int i){
  int w, h;
  get_screen_size(&w, &h);
  dx[i] = rand() % w;
  dy[i] = rand() % h;
  while (dy[i] < 5 || dy[i]>(h-DUST_HEIGHT-3))   dy[i] = rand() % h;
  while (dx[i] < 1 || dx[i]> (w-DUST_WIDTH-1))   dx[i] = rand() % w;
  }


/** setup_trashs(), setup_slimes(), setup_dusts()
 *  Sets up all rubbish (0 <= i < num_rubbish) so that they are all
 *  wholly contained within the border, and none overlap with the robot, charger, or other pieces of rubbish. */
void setup_trashs(){
  num_trash = get_int("How much trash would you like?");
  if (num_trash > MAX_TRASH) {
      num_trash = MAX_TRASH;
  }
  for (int i = 0; i < num_trash; i++){
      do {
          setup_trash(i);
      }
      while (collision(get_robot_x(),get_robot_y(),get_robot_width(),get_robot_height(),char_robot(),tx[i],ty[i],TRASH_WIDTH, TRASH_HEIGHT,trash)
            || collision(get_charger_x(),get_charger_y(),get_charger_width(),get_charger_height(),char_charger(),tx[i],ty[i],TRASH_WIDTH,TRASH_HEIGHT,trash)
            || trash_trash(i));
  }
}

 void setup_slimes(){
   // bool redo = true;
  num_slime = get_int("How much slime would you like?");
  if (num_slime > MAX_SLIME) {
      num_slime = MAX_SLIME;
  }
  for (int i = 0; i < num_slime; i++){
    setup_slime(i);
  }

  for (int i = 0; i < num_slime; i++){
      do {
          setup_slime(i);
      }
      while (collision(get_robot_x(),get_robot_y(),get_robot_width(),get_robot_height(),char_robot(),sx[i],sy[i],SLIME_WIDTH, SLIME_HEIGHT,slime)
            || collision(get_charger_x(),get_charger_y(),get_charger_width(),get_charger_height(),char_charger(),sx[i],sy[i],SLIME_WIDTH,SLIME_HEIGHT,slime)
            || slime_slime(i) || slime_trash(i));
  }
}


void setup_dusts(){
  num_dust = get_int("How much dust would you like?");
  if (num_dust > MAX_DUST) {
      num_dust = MAX_DUST;
  }
  for (int i = 0; i < num_dust; i++) {
    do{
      setup_dust(i);
    }
    while(collision(get_robot_x(), get_robot_y(), get_robot_width(), get_robot_height(),char_robot(), dx[i], dy[i], DUST_WIDTH, DUST_HEIGHT,dust)
    || collision(get_charger_x(),get_charger_y(),get_charger_width(),get_charger_height(),char_charger(),dx[i],dy[i],DUST_WIDTH,DUST_HEIGHT,dust)
    || dust_dust(i) || dust_slime(i) || dust_trash(i));
    }
}

/***  Draw one rubbish, indicated by ID i. The object is on
 *  (dx[i],dy[i]). (top left corner)
 *  i: An integer which satisfiess 0 <= i < num_rubbish.*/
 void draw_trash( int i ) {
     draw_pixels(tx[i],ty[i],TRASH_WIDTH,TRASH_HEIGHT,trash,true);
 }
void draw_slime( int i ) {
    draw_pixels(sx[i],sy[i],SLIME_WIDTH,SLIME_HEIGHT,slime,true);
}
void draw_dust( int i ) {
    draw_char(dx[i],dy[i],'.');
}

/***  Draws all rubbish. */
void draw_all_rubbish(){
  for ( int k = 0; k < num_trash; k++ ) {
      draw_trash( k );
  }
  for ( int j = 0; j < num_slime; j++ ) {
      draw_slime( j );
  }
  for ( int i = 0; i < num_dust; i++) {
      draw_dust( i );
  }
}

/*Action taken when the vacuum has collided with a piece of rubbish.
If a rubbish has collided with the vacuum, it is transported off the screen*/
void do_collided_rubbish(int Rubbish_type, int i){
  switch(Rubbish_type){
    case 1: //case 1 : if dust[i] collides with robot vacuum
      dx[i]=-30;
      dy[i]=-30;
      break;
    case 2: //case 2 : if slime[i] collides with robot vacuum
      sx[i]=-30;
      sy[i]=-30;
      break;
    case 3: //case 3 : if trash[i] collides with robot vacuum
      tx[i]=-30;
      ty[i]=-30;
      break;
  }
}

//Adds a piece of dust to the screen based on specified coordinates
void add_dust(){
  int i = num_dust;
  int x = get_int("dust x?");
  int y = get_int("dust y?");
  if (i >= 0 && i < MAX_DUST) {
    num_dust++;
    dx[i]= x;
    dy[i]= y;
    draw_dust(i);
  }
}
//Adds a piece of slime to the screen based on specified coordinates
void add_slime() {
	int i = num_slime;
    int x = get_int("slime x?");
    int y = get_int("slime y?");
	    if (i >= 0 && i < MAX_SLIME) {
			num_slime++;
			sx[i]= x;
			sy[i]= y;
			draw_slime(i);
		}
}
//Adds a piece of trash to the screen based on specified coordinates
void add_trash() {
	int i = num_trash;
    int x = get_int("trash x?");
    int y = get_int("trash y?");
	    if (i >= 0 && i < MAX_TRASH) {
			num_trash++;
			tx[i]= x;
			ty[i]= y;
			draw_trash(i);
		}
}

//returns x coordinate of dust/slime/trash
double get_dust_x(int i) {
  return dx[i];
}
double get_slime_x(int i){
  return sx[i];
}
double get_trash_x(int i){
  return tx[i];
}

//returns y coordinate of dust/slime/trash
double get_dust_y(int i) {
  return dy[i];
}
double get_slime_y(int i){
  return sy[i];
}
double get_trash_y(int i){
  return ty[i];
}

//returns width of dust/slime/trash
int get_dust_width(){
  return DUST_WIDTH;
}
int get_slime_width(){
  return SLIME_WIDTH;
}
int get_trash_width(){
  return TRASH_WIDTH;
}

//returns height of dust/slime/trash
int get_dust_height(){
  return DUST_HEIGHT;
}
int get_slime_height() {
  return SLIME_HEIGHT;
}
int get_trash_height(){
  return TRASH_HEIGHT;
}

//returns number of dust/slime/trash (initial)
int get_num_dust() {
  return num_dust;
}
int get_num_slime() {
  return num_slime;
}
int get_num_trash(){
  return num_trash;
}

// returns character arrays of dust/slime/trash
char * char_dust(){
  return dust;
}
char * char_slime(){
  return slime;
}
char * char_trash(){
  return trash;
}

//returns amount of dust left
int get_dust_left(){
  dust_left = num_dust - get_dust_collected();
  return dust_left;
}

//returns amount of slime left
int get_slime_left(){
  slime_left = num_slime - get_slime_collected();
  return slime_left;
}

//returns amount of trash left
int get_trash_left(){
  trash_left = num_trash - get_trash_collected();
  return trash_left;
}

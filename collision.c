#include "collision.h"
#include "trash.h"
#include "robot.h"
#include "helpers.h"
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include <math.h>
#include <string.h>
int dust_collected,slime_collected,trash_collected;
int weight;
bool timed_out( void );
bool full_weight;
void reset( void );

//character pointer array for charger2
char * charger2 =
"###########"
"##!CHARGE##"
"###########"
" ######### ";

/**
 *  Returns true if and only if two non-space pixels overlap.
 *
 *  Parameters:
 *      (x0,y0) - real-valued coordinates of the top left corner of the first image.
 *      (w0,h0) - integer-valued dimensions of the first image.
 *      (x1,y1) - real-valued coordinates of the top left corner of the second image.
 *      (w1,h1) - integer-valued dimensions of the second image.
 */
bool collision(int x0, int y0, int w0, int h0, char pixels0[], int x1, int y1, int w1, int h1, char pixels1[]){
int x = 0;
int y = 0;
for (x = x0; x < x0 + w0; x++) {
	for ( y = y0; y < y0 + h0; y++) {
		if (( x >= x1) && (x < x1 + w1)) {
			if (( y >= y1) && (y < y1+h1)) {
				if ((pixels1[(x-x1)+(y-y1)*w1] != ' ')&&(pixels0[(x-x0)+(y-y0)*w0] != ' ')){
					return true;
				}}}}}
	return false;
}

/**
 *  Returns true if and only if two supplied bounding boxes overlap.
 *
 *  Parameters:
 *      (x0,y0) - real-valued coordinates of the centre of the first bounding box.
 *      (w0,h0) - integer-valued dimensions of the first bounding box.
 *      (x1,y1) - real-valued coordinates of the centre of the second bounding box.
 *      (w1,h1) - integer-valued dimensions of the second bounding box.
 */
bool collided(
    double x0, double y0, int w0, int h0,
    double x1, double y1, int w1, int h1
) {
    int left0 = round( x0 ) , right0 = left0 + w0 - 1;
    int left1 = round( x1 ) , right1 = left1 + w1 - 1;

    int top0 = round( y0 ) , bottom0 = top0 + h0 - 1;
    int top1 = round( y1 ) , bottom1 = top1 + h1 - 1;

    if ( right1 < left0 || right0 < left1 ) return false;
    if ( bottom1 < top0 || bottom0 < top1 ) return false;

    return true;
}


/***  Tests to see if any zombie has collided with the robot. If so,
 *  do_collided is called to restart the simulation.*/
void check_collision(){
	for ( int i = 0; i < get_num_dust(); i++ ) {
			if ( collision(get_robot_x(),get_robot_y(),get_robot_width(),get_robot_height(),char_robot(),get_dust_x(i),get_dust_y(i),get_dust_width(),get_dust_height(),char_dust())) {
					do_collided_rubbish(1, i);
					dust_collected ++;
					weight +=1;
			}
	}
	for ( int i = 0; i < get_num_slime(); i++ ) {
			if ( collision(get_robot_x(),get_robot_y(),get_robot_width(),get_robot_height(),char_robot(),get_slime_x(i),get_slime_y(i),get_slime_width(),get_slime_height(),char_slime())) {
					do_collided_rubbish(2,i);
					slime_collected ++;
					weight+=5;
			}
	}
	for ( int i = 0; i < get_num_trash(); i++ ) {
			if ( collision(get_robot_x(),get_robot_y(),get_robot_width(),get_robot_height(),char_robot(),get_trash_x(i),get_trash_y(i),get_trash_width(),get_trash_height(),char_trash())) {
					do_collided_rubbish(3,i);
					trash_collected ++;
					weight+=20;
			}
	}
}

//returns true if robot vacuum collides with charging station(charger2)
bool charger_collided(){
 if (collision(get_robot_x(),get_robot_y(),get_robot_width(),get_robot_height(),char_robot(),get_charger_x()-1,get_charger_y(),get_charger_width()+2,get_charger_height()+1,charger2)){
	 return true;
 }
 return false;
 // show_screen();
}

//returns true if trash overlaps trash
bool trash_trash(int i){
  for (int j = 0; j < i; j++) {
      if (collision(get_trash_x(i), get_trash_y(i), get_trash_width(), get_trash_height(), char_trash(), get_trash_x(j), get_trash_y(j), get_trash_width(), get_trash_height(), char_trash()) && j!=i) {
          return true;
      }
  }
  return false;
}
// Returns true if slime overlaps slime
bool slime_slime(int i){
  for (int j = 0; j < i; j++) {
      if (collision(get_slime_x(i), get_slime_y(i), get_slime_width(), get_slime_height(), char_slime(), get_slime_x(j), get_slime_y(j), get_slime_width(), get_slime_height(), char_slime()) && j!=i) {
          return true;
      }
  }
  return false;
}

// Returns true if dust overlaps dust
bool dust_dust(int i){
  for (int j = 0; j < i; j++) {
      if (collision(get_dust_x(i), get_dust_y(i), get_dust_width(), get_dust_height(), char_dust(), get_dust_x(j), get_dust_y(j), get_dust_width(), get_dust_height(), char_dust()) && j!=i) {
          return true;
      }
  }
  return false;
}

// // Returns true if slime overlaps trash
bool slime_trash(int i) {
    for (int k = 0; k < get_num_trash(); k++) {
        if (collision(get_slime_x(i), get_slime_y(i), get_slime_width(), get_slime_height(), char_slime(), get_trash_x(k), get_trash_y(k), get_trash_width(), get_trash_height(), char_trash())) {
            return true;
        }
    }
    return false;
}

// Returns true if dust overlaps slime
bool dust_slime(int i) {
    for (int j = 0; j < get_num_slime(); j++) {
        if (collision(get_dust_x(i), get_dust_y(i), get_dust_width(), get_dust_height(), char_dust(), get_slime_x(j), get_slime_y(j), get_slime_width(), get_slime_height(), char_slime())) {
            return true;
        }
    }
    return false;
}
// Returns true if dust overlaps trash
bool dust_trash(int i) {
    for (int j = 0; j < get_num_trash(); j++) {
        if (collision(get_dust_x(i), get_dust_y(i), get_dust_width(), get_dust_height(), char_dust(), get_trash_x(j), get_trash_y(j), get_trash_width(), get_trash_height(), char_trash())) {
            return true;
        }
    }
    return false;
}

//resets rubbish collected count
void reset_rubbish_collected(){
  dust_collected = 0;
  slime_collected = 0;
  trash_collected = 0;
}

//returns amt of dust collected
int get_dust_collected() {
	return dust_collected;
}

//returns amt of slime collected
int get_slime_collected(){
	return slime_collected;
}

//returns amt of trash collected
int get_trash_collected(){
	return trash_collected;
}

int get_weight(){
	return weight;
}

#pragma once

#include <stdbool.h>
/**
 *  Tests to see if any dust has collided with the vacuum. If so,
 *  do_collided is called to restart the simulation.
 */
void check_collision();

/**
 *  Returns true if and only if two non-space pixels overlap.
 *
 *  Parameters:
 *      (x0,y0) - real-valued coordinates of the top left corner of the first image.
 *      (w0,h0) - integer-valued dimensions of the first image.
 *      (x1,y1) - real-valued coordinates of the top left corner of the second image.
 *      (w1,h1) - integer-valued dimensions of the second image.
 */
bool collision(int x0, int y0, int w0, int h0, char pixels0[], int x1, int y1, int w1, int h1, char pixels1[]);

//return dust collected
int get_dust_collected();

//return slime collected
int get_slime_collected();

//return trash collected
int get_trash_collected();

//resets amt of rubbish collected
void reset_rubbish_collected();


//returns true if vacuum collides with charger
bool charger_collided();

//returns current robot vacuum load weight
 int get_weight();

 bool trash_trash(int i);
 bool slime_slime(int i);
 bool slime_trash(int i);

 bool dust_dust(int i);
 bool dust_slime(int i);
 bool dust_trash(int i);

#pragma once
#include <stdbool.h>
/** setup_trashs(), setup_slimes(), setup_dusts()
 *  Sets up all dust/slime/trash (0 <= i < num_dust/num_slime/num_trash) so that they are all
 *  wholly contained within the border, and none overlap with the robot. */
void setup_trashs();
void setup_slimes();
void setup_dusts();

// character pointer arrays for dust/slime/trash.
char *char_dust();
char *char_slime();
char *char_trash();

//returns amount of dust/slime/trash left
int get_dust_left();
int get_slime_left();
int get_trash_left();

//Adds a piece of rubbish to the screen based on specified coordinates
void add_dust();
void add_slime();
void add_trash();

//Draws all rubbish
void draw_all_rubbish();

/*Action taken when the vacuum has collided with a piece of rubbish.
If a rubbish has collided with the vacuum, it is transported off the screen*/
void do_collided_rubbish(int Rubbish_type,int i);

//returns number of dust/slime/trash (initial)
int get_num_dust();
int get_num_slime();
int get_num_trash();

//returns x coordinate of dust/slime/trash
double get_dust_x(int i);
double get_slime_x(int i);
double get_trash_x(int i);

//returns y coordinate of dust/slime/trash
double get_dust_y(int i);
double get_slime_y(int i);
double get_trash_y(int i);

//returns width of dust/slime/trash
int get_dust_width();
int get_slime_width();
int get_trash_width();

//returns height of dust/slime/trash
int get_dust_height();
int get_slime_height();
int get_trash_height();

#pragma once
#include <stdbool.h>

/* Setup the charging station*/
void setup_charger();
/* Setup the robot vacuum*/
void setup_robot();
/* Draw the charging station on (cx,cy)*/
void draw_charger();
/* Draw the robot vacuum on (rx,ry)*/
void draw_robot();
/***  Returns true iff and only if the supplied argument is a robot navigation control.*/
bool is_robot_ctrl( int ch );
/*  Update the robot location based on the supplied argument.*/
void do_robot_cheat();
/*  Update the location of a vacuum based on its stored velocity.*/
void update_robot();
/** Return vacuum to base */
void return_to_base();
/* returns character array for robot vacuum*/
char * char_charger();
/* returns character array for  charger*/
char * char_robot();
/**  A dialogue which allows the position of the robot to be specified directly*/
void move_robot();
/* return heading of vacuum */
int get_heading();

double get_charger_x(); //return charger x location
double get_charger_y(); //return charger y location
int get_charger_width(); //return charger width
int get_charger_height(); //return charger height
double get_robot_x(); //return robot x location
double get_robot_y(); //return robot y location
int get_robot_width(); //return robot width
int get_robot_height(); //return robot width

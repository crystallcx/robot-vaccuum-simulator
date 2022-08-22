#include <math.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include <stdlib.h>
#include "robot.h"
#include "helpers.h"
#include "collision.h"

// robot state
#define ROBOT_WIDTH 11
#define ROBOT_HEIGHT 9
#define ROBOT_SPEED 0.2
// charger state
#define CHARGER_WIDTH 9
#define CHARGER_HEIGHT 3

//global variables
static double rx, ry, cx, cy ,rdx ,rdy;
int heading;

/* character arrays for robot vacuum & charger*/
char * robot = "   @@@@@    @@@@@@@@@ @@@@@@@@@@@@@@ROBOT@@@@@@@@@@@@@@@@VACCUUM@@@@@@@@@@@@@ @@@@@@@@@    @@@@@   ";
char * charger = "##########!CHARGE##########";

/* Setup the charging station at just below the status border (in the centre)*/
void setup_charger() {
  int w, h;
  get_screen_size(&w, &h);
  cx = round(w / 2) - CHARGER_WIDTH/2;
  cy = 5;
}
/* Draw the charging station on (cx,cy)*/
void draw_charger(){
  draw_pixels(cx, cy, CHARGER_WIDTH, CHARGER_HEIGHT, charger, true);
}

/** Return vacuum to base */
void return_to_base(){
  int w, h;
  get_screen_size(&w, &h);
  double step = ROBOT_SPEED;
  double t1 = cx+CHARGER_WIDTH/2 - rx-ROBOT_WIDTH/2;
  double t2 = cy+CHARGER_HEIGHT/2 - ry ;
  double d = sqrt(t1*t1 + t2*t2);
  rdx =t1*step/d;
  rdy =t2*step/d;
 }

/**  A dialogue which allows the position of the robot to be specified directly*/
void move_robot() {
    rx = get_int( "vaccuum x?" );
    ry = get_int( "vaccuum y?" );
    int angle = get_int( "vacuum heading?" );
    rdx = ROBOT_SPEED * cos( angle * M_PI / 180 );
    rdy = ROBOT_SPEED * sin( angle * M_PI / 180 );
}

/***  Set up the robot at the centre of the terminal.
with a starting heading of 90 degrees */
void setup_robot() {
    int w, h;
    get_screen_size( &w, &h );
    rx = round(w / 2 - ROBOT_WIDTH/2);
    ry = round(h / 2 - ROBOT_HEIGHT/2);
    const double step = ROBOT_SPEED;
    rdx = 0; //cos(90) = 0
    rdy = step * 1; //sin(90) = 1
}

/***  Draw the robot on (rx, ry).*/
 void draw_robot() {
     draw_pixels(rx, ry, ROBOT_WIDTH, ROBOT_HEIGHT, robot, true);
 }

/***  Returns true iff and only if the supplied argument is a robot navigation control.*/
bool is_robot_ctrl( int ch ) {
    return ( ch == 'j' || ch == 'k' || ch == 'l' || ch == 'i' );
}

/**
 *  Update the robot location based on the supplied argument.
 *  Parameters:
 *      ch: a character code which is expected to be one of [jkli].
 *          'j' -> move left
 *          'k' -> move down
 *          'l' -> move right
 *          'i' -> move up
 */
void do_robot_cheat( int ch ) {
   //to undo user input if robot will collide with charger
    if ( ch =='i' && collision(rx,ry,ROBOT_WIDTH,ROBOT_HEIGHT,robot,cx,cy,CHARGER_WIDTH,CHARGER_HEIGHT+1,charger)) {
      ry++;
    }
    if (ch == 'l' && collision(rx,ry,ROBOT_WIDTH,ROBOT_HEIGHT,robot,cx-1,cy,CHARGER_WIDTH+1,CHARGER_HEIGHT,charger)) {
      rx--;
    }
    if (ch == 'j' && collision(rx,ry,ROBOT_WIDTH,ROBOT_HEIGHT,robot,cx,cy,CHARGER_WIDTH+1,CHARGER_HEIGHT,charger)) {
      rx++;
    }

    else if ( ch == 'j' && rx -1> 1) {//    if ( ch == 'j' && rx - ROBOT_WIDTH / 2 > 1 ) {
        rx--;
    }
    else if ( ch == 'l' && rx +ROBOT_WIDTH< screen_width() - 1 ) {//    else if ( ch == 'l' && rx + ROBOT_WIDTH / 2 < screen_width() - 2 ) {
        rx++;
    }
    else if ( ch == 'k' && ry +ROBOT_HEIGHT< screen_height()-1-2) {//    else if ( ch == 'k' && ry + ROBOT_HEIGHT / 2 < screen_height() - 2 ) {
        ry++;
    }
    else if ( ch == 'i' && ry -4 > 1 ) {//    else if ( ch == 'i' && ry - ROBOT_HEIGHT / 2 > 1 ) {
        ry--;
    }
}

/*  Update the location of a vacuum based on its stored velocity.
* Notes:
*      If the designated move would cause the robot
*  icon to overlap the border or charger then one or both of the velocity components
*  is negated, as appropriate to cause the vacuum to reflect off the wall.
*  When that happens the vacuum does not move.
*/
void update_robot() {
  int w, h;
  get_screen_size( &w, &h );
  const double step = ROBOT_SPEED;
  int new_x = round( rx + rdx );
  int new_y = round( ry + rdy );
  bool bounced = false;
        if (collision(rx,ry,ROBOT_WIDTH,ROBOT_HEIGHT,robot,cx,cy,CHARGER_WIDTH,CHARGER_HEIGHT,charger) ) {//&& !rtbmode
          ry = ry + 1;
          bounced = true;
        }
        if ((new_x < cx+CHARGER_WIDTH/2 && new_x>cx) && (new_y < 8 && new_y >5)) {
          bounced = true;
        }
        if ( new_x == 0 || new_x + ROBOT_WIDTH == screen_width()) {
        bounced = true;
        }
        if ( new_y == 5 || new_y+ ROBOT_HEIGHT +2 == screen_height()){// || (new_y == 7 && (new_x < w/2 + CHARGER_WIDTH/2 && new_x >=w/2-CHARGER_WIDTH))){// when y = 4 , bounce
        bounced = true;
    }
    if (bounced) {
      get_heading();
      int change = 30 + rand() % 31; // generates a number between 30 & 60.
      if(rand()%2==0) change*=-1; //50:50 chance of going either right or left
      heading += change;
      rdx = -step * cos(heading * M_PI/180);
      rdy = -step * sin(heading * M_PI/180);
    }
    if ( !bounced ) {
        rx += rdx;
        ry += rdy;
    }
}

//Robot heading (direction) in degrees.
int get_heading(){
  heading = atan2(rdy,rdx) * 180/M_PI;
  return heading;
}

//return robot width
int get_robot_width() {
    return ROBOT_WIDTH;
}

//return robot height
int get_robot_height() {
    return ROBOT_HEIGHT;
}

//return charger width
int get_charger_width() {
  return CHARGER_WIDTH;
}

//return charger height
int get_charger_height() {
  return CHARGER_HEIGHT;
}

//return charger x coordinate
double get_charger_x(){
  return cx;
}
//return charger y coordinate
double get_charger_y(){
  return cy;
}

//return robot x coordinate
double get_robot_x() {
    return rx;
}

//return robot y coordinate
double get_robot_y() {
    return ry;
}

//return character pointer array for robot
char * char_robot() {
  return robot;
}
//return character pointer array for charger
char * char_charger(){
  return charger;
}

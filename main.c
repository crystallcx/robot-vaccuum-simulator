#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "trash.h"
#include "helpers.h"
#include "robot.h"
#include "collision.h"

//robot unpausing when battery =100 in is_robot_ctrl mode.

// Global variables
bool game_over = false;
int delay = 10;
int timeout = INT_MAX;
double start_time,reset_time;
bool paused = false; bool stop = false; bool full_weight=false;// bool docked = false;
int weight,heading;
bool rtbmode = false;
double battery;
int w, h;

//Drawing the outside square border
 void draw_border() {
     get_screen_size(&w, &h);
     const int chv = '|';
     const int chh = '-';
     draw_line(0, 0, 0, h - 1-2, chv);
     draw_line(0, 0, w - 1, 0, chh);
     draw_line(0, h - 1-2, w - 1, h - 1-2, chh);
     draw_line(w - 1, 0, w - 1, h - 1-2, chv);
     draw_char(0, 0, '+');
     draw_char(w-1, h-1-2, '+');
     draw_char(0, h-1-2, '+');
     draw_char(w-1, 0, '+');
 }

//checks the weight & battery percentage of the vacuum
void check_values(){
  if (weight<=45) {
    full_weight=false;
  }
  if (weight>45) {
    full_weight=true;
    return_to_base();
    rtbmode = true;
  }
  if (battery < 25) {
    return_to_base();
    rtbmode = true;
  }
}

//charges battery back to 100.
void charge_battery(){
  get_screen_size(&w, &h);
  if (battery<100) {
    battery+=0.25;
    if (!paused) {
      paused = true;
    }
  }
  if (battery >= 100){
    full_weight=false;
    paused = false;
    rtbmode = false;
  }
  draw_formatted(round(w-(w/7)),1,"++");
}

//Updates battery depending on vacuum's current state.
void update_battery() {
  bool seconds = false;
   int timer = get_current_time()-reset_time;
   if (timer >= 1){
     seconds = true;
     reset_time = get_current_time();
   }
   /** The device docks with the charging station to reload its battery and unload any rubbish it has collected.
  Vacuum will only be docked while in return to base mode & is collided with charger.*/
   if (rtbmode && charger_collided()){
     draw_formatted(screen_width()/2,screen_height()-2,"DOCKED");
     if (full_weight || battery<100) {
      weight = 0;
      charge_battery();
      // double sectime=get_current_time();
     }
  }
  //battery counts down when game is not in paused state & not in collision w/ charger
   if(battery != 0 && !paused && !charger_collided() && seconds){
        battery = battery - 1;
      }
  //else if battery = 0, simulation over message is displayed.
   else if (battery == 0) stop = true;
}

//Changes the amount(weight) of the rubbish currently on board the vacuum.
 void set_robot_weight(){
   weight = get_int("New weight for the vaccuum?");
 	if (weight>65) {
 		weight = 65;
    full_weight = true;
 	}
  if (weight < 0) {
    weight = 0;
    full_weight=false;
  }
 }

 //Changes the current battery percentage of the robot vacuum.
 void set_battery(){
   battery = get_int("Battery percentage?");
   if (battery>100 || battery<0) {
     battery = 100;
   }
 }

/* draws the status display located at the top of the terminal window.
* displays robot direction, remaining battery life, elapsed time, load weight, and rubbish available.*/
void draw_status_display(){
   get_screen_size(&w, &h);
   const int chv = '|';
   const int chh = '-';
   draw_line(0,2,w-1,2,chh); draw_char(0,2,'+');
   draw_line(0,4,w-1,4,chh); draw_char(0,4,'+');
   draw_line(round(w/3),0,round(w/3),4,chv); draw_char(round(w/3),0,'+');
   draw_line(round(2*w/3),0,round(2*w/3),4,chv); draw_char(round(2*w/3),0,'+');
   draw_char(round(w/3),2,'+'); draw_char(round(w/3),4,'+');
   draw_char(round(2*w/3),2,'+'); draw_char(round(2*w/3),4,'+');
   draw_char(w-1,2,'+'); draw_char(w-1,4,'+');

   draw_formatted(round((w/3)-(2*w/9)),1,"n10203478");

   // Elapsed time since reset.
   int timer = get_current_time()-start_time;
   int seconds =timer%60;
   int minutes =timer/60;
   if (minutes<10){
     if(seconds<10) draw_formatted(round((w/3)-(2*w/9)),3,"Elapsed: 0%i:0%i",minutes,seconds);
     else draw_formatted(round((w/3)-(2*w/9)),3,"Elapsed: 0%i:%i",minutes,seconds);
   }
   else draw_formatted(round((w/3)-(2*w/9)),3,"Elapsed: %i:%i",minutes,seconds);

   // Robot vacuum direction
   get_heading();
   if (heading < 0) { //if negative, add 360 to make it
     heading += 360;
   }
   draw_formatted(round((2*w/3) -(w/4)),1,"Heading: %d",heading);

   // Robot Vacuum load weight.
   draw_formatted(round((2*w/3)-(w/4)),3,"Load weight (g): %d",get_weight());

   // Vacuum remaining battery.
   update_battery();
   draw_formatted(round(w-(w/4)),1,"Battery: %d%%",(int)battery);

   // Remaining rubbish lef ton the screen.
   draw_formatted(round(w-(w/4)),3,"Litter(d,s,l): %i,%i,%i",get_dust_left(),get_slime_left(),get_trash_left());
 }

/***  Draw all objects in the simulation.*/
void draw_all() {
    clear_screen();
    draw_border();
    draw_status_display();
    draw_charger();
    draw_all_rubbish();
    draw_robot();
    show_screen();
}

/***  Set up all objects in the simulation.*/
void setup( void ) {
    delay = 10;
    stop = false;
    weight = 0;
    paused = true;
    battery = 100; //initialise battery percentage
    start_time = get_current_time();
    reset_time = start_time;
    reset_rubbish_collected();
    update_battery();
    setup_charger();
    setup_robot();
    setup_trashs();
    setup_slimes();
    setup_dusts();
    draw_all();
}
/***  Resets the simulation.*/
void reset() {
  setup();
}

/***  Obtain a time-out value from standard input and store it for future use.*/
void do_timeout() {
    timeout = get_int( "How many seconds shall the program run?" );
    if (timeout < 0) timeout = INT_MAX; //max run time
}

/***  Returns true iff the simulation has been running for longer than the
 *  current designated time-out value.*/
bool timed_out() {
    return get_current_time() - start_time >= timeout;
}
/***  Toggles robot motion between paused and not-paused.*/
void do_pause() {
    paused = !paused;
}

/***  Dialogue which allows the loop delay to be manually overridden.*/
void do_delay_cheat() {
    delay = get_int( "New delay (milliseconds)?" );
    if ( delay < 0 ) delay = 0;
}

/* Displays message upon quitting simulation */
void quit_simulation(){
 get_int("Vacuum Simulation has finished. Press any key to exit.");
  show_screen();
  game_over = true;
  while ( get_char() > 0 ) {}
}

/* Displays simulation over message once battery reaches zero. */
void no_battery() {
    const char *message[] = {
        "Battery has depleted!",
        "Press r to restart...",
        "Press q to quit......"
    };
    const int rows = 3;
    for ( int i = 0; i < rows; i++ ) {
        int len = strlen( message[i] );
        int x = ( screen_width() - len ) / 2;
        int y = i + ( screen_height()- rows ) / 2;
        int y2 = ( screen_height()- rows ) / 2;
        draw_formatted( x, y, message[i] );

        //square border
        draw_line(x-1,y2-1,x+21,y2-1,'-'); //top
        draw_line(x-1,y2+3,x+len+1,y2+3,'-'); //bottom
        draw_line(x-1,y2-1,x-1,y2+3,'|'); //left
        draw_line(x+len+1,y2-1,x+len+1,y2+3,'|'); //right
        draw_char(x-1,y2-1,'+');
        draw_char(x+len+1,y2-1,'+');
        draw_char(x-1,y2+3,'+');
        draw_char(x+len+1,y2+3,'+');
    }
    show_screen();
    timer_pause( 1000 );
    int ch = get_char();
      if ( ch == 'r' ) reset();
      if ( ch == 'q' ) quit_simulation();

    while ( get_char() > 0 )
    while ( !timed_out() && wait_char() <= 0 ) {}
  }


/***  Dialog which displays a help screen.*/
void do_help() {
    clear_screen();
    draw_formatted_lines( screen_width()/2 - 30, screen_height()/2 - 10,
        "!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "ROBOT VACCUUM SIMULATION!\n"
        "!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "\n"
        "j, k, l, i: Move robot left, down, right, up respectively\n"
        "b: return robot vacuum to base\n"
        "d, s, t: litter cheat - drop litter"
        "r: reset simulation\n"
        "o: time out for a user specified number of seconds\n"
        "v: robot cheat - override the position and direction the robot vacuum.\n"
        "w: robot cheat - override the current load of the robot vacuum.\n"
        "y: robot cheat - override current battery % of the robot vacuum.\n"
        "p: pause or resume robot vacuum movement\n"
        "m: change the millisecond loop delay\n"
        "q: quit simulation\n"
        "?: display this help screen\n\n\n"
        "Press a key to return to the game."
    );
    show_screen();

    while ( get_char() > 0 ) {}
    while ( !timed_out() && wait_char() <= 0 ) {}
}

/**
 *  Jump table which chooses the action (if any) which corresponds to a
 *  supplied command character.
 *
 *  Parameters:
 *      ch: a command character. Currently recognised comands are listed in
 *          the do_help function.
 */
void do_operation( int ch ) {
    if ( ch == 'r' ) { //reset simulation
        reset();
    }
    else if (ch == 'b') {
      return_to_base();
      rtbmode = true;
    }
    //
    else if (ch == 'd') { //drops a piece of dust on the floor
      add_dust();
    }
    else if (ch == 's') { //drops a piece of slime on the floor
      add_slime();
    }

    else if (ch == 't'){ //drops a piece of trash on the floor
      add_trash();
    }

    else if ( is_robot_ctrl( ch ) ) {
        do_robot_cheat( ch );
    }

    else if ( ch == 'o' ) { // set timeout period
        do_timeout();
    }

    else if (ch == 'v') { //specify new location & heading of device
      move_robot();
    }
    else if (ch == 'w') {//change the amt of rubbish (weight) currently on the robot device
      set_robot_weight();
    }

    else if (ch == 'y') {//change battery level of the device
      set_battery();
    }

    else if ( ch == 'p') { //cause the device to start moving (if it is stationary) or stop moving
        do_pause();
    }
    else if ( ch == 'm' ) { //set the millisecond delay between successive invocations of the loop function.
        do_delay_cheat();
    }
    else if ( ch == '?' ) { //display help screen.
        do_help();
    }
}

/***  The loop function: called once per time slice to run the simulation.*/
void loop() {
    int ch = get_char();

    if (!full_weight){
      check_collision();
      check_values();

    }
    if (stop){
      no_battery();
      paused = true;
    }
    else if ( ch == 'q') { // quit simulation.
        quit_simulation();
        stop = true;
        game_over = true;
        return;
    }
    else if ( ch >= ' ' ) {
        do_operation( ch );
    }
    else if ( !paused) {
      update_robot();
    }
    draw_all();
}

/***  Program start and event loop.*/
int main( void ) {
    // srand(9139532); // For initial testing
    srand( get_current_time() ); // For production
    setup_screen();
    setup();
    while ( !game_over && !timed_out() ) {
        loop();
        timer_pause( delay );
    }
    return 0;
}

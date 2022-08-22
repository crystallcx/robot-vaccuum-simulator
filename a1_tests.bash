#!/bin/bash

# chmod +x a1_tests.bash
# ./zdj_tests.bash

test_num=0
REPORT_FILE=test_report.txt

echo "Remember to do these two commands before running this script:
export COLUMNS
export LINES
"
echo "Test run starting at $(date)" >>${REPORT_FILE}

#==============================================================================
#	Operations available in the Robot Vacuum simulation.
#==============================================================================
function setup_rubbish(){
	printf "${1},${2},${3} "
}

function change_weight(){
	printf "w${1} "
}

function change_battery(){
	printf "y${1} "
}

function move_vacuum () {
	printf "v${1},${2},${3} "
}

function add_dust () {
	printf "d${1},${2} "
}

function add_slime () {
	printf "s${1},${2} "
}

function add_trash () {
	printf "t${1},${2} "
}

function set_timeout () {
	printf "o${1} "
}

function return_to_base (){
	printf "b "
}

function set_delay () {
	printf "m${1} "
}

function pause_resume () {
	printf "p "
}

function reset () {
	printf "r${1}"
}

function left () {
	printf "j "
}

function right () {
	printf "l "
}

function up () {
	printf "i "
}

function down () {
	printf "k "
}

function quit () {
	printf "q${1}"
}

function loop () {
	if (( $# >= 1 )) && (( $1 > 0 ))
	then
		for (( i = 0; i < $1; i++ ))
		do
			printf " "
		done
	else
		printf " "
	fi
}

function enter () {
	printf "${1} "
}
#test docked message
#========================================================================
#	Run a single test.
#========================================================================
function run_test () {
	(( test_num ++ ))
	echo "Test    : ${test_num}"
	echo "Category: ${category}"
	echo "Details : ${details}"
	echo "Expect  : ${expect}"

	read -p "Press r to run the test or s to skip..." run_or_skip

	if [ "${run_or_skip}" == "r" ]
	then
		echo ${cmd} | ./a1_n10203478
		read -p "Did the test pass or fail (p/f)? " result
		echo "Test ${test_num}: ${result}" >>${REPORT_FILE}
	fi
}
#============================================================================== FIN
category="Setup vacuum"
details="vacuum alone in center of small terminal window. Timeout 3s."
expect="vacuum is displayed in centre; no trash is visible."

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 3
)

run_test

#============================================================================== FIN
category="Setup rubbish"
details="Vacuum and 3 rubbish. Timeout 3s."
expect="Vacuum is displayed in centre; 3 pieces of rubbish are visible (1 dust, 1 slime, 1 trash); none overlap the vacuum, charger or the border."

cmd=$(
	setup_rubbish 1 1 1
	set_timeout 3
)

run_test

#============================================================================== FIN
category="Setup rubbish"
details="Vacuum and 9 rubbish (3 dust, 3 slime, 3 trash). Timeout 3s."
expect="Vacuum is displayed in centre; 9 pieces of rubbish are visible (3 dust, 3 slime, 3 trash); none overlap the hero or the border."

cmd=$(
	setup_rubbish 3 3 3
	set_timeout 3
)

run_test

#============================================================================== FIN
category="Setup rubbish"
details="Vacuum and 15 rubbish. Timeout 5s."
expect="Vacuum is displayed in centre; 15 pieces of rubbish are visible (5 dust, 5 slime, 5 trash); none overlap the hero or the border."

cmd=$(
  setup_rubbish 5 5 5
	set_timeout 5
)

run_test

#============================================================================== FIN
category="Setup max rubbish"
details="Vacuum and maximum rubbish. Timeout 5s."
expect="Vacuum is displayed in centre; max rubbish is visible (1000 dust, 10 slime, 5 trash); none overlap the hero or the border."

cmd=$(
	setup_rubbish 5 10 1000
	set_timeout 5
)

run_test

#============================================================================== FIN
category="Do Vacuum Cheat"
details="Vacuum alone; placed in four corners of display, clockwise."
expect="Vacuum is displayed briefly in centre; no rubbish is visible; hero teleports in turn to [5,5], [w-5,5], [w-5,h-5], [5,h-5]."
(( LEFT = 1, TOP = 5, RIGHT = COLUMNS - 12, BOTTOM = LINES - 12 ))

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 6
	set_delay 500
	move_vacuum ${LEFT} ${TOP} 0
	move_vacuum ${RIGHT} ${TOP} 0
	move_vacuum ${RIGHT} ${BOTTOM} 0
	move_vacuum ${LEFT} ${BOTTOM} 0
	quit z
)

run_test

#============================================================================== FIN
category="Update Vacuum - Manual"
details="Vacuum alone; moves left."
expect="Vacuum is displayed in centre; no rubbish visible; vacuum moves left until hitting the wall."

cmd=$(
  setup_rubbish 0 0 0
	set_timeout 5
	for (( i = 0; i < COLUMNS/2; i++ )) ; do left ; done
	set_delay 1000
	quit z
)

run_test

#============================================================================== FIN
category="Update Vacuum - Manual"
details="Vacuum alone; moves right."
expect="Vacuum is displayed in centre; no rubbish is visible; Vacuum moves right until hitting the wall."

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 5
	for (( i = 0; i < COLUMNS/2; i++ )) ; do right ; done
	set_delay 1000
	quit z
)

run_test

#============================================================================== FIN
category="Update Vacuum - Manual"
details="Vacuum alone; moves north."
expect="Vacuum is displayed in centre; no rubbish is visible; Vacuum moves north until hitting the wall."

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 5
	for (( i = 0; i < LINES/2; i++ )) ; do up ; done
	set_delay 1000
	quit z
)

run_test

#============================================================================== FIN
category="Update Vacuum - Manual"
details="Vacuum alone; moves south."
expect="Hero is displayed in centre; no zombie is visible; hero runs right until hitting the wall."

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 5
	for (( i = 0; i < LINES/2; i++ )) ; do down ; done
	set_delay 1000
	quit z
)

run_test

#============================================================================== FIN
category="Update Vacuum - Manual"
details="Vacuum alone; runs left and right at top. Delay = 10."
expect="Vacuum is displayed at top left corner; no rubbish is visible; vacuum moves right and left until hitting the charger."

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	move_vacuum 5 5 0
	set_timeout 15
	for (( i = 0; i < COLUMNS /2; i++ )) ; do right ; done
	for (( i = 0; i < COLUMNS /2; i++ )) ; do left ; done
	for (( i = 0; i < COLUMNS /2; i++ )) ; do right ; done
	for (( i = 0; i < 3; i++ )) ; do down ; done
	for (( i = 0; i < 20 ; i++ )) ; do right ; done
	for (( i = 0; i < 3; i++ )) ; do up ; done
	for (( i = 0; i < COLUMNS/2 ; i++ )) ; do right ; done
	for (( i = 0; i < COLUMNS /2; i++ )) ; do left ; done
	for (( i = 0; i < COLUMNS/2 ; i++ )) ; do right ; done
	set_delay 1000
	quit z
)

run_test

#============================================================================== FIN
category="Update Vacuum - Manual"
details="Vacuum alone; runs left and right at centre. Delay = 5."
expect="Vacuum is displayed at top left corner; no rubbish is visible; vacuum moves right and left until hitting the charger."

cmd=$(
	setup_rubbish 0 0 0
	set_delay 5
	set_timeout 15
	for (( i = 0; i < COLUMNS/2; i++ )) ; do right ; done
	for (( i = 0; i < COLUMNS; i++ )) ; do left ; done
	for (( i = 0; i < COLUMNS; i++ )) ; do right ; done
	for (( i = 0; i < COLUMNS; i++ )) ; do left ; done
	set_delay 1000
	quit z
)

run_test

#============================================================================== FIN
category="Update Vacuum - Manual"
details="Vacuum alone; runs up and down at top. Delay = 10."
expect="Vacuum is displayed at top left corner; no rubbish is visible; vacuum moves up and down until hitting the charger."

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	move_vacuum 5 5 0
	set_timeout 15
	for (( i = 0; i < LINES; i++ )) ; do down ; done
	for (( i = 0; i < LINES; i++ )) ; do up ; done
	for (( i = 0; i < LINES; i++ )) ; do down ; done
	for (( i = 0; i < LINES; i++ )) ; do up ; done
	set_delay 1000
	quit z
)

run_test
#============================================================================== FIN
category="Update Vacuum - Manual"
details="Vacuum alone; runs up and down at top (In centre of screen). Delay = 10."
expect="Vacuum is displayed at top left corner; no rubbish is visible; vacuum moves up and down until hitting the charger."

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	set_timeout 15
	for (( i = 0; i < LINES ; i++ )) ; do down ; done
	for (( i = 0; i < LINES ; i++ )) ; do up ; done
	for (( i = 0; i < LINES ; i++ )) ; do down ; done
	for (( i = 0; i < LINES ; i++ )) ; do up ; done
	set_delay 1000
	quit z
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; run west and bounce. Delay = 5. Timeout = 4. Unpaused."
expect="Vacuum is displayed in centre, moves left until hitting wall, and rebounds. No rubbish visible."

(( CX = COLUMNS / 2 - 5, CY = LINES / 2 - 4, DIR = 180 ))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 5
	move_vacuum ${CX} ${CY} ${DIR}
	set_timeout 4
	pause_resume
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; run east and bounce. Delay = 5. Timeout = 4. Unpaused."
expect="Vacuum is displayed in centre, moves right until hitting wall, and rebounds. No rubbish visible."

(( CX = COLUMNS / 2 - 5, CY = LINES / 2 - 4, DIR = 0 ))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 5
	move_vacuum ${CX} ${CY} ${DIR}
	set_timeout 4
	pause_resume
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; run north and bounce. Delay = 10. Timeout = 3. Unpaused."
expect="Vacuum is displayed in centre, moves north until hitting wall, and rebounds. No rubbish visible."

(( CX = COLUMNS / 2 - 5, CY = LINES /2 - 4, DIR = 270 ))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	move_vacuum ${CX} ${CY} ${DIR}
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; run south and bounce. Delay = 10. Timeout = 3. Unpaused."
expect="Vacuum is displayed in centre, moves south until hitting wall, and rebounds. No rubbish visible."

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; run south-east and bounce. Delay = 10. Timeout = 3. Unpaused."
expect="Vacuum is displayed in centre, moves south-east until hitting wall, and rebounds. No rubbish visible."

(( CX = COLUMNS / 2 - 5, CY = LINES / 2 - 4, DIR = 45 ))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	move_vacuum ${CX} ${CY} ${DIR}
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; run north-east and bounce. Delay = 10. Timeout = 3. Unpaused."
expect="Vacuum is displayed in centre, moves north-east until hitting wall, and rebounds. No rubbish visible."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 315 ))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	move_vacuum ${CX} ${CY} ${DIR}
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; run south-west and bounce. Delay = 10. Timeout = 3. Unpaused."
expect="Vacuum is displayed in centre, moves south-west until hitting wall, and rebounds. No rubbish visible."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 135 ))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	move_vacuum ${CX} ${CY} ${DIR}
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; run north-west and bounce. Delay = 10. Timeout = 3. Unpaused."
expect="Vacuum is displayed in centre, moves north-west until hitting wall, and rebounds. No rubbish visible."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 180 + 45 ))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	move_vacuum ${CX} ${CY} ${DIR}
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Update Vacuum - Auto"
details="Vacuum alone; moves around the screen randomly. Delay = 10. Timeout = 8. Unpaused."
expect="Vacuum is initialised at center; no rubbish is visible; vacuum moves around the screen randomly. Upon hitting the wall or charger the vacuum chooses a random angle between 30 & 60  or -30 & -60 to add on to the heading."

cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	pause_resume
	set_timeout 8
)

run_test

#============================================================================== FIN
category="Do Rubbish Cheat - Dust"
details="Vacuum present; Dust is placed in four corners of display. Timeout = 4."
expect="Dust is present at all 4 corners of the display."

(( LEFT = 1, TOP = 5, RIGHT = COLUMNS - 2, BOTTOM = LINES - 4 ))

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 4
	add_dust ${LEFT} ${TOP}
	add_dust ${RIGHT} ${TOP}
	add_dust ${RIGHT} ${BOTTOM}
	add_dust ${LEFT} ${BOTTOM}
	set_delay 1000
)

run_test

#============================================================================== FIN
category="Do Rubbish Cheat - Slime"
details="Vacuum present; Slime is placed in four corners of display. Timeout = 4."
expect="Slime is present at all 4 corners of the display."

(( LEFT = 1, TOP = 5, RIGHT = COLUMNS - 2-4, BOTTOM = LINES - 4 -4))

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 4
	add_slime ${LEFT} ${TOP}
	add_slime ${RIGHT} ${TOP}
	add_slime ${RIGHT} ${BOTTOM}
	add_slime ${LEFT} ${BOTTOM}
	set_delay 1000
)

run_test

#============================================================================== FIN
category="Do Rubbish Cheat - Trash"
details="Vacuum present; Trash is placed in four corners of display. Timeout = 4."
expect="Trash is present at all 4 corners of the display."

(( LEFT = 1, TOP = 5, RIGHT = COLUMNS - 2-10, BOTTOM = LINES - 4 -5))

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 4
	add_trash ${LEFT} ${TOP}
	add_trash ${RIGHT} ${TOP}
	add_trash ${RIGHT} ${BOTTOM}
	add_trash ${LEFT} ${BOTTOM}
	set_delay 1000
)

run_test

#============================================================================== FIN
category="Collision - Check"
details="Vacuum passes just south of each piece of rubbish (trash, dust, slime). Delay = 10. Timeout = 4."
expect="Vacuum is displayed in centre; rubbish is above vacuum; Vacuum runs past rubbish, just missing."

(( VX = COLUMNS / 2 - 5, VY = LINES / 2 - 4, DIR = 0 ))
(( DX = VX + 20, DY = VY-1, SX = VX+30, SY = VY-5, TX =VX + 50, TY = VY-6))
cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	add_dust ${DX} ${DY}
	add_slime ${SX} ${SY}
	add_trash ${TX} ${TY}
	move_vacuum ${VX} ${VY} ${DIR}
	set_timeout 4
	pause_resume
)

run_test

#============================================================================== FIN
category="Collision - Check"
details="Vacuum passes just north of each piece of rubbish (trash, dust, slime). Delay = 10. Timeout = 4."
expect="Vacuum is displayed in centre; rubbish is below vacuum; Vacuum runs past rubbish, just missing."

(( VX = COLUMNS / 2 - 5, VY = LINES / 2 - 4, DIR = 0 ))
(( DX = VX + 20, DY = VY+9, SX = VX+30, SY = VY+9, TX =VX + 50, TY = VY+9))
cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	add_dust ${DX} ${DY}
	add_slime ${SX} ${SY}
	add_trash ${TX} ${TY}
	move_vacuum ${VX} ${VY} ${DIR}
	set_timeout 4
	pause_resume
)

run_test

#============================================================================== FIN
category="Collision - Check"
details="Vacuum passes just west of each piece of rubbish (trash, dust, slime). Delay = 15. Timeout = 4."
expect="Vacuum is displayed in top centre; rubbish is to the right; Vacuum runs past rubbish, just missing."

(( VX = COLUMNS / 2 - 5, VY = LINES / 2 - 4 - 10, DIR = 90 ))
(( DX = VX + 11, DY = VY+5, SX = VX+11, SY = VY+10, TX =VX + 11, TY = VY+15))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 15
	add_dust ${DX} ${DY}
	add_slime ${SX} ${SY}
	add_trash ${TX} ${TY}
	move_vacuum ${VX} ${VY} ${DIR}
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Collision - Check"
details="Vacuum passes just east of each piece of rubbish (trash, dust, slime). Delay = 15. Timeout = 3."
expect="Vacuum is displayed in top centre; rubbish is to the left; Vacuum runs past rubbish, just missing."

(( VX = COLUMNS / 2 - 5, VY = LINES / 2 - 4 - 10, DIR = 90 ))
(( DX = VX-1, DY = VY+5, SX = VX-5, SY = VY+10, TX =VX-11, TY = VY+15))

cmd=$(
	setup_rubbish 0 0 0
	set_delay 15
	add_dust ${DX} ${DY}
	add_slime ${SX} ${SY}
	add_trash ${TX} ${TY}
	move_vacuum ${VX} ${VY} ${DIR}
	set_timeout 3
	pause_resume
)

run_test
#============================================================================== FIN
category="Collision - Rubbish"
details="Vacuum clips north edge of each piece of rubbish (hit). Delay = 10. Timeout = 4."
expect="Vacuum is displayed in centre; Rubbish is below Vacuum; Vacuum runs east and collides with rubbish."

(( VX = COLUMNS / 2 - 5, VY = LINES / 2 - 4, DIR = 0 ))
(( DX = VX + 20, DY = VY+8, SX = VX+30, SY = VY+8, TX =VX + 50, TY = VY+8))
cmd=$(
	setup_rubbish 0 0 0
	set_delay 10
	add_dust ${DX} ${DY}
	add_slime ${SX} ${SY}
	add_trash ${TX} ${TY}
	move_vacuum ${VX} ${VY} ${DIR}
	set_timeout 4
	pause_resume
)

run_test

#============================================================================== FIN
category="Collision - Rubbish"
details="Vacuum clips south edge of each piece of rubbish (hit). Delay = 10. Timeout = 4."
expect="Vacuum is displayed in centre; Rubbish is above Vacuum; Vacuum runs east and collides with rubbish."


(( VX = COLUMNS / 2 - 5, VY = LINES / 2 - 4, DIR = 0 ))
(( DX = VX + 20, DY = VY, SX = VX+30, SY = VY-4, TX =VX + 50, TY = VY-5))
cmd=$(
	setup_rubbish 0 0 0
	move_vacuum ${VX} ${VY} ${DIR}
	set_delay 10
	add_dust ${DX} ${DY}
	add_slime ${SX} ${SY}
	add_trash ${TX} ${TY}
	set_timeout 4
	pause_resume
)

run_test

#============================================================================== FIN
category="Collision - Rubbish"
details="Zombie clips east edge of each piece of rubbish (hit). Delay = 15. Timeout = 3."
expect="Rubbish is to the right of Vacuum; Vacuum runs south and collides with rubbish."

(( VX = COLUMNS / 2 - 5, VY = LINES / 2 - 4 - 10, DIR = 90 ))
(( DX = VX, DY = VY+12, SX = VX-4, SY = VY+15, TX =VX-10, TY = VY+25))

cmd=$(
	setup_rubbish 0 0 0
	move_vacuum ${VX} ${VY} ${DIR}
	set_delay 15
	add_dust ${DX} ${DY}
	add_slime ${SX} ${SY}
	add_trash ${TX} ${TY}
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Collision - Rubbish"
details="Vacuum clips west edge of each piece of rubbish (hit). Delay = 15. Timeout = 3."
expect="Rubbish is to the left of Vacuum; Vacuum runs south and collides with rubbish."

(( VX = COLUMNS / 2 - 5, VY = LINES / 2 - 4 - 10, DIR = 90 ))
(( DX = VX + 10, DY = VY+12, SX = VX+10, SY = VY+15, TX =VX + 10, TY = VY+25))

cmd=$(
	setup_rubbish 0 0 0
	move_vacuum ${VX} ${VY} ${DIR}
	set_delay 15
	add_dust ${DX} ${DY}
	add_slime ${SX} ${SY}
	add_trash ${TX} ${TY}
	set_timeout 3
	pause_resume
)

run_test

#============================================================================== FIN
category="Checking pixel collision"
details="Vacuum moves around a piece of dust. Delay = 200."
expect="Vacuum is displayed in centre; Dust is to the right; Vacuum moves aorund dust"

((VX = COLUMNS / 2, VY = LINES / 2))
((DX = VX + 6, DY = VY))

cmd=$(
	setup_rubbish 0 0 0
	add_dust ${DX} ${DY}
	set_delay 200
	for (( i = 0; i < 3; i++ )) ; do up ; done
	right
	up
	for (( i = 0; i < 2; i++ )) ; do right ; done
	up
	for (( i = 0; i < 6; i++ )) ; do right ; done
	down
	for (( i = 0; i < 2; i++ )) ; do right ; done
	down
	right
	for (( i = 0; i < 6; i++ )) ; do down ; done
	left
	down
	for (( i = 0; i < 2; i++ )) ; do left ; done
	down
	for (( i = 0; i < 6; i++ )) ; do left ; done
	up
	for (( i = 0; i < 2; i++ )) ; do left ; done
	up
	left
	for (( i = 0; i < 4; i++ )) ; do up ; done
	quit
)

run_test

#============================================================================== FIN
category="Checking pixel collision - Slime"
details="Vacuum moves around a piece of slime. Delay = 200."
expect="Hero is displayed in centre; Zombie is above hero and to the right; Zombie runs west and collides with hero."

((VX = COLUMNS / 2, VY = LINES / 2))
((SX = VX + 6, SY = VY-2))

cmd=$(
	setup_rubbish 0 0 0
	add_slime ${SX} ${SY}
	set_delay 200
	for (( i = 0; i < 4; i++ )) ; do up ; done
	right
	up
	right
	up
	for (( i = 0; i < 2; i++ )) ; do right ; done
	up
	for (( i = 0; i < 8; i++ )) ; do right ; done
  down
	for (( i = 0; i < 2; i++ )) ; do right ; done
  down
	right
	down
	right
	for (( i = 0; i < 8; i++ )) ; do down ; done
	left
	down
	left
	down
	for (( i = 0; i < 2; i++ )) ; do left ; done
 	down
	for (( i = 0; i < 8; i++ )) ; do left ; done
 	up
	for (( i = 0; i < 2; i++ )) ; do left ; done
 	up
	left
	up
	left
	for (( i = 0; i < 5; i++ )) ; do up ; done
	quit
)

run_test

#============================================================================== FIN
category="Checking pixel collision - Trash"
details="Vacuum moves around a piece of trash. Delay = 200."
expect="Hero is displayed in centre; Zombie is above hero and to the right; Zombie runs west and collides with hero."

((VX = COLUMNS / 2, VY = LINES / 2))
((SX = VX + 5, SY = VY-2))

cmd=$(
	setup_rubbish 0 0 0
	add_trash ${SX} ${SY}
	set_delay 200

	up
	right
	up
	right
	up
	right
	up
	right
	up
	right
	up
	for (( i = 0; i < 2; i++ )) ; do right ; done
	up
	for (( i = 0; i < 6; i++ )) ; do right ; done
	down
	for (( i = 0; i < 2; i++ )) ; do right ; done
	down
	right
	down
	right
	down
	right
	down
	right
	down
	right
	down
	right
	down
	for (( i = 0; i < 5; i++ )) ; do down ; done
	left
	down
	for (( i = 0; i < 2; i++ )) ; do left ; done
	down
	for (( i = 0; i < 16; i++ )) ; do left ; done
	up
	for (( i = 0; i < 2; i++ )) ; do left ; done
	up
	left up
	for (( i = 0; i < 5; i++ )) ; do up ; done
	right
	quit
)

run_test

#============================================================================== FIN
category="Vacuum Capacity - Trash"
details="Vacuum collects 3 pieces of trash. Each trash has a mass of 20 grams. Delay = 10. Timeout = 7."
expect="Vacuum is displayed in centre(left); 5 pieces of trash are to the right; Vacuum runs east and collects the trash. Resulting vacuum capacity is 60 grams. Trash count decreases once a trash has been picked up."

((Y0 = LINES /2))
((CX = 10, CY = LINES / 2 - 4, DIR = 0 ))

cmd=$(
	setup_rubbish 0 0 0
	move_vacuum ${CX} ${CY} ${DIR}
	set_delay 300
	for (( x = 30; x < 80; x=x+20 )) ; do add_trash ${x} ${Y0} ; done
	set_timeout 7
	set_delay 10
	pause_resume
)

run_test

#============================================================================== FIN
category="Vacuum Capacity - Slime"
details="Vacuum collects 10 pieces of slime. Each slime has a mass of 5 grams. Delay = 10. Timeout = 10."
expect="Vacuum is displayed in centre(left); 10 pieces of slime are to the right; Vacuum runs east and collects the slime. Resulting vacuum capacity is 50 grams. Slime count decreases once a slime has been picked up."

((Y0 = LINES /2))
((CX = 10, CY = LINES / 2 - 4, DIR = 0 ))

cmd=$(
	setup_rubbish 0 0 0
	move_vacuum ${CX} ${CY} ${DIR}
	set_delay 300
	for (( x = 30; x < 130; x=x+10 )) ; do add_slime ${x} ${Y0} ; done
	set_timeout 10
	set_delay 10
	pause_resume
)

run_test

#============================================================================== FIN
category="Vacuum Capacity - Dust"
details="Vacuum collects 5 pieces of dust. Each dust has a mass of 1 gram. Delay = 10. Timeout = 10."
expect="Vacuum is displayed in centre(left); 10 pieces of dust are to the right; Vacuum runs east and collects the dust. Resulting vacuum capacity is 10 grams. Dust count decreases once a dust has been picked up."

((Y0 = LINES /2))
((CX = 10, CY = LINES / 2 - 4, DIR = 0 ))


cmd=$(
	setup_rubbish 0 0 0
	move_vacuum ${CX} ${CY} ${DIR}
	set_delay 300
	for (( x = 30; x < 130; x=x+10 )) ; do add_dust ${x} ${Y0} ; done
	set_timeout 13
	set_delay 10
	pause_resume
)

run_test

#============================================================================== FIN
category="Return to Base Mode - Docked Message"
details="Vacuum alone; moves north & collides with charger (in return to base mode). 'Docked' message is shown."
expect="Vacuum is displayed in centre; no rubbish is visible; Vacuum moves north until hitting the charger. Docked message is displayed."

cmd=$(
	setup_rubbish 0 0 0
	set_timeout 5
	return_to_base
	for (( i = 0; i < LINES/2; i++ )) ; do up ; done
	set_delay 1000
	quit z
)

run_test

#============================================================================== FIN
category="Return to Base Mode - manual"
details="Return to base mode is activated once 'b' key is pressed. Delay = 20. Timeout = 3."
expect="Robot vacuum goes straight to the charging station to charge back to 100% battery. Once fully charged, the vacuum undocks."

cmd=$(
	setup_rubbish 0 0 0
	return_to_base
	pause_resume
	set_delay 15
	set_timeout 3
)

run_test

#============================================================================== FIN
category="Return to Base Mode - low battery"
details="Return to base mode is activated once battery drops below 25% or weight is larger than 45. Battery is set to 27%. Delay = 8. Timeout = 10."
expect="If robot vacuum has less than 25% battery, it goes straight to the charging station to charge back to 100% battery. Once fully charged, the vacuum undocks."

cmd=$(
	setup_rubbish 0 0 0
	change_battery 27
	set_delay 8
	set_timeout 10
	pause_resume
)

run_test

#============================================================================== FIN
category="Return to Base Mode - full weight"
details="Return to base mode is activated once battery drops below 25% or weight is larger than 45. Weight is set to 46g, battery set to 90%. Delay = 15. Timeout = 5."
expect="If robot vacuum holds more than 45g, the vacuum heads straight towards the charging station to empty the weight and charge to 100%. Once fully charged, the vacuum undocks."

cmd=$(
	setup_rubbish 0 0 0
	change_battery 90
	change_weight 46
	set_delay 15
	set_timeout 5
	pause_resume
)

run_test

#============================================================================== FIN
category="Vacuum simulation -  rubbish"
details="Vacuum with 100 dust, 10 slime, 5 trash is present; paused mode is released. Delay = 5. Timeout = 15s."
expect="Vacuum is initialised at center; max rubbish is visible; vacuum moves around the screen. Upon hitting the wall or charger the vacuum rebounds. The vacuum returns to the charger every time the weight exceeds 45g."

cmd=$(
	setup_rubbish 5 10 1000
	set_delay 5
	pause_resume
	set_timeout 15
)

run_test

#============================================================================== FIN
category="Vacuum simulation - max rubbish"
details="Vacuum with max rubbish present; paused mode is released. Delay = 5. Timeout = 15s."
expect="Vacuum is initialised at center; max rubbish is visible; vacuum moves around the screen. Upon hitting the wall or charger the vacuum rebounds. The vacuum returns to the charger every time the weight exceeds 45g."

cmd=$(
	setup_rubbish 5 10 1000
	set_delay 5
	pause_resume
	set_timeout 15
)

run_test

#==============================================================================
category="End of Simulation - No Battery"
details="Simulation is over once battery reaches 0. Delay = 30. Timeout = 5."
expect="A simulation over message is displayed, prompting user to either reset or quit the simulation."

cmd=$(
	setup_rubbish 0 0 0
	change_battery 1
	set_delay 30
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Quit simulation"
details="Simulation displays a farewell message once 'q' is pressed."
expect="A simulation over message is displayed, prompting user to enter any key to quit."

cmd=$(
	setup_rubbish 0 0 0
	set_delay 1000
	quit z
	set_timeout 5
)

run_test

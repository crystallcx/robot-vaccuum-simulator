# robot-vaccuum-simulator
 computer program that simulates a robotic vacuum cleaner operating in a simple virtual environment. 


## Troubleshooting
- /usr/bin/ld: cannot find -lncurses: No such file or directory
`sudo apt-get install libncurses-dev`

- /usr/bin/ld: cannot find Scrt1.o: No such file or directory and /usr/bin/ld: cannot find crti.o: No such file or directory:
`sudo apt-get install libc6-dev-i386`
- /usr/bin/ld: cannot find libncurses.so.6: No such file or directory:
`sudo apt-get install libncurses5-dev`
`sudo apt-get install libtinfo-dev`
---
Need:
1. Unix shell: this is provided by default in Mac / Linux, in Windows we will use Cygwin to emulate this.
2. Text editor
3. Standard set of C compiler tools:  the following is required:
    - gcc compiler
    - make
    - libncurses-devel (ncurses developer library)

For the second half of the semester (microcontrollers), you will need:
- Compiler for AVR microcontrollers: avr-gcc is used for this
- Serial Port Terminal: (recommendations will be decided later in - the semester...)
- USB Serial Drivers: setup instructions available from https://www.- pjrc.com/teensy/usb_serial.html

---
https://blackboard.qut.edu.au/bbcswebdav/pid-7904345-dt-content-rid-23334445_1/courses/CAB202_19se1/CAB202_Topic_1_Notes.html#intro

#### Compilation
C is a compiled language, meaning the text documents you write are converted through several stages to finally become
the executable that can be run on your system or device. The above program, while complete, will not run without going
through these conversion stages.
To compile the program, navigate to the directory the .c file is stored in in a command line window, then use the following
command
`gcc helloWorld.c -o helloWorld`

# Additional Resources

https://www.learn-c.org/ \
https://www.programiz.com/c-programming \
https://en.wikibooks.org/wiki/C_Programming 


#### todo:
try setup ci for cygwin
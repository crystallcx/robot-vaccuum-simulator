OBJ= main.o helpers.o robot.o collision.o trash.o
CC_FLAGS = -std=gnu99 -I./ZDK -Wall -Werror -g
LD_FLAGS = -L./ZDK -lzdk -lncurses -lm

a1_n10203478: $(OBJ)
	gcc $(OBJ) $(LD_FLAGS) -o $@

%.o : %.c
	gcc -c $< $(CC_FLAGS)

clean:
	for f in $(OBJ); do \
		if [ -f $${f} ]; then rm $${f}; fi; \
		if [ -f $${f}.exe ]; then rm $${f}.exe; fi; \
	done

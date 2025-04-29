CFLAGS = -std=c11 -g -Wall --pedantic -Wshadow -Wvla -Wextra -Wunreachable-code
OBJS = main.o hw14.o util.o
HEADERS = hw14.h
APP = layout

$(APP): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(APP)

render: libbmp render.o libbmp/libbmp.o
	$(CC) $(CFLAGS) render.o libbmp/libbmp.o -o render

libbmp:
	git clone git@github.com:marc-q/libbmp.git

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	/bin/rm -rf **/*.o
	/bin/rm -rf *.o
	/bin/rm -rf $(APP)
	/bin/rm -rf render

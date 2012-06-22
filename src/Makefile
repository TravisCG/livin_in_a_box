CC = gcc
CFLAGS = -Wall -W -pedantic `pkg-config --cflags gtk+-3.0`
CLIBS = -L/usr/lib/nvidia-current-updates -lGL -lX11 -lXrandr -lm -lpng12 -lvorbisfile -lasound `pkg-config --libs gtk+-3.0`

DEMO = livin

$(DEMO): main.o texture.o mesh3d.o shader.o cg_music.o cg_x11.o cg_dialog.o matrix.o cg_time.o depthvideo.o face.o outline.o squares.o explosion.o tridlist.o bigcube.o $(DEMO).o
	$(CC) *.o $(CLIBS) -o $(DEMO)
	mv $(DEMO) ../

# Common demo stuff
main.o: main.c
	$(CC) main.c -c
texture.o: texture.c texture.h
	$(CC) $(CFLAGS) texture.c -c
mesh3d.o: mesh3d.c mesh3d.h
	$(CC) $(CFLAGS) mesh3d.c -c
shader.o: shader.c shader.h
	$(CC) $(CFLAGS) shader.c -c
cg_music.o: cg_music.c cg_music.h
	$(CC) $(CFLAGS) cg_music.c -c
cg_x11.o: cg_x11.c cg_x11.h
	$(CC) $(CFLAGS) cg_x11.c -c
cg_dialog.o: cg_dialog.c cg_dialog.h
	$(CC) $(CFLAGS) cg_dialog.c -c
cg_time.o: cg_time.c cg_time.h
	$(CC) $(CFLAGS) cg_time.c -c
matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) matrix.c -c
face.o: face.c face.h
	$(CC) $(CFLAGS) face.c -c
depthvideo.o: depthvideo.c depthvideo.h
	$(CC) $(CFLAGS) depthvideo.c -c
outline.o: outline.c outline.h
	$(CC) $(CFLAGS) outline.c -c
tridlist.o: tridlist.c tridlist.h
	$(CC) $(CFLAGS) tridlist.c -c
squares.o: squares.c squares.h
	$(CC) $(CFLAGS) squares.c -c
explosion.o: explosion.c explosion.h
	$(CC) $(CFLAGS) explosion.c -c
bigcube.o: bigcube.c bigcube.h
	$(CC) $(CFLAGS) bigcube.c -c
$(DEMO).o: mesh3d.h texture.h $(DEMO).c $(DEMO).h
	$(CC) $(CFLAGS) $(DEMO).c -c

clean:
	@echo Cleaning up...
	@rm -f *.o *~ core* 
	@echo Done.

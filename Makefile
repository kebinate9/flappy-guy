CC = gcc

CFLAGS = -Wall -Wextra -std=c99 \
         -I/opt/homebrew/opt/raylib/include

LDFLAGS = -L/opt/homebrew/opt/raylib/lib

LIBS = -lraylib \
       -framework OpenGL \
       -framework Cocoa \
       -framework IOKit \
       -framework CoreVideo

app: flappy_guy.c
	$(CC) flappy_guy.c -o app $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f app

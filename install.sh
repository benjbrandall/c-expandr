rm expandr
gcc -o expandr expand.c `pkg-config --libs --cflags gtk+-2.0`

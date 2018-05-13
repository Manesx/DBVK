PREFIX = '/usr/'
LIBS = 'lib/'
libdbvk.so:	*.o
	gcc -shared $^ -o$@
*.o:	*.c
	gcc -fPIC -c $^ -Iinclude
install:	*.so
	cp -R $^ ${PREFIX)$(LIBS)
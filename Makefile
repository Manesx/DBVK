libdbvk.so:	*.o
	gcc -shared $^ -o$@
*.o:	*.c
	gcc -fPIC -c $^ -Iinclude
install:
	cp -R '*.so' '/usr/lib'
	cd 'include'
	cp -R '*.h' '/usr/include'

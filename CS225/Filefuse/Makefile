PRG=gcc0.exe

GCC=g++
GCCFLAGS=-Wall -Wextra -ansi -pedantic -Wunused
DRIVER0=driver.c
DRIVER1=driver.c

VALGRIND_OPTIONS=-q --leak-check=full

OSTYPE := $(shell uname)
ifeq (,$(findstring CYGWIN,$(OSTYPE)))
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif


all: # used by online testing
	@echo Compile with C and C++
	gcc -c fuse.c $(GCCFLAGS)
	g++ -c driver.c $(GCCFLAGS)
	g++ -o $(PRG) fuse.o driver.o $(GCCFLAGS)

gcc_c:
	echo Compile all with C
	gcc -c fuse.c -Wall -Wextra -ansi -pedantic
	gcc -c driver.c -Wall -Wextra -ansi -pedantic
	gcc -o $@.exe fuse.o driver.o -Wall -Wextra -ansi -pedantic

gcc_c_cpp:
	echo Compile with C and C++
	gcc -c fuse.c -Wall -Wextra -ansi -pedantic
	g++ -c driver.c -Wall -Wextra  -ansi -pedantic
	g++ -o $@.exe fuse.o driver.o $(CYGWIN) -Wall -Wextra  -ansi -pedantic

gcc_cpp:
	echo Compile all with C++
	g++ -c fuse.c -Wall -Wextra -ansi -pedantic
	g++ -c driver.c -Wall -Wextra -ansi -pedantic
	g++ -o $@.exe fuse.o driver.o $(CYGWIN) -Wall -Wextra -ansi -pedantic

0 1 2 3 4 5:
	watchdog 300 ./$(PRG) $@ | grep "+++" > studentout$@
	diff studentout$@ out$@ >difference$@

mem0 mem1 mem2 mem3 mem4:
	watchdog 3000 valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2> difference$@

clean:
	rm -f *.exe *.o  *student*

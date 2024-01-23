
GCC=g++
GCCFLAGS=-O2 -Wall -Wextra -std=c++11 -pedantic -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder

OBJECTS0=
DRIVER0=driver.cpp

VALGRIND_OPTIONS=-q --leak-check=full

DIFF_OPTIONS=--strip-trailing-cr --ignore-trailing-space

OSTYPE := $(shell uname)
ifeq (,$(findstring CYGWIN,$(OSTYPE)))
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif


gcc0:
	$(GCC) -o $(PRG) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) 
0 1 3 4 5 6:
	@echo "running test$@"
	watchdog 500 ./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@ 2>&1
2:
	@echo "running test$@"
	watchdog 2000 ./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@ 2>&1
mem0 mem1 mem2 mem3 mem4 mem5 mem6:
	@echo "running memory test $@"
	watchdog 2500 valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@
clean:
	rm -f *.exe *.tds *.o *.obj *manifest*

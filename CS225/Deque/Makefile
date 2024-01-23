PRG=gnu.exe

GCC=g++
GCCFLAGS=-O2 -Wall -Wextra -std=c++11 -pedantic -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder

OBJECTS0=mydeque.cpp
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
0 1 2 3 4 5 6 7 8 9     11 12 13 14 15 16    18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39:
	@echo "running test$@"
	@echo "should run in less than 500 ms"
	./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@ 2>&1
10: # timed !!!!
	@echo "running test$@"
	@echo "should run in less than 1800 ms"
	./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@ 2>&1
17: # timed !!!!
	@echo "running test$@"
	@echo "should run in less than 1300 ms"
	./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@ 2>&1
mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 mem9 mem10 mem11 mem12 mem13 mem14 mem15 mem16 mem17 mem18 mem19 mem20 mem21 mem22 mem23 mem24 mem25 mem26 mem27 mem28 mem29 mem30 mem31 mem32 mem33 mem34 mem35 mem36 mem37 mem38 mem39:
	@echo "running memory test $@"
	@echo "should run in less than 8000 ms"
	valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@
	@echo "lines after this are memory errors"; cat difference$@
clean:
	rm -f *.exe *.tds *.o *.obj *manifest*

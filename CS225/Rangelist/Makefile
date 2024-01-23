PRG=gcc0.exe
GCC=gcc

GCCFLAGS=-ansi -Wall -Wextra -pedantic -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -O2

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

OBJECTS0=rangelist.c
DRIVER0=driver.c

gcc0:
	$(GCC) -o $(PRG) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) 
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16:
	echo "running test$@"
	./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@
mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 mem9 mem10 mem11 mem12 mem13 mem14 mem15 mem16:
	echo "running memory test $@"
	valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@
drm0 drm1 drm2 drm3 drm4 drm5 drm6 drm7 drm8 drm9 drm10 drm11 drm12 drm13 drm14 drm15 drm16:
	@echo "running memory test $@"
	drmemory -batch -no_summary --  ./$(PRG) $(subst drm,,$@) 1>/dev/null 2>difference$@
clean:
	rm -f *.exe *.tds *.o *.obj *manifest*

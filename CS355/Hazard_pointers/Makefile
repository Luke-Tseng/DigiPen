PRG=gnu.exe
GCC=g++
GCCFLAGS=-O3 -Wall -Wextra -std=c++17 -pedantic -Weffc++ 

MSC=cl
MSCFLAGS=/EHa /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE /D"_SECURE_SCL 0" /O2i /GL

VALGRIND_OPTIONS=-q --leak-check=full
DIFFLAGS=--strip-trailing-cr -y --suppress-common-lines 

OBJECTS0=HPRecType.cpp
DRIVER0=driver.cpp
DRIVER_D= driver.cpp

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS)
msc0:
	$(MSC) /Fe$@.exe           $(DRIVER0) $(OBJECTS0) $(MSCFLAGS)
gcc_debug:
	$(GCC) -o $@.exe $(CYGWIN) $(DRIVER1) $(OBJECTS0) $(GCCFLAGS)
msc_debug:
	$(MSC) /Fe$@.exe           $(DRIVER1) $(OBJECTS0) $(MSCFLAGS)

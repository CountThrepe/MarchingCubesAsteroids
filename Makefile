# Project
EXE=final

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lglew32 -lSDL2 -lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lGLU -lGL -lSDL2 -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

asteroids.o: asteroids.cpp Point.hpp CSCIx229.h Mesh.hpp PointArray.hpp TextureGenerator.hpp
PointArray.o: PointArray.cpp Point.hpp OpenSimplexNoise.hpp TextureGenerator.hpp
TextureGenerator.o: TextureGenerator.cpp OpenSimplexNoise.hpp
Mesh.o: Mesh.cpp
Object.o: Object.cpp
Point.o: Point.cpp Object.hpp Mesh.hpp
errcheck.o: errcheck.c CSCIx229.h
fatal.o: fatal.c CSCIx229.h
loadtexbmp.o: loadtexbmp.c CSCIx229.h
print-dl.o: print-dl.c CSCIx229.h
OpenSimplexNoise.o: OpenSimplexNoise.cpp

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

#  Link
final:asteroids.o fatal.o loadtexbmp.o errcheck.o print-dl.o Object.o Point.o Mesh.o PointArray.o TextureGenerator.o OpenSimplexNoise.o
	g++ -O3 -o $@ $^ $(LIBS)

#  Clean
clean:
	$(CLEAN)

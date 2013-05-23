# Makefile para o jogo PEGA!
CXX=g++
CXXFLAGS=-Wall -O3 -march=native
LDFLAGS=-lSDL_net -lSDL_mixer `pkg-config --cflags --libs sdl ftgl` `python2-config --libs` `python2-config --cflags`
COMPILE=$(CXX) $(CXXFLAGS)

SRCDIR=src/
SRCDIRAUX=srcAux/
OBJDIR=obj/

SOURCES=$(SRCDIRAUX)listaC.cpp\
	$(SRCDIRAUX)agua.cpp\
	$(SRCDIRAUX)ar.cpp\
	$(SRCDIRAUX)ModelType.cpp\
	$(SRCDIRAUX)fogo.cpp\
	$(SRCDIR)mapa.cpp\
	$(SRCDIR)jogo.cpp\
	$(SRCDIR)rede.cpp\
	$(SRCDIR)entidades.cpp\
	$(SRCDIR)globais.cpp\

HEADERS=$(SRCDIRAUX)listaC.h\
	$(SRCDIRAUX)agua.h\
	$(SRCDIRAUX)ar.h\
	$(SRCDIRAUX)ModelType.h\
	$(SRCDIRAUX)fogo.h\
	$(SRCDIR)pega.h

OBJECTS=$(OBJDIR)mapa.o\
	$(OBJDIR)jogo.o\
	$(OBJDIR)rede.o\
	$(OBJDIR)entidades.o\
	$(OBJDIR)globais.o\
	
OBJECTSAUX=$(OBJDIR)listaC.o\
	$(OBJDIR)agua.o\
	$(OBJDIR)ar.o\
	$(OBJDIR)ModelType.o\
	$(OBJDIR)fogo.o\

EXECUTABLE=pega

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTSAUX) $(OBJECTS)
	$(COMPILE) $(OBJECTS) $(OBJECTSAUX) -o $@ $(LDFLAGS)

$(OBJDIR)%.o: $(SRCDIRAUX)%.cpp $(SRCDIRAUX)%.h
	$(COMPILE) $(SRCDIRAUX)$*.cpp -c -o $@ $(LDFLAGS)
	
$(OBJDIR)%.o: $(SRCDIR)%.cpp $(SRCDIR)pega.h
	$(COMPILE) $(SRCDIR)$*.cpp -c -o $@ $(LDFLAGS)

%.cpp:
	

%.h:
	

Debug: $(EXECUTABLE)

cleanDebug:
	make clean; make all

#Limpa os arquivos. O "-" antes do rm faz com q o make não pare caso rm retorne algum erro.
clean:
	-rm -f obj/*.o pega

cleanRun:
	make clean; make all;./pega;

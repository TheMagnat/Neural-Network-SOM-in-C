# Makefile Generique
 
EXT = c
CXX = gcc
EXEC = exe
 
CXXFLAGS = -Wall -W -O3
LDFLAGS = 
 
 
OBJDIR = obj
SRC = $(wildcard *.$(EXT))
HEAD = $(wildcard *.h)
OBJ = $(SRC:.$(EXT)=.o)
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))
 
all: makedir $(EXEC)

makedir:
	mkdir -p obj

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.$(EXT)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

createlib:
	mkdir -p lib
	gcc -shared -o lib/libneural.so $(OBJDIR)/vector.o $(OBJDIR)/learningStruct.o $(OBJDIR)/list.o $(OBJDIR)/neuralStruct.o
	mkdir -p lib/Neural
	cp $(HEAD) lib/Neural

clean:
	rm -rf $(OBJDIR)/*.o

cleanall:
	rm -rf $(OBJDIR)/*.o exe lib/Neural/*.h lib/libneural.so

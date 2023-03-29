CXX=g++

IDIR=include
CFLAGS=-I$(IDIR) -I/home/samm/miniconda3/include -I/home/samm/projects/fastmatrix/fastmatrix/include/gzstream

#_DEPS = Eigen/Dense fastmatrix.h 
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = fastmatrix.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

SRC=src

BIN=bin

ODIR=obj

$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

$(BIN)/fastmatrix: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BIN)/*

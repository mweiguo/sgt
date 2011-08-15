BIN    = sgrender.so
OBJS   = sgr_lc.o sgr_node2lc.o sgr_nodes.o sgr_state.o sgr_stateset.o sgr_statesetbuilder.o
SRC    = $(OBJS:%.o=%.cpp)
INC    = $(OBJS:%.o=%.h)
CXX     = g++
CXXFLAGS = -c -g
LINKOPT = -shared -L"../build/lib" -L"/c/Qt/2010.02.1/mingw/lib" -lglew32.dll -lopengl32 -lglu32
INCPATH = -I"../graphbase" -I"../build/include"

all : $(BIN)

$(BIN) : $(OBJS)
	$(CXX) $(OBJS) -o $(BIN) $(LINKOPT)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

clean :
	rm -rf *.o
	rm -rf *.so

install : install.include install.bin

install.include :
	cp sgr_lc.h ../build/include

install.bin :
	cp *.so ../build/bin


TESTS  = test_nodes test_lc2kdtree test_node2lc test_vfculler test_statesetbuilder
BIN    = librender2d.so
OBJS   = sgr_nodes.o test_nodes.o 
SRC    = $(OBJS:%.o=%.cpp)
INC    = $(OBJS:%.o=%.h)
CXX     = g++
CXXFLAGS = -c -g
LINKOPT = -lopengl32 -lglu32 -lfreetype -L"../../thirdparty/DevIL-SDK-x86-1.7.8/lib" -lDevIL -lILUT
SHARELINKOPT = -shared  -lopengl32 -lglu32 -lfreetype -L"../../thirdparty/DevIL-SDK-x86-1.7.8/lib" -lDevIL -lILUT
INCPATH = -I"../../graphbase" -I"." -I"../../thirdparty/utf8_v2_3_1/source" -I"../../thirdparty/DevIL-SDK-x86-1.7.8/include"

all : $(TESTS) $(BIN)


# all TESTS
test_nodes : sgr_nodes.o test_nodes.o
	$(CXX) sgr_nodes.o test_nodes.o -o $@ $(LINKOPT)

test_lc2kdtree : sgr_lc.o test_lc2kdtree.o sgr_lc2kdtree.o sgr_font.o sgr_texture.o
	$(CXX) sgr_lc.o test_lc2kdtree.o sgr_lc2kdtree.o sgr_font.o sgr_texture.o -o $@ $(LINKOPT)

test_node2lc : sgr_nodes.o test_node2lc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o
	$(CXX) sgr_nodes.o test_node2lc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o -o $@ $(LINKOPT)

test_vfculler : sgr_lc.o test_vfculler.o sgr_vfculler.o ../../graphbase/sgr_math.o sgr_bboxupdater.o sgr_lcreport.o sgr_font.o sgr_texture.o
	$(CXX) sgr_lc.o test_vfculler.o sgr_vfculler.o ../../graphbase/sgr_math.o sgr_bboxupdater.o sgr_lcreport.o sgr_font.o sgr_texture.o -o $@ $(LINKOPT)

test_statesetbuilder : sgr_lc.o sgr_stateset.o sgr_statesetbuilder.o test_statesetbuilder.o sgr_vfculler.o sgr_lcreport.o ../../graphbase/sgr_math.o sgr_font.o sgr_bboxupdater.o sgr_texture.o
	$(CXX) sgr_lc.o sgr_stateset.o sgr_statesetbuilder.o test_statesetbuilder.o sgr_vfculler.o sgr_lcreport.o ../../graphbase/sgr_math.o sgr_font.o sgr_bboxupdater.o sgr_texture.o -o $@ $(LINKOPT)

# all bin
#librender2d.a : sgr_render2d.o sgr_lc.o sgr_vfculler.o sgr_statesetbuilder.o ../../graphbase/sgr_math.o sgr_stateset.o
#	ar cq $@ sgr_render2d.o sgr_lc.o sgr_vfculler.o sgr_statesetbuilder.o ../../graphbase/sgr_math.o sgr_stateset.o

librender2d.so : sgr_render2d.o sgr_lc.o sgr_vfculler.o sgr_statesetbuilder.o ../../graphbase/sgr_math.o sgr_stateset.o sgr_renderer.o sgr_font.o sgr_texture.o
	$(CXX) sgr_render2d.o sgr_lc.o sgr_vfculler.o sgr_statesetbuilder.o ../../graphbase/sgr_math.o sgr_stateset.o sgr_renderer.o sgr_font.o sgr_texture.o -o $@ $(SHARELINKOPT)

# all src
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ $<

clean :
	rm -rf *.o *.so *.a *.exe *.idx *~ *.xml *.slc

install : install.bin

install.bin : all
	cp *.exe ../../build/bin
	cp *.so ../../build/bin

run : all
	./test_nodes test_nodes.xml
	./test_node2lc geos.slc
	./test_vfculler geos.slc
	./test_statesetbuilder geos.slc ss.xml
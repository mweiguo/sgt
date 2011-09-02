TESTS   = test_nodes test_lc2kdtree test_node2lc test_vfculler test_statesetbuilder
OBJS   = sgr_nodes.o test_nodes.o 
SRC    = $(OBJS:%.o=%.cpp)
INC    = $(OBJS:%.o=%.h)
CXX     = g++
CXXFLAGS = -c -g
LINKOPT = 
INCPATH = -I"../../graphbase" -I"."

all : $(TESTS)


# all TESTS
test_nodes : sgr_nodes.o test_nodes.o
	$(CXX) -o $@ $(LINKOPT) sgr_nodes.o test_nodes.o

test_lc2kdtree : sgr_lc.o test_lc2kdtree.o sgr_lc2kdtree.o
	$(CXX) -o $@ $(LINKOPT) sgr_lc.o test_lc2kdtree.o sgr_lc2kdtree.o

test_node2lc : sgr_nodes.o test_node2lc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o
	$(CXX) -o $@ $(LINKOPT) sgr_nodes.o test_node2lc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o

test_vfculler : sgr_lc.o test_vfculler.o sgr_vfculler.o ../../graphbase/sgr_math.o sgr_bboxupdater.o sgr_lcreport.o
	$(CXX) -o $@ $(LINKOPT) sgr_lc.o test_vfculler.o sgr_vfculler.o ../../graphbase/sgr_math.o sgr_bboxupdater.o sgr_lcreport.o

test_statesetbuilder : sgr_lc.o sgr_stateset.o sgr_statesetbuilder.o test_statesetbuilder.o sgr_vfculler.o sgr_lcreport.o ../../graphbase/sgr_math.o
	$(CXX) -o $@ $(LINKOPT) sgr_lc.o sgr_stateset.o sgr_statesetbuilder.o test_statesetbuilder.o sgr_vfculler.o sgr_lcreport.o ../../graphbase/sgr_math.o

# all src
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

clean :
	rm -rf *.o *.so *.exe *.idx *~ *.xml *.slc

install : install.bin

install.bin :
	cp *.exe ../../build/bin


run :
	./test_nodes test_nodes.xml
	./test_node2lc geos.slc
	./test_vfculler geos.slc
	./test_statesetbuilder geos.slc ss.xml
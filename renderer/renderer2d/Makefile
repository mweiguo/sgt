TESTS  = test_nodes test_lc2kdtree test_node2lc test_vfculler test_statesetbuilder
BIN    = librender2d.so buildchip buildmisc buildlod buildline buildbirdviewmisc buildlayout buildlayoutitem buildtext
OBJS   = sgr_nodes.o test_nodes.o 
SRC    = $(OBJS:%.o=%.cpp)
INC    = $(OBJS:%.o=%.h)
CXX     = g++
CXXFLAGS = -c -g
LINKOPT = -lopengl32 -lglu32 -lfreetype -L"../../thirdparty/DevIL-SDK-x86-1.7.8/lib" -lDevIL -lILUT
SHARELINKOPT = -shared  -lopengl32 -lglu32 -lfreetype -L"../../thirdparty/DevIL-SDK-x86-1.7.8/lib" -lDevIL -lILUT
INCPATH = -I"../../graphbase" -I"." -I"../../thirdparty/utf8_v2_3_1/source" -I"../../thirdparty/DevIL-SDK-x86-1.7.8/include"
INSTALLDIR = ../../build
#INSTALLDIR = d:/svn_workspace/lightsimulator/build

all : $(TESTS) $(BIN)


# all TESTS
test_nodes : sgr_nodes.o test_nodes.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o test_nodes.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

test_lc2kdtree : sgr_lc.o test_lc2kdtree.o sgr_lc2kdtree.o sgr_font.o sgr_texture.o
	$(CXX) sgr_lc.o test_lc2kdtree.o sgr_lc2kdtree.o sgr_font.o sgr_texture.o -o $@ $(LINKOPT)

test_node2lc : sgr_nodes.o test_node2lc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o test_node2lc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

test_vfculler : sgr_lc.o test_vfculler.o sgr_vfculler.o ../../graphbase/sgr_math.o sgr_bboxupdater.o sgr_lcreport.o sgr_font.o sgr_texture.o
	$(CXX) sgr_lc.o test_vfculler.o sgr_vfculler.o ../../graphbase/sgr_math.o sgr_bboxupdater.o sgr_lcreport.o sgr_font.o sgr_texture.o -o $@ $(LINKOPT)

test_statesetbuilder : sgr_lc.o sgr_stateset.o sgr_statesetbuilder.o test_statesetbuilder.o sgr_vfculler.o sgr_lcreport.o ../../graphbase/sgr_math.o sgr_font.o sgr_bboxupdater.o sgr_texture.o
	$(CXX) sgr_lc.o sgr_stateset.o sgr_statesetbuilder.o test_statesetbuilder.o sgr_vfculler.o sgr_lcreport.o ../../graphbase/sgr_math.o sgr_font.o sgr_bboxupdater.o sgr_texture.o -o $@ $(LINKOPT)

# all bin
#librender2d.a : sgr_render2d.o sgr_lc.o sgr_vfculler.o sgr_statesetbuilder.o ../../graphbase/sgr_math.o sgr_stateset.o
#	ar cq $@ sgr_render2d.o sgr_lc.o sgr_vfculler.o sgr_statesetbuilder.o ../../graphbase/sgr_math.o sgr_stateset.o

librender2d.so : sgr_render2d.o sgr_lc.o sgr_vfculler.o sgr_statesetbuilder.o ../../graphbase/sgr_math.o sgr_stateset.o sgr_renderer.o sgr_font.o sgr_texture.o sgr_bboxupdater.o sgr_nodes.o sgr_node2lc.o ../../graphbase/mat4f.o sgr_lc2kdtree.o
	$(CXX) sgr_render2d.o sgr_lc.o sgr_vfculler.o sgr_statesetbuilder.o ../../graphbase/sgr_math.o sgr_stateset.o sgr_renderer.o sgr_font.o sgr_texture.o sgr_bboxupdater.o sgr_nodes.o sgr_node2lc.o ../../graphbase/mat4f.o sgr_lc2kdtree.o -o $@ $(SHARELINKOPT)

# libslcconverter.so : sgr_slcconverter.o sgr_nodes.o sgr_lc.o sgr_lc2kdtree.o sgr_font.o sgr_texture.o sgr_node2lc.o sgr_lcreport.o sgr_bboxupdater.o sgr_stateset.o sgr_statesetbuilder.o sgr_vfculler.o ../../graphbase/sgr_math.o ../../graphbase/mat4f.o
# 	$(CXX) sgr_slcconverter.o sgr_nodes.o sgr_lc.o sgr_lc2kdtree.o sgr_font.o sgr_texture.o sgr_node2lc.o sgr_lcreport.o sgr_bboxupdater.o sgr_stateset.o sgr_statesetbuilder.o sgr_vfculler.o ../../graphbase/sgr_math.o ../../graphbase/mat4f.o -o $@ $(SHARELINKOPT)

buildchip : sgr_nodes.o buildchip.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o buildchip.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

buildmisc : sgr_nodes.o buildmisc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o buildmisc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

buildlod : sgr_nodes.o buildlod.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o buildlod.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

buildline : sgr_nodes.o buildline.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o buildline.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

buildbirdviewmisc : sgr_nodes.o buildbirdviewmisc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o buildbirdviewmisc.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

buildlayout : sgr_nodes.o buildlayout.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o buildlayout.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

buildlayoutitem : sgr_nodes.o buildlayoutitem.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o buildlayoutitem.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

buildtext : sgr_nodes.o buildtext.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o
	$(CXX) sgr_nodes.o buildtext.o sgr_lc.o sgr_node2lc.o sgr_lcreport.o sgr_lc2kdtree.o sgr_bboxupdater.o sgr_font.o sgr_texture.o ../../graphbase/mat4f.o -o $@ $(LINKOPT)

# all src
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ $<

clean :
	rm -rf *.o *.so *.a *.exe *.idx *~ *.xml *.slc

install : install.bin

install.bin : all
	cp *.exe $(INSTALLDIR)/bin
	cp *.so $(INSTALLDIR)/bin
	cp *.h $(INSTALLDIR)/include

runtest : all
	./test_nodes test_nodes.xml
	./test_node2lc geos.slc
	./test_vfculler geos.slc
	./test_statesetbuilder geos.slc ss.xml
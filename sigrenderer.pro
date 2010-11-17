######################################################################
# Automatically generated by qmake (2.01a) ??? ?? 18 08:36:45 2010
######################################################################

TEMPLATE = lib
TARGET = 
DESTDIR = ./bin
DEPENDPATH += . graphbase renderer unittest
INCLUDEPATH += . graphbase renderer /local/include ../stratumSimulatior/tinylog/src
DEFINES += _USE_XERCES3_
LIBS += -L/local/lib -L/d/usr/mingw/lib -lxerces-c.dll -ltcl85 -ltk85 ../stratumSimulatior/tinylog/src/tinylog.a -lopengl32

# Input
HEADERS += sgr_global.h \
           sgr_sgrenderer.h \
           sgr_sgrenderer_global.h \
           graphbase/sgr_bbox.h \
           graphbase/sgr_colorconv.h \
           graphbase/sgr_gcolor.h \
           graphbase/sgr_grhagl.h \
           graphbase/sgr_kdtree.h \
           graphbase/sgr_line.h \
           graphbase/sgr_mat3.h \
           graphbase/sgr_mat4.h \
           graphbase/sgr_polyline.h \
           graphbase/sgr_quickvector.h \
           graphbase/sgr_rectangle.h \
           graphbase/sgr_vec2.h \
           graphbase/sgr_vec3.h \
           graphbase/sgr_vec4.h \
           renderer/sgr_arrayexpander.h \
           renderer/sgr_arraynode.h \
           renderer/sgr_attrset.h \
           renderer/sgr_boxpicker.h \
           renderer/sgr_cameraortho.h \
           renderer/sgr_childrenfinder.h \
           renderer/sgr_childvisitor.h \
           renderer/sgr_colornode.h \
           renderer/sgr_colortable.h \
           renderer/sgr_coordflater.h \
           renderer/sgr_copier.h \
           renderer/sgr_crosspicker.h \
           renderer/sgr_culling.h \
           renderer/sgr_drawablenode.h \
           renderer/sgr_fontmetric.h \
           renderer/sgr_fontnode.h \
           renderer/sgr_groupnode.h \
           renderer/sgr_includes.h \
           renderer/sgr_interface.h \
           renderer/sgr_kdtreenode.h \
           renderer/sgr_layerchangger.h \
           renderer/sgr_layernode.h \
           renderer/sgr_linenode.h \
           renderer/sgr_loadscene.h \
           renderer/sgr_lodnode.h \
           renderer/sgr_meshlinenode.h \
           renderer/sgr_meshnode.h \
           renderer/sgr_meshpointnode.h \
           renderer/sgr_nodedumper.h \
           renderer/sgr_nodeleter.h \
           renderer/sgr_nodemgr.h \
           renderer/sgr_nodes.h \
           renderer/sgr_nodetransformer.h \
           renderer/sgr_nodevisible.h \
           renderer/sgr_nodevisitor.h \
           renderer/sgr_parentfinder.h \
           renderer/sgr_parentvisitor.h \
           renderer/sgr_pickablegroup.h \
           renderer/sgr_pointnode.h \
           renderer/sgr_polylinenode.h \
           renderer/sgr_polynode.h \
           renderer/sgr_projection.h \
           renderer/sgr_rectitem.h \
           renderer/sgr_renderer.h \
           renderer/sgr_renderflow.h \
           renderer/sgr_renderfunctor.h \
           renderer/sgr_glrenderfunctor.h \
           renderer/sgr_renderlist.h \
           renderer/sgr_rendernodecollector.h \
           renderer/sgr_savemesh.h \
           renderer/sgr_savepickable.h \
           renderer/sgr_scenemgr.h \
           renderer/sgr_scenenode.h \
           renderer/sgr_seedgen.h \
           renderer/sgr_sgnode.h \
           renderer/sgr_sombuilder.h \
           renderer/sgr_switchnode.h \
           renderer/sgr_tclapi.h \
           renderer/sgr_textnode.h \
           renderer/sgr_transformnode.h \
           renderer/sgr_viewport.h \
           renderer/sgr_renderoption.h \
           renderer/sgr_volumepicker.h 
SOURCES += sgr_sgrenderer.cpp \
           graphbase/sgr_grhagl.cpp \
           renderer/sgr_arrayexpander.cpp \
           renderer/sgr_drawablenode.cpp \
           renderer/sgr_interface.cpp \
           renderer/sgr_layerchangger.cpp \
           renderer/sgr_layernode.cpp \
           renderer/sgr_loadscene.cpp \
           renderer/sgr_meshlinenode.cpp \
           renderer/sgr_meshpointnode.cpp \
           renderer/sgr_nodeleter.cpp \
           renderer/sgr_nodetransformer.cpp \
           renderer/sgr_viewport.cpp \
           renderer/sgr_parentvisitor.cpp \
           renderer/sgr_pickablegroup.cpp \
           renderer/sgr_renderflow.cpp \
           renderer/sgr_renderfunctor.cpp \
           renderer/sgr_glrenderfunctor.cpp \
           renderer/sgr_seedgen.cpp \
           renderer/sgr_sgnode.cpp \
           renderer/sgr_tclapi.cpp \
           renderer/sgr_projection.cpp \
           renderer/sgr_textnode.cpp


TARGET=importer.exe
CXX=g++
CXXFLAGS=-c -g
INCPATH=-I"../build/include/COLLADAFramework" -I"../build/include/COLLADASaxFrameLoder" -I"../build/include/COLLADABaseUtils" -I"../build/include/COLLADAStreamWriter" -I"../build/include/GeneratedSaxParser"

LINKOPT=-L"../build/lib" -lOpenCOLLADAFramework -lOpenCOLLADASaxFrameworkLoader -lOpenCOLLADABaseUtils
OBJS=main.o slcimporter.o
SRC=$(OBJS:%.o=%.cpp)
INC=$(OBJS:%.o=%.h)

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) $(OBJS) $(LINKOPT) -o $@

.cpp.o :
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ $< 


clean :
	rm -rf *.o
	rm -rf *.exe


install : $(TARGET)
	cp *.exe ../build/bin
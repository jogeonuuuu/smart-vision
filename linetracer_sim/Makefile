CX = g++
CXFLAGS = -g -Wall
CXFLAGS += `pkg-config opencv4 --cflags` -I/usr/local/include/dynamixel_sdk
LDFLAGS = `pkg-config opencv4 --libs` -ldxl_x64_cpp

TARGET = line_tracer
OBJS = main.o mylib.o dxl.o

$(TARGET) : $(OBJS)
	$(CX) -o $(TARGET) $(OBJS) $(LDFLAGS)
main.o : main.cpp
	$(CX) $(CXFLAGS) -c main.cpp
mylib.o : mylib.hpp mylib.cpp
	$(CX) $(CXFLAGS) -c mylib.cpp
# mylib.o 만들 때 $(CXFLAGS) 필요
dxl.o : dxl.hpp dxl.cpp
	$(CX) $(CXFLAGS) -c dxl.cpp

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)
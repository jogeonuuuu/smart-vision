CX = g++
CXFLAGS = -g -Wall
CXFLAGS += `pkg-config opencv4 --cflags`
LDFLAGS = `pkg-config opencv4 --libs`

TARGET = detect
OBJS = main.o mylib.o

$(TARGET) : $(OBJS)
	$(CX) -o $(TARGET) $(OBJS) $(LDFLAGS)
main.o : main.cpp
	$(CX) $(CXFLAGS) -c main.cpp
mylib.o : mylib.hpp mylib.cpp
	$(CX) $(CXFLAGS) -c mylib.cpp 
# mylib.o 만들 때 $(CXFLAGS) 필요

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)
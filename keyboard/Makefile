.SUFFIXES : .c .cpp .o

CC = g++

INC = -I/usr/include
LIBS = -L/usr/lib
CXXFLAGS = -std=c++11 -g $(INC) -fPIC

OBJS = getopt.o edit_distance.o edit_distance_pos.o util.o cmain.o
SRCS = getopt.c edit_distance.cpp edit_distance_pos.cpp util.cpp cmain.cpp


TARGET = asm1

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

dep :
	gccmakedep $(INC) $(SRCS)

clean :
	rm -rf $(OBJS) $(TARGET) core

new : 
	$(MAKE) clean 
	$(MAKE) 

CXX=g++
DEFINES=-DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB
INCPATH=-I/usr/lib/qt/mkspecs/linux-g++ -I/usr/include/qt -I/usr/include/qt/QtGui -I/usr/include/qt/QtCore
LIBS=-lQt5Core -lconfig++ -lboost_filesystem
CXXFLAGS=-std=c++11 -Wall -Wextra -g $(LIBS) -fPIE $(DEFINES) $(INCPATH)

EXEC=dataFilter
CPP_SOURCES=$(wildcard *.cpp)
OBJS=$(CPP_SOURCES:%.cpp=%.o)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(EXEC)


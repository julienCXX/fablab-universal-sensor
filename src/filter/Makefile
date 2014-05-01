CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra

EXEC=filter_run
OBJS=filter.o filter_run.o

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(EXEC)


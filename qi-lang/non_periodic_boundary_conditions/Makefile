#!/usr/bin/env make

CXX=g++

#CXX_FLAGS=-Wall -g -O0
CXX_FLAGS=-Wall -O3

# Add all h files here
DEPS = cc_vehicle.h cc_qilang.h

# Add all object files that should be re-generated when compiling the
# target main
OBJ = main.o cc_vehicle.o cc_qilang.o

# Every .o file depends on its .cpp version
%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

# Build the 'main' file by compiling the $(OBJ) files (if necessary)
main : $(OBJ)
	$(CXX) -o $@ $^ $(CXX_FLAGS)

# Remove all '.o' files, all '*~' files, and all 'core' files
clean:
	rm -f *.o *~ core
	rm main

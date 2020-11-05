CC=gcc
WARNING_FLAGS=-Wall -Wextra
EXE=537make
SCAN_BUILD_DIR = scan-build-out
# LIBS is placed at the end of gcc's linking stage (after all .o files) so it links the necessary library functions (like pthread) to your code
LIBS=-lpthread # if needed, add more libraries here

# the -g flag at all gcc compilation stages ensures that you can use gdb to debug your code
$(EXE): 537make.o buildSpecificationGraph.o makeFileParser.o buildSpecification.o executeSpecificationGraph.o
	$(CC) -g -o $(EXE) 537make.o buildSpecificationGraph.o makeFileParser.o buildSpecification.o executeSpecificationGraph.o $(LIBS)

537make.o: 537make.c buildSpecificationGraph.h makeFileParser.h buildSpecification.h executeSpecificationGraph.h
	$(CC) -g $(WARNING_FLAGS) -c 537make.c

buildSpecificationGraph.o: buildSpecificationGraph.c buildSpecificationGraph.h
	$(CC) -g $(WARNING_FLAGS) -c buildSpecificationGraph.c

makeFileParser.o: makeFileParser.c makeFileParser.h
	$(CC) -g $(WARNING_FLAGS) -c makeFileParser.c

buildSpecification.o: buildSpecification.c buildSpecification.h
	$(CC) -g $(WARNING_FLAGS) -c buildSpecification.c

executeSpecificationGraph.o: executeSpecificationGraph.c executeSpecificationGraph.h
	$(CC) -g $(WARNING_FLAGS) -c executeSpecificationGraph.c

# the -f flag for rm ensures that clean doesn't fail if file to be deleted doesn't exist
clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

# recompile runs clean and then makes everything again to generate executable
# this is equivalent to running "make clean" followed by "make"
recompile: clean $(EXE)


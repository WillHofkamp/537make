CC=gcc
WARNING_FLAGS=-Wall -Wextra
EXE=537make
SCAN_BUILD_DIR = scan-build-out
LIBS=-lpthread

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

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

recompile: clean $(EXE)


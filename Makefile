#
# Sample makefile updated to include the commands to run the Clang Static Analyzer
#
#    scan-build will "make" your program and run CSA at the same time.
#
#    scan-view will do a scan-build (if necessary) and then open a web browser
#      window to review the results.
#
CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537make
SCAN_BUILD_DIR = scan-build-out
all: 537make.o makeFileParser.o buildSpecificationGraph.o buildSpecification.o
	$(CC) -o $(EXE) 537make.o makeFileParser.o buildSpecificationGraph.o buildSpecification.o
main.o: 537make.c makeFileParser.h
	$(CC) -g -O0 $(WARNING_FLAGS) -c 537make.c
makeFileParser.o: makeFileParser.c makeFileParser.h
	$(CC) -g -O0 $(WARNING_FLAGS) -c makeFileParser.c
buildSpecificationGraph.o: buildSpecificationGraph.c buildSpecificationGraph.h
	$(CC) -g -O0 $(WARNING_FLAGS) -c buildSpecificationGraph.c
buildSpecification.o: buildSpecification.c buildSpecification.h
	$(CC) -g -O0 $(WARNING_FLAGS) -c buildSpecification.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 

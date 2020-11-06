537make

For this assignment we will be creating a simple version of make, which will be able to read a makefile, analyze dependencies and execute the commands that are specified in the makefile

With our 537make, the program can be used in 3 different ways. It can be called first with just the "537make" command, which runs a standard makefile and the root is the "all" target. The next is with a specific makefile name and the -f command, and is structured like "537make -f filename". This allows for differently named makefiles to be ran. The last option is setting the makefile name and the target within the file. This is structured like "537make -f filename target". This allows for making specific targets within the file and not all of it.

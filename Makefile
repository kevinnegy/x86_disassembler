all: x86 

# if file doesn't exist, make will look for it in its target rules
# must go after "all" target so that make -> make all instead of something else 
include .depend  # Must include, otherwise.dependency file could be made, but not used in the makefile itself

src = $(wildcard *.c) # Find all .c files in current directory
obj = $(src:.c=.o) # take all .c files and rename them as .o files
header = $(obj:.o=.h) 

x86: $(obj) 
	gcc $(obj) -o x86

# Rule not required since the.depends file shows target.dependencies and make will know implicit rules for compilation. Use this for any extra flags
%.o: %.c    # % is a placeholder variable name. All %.o files.depend on %.c files
	gcc -c -o $@ $<     # -c doesn't include linking, outputs .o files. $@ is target filename, $< first prereq

depend: .depend     # Only necessary to include if you want to do "make.depend" from terminal

.depend: $(src)     # Create dependency file for all src files (can be named anything)
	rm -f ./.depend 
	gcc  -MM $^ > ./.depend; # -MM flag is for gcc to pump out.dependency targets, $^ is all prereqs

clean:
	rm -f $(obj) x86

clean-all:
	rm -f $(obj) x86 ./.depend
    

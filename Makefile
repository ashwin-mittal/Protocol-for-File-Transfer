# Networking Examples
CXX = g++

# In this section, you list the files that are part of the project.
# If you add/change names of header/source files, here is where you
# edit the Makefile.

HEADERS = client-socket.h server-socket.h
PROGRAMS = server.cc client.cc
EXTRAS = client-socket.cc server-socket.cc
SOURCES = $(PROGRAMS) $(EXTRAS)
OBJECTS = $(SOURCES:.cc=.o)
TARGETS = $(PROGRAMS:.cc=)

default: $(TARGETS)

%: %.o $(EXTRAS:.cc=.o)
	$(CXX) -o $@ $^

# In make's default rules, a .o automatically depends on its .cc file
# (so editing the .cc will cause recompilation into its .o file).
# The line below creates additional dependencies, most notably that it
# will cause the .cc to recompiled if any included .h file changes.

Makefile.dependencies:: $(SOURCES) $(HEADERS)
	$(CXX) -MM $(SOURCES) > Makefile.dependencies

-include Makefile.dependencies

# Phony means not a "real" target, it doesn't build anything
# The phony target "clean" is used to remove all compiled object files.
# The phony target "spartan" is used to remove all compilation products and extra backup files. 

.PHONY: clean spartan

clean:
	@rm -f $(TARGETS) $(OBJECTS) core Makefile.dependencies

spartan: clean
	@rm -f *~

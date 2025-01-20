# Compiler to use (g++ or gcc):
CC = g++

# Main source file:
MAINFILE = a

# output executable:
OUTPUT = a

# Other Files to compile Too:
OTHERFILES =

# Other Flags to add:
OTHERFLAGS =

# Language standart the compiler should use:
# (automatically the version amCode.cpp was compiled in)
# Can be replaced by an std=c++VERSION in OTHERFLAGS as Last takes priority 
LANGUAGESTD = --std=c++23

# Default target (for development/debugging)
all: $(OUTPUT)

$(OUTPUT): $(MAINFILE) $(OTHERFILES)
	$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) $(LANGUAGESTD) $(OTHERFLAGS) -Og

# Production target (optimized and stripped binary)
prod: $(MAINFILE) $(OTHERFILES)
	$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) $(LANGUAGESTD) $(OTHERFLAGS) -Os -s -DNDEBUG

# optimized for speed
fast: $(MAINFILE) $(OTHERFILES)
	$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) $(LANGUAGESTD) $(OTHERFLAGS) -Ofast -Og

# optimized for speed, but without debug
pfast: $(MAINFILE) $(OTHERFILES)
	$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) $(LANGUAGESTD) $(OTHERFLAGS) -Ofast -s -DNDEBUG

# optimized for space/memory efficiency
space: $(MAINFILE) $(OTHERFILES)
	$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) $(LANGUAGESTD) $(OTHERFLAGS) -Oz -Og

# optimized for space/memory efficiency, but without debug
pspace: $(MAINFILE) $(OTHERFILES)
	$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) $(LANGUAGESTD) $(OTHERFLAGS) -Oz -s -DNDEBUG

# Just everything thrown together
max: $(MAINFILE) $(OTHERFILES)
	$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) $(LANGUAGESTD) $(OTHERFLAGS) -Os -Ofast -Oz -Og

# Just everything thrown together, but without debug
pmax: $(MAINFILE) $(OTHERFILES)
	$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) $(LANGUAGESTD) $(OTHERFLAGS) -Os -Ofast -Oz -s -DNDEBUG

# Clean target (works for Linux and Windows)
clean:
	rm $(OUTPUT)
clear:
	rm $(OUTPUT)
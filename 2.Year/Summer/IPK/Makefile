PROJECT=ipkcpd
COMPILER=g++
CCFLAGS=-std=c++20 -Wextra -Wpedantic  -g

.PHONY: all
all: $(PROJECT).cpp
	$(COMPILER) $(CCFLAGS) -o $(PROJECT) $(PROJECT).cpp

.PHONY: clean
clean:
	rm $(PROJECT)
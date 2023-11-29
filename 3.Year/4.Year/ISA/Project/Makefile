CXX = g++
CXXFLAGS = -g
LDLIBS = -lpcap -lncurses

TARGET = dhcp-stats
SRC_DIR = src
SOURCE = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
DOCS = dhcp-stats.1 manual.pdf README.md

all: $(TARGET)

$(TARGET): $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDLIBS)

clean:
	rm -f $(TARGET)

tarball:
	tar -cvf xhorva14.tar $(SRC_DIR) Makefile $(DOCS)

.PHONY: all clean tarball
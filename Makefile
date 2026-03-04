# Automatically finds the SFML path so you don't have to hardcode "3.0.2"
SFML_PATH = $(shell brew --prefix sfml)

# Standard C++17 is required for SFML 3.0+
CXXFLAGS = -std=c++17 -I$(SFML_PATH)/include
LDFLAGS = -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# Finds all .cpp files in the src directory
cppFileNames := $(shell find ./src -type f -name "*.cpp")

all: compile

compile:
	mkdir -p bin
	g++ $(CXXFLAGS) $(cppFileNames) -o bin/main $(LDFLAGS)

clean:
	rm -rf bin
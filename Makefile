CXX=c++
OUTPUT=app.exe

SFML_DIR=c:/SFML-3.0.0

CXX_FLAGS=-O3 -std=c++20 -Wall -Werror
INCLUDES=-I$(SFML_DIR)/include
LDFLAGS=-L$(SFML_DIR)/lib -s -lsfml-graphics -lsfml-window -lsfml-system

OBJECTS=\
	vector2d.o \
	component.o \
	entity.o \
	manager.o \
	game.o \
	main.o

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(info "Link...")
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(info "Build...")
	$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

run: $(OUTPUT)
	$(OUTPUT)

clean:
	del $(OBJECTS)
	del $(OUTPUT)

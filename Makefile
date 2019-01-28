default:

sources = demo.cc
objects = $(sources:.cc=.o)

arch = -march=native -mlzcnt -mpopcnt -mbmi -mbmi2
flag = -std=c++17 -Wall -O3
link =

ifeq ($(OS), Windows_NT)
link = -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
endif

CXXFLAGS = $(arch) $(flag)

default: demo
demo: $(objects)
	$(CXX) $(link) -o $@ $^

clean:
	-$(RM) demo $(objects)

.PHONY: default clean
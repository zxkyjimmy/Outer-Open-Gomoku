default:

sources = Cdefense.cc
objects = $(sources:.cc=.o)

arch = -march=native -mlzcnt -mpopcnt -mbmi -mbmi2
flag = -std=c++17 -Wall -O3
link =

ifeq ($(OS), Windows_NT)
link = -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
endif

CXXFLAGS = $(arch) $(flag)

default: gen_state
gen_state: $(objects)
	$(CXX) $(link) -o $@ $^

clean:
	-$(RM) gen_state $(objects)

.PHONY: default clean
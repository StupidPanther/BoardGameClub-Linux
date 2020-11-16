CC := g++
CFLAGS := -g
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))
DEP := $(patsubst %.cpp, %.d, $(SRC))
HDR := $(wildcard *.h)
TAR := BoardGameClub

$(TAR): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TAR)

$(OBJ): %.o: %.cpp %.d
	$(CC) $(CFLAGS) -c $< -o $@

$(DEP): %.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm $@.$$$$

clean:
	@-rm $(OBJ) $(TAR) $(DEP)


CFLAGS = -O2 -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi -lm
DFLAGS = -g3 -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi -lm
SFLAGS = -g3 -fsanitize=undefined -fsanitize=address -lm
TESTBASE = puzzle
SOURCES =  $(TESTBASE).c
EXECS = $(TESTBASE) $(TESTBASE)_d $(TESTBASE)_s 
CC = clang

run: $(TESTBASE)
	./$(TESTBASE) "16 752843"

all: $(EXECS)

$(TESTBASE): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(TESTBASE)_d: $(SOURCES) $(INCS) 
	$(CC) $(SOURCES) -o $@ $(DFLAGS) 

$(TESTBASE)_s: $(SOURCES) $(INCS) 
	$(CC) $(SOURCES) -o $@ $(SFLAGS) 

clean:
	rm -f $(EXECS)



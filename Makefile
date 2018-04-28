CC = gcc
CFLAGS = -Wall -std=c99 -g
QUESTION_FILES = questions.h questions.c
QTREE_FILES = qtree.h qtree.c

ALL_PROGRAMS=categorizer

all: $(ALL_PROGRAMS)

categorizer: $(QUESTION_FILES) $(QTREE_FILES) categorizer.c
	$(CC) $(CFLAGS) $^ -o $@

clean:  
	rm -f $(ALL_PROGRAMS)

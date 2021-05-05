CC = gcc
LINKER = gcc
CFLAGS = -std=c99 -Wall # -Werror -Wpointer-arith -Wfatal-errors
DEBUG = -g
PROG1LIBNAME = prog1
PROG1LIBDIR = ../prog1lib/lib

BOOKS = book_list
SRCS = book_list.c pointer_list.c
OBJS = $(SRCS:.c=.o)

CITIES = city_list
SRCS_CITIES = city_list.c pointer_list.c
OBJS_CITIES = $(SRCS_CITIES:.c=.o)

STUDENT = student_list
SRCS_STUDENT = student_list.c pointer_list.c
OBJS_STUDENT = $(SRCS_STUDENT:.c=.o)

EXT = pointer_list_ext
SRCS_EXT = pointer_list_ext.c pointer_list.c
OBJS_EXT = $(SRCS_EXT:.c=.o)

# disable default suffixes
.SUFFIXES:

# library
prog1lib:
	cd $(PROG1LIBDIR) && make

# pointer list
pointer_list.o: pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR) 

pointer_list: pointer_list.o
	$(LINKER) $(CFLAGS) $(DEBUG) -o pointer_list pointer_list.o -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR)

# book_list
book_list.o: book_list.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR)

$(BOOKS): $(OBJS)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(BOOKS) $(OBJS) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR)

# city_list
city_list.o: city_list.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR)

$(CITIES): $(OBJS_CITIES)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(CITIES) $(OBJS_CITIES) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR)
	
# people_list
student_list.o: student_list.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR)

$(STUDENT): $(OBJS_STUDENT)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(STUDENT) $(OBJS_STUDENT) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR)

# pointer_list_ext
pointer_list_ext.o: pointer_list_ext.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR) 

$(EXT): $(OBJS_EXT)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(EXT) $(OBJS_EXT) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR) 

# delete intermediary files
.PHONY: clean		# do not treat "clean" as a file name
clean:				# special target to remove files: make clean
	rm -f $(BOOKS) $(OBJS) $(CITIES) $(OBJS_CITIES) $(OBJS_EXT)

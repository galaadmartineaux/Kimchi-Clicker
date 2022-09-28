
CFLAGS=-O0 -g -Werror=implicit-function-declaration
CPPFLAGS=-I.. -I../externals/include -MMD

LDFLAGS=-L.. -L../externals/x86_64-linux-gnu
LDLIBS=-lpg -lglfw3 -lm

#LDFLAGS=-L.. -L../externals/x86_64-pc-cygwin
#LDLIBS=-lpg -lglfw3 -lgdi32

OBJS=hello_world.o scrolling.o
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: hello_world scrolling

-include $(DEPS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

hello_world: hello_world.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

scrolling: scrolling.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(DEPS) hello_world scrolling


OUTPUT=libpg.a
CFLAGS=-O0 -g -Werror=implicit-function-declaration
CPPFLAGS=-Iexternals/include -MMD -DGLFW_INCLUDE_NONE

OBJS=playground.o config.o playground_backend_glfw.o
OBJS+=externals/src/gl.o externals/src/stb_image.o externals/src/miniaudio.o
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(OUTPUT)

-include $(DEPS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(OUTPUT): $(OBJS)
	$(AR) -crs $@ $^

clean:
	rm -f $(OBJS) $(DEPS) $(OUTPUT)

CC := gcc

CFLAGS := $(shell pkg-config sdl2 --cflags)
CFLAGS += -fPIE -Iinclude/ -Ilib/

SRCDIR := src/
LIBDIR := lib/
OBJDIR := build/obj/
LIBDIR := lib/

NAME := silly-emulator

SFILES := c
OFILES := o

LIBS := -lSDL2 -lm

SOURCES := $(shell find $(SRCDIR) -name "*.$(SFILES)")
LIBSOURCES := $(shell find $(LIBDIR) -name "*.$(SFILES)")
OBJECTS := $(patsubst $(SRCDIR)%.$(SFILES), $(OBJDIR)%.$(OFILES), $(SOURCES))
OBJECTS += $(patsubst $(LIBDIR)%.$(SFILES), $(OBJDIR)%.$(OFILES), $(LIBSOURCES))

all: directories $(NAME)

directories:
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)quarklib/qprint/
# 	mv $(NAME)_backup $(NAME)_backup1
# 	mv $(NAME) $(NAME)_backup

$(NAME): $(OBJECTS)
	$(CC) $^ $(LIBS) -o $@
$(OBJDIR)%$(OFILES): $(SRCDIR)%$(SFILES)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJDIR)%$(OFILES): $(LIBDIR)%$(SFILES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
# 	mv $(NAME)_backup $(NAME)_backup1
# 	mv $(NAME) $(NAME)_backup
	rm -rf $(OBJDIR)		


TARGET := fourDots
INCDIR := ./
SRCDIR := ./
SRCS = $(TARGET).c
LIBS = -lm
BUILDDIR := .build

CFLAGS = -O2 -std=gnu18 -Wall -Wextra -Wpedantic

CC := gcc

.PHONY: all clean tidy

all: $(TARGET)

$(TARGET): $(BUILDDIR)/$(TARGET)
	cp $< $@

$(BUILDDIR)/$(TARGET): $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o))
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(BUILDDIR)/%.o: $(addprefix $(SRCDIR)/,%.c) $(BUILDDIR)
	$(CC) $(CFLAGS) $(LIBS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILDDIR): 
	mkdir -p $@

clean:
	@echo Cleaning up
	-rm -rf $(BUILDDIR)/*.o

tidy: clean
	-rm -f $(BUILDDIR)/$(TARGET)
	-rm -f ./$(TARGET)




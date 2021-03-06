CC 	:=	gcc

TARGET	:=	matmul
MAIN	:=	main.c
SOURCES :=	$(wildcard source/*.c)
INCLUDE := 	./include
OBJECTS :=	$(SOURCES:.c=.o)
DEPS	:=	$(OBJECTS:.o=.d)
CFLAGS 	:= 	-Wall -pedantic -std=c99 -I$(INCLUDE) -Wvariadic-macros	
LIBS	:=	-lm -lpthread

.PHONY:		clean depend .depend

-include $(DEPS)

%.o:	%.c
	@echo Compiling $<...
	$(CC) -c $(CFLAGS) -MMD -o $@ $<

matmul:		$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS) -D DEBUG

clean:
	$(MAKE) -C source clean

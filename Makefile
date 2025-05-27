EXEC = tema

CC = gcc

SRC = src/main.c src/liste.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC)

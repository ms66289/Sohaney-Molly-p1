NAME = fifteen
FLAGS = -Wall -pedantic-errors
cl: $(NAME).c
	gcc -o $(NAME) $(FLAGS) $(NAME).c
clean:
	rm -f $(NAME)

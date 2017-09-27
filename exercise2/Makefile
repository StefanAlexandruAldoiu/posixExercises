all: parent2

parent2:
	gcc parent.c -o parent2 -lrt
run: parent2
	./parent2 $(arg)
clean:
	rm parent2

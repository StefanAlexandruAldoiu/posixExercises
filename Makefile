all: exercise2 exercise3

exercise2: parent2

parent2:
	gcc exercise2/parent.c -o exercise2/parent2 -lrt
run2: exercise2
	./exercise2/parent2 $(arg)
clean2:
	rm exercise2/parent2

exercise3: parent3 child3

parent3:
	gcc exercise3/parent.c -o exercise3/parent3 -lrt
child3:
	gcc exercise3/child.c -o exercise3/child -lrt
run3: exercise3
	./exercise3/parent3
clean3:
	rm exercise3/parent3
	rm exercise3/child

clean:
	rm exercise3/parent3
	rm exercise3/child
	rm exercise2/parent2

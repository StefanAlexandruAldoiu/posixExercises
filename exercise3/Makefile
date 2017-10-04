all: parent3 child

parent3:
	gcc parent.c -o parent3 -lrt
child:
	gcc child.c -o child -lrt
run:
	./parent3
clean:
	rm parent3
	rm child

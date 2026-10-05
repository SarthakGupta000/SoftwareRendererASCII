util.o:
	gcc util.c -c

main.o:
	gcc main.c -c

proj: main.o util.o
	gcc main.o util.o -o proj -lm

run: proj
	./proj

clean:
	rm main.o util.o proj
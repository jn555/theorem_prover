build: main

main: main.c
	cc main.c -o main

run: main
	./main

clean:
	rm -rf main
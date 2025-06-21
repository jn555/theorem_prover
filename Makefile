build: main

main: main.c utils.c utils.h types.c types.h knowledge_set.c knowledge_set.h inference.c inference.h hashcons.c hashcons.h
	cc main.c utils.c types.c knowledge_set.c inference.c hashcons.c -o main

run: main
	./main

build_parser: main_parser

main_parser: parser.c
	cc parser.c -o parser

run_parser: main_parser
	./parser

clean:
	rm -rf main
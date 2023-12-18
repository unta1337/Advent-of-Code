TARGET=part_one part_two

all: $(TARGET)

part_one: main.c
	gcc -o part_one main.c -lm

part_two: main.c
	gcc -o part_two main.c -DPART_TWO -lm

clean:
	rm $(TARGET)

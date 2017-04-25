main: main.o ADS.o SAD.o dialog.o
	gcc -o main $^ `pkg-config --libs --cflags gtk+-3.0` libbt.a

%.o: %.c
	gcc -c $^ `pkg-config --libs --cflags gtk+-3.0`

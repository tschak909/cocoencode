cocoencode: cocoencode.c
	$(CC) -ococoencode cocoencode.c

clean:
	$(RM) cocoencode

test: cocoencode megabug.raw
	cat megabug.raw | ./cocoencode >megabug.raw

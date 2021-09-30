CC=aarch64-linux-gnu-gcc
all:
	${CC} --static rawcapture.c -o rawcapture
clean:
	rm rawcapture

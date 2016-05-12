obj-m += lcdd.o

all:
	make -C /lib/modules/4.2.0-27-generic/build M=$$PWD

clean:
	make -C /lib/modules/4.2.0-27-generic/build M=$(PWD) clean

lcddtest:lcddtest.c
	cc -o lcddtest lcddtest.c

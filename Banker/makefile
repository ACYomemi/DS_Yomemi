flag=0
all: bankerMain.c
	gcc bankerMain.c -o bankerMain
clean: 
	rm -rf *.o
	if [ ${flag} -eq 1 ] ; then rm -rf bankerMain; fi
run:
	./bankerMain < input.md


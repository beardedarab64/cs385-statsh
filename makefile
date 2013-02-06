
t1: t1.c
	@gcc -g -o t1 t1.c
clean1:
	@rm -f ./t1
run1: clean1 t1
	@./t1
	@rm -f ./t1

statsh: statsh.cpp Job.o Command.o CFuncs.o Misc.o
	@g++ -Wall -g -o$@ $< Job.o Command.o CFuncs.o Misc.o
	@rm -f ./*.o
clean:
	@rm -f ./statsh
	@rm -f ./*.o
rebuild: clean statsh
run: clean statsh
	@./statsh
	@rm -f ./statsh
Job.o: Job.cpp Job.h
	@g++ -Wall -g -c -oJob.o Job.cpp
Command.o: Command.cpp Command.h
	@g++ -Wall -g -c -oCommand.o Command.cpp
CFuncs.o: CFuncs.cpp CFuncs.h
	@g++ -Wall -g -c -oCFuncs.o CFuncs.cpp
Misc.o: Misc.cpp Misc.h
	@g++ -Wall -g -c -o$@ $<

t3: t3.cpp Command.o CFuncs.o Misc.o
	@g++ -g -Wall -ot3 $< Command.o CFuncs.o Misc.o
clean3: 
	@rm -f ./t3
run3: clean3 t3
	@./t3
	@rm -f ./t3


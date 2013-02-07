statsh: statsh.cpp Job.o Command.o CFuncs.o Misc.o
	@g++ -Wall -g -o$@ $< Job.o Command.o CFuncs.o Misc.o
	@rm -f ./*.o
rebuild: clean statsh
run: clean statsh
	@./statsh
	@rm -f ./statsh
Job.o: Job.cpp Job.h
	@g++ -Wall -g -c -o$@ $<
Command.o: Command.cpp Command.h
	@g++ -Wall -g -c -o$@ $<
CFuncs.o: CFuncs.cpp CFuncs.h
	@g++ -Wall -g -c -o$@ $<
Misc.o: Misc.cpp Misc.h
	@g++ -Wall -g -c -o$@ $<
clean:
	@rm -f ./statsh
	@rm -f ./*.o




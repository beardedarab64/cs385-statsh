TARGET = statsh
COMPILER_OPTIONS = -Wall -g -Wno-format-security
COMPILE = @g++ -c $(COMPILER_OPTIONS) -o$@ $<
COMPILE_N_LINK = @g++ $(COMPILER_OPTIONS) -o$@ $<

OBJECTS =	obj/Job.o \
			obj/Command.o \
			obj/CFuncs.o \
			obj/Misc.o		

$(TARGET): statsh.cpp $(OBJECTS)
	@echo compiling and linking [$@]
	$(COMPILE_N_LINK) $(OBJECTS)

rebuild: clean statsh

obj/%.o: %.cpp %.h
	@echo building [$@]
	$(COMPILE)

run: statsh
	@./$(TARGET)

clean:
	@rm -f ./$(TARGET)
	@rm -f ./obj/*.o




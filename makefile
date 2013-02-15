TARGET = statsh
COMPILER_OPTIONS = -Wall -g -Wno-format-security
COMPILE = @g++ -c $(COMPILER_OPTIONS) -o$@ $<
COMPILE_N_LINK = @g++ $(COMPILER_OPTIONS) -o$@ $<

OBJECTS =	Job.o \
			Command.o \
			CFuncs.o \
			Misc.o		

$(TARGET): statsh.cpp $(OBJECTS)
	@echo compiling and linking [$@]
	$(COMPILE_N_LINK) $(OBJECTS)

rebuild: clean statsh

%.o: %.cpp %.h
	@echo building [$@]
	$(COMPILE)

run: statsh
	@./$(TARGET)

clean:
	@rm -f ./$(TARGET)
	@rm -f ./*.o




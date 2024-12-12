CC = gcc
CFLAGS = -Wall -Wextra -I./include -g  # Add -g here for debugging symbols
SRC = src/indexing_library.c
OBJ = obj/indexing_library.o
LIBRARY = lib/libindexing.a
TEST_SRC = test/test_program.c
TEST_OBJ = obj/test_program.o
EXEC = test_program

# Target to build the library
$(LIBRARY): $(OBJ)
#Ensure the lib directory exists
	mkdir -p lib  
	ar rcs $(LIBRARY) $(OBJ)

# Compile the object file for the library
obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

# Target to build and test the program
test: $(TEST_OBJ) $(LIBRARY)
	$(CC) $(CFLAGS) $(TEST_OBJ) -L./lib -lindexing -o $(EXEC)
	./$(EXEC)

# Compile the test program
obj/test_program.o: $(TEST_SRC)
	mkdir -p obj
	$(CC) $(CFLAGS) -c $(TEST_SRC) -o $(TEST_OBJ)

# Install the library (for other users)
install: $(LIBRARY)  # This installs the library and header file
	mkdir -p /usr/local/include/indexing-library
	mkdir -p /usr/local/lib
	cp include/indexing_library.h /usr/local/include/indexing-library
	cp $(LIBRARY) /usr/local/lib
	ldconfig  # Update the library cache for dynamic linking (if needed)

# Clean the build artifacts
clean:
	rm -rf obj/*.o $(EXEC) $(LIBRARY) test/test_program.o

# Uninstall the library (optional)
uninstall:
	rm -rf /usr/local/include/indexing-library
	rm -f /usr/local/lib/libindexing.a

.PHONY: clean test install uninstall

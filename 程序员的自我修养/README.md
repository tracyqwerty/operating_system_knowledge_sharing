# README



## BUILD: COMPILE + LINK

### 1. Preprocessing
First, you preprocess `hello.cpp` to generate `hello.i`:
```sh
g++ -E hello.cpp -o hello.i
```
This command creates `hello.i`, which contains the result of preprocessing `hello.cpp`.

### 2. Compiling
Next, you compile `hello.i` (the preprocessed file) into assembly code, creating `hello.s`:
```sh
g++ -S hello.i -o hello.s
```
Now, `hello.s` contains the assembly code generated from `hello.i`.

### 3. Assembling
After that, you assemble `hello.s` into machine code, producing `hello.o`:
```sh
g++ -c hello.s -o hello.o
```
`hello.o` is the object file with machine code, ready for linking.

### 4. Linking
Finally, you link `hello.o` to create the executable file:
```sh
g++ hello.o -o hello
```
This command links `hello.o` (and any other object files you might have) along with the necessary system libraries to create an executable named `hello`.

By following these steps, you systematically transform your C++ source code into an executable program, without needing to refer back to the original `.cpp` file after the first step. This method gives you a clearer understanding of what happens at each stage of compilation and linking.

## OBJDUMP

To inspect the contents of object files or executables, you can use the `objdump` tool. `objdump` is a powerful utility for displaying various information about object files, executables, or library files. It is particularly useful for debugging and analyzing how your source code translates into machine instructions.

Here's a basic guide on how to use `objdump` to check different aspects of your compiled files:

### 1. Viewing Assembly Code of an Object File
To view the assembly code contained in your object file (`hello.o`), you can use the `-d` (or `--disassemble`) option. This command displays the assembly code for the machine instructions in the object file:

```sh
objdump -d hello.o
```

### 2. Viewing the Symbol Table
The symbol table of an object file or executable includes information about function and variable names. To view the symbol table, you can use the `-t` (or `--syms`) option. This is useful for seeing the names of functions and variables that are present in your file:

```sh
objdump -t hello.o
```

### 3. Viewing Headers
To see the headers of an object file or executable, which include the file format, sections, and sizes, you can use the `-x` option. This provides a detailed view of the file's structure:

```sh
objdump -x hello.o
```

### 4. Disassembling the Executable
Similar to how you can disassemble an object file, you can also disassemble an executable file to view its assembly code. This is useful for understanding the final instructions that will be executed by the CPU:

```sh
objdump -d hello
```

### Using `objdump` with Source Code (Debugging Information)
If you compile your C++ code with debugging information (using the `-g` option with `g++`), `objdump` can also show source code lines alongside the assembly instructions. This is immensely helpful for debugging and understanding how specific lines of code translate into assembly. To compile with debugging information and then use `objdump`:

```sh
g++ -g hello.cpp -o hello
objdump -S hello
```

The `-S` (capital S) option tells `objdump` to intermix source code with assembly, assuming the executable contains debugging information.

These commands and options will help you get started with using `objdump` to inspect and understand your compiled C++ programs. Remember, `objdump` has many more options and capabilities, which you can explore by reading its manual page (`man objdump` or `objdump --help`).



## `file`

```bash
tracyqwerty@tracyqwertydezhuantou hello % file hello.i
hello.i: C++ source text, ASCII text
tracyqwerty@tracyqwertydezhuantou hello % file hello.s
hello.s: assembler source text, ASCII text, with very long lines (324)
tracyqwerty@tracyqwertydezhuantou hello % file hello.o
hello.o: Mach-O 64-bit object x86_64
tracyqwerty@tracyqwertydezhuantou hello % file hello  
hello: Mach-O 64-bit executable x86_64
```


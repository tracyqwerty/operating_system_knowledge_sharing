# Operating System Knowledge Sharing
OS self-learning material. Powered by ChatGPT4 >w<

## UCB CS162

Based on Fall 2022.

Lectures: https://www.youtube.com/watch?v=4FpG1DcvHzc&list=PLMcZASLnrVs5qyBC-dc4bFRtC8ROPYmS0&index=2

Website: https://inst.eecs.berkeley.edu/~cs162/fa22/

BaseCode: https://github.com/Berkeley-CS162

### Hw1: Intro

Basic usage of c, combined with I/O, editor, gdb and other useful tools.

Personally I would recommend go through these before we start: 

* basic vim usage: http://www2.geog.ucl.ac.uk/~plewis/teaching/unix/vimtutor

#### Counting Words

Also here're some tips:

**Why by using `char*` can we get an entire string?**

In C, a string is essentially an array of characters, but it's typically represented as a pointer to its first element -- a `char *`. So when you have a `char *` variable, it is effectively a string.

The `strcmp` function compares two strings. It expects as arguments two pointers to the beginning of each string (two `char *`). When you call `strcmp(wc1->word, wc2->word)`, you're saying "compare the string that begins at `wc1->word` with the string that begins at `wc2->word`".

If you were to dereference the pointers with `*wc1->word` and `*wc2->word`, you would be providing the `char` values (i.e., the first characters of each string) at those memory locations instead of the pointers themselves, which is not what `strcmp` expects.

In other words, `strcmp` doesn't want to know what the first characters of the strings are -- it wants to know where the strings start. By giving it `char *` arguments, you're providing it with that information.



**Why when initializing, we get `WordCount **wclist`, instead of just use `WordCount *wclist?`**

This double pointer `WordCount **wclist` is used in this context because the function `init_words` needs to modify the pointer `wclist` itself, not the data it points to.

In C, function arguments are passed by value. This means that if you passed a single pointer `WordCount *wclist` to a function and then tried to change `wclist` within that function, you would only be changing a copy of the pointer, and not the original pointer itself.

However, if you **want to modify the original pointer (for example, to point it to a different memory location or to set it to NULL)**, you need to pass a pointer to that pointer, hence `WordCount **wclist`.

In the given code snippet, the line `*wclist = NULL;` is setting the original pointer `wclist` (which was passed into `init_words` as a pointer to a pointer) to NULL. If you had passed `wclist` as a single pointer, this operation would not affect the original `wclist` pointer outside of the function.

To summarize, if you just want to change the data that the pointer points to, you can pass a single pointer. But if you need to change what the pointer itself points to, you should pass a double pointer.



**How to use `malloc` in C?**

The `new` keyword is used in C++ to dynamically allocate memory. It's not part of the C language. 

In C, you use `malloc()`, `calloc()`, or `realloc()` functions to allocate memory dynamically. Here's how you might do it for a `WordCount` struct:

```c
*wclist = (WordCount*)malloc(sizeof(WordCount));
```

Remember to check the result of `malloc()`, because it will return `NULL` if the memory allocation fails. Also, when you're done with the memory, you should free it with `free()` to avoid memory leaks.

Don't forget to `#include <stdlib.h>` at the top of your file, as `malloc`, `calloc`, `realloc`, and `free` are all defined in that header.



**Why I have to run ` infile = fopen(argv[optind], "r");` twice if I want to use it twice?**

Opening a file with `fopen()` returns a `FILE` pointer that is used to track the file's position, among other things. When you read from a file, the position moves forward in the file, so when you're done reading the file, the position is at the end. If you try to read from the file again without resetting the position, you won't get any data because you're already at the end of the file.

One way to reset the file position to the beginning is to use the `fseek()`, `rewind()`, or `fsetpos()` functions. For example:

```c
infile = fopen(argv[optind], "r");
// Do something
rewind(infile);
// Do something else
```

Another way, as in your example, is to close the file with `fclose()` and then open it again with `fopen()`. This will also reset the file position to the beginning:

```c
infile = fopen(argv[optind], "r");
// Do something
fclose(infile);

infile = fopen(argv[optind], "r");
// Do something else
fclose(infile);
```

Both of these ways work, but the first one is often preferable because it doesn't involve the overhead of closing and opening the file again. However, if you're done with the file after the first operation and you don't need to use it again until some time later, it might be better to close it as soon as you're done with it to free up system resources. In such a case, you would need to open the file again before you can use it.



**Also here's a full explanation of the [hint](https://cs162.org/static/hw/hw-intro/docs/counting-words/#total-count):**

This hint is providing you a series of Unix commands that are chained together using pipes (`|`) to accomplish a similar goal to your program: to count the frequency of words in a file and then sort them. Here's what each command is doing:

1. `cat <filename>`: This command prints the content of the file to the standard output (stdout).

2. `tr " " "\n"`: This command replaces all spaces with newline characters, effectively putting each word on its own line.

3. `tr -s "\n"`: This command squeezes multiple consecutive newline characters into a single newline.

4. `tr "[:upper:]" "[:lower:]"`: This command translates all uppercase characters to lowercase.

5. `tr -d -C "[:lower:]\n"`: This command deletes every character that is not a lowercase letter or a newline. This helps to remove punctuation and other non-alphabetical characters.

6. `sort`: This command sorts its input lines.

7. `uniq -c`: This command removes duplicate lines and precedes each line with the count of occurrences.

8. `sort -n`: This command sorts its input lines numerically.

Specifically in ` tr -d -C "[:lower:]\n":
 `

`tr` command stands for "translate" and it is used in Unix systems to translate, squeeze, and/or delete characters from standard input, writing the result to standard output. 

In `tr -d -C "[:lower:]\n"`, there are two key parts to understand:

1. The `-d` option: This option tells `tr` to delete characters.

2. The `-C` option: This option complements the set of characters, meaning it inverts the set. It selects all characters that are NOT in the specified set.

So the command `tr -d -C "[:lower:]\n"` means "delete all characters that are not lowercase letters or newline characters". 

To understand this in more detail:

- `[:lower:]` matches all lowercase letters.
- `\n` matches newline characters.

The `-C` option inverts this set, so it matches all characters that are not lowercase letters and not newline characters. The `-d` option then deletes these matched characters.

#### User Limit

In Unix-based systems, there are two types of limits that can be set for a user: a soft limit and a hard limit.

**Soft Limit:** This is the value that the kernel enforces for the corresponding resource. The soft limit can be increased by the user up to the hard limit. If a process tries to consume more of the resource than the soft limit allows, the kernel will typically send a signal to the process that will cause it to terminate.

**Hard Limit:** This is the maximum value to which the soft limit can be increased by a process. The hard limit can only be reduced; once it has been lowered, it cannot be increased without rebooting the system or having superuser (root) privileges. The hard limit acts as a ceiling for the soft limit. If a process tries to consume more of the resource than the hard limit allows, the kernel will not allow it.

For example, suppose we have a user limit for open files, with a soft limit of 1024 and a hard limit of 2048. This means that by default, a user can open up to 1024 files simultaneously. However, the user can increase this limit up to 2048 (the hard limit) if they need to. But once they do so, they can't go back to the previous limit without superuser intervention.

These limits are used for managing resources on a system and ensuring that a single user or process can't consume too much of a resource and thereby impact other users or processes. They also provide a way for users to manage their own resource use.



Everything else we need to know are in `man getlimit`.



Also here're some tips:

` if (getrlimit(RLIMIT_NOFILE, &rlim) == 1) perror("getLimit")`

`perror("getrlimit")` is a function provided by the C standard library that prints an error message corresponding to the current value of the `errno` variable. The `getrlimit` function sets the `errno` variable if it encounters an error.

In the code snippet `perror("getrlimit")`, the string `"getrlimit"` is passed as an argument to `perror()`. This string is typically used to provide additional context or description about the error being reported.

When `perror()` is called, it looks at the value of `errno` and prints a descriptive error message to the standard error stream (stderr), along with the provided string and a colon. For example, it might output something like:

```
getrlimit: Invalid argument
```

The specific error message depends on the value of `errno`, which is set by the system or library function that encountered the error. In the case of `getrlimit()`, it could indicate an invalid argument, insufficient privileges, or other possible errors.

Using `perror()` is a convenient way to print descriptive error messages related to system or library function failures, allowing you to provide more information about what went wrong during program execution.

#### GDB Basics

Manuall: http://www.unknownroad.com/rtfm/gdbtut/gdbtoc.html

Offical documentation: https://www.sourceware.org/gdb/documentation/

Also here're the tips:

`printf("%i%p", ...)`

In the `printf` function in C, `%i` and `%p` are format specifiers used to display integers and pointers, respectively.

- `%i`: This format specifier is used to print integers. It can **handle both signed and unsigned integer values**. When using `%i`, the corresponding argument in the `printf` function should be an integer value.
- `%p`: This format specifier is used to print pointers. It is typically used to **display the memory address represented by a pointer variable**. When using `%p`, the corresponding argument in the `printf` function should be a pointer value.



**Difference between `step` and `next`:**

1. **step (`s`)**: The `step` command is used to step through the program one line at a time. If the current line contains a function call, the `step` command will enter that function and allow you to step through its code as well. It means that if the line being executed contains a function call, GDB will move to the first line of the called function and pause there.

2. **next (`n`)**: The `next` command is used to execute the next line of code in the program without stepping into any function calls. If the current line contains a function call, the `next` command will execute that entire function without entering it. It means that if the line being executed contains a function call, GDB will execute the entire function as a single step and pause at the next line after the function call.



Also here's a demo with explanation.
```shell
fudanicpc@cat:~/Desktop/student0-main/hw-intro$ gdb map
Reading symbols from map...done.
(gdb) break map.c:6
# 0x699: The memory address where line 6 resides in the compiled program.
Breakpoint 1 at 0x699: file map.c, line 6.
(gdb) break map.c:7
Note: breakpoint 1 also set at pc 0x699.
Breakpoint 2 at 0x699: file map.c, line 7.
(gdb) break recurse.c:7
Breakpoint 6 at 0x6f9: file recurse.c, line 7.
(gdb) run
Starting program: /home/fudanicpc/Desktop/student0-main/hw-intro/map

.....

(gdb) run
Starting program: /home/fudanicpc/Desktop/student0-main/hw-intro/map

Breakpoint 1, main (argc=1, argv=0x7fffffffdee8) at map.c:16
16	    volatile int i = 0;
(gdb) print(i)
$1 = 0
(gdb) print(i)
$2 = 0
(gdb) p i
$3 = 0
(gdb) p x
No symbol "x" in current context.
(gdb) backtrace
#0  main (argc=1, argv=0x7fffffffdee8) at map.c:19
(gdb) kill
Kill the program being debugged? (y or n) y
(gdb) 
```



#### From Source Code to Executable

Think we need some prerequisite here:

`objdump` is a command-line program used to display various information about object files in Unix-like operating systems. This can include object files, executable files, and relocatable files. Each file type you mentioned carries a different kind of information:

1. **Executable file**: When `objdump` is used on an executable file, it can display a lot of information. It can show the header information, assembly level instructions, and section information, among others. You can control what it shows through various flags:

   - `-h` displays the section headers
   - `-d` disassembles the executable sections
   - `-s` displays the full contents of all sections requested
   - `-j` displays the specified section

2. **Object (*.o) file**: Object files are typically generated by a compiler and consist of machine code along with data used by that code. `objdump` can show you similar information as for executable files: header info, assembly code, section info, etc. Object files typically represent modules of a program that are linked together to create the final executable.

3. **Assembly (*.s) file**: This is a file written in assembly language, a low-level programming language. `objdump` wouldn't usually be used directly on an assembly file as it's not an object file or an executable file. Rather, assemblers like `as` are used to translate assembly files into object files. `objdump` could be used on the resulting object file to display its contents.

The main difference between these files is their role in the process of going from source code to executable:

- The `.s` file is the assembly language translation of your source code. It is a human-readable text file.
- The `.o` file is the compiled version of your `.s` file (or directly from your source code). It is in machine language, but isn't yet a complete program.
- The executable file is the linked version of one or more `.o` files. It is a complete program that can be run by the operating system.

Remember, the output of `objdump` can be quite extensive and somewhat hard to read unless you're familiar with assembly language and the layout of object files and executables.



let's take a peek at [map's assembly code](materials/map.s) : 

1. `.file "map.c"`: Specifies the name of the source file.

2. `.text`: Indicates the start of the text (code) section.

3. `.comm foo,4,4`: Declares a common symbol `foo` with a size of 4 bytes and alignment of 4 bytes. Common symbols are global variables that may be defined in multiple translation units, and the final definition is resolved during linking.

4. `.globl stuff`: Declares the symbol `stuff` as a global symbol, meaning it can be accessed from other translation units.

5. `.data`: Indicates the start of the data section.

6. `.align 4`: Aligns the following data on a 4-byte boundary.

7. `.type stuff, @object`: Specifies that the symbol `stuff` is of type "object".

8. `.size stuff, 4`: Specifies the size of the `stuff` object as 4 bytes.

9. `stuff: .long 7`: Defines the `stuff` object and initializes it with the value 7.

10. `.text`: Indicates the start of the text (code) section again.

11. `.globl main`: Declares the symbol `main` as a global symbol.

12. `.type main, @function`: Specifies that the symbol `main` is of type "function".

13. `.LFB5`: Indicates the start of a function named `.LFB5`.

14. `.cfi_startproc`: Specifies the start of a new procedure for Call Frame Information (CFI).

15. `pushq %rbp`: Pushes the value of the base pointer onto the stack.

16. `.cfi_def_cfa_offset 16`: Defines the Canonical Frame Address (CFA) offset.

17. `.cfi_offset 6, -16`: Specifies the offset of the saved base pointer.

18. `movq %rsp, %rbp`: Moves the value of the stack pointer to the base pointer.

19. `.cfi_def_cfa_register 6`: Defines the base pointer as the Canonical Frame Address (CFA) register.

20. `subq $48, %rsp`: Allocates 48 bytes of space on the stack for local variables.

21. `movl %edi, -36(%rbp)`: Moves the value of the first function argument (`%edi`) to a specific location on the stack.

22. `movq %rsi, -48(%rbp)`: Moves the value of the second function argument (`%rsi`) to a specific location on the stack.

23. `movl $0, -20(%rbp)`: Moves the value 0 to a specific location on the stack.

24. `movl $100, %edi`: Moves the value 100 to the `%edi` register.

25. `call malloc@PLT`: Calls the `malloc` function to allocate memory for 100 bytes.

26. `movq %rax, -16(%rbp)`: Moves the returned value from `malloc` to a specific location on the stack.

27. `movl $3, %edi`: Moves the value 3 to the `%edi` register.

28. `call recur@PLT`: Calls the `recur` function.

29. `movl $0, %eax`: Moves the value 0 to the `%eax` register.

30. `leave`: Restores the stack frame by setting the stack pointer to the base pointer.

31. `.cfi_def_cfa 7, 



Then [the object accordingly(already processed by `objdump -D `)](materials/map.o) : 

1. Disassembly of section .text: This section contains the disassembly of the code section.
2. `<main>:`, `0: push %rbp`, `1: mov %rsp, %rbp`, etc.: These lines represent the disassembled instructions of the `main` function. Each line corresponds to a machine instruction along with its hexadecimal address.
3. `push %rbp`, `mov %rsp, %rbp`, `sub $0x30, %rsp`, etc.: These instructions set up the function's stack frame by saving the base pointer, adjusting the stack pointer, and allocating space for local variables.
4. `<stuff>:`, `0: 07`, `1: 00 00`, etc.: These lines represent the disassembly of the `.data` section. It shows the hexadecimal values of the bytes stored in the `stuff` object.
5. Disassembly of section .comment: This section contains some compiler and linker-generated comments.
6. Disassembly of section .eh_frame: This section contains information related to Exception Handling (EH) and stack unwinding.

or use `objdump -h` to show the ELF:

```
fudanicpc@cat:~/Desktop/student0-main/hw-intro$ objdump -h map.o

map.o:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000043  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000004  0000000000000000  0000000000000000  00000084  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  00000088  2**0
                  ALLOC
  3 .comment      00000025  0000000000000000  0000000000000000  00000088  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000ad  2**0
                  CONTENTS, READONLY
  5 .eh_frame     00000038  0000000000000000  0000000000000000  000000b0  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATAs
```

Here's an explanation of the columns:

- `Idx`: The index of the section.
- `Name`: The name of the section.
- `Size`: The size of the section in bytes.
- `VMA`: The virtual memory address of the section.
- `LMA`: The load memory address of the section.
- `File off`: The offset of the section in the file.(offset: an integer indicating the distance (displacement) between the beginning of the object and a given element or point, presumably within the same object.)
- `Algn`: The alignment requirement of the section.

Here's a breakdown of the sections in the `map.o` file:

1. `.text`(code/text segment): This section contains the executable code of the program. It is marked as CONTENTS, ALLOC, LOAD, RELOC, READONLY, and CODE. It has a size of `00000043` bytes.
2. `.data`: This section contains initialized global and static data. It is marked as CONTENTS, ALLOC, LOAD, and DATA. It has a size of `00000004` bytes.
3. `.bss`(Block Started by Symbol): This section represents uninitialized global and static data. It is marked as ALLOC. It has a size of `00000000` bytes.
4. `.comment`: This section contains compiler-generated comments. It is marked as CONTENTS and READONLY. It has a size of `00000025` bytes.
5. `.note.GNU-stack`: This section provides information about the GNU stack usage. It is marked as CONTENTS and READONLY. It has a size of `00000000` bytes.
6. `.eh_frame`: This section contains exception handling and stack unwinding information. It is marked as CONTENTS, ALLOC, LOAD, RELOC, READONLY, and DATA. It has a size of `00000038` bytes.

Also plz note that the output of `objdump -h` for an object file typically **does not include a specific section for the heap because the heap is managed by the dynamic memory allocation functions (`malloc`, `free`, etc.) provided by the C runtime library.**

In the output you provided for `map.o`, you can see sections such as `.text`, `.data`, `.bss`, `.comment`, `.note.GNU-stack`, and `.eh_frame`. These sections represent parts of the object file related to the program's code, initialized data, uninitialized data, comments, stack unwinding, and stack-related information.

The heap, on the other hand, is managed at runtime by the C runtime library using memory allocation functions like `malloc`. The memory allocated on the heap is not directly represented as a section in the object file itself. Instead, it is managed dynamically during program execution by the runtime environment.

Therefore, when inspecting the object file with `objdump`, you won't see a specific section dedicated to the heap. The allocation and management of heap memory occur during runtime and are not directly reflected in the object file.

Also:

```
fudanicpc@cat:~/Desktop/student0-main/hw-intro$ objdump -t map.o

map.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*	0000000000000000 map.c
0000000000000000 l    d  .text	0000000000000000 .text
0000000000000000 l    d  .data	0000000000000000 .data
0000000000000000 l    d  .bss	0000000000000000 .bss
0000000000000000 l    d  .note.GNU-stack	0000000000000000 .note.GNU-stack
0000000000000000 l    d  .eh_frame	0000000000000000 .eh_frame
0000000000000000 l    d  .comment	0000000000000000 .comment
0000000000000004       O *COM*	0000000000000004 foo
0000000000000000 g     O .data	0000000000000004 stuff
0000000000000000 g     F .text	0000000000000043 main
0000000000000000         *UND*	0000000000000000 _GLOBAL_OFFSET_TABLE_
0000000000000000         *UND*	0000000000000000 malloc
0000000000000000         *UND*	0000000000000000 recur
```

In this symbol table, each line represents a symbol entry. Here's an explanation of each column:

- The first column represents the symbol's address or offset. This is the first set of digits (all zeros in this case). In a relocatable object file, **these will typically be zeros because the final address hasn't been determined yet**. **Once the object file is linked into an executable or a library, each symbol will be given a unique address in memory where it resides.**
- The second column represents the symbol's visibility. `l` indicates a local symbol, and `g` indicates a global symbol. If empty, it means that the visibility of the symbol is not explicitly specified. 
- The third column represents the symbol's type. `F` indicates a function symbol, `O` indicates an object/data symbol, and `*UND*` indicates an undefined symbol. `d`indicates that the symbol is a debugging symbol.
- The fourth column represents the size. This value represents the size of the symbol in bytes. For example, for the `main` function, the size is `43`, which in hexadecimal translates to 67 bytes in decimal.
- The last column represents the symbol's name.

Based on this table, you can see the symbols `foo`, `stuff`, `main`, `_GLOBAL_OFFSET_TABLE_`, `malloc`, and `recur`, along with their respective attributes.

### Pj0: 

https://blog.csdn.net/ingsuifon/article/details/125507849

#### Find the Faulting Instruction

This is the pintos manual: https://cs162.org/static/proj/pintos-docs/

Do plz set the environment variable.
```
export PATH=/home/fudanicpc/Desktop/group0-main/src/utils:$PATH
export PATH=/home/fudanicpc/Desktop/student0-main/proj-pregame/src/utils:$PATH

fudanicpc@cat:~/Desktop/student0-main/proj-pregame/src/utils$ pwd
export PATH=/home/fudanicpc/Desktop/student0-main/proj-pregame/src/utils:$PATH

```

```
fudanicpc@cat:~/Desktop/group0-main/src/threads$ ../utils/pintos run alarm-multiple
warning: can't find squish-pty, so terminal input will fail
bochs -q
========================================================================
                        Bochs x86 Emulator 2.6
            Built from SVN snapshot on September 2nd, 2012
========================================================================
```

```bash
fudanicpc@cat:~/Desktop/group0-main/src/userprog$ make check
cd build && make check
make[1]: Entering directory '/home/fudanicpc/Desktop/group0-main/src/userprog/build'
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/do-nothing -a do-nothing -- -q   -f run do-nothing < /dev/null 2> tests/userprog/do-nothing.errors > tests/userprog/do-nothing.output
perl -I../.. ../../tests/userprog/do-nothing.ck tests/userprog/do-nothing tests/userprog/do-nothing.result
FAIL tests/userprog/do-nothing
Test output failed to match any acceptable form.

Acceptable output:
  do-nothing: exit(162)
Differences in `diff -u' format:
- do-nothing: exit(162)
+ Page fault at 0xc0000008: rights violation error reading page in user context.
+ do-nothing: dying due to interrupt 0x0e (#PF Page-Fault Exception).
+ Interrupt 0x0e (#PF Page-Fault Exception) at eip=0x80488ee
+  cr2=c0000008 error=00000005
+  eax=00000000 ebx=00000000 ecx=00000000 edx=00000000
+  esi=00000000 edi=00000000 esp=bfffffe4 ebp=bffffffc
+  cs=001b ds=0023 es=0023 ss=0023
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/stack-align-0 -a stack-align-0 -- -q   -f run stack-align-0 < /dev/null 2> tests/userprog/stack-align-0.errors > tests/userprog/stack-align-0.output
perl -I../.. ../../tests/userprog/stack-align-0.ck tests/userprog/stack-align-0 tests/userprog/stack-align-0.result
FAIL tests/userprog/stack-align-0
Test output failed to match any acceptable form.
```

```bash
fudanicpc@cat:~/Desktop/group0-main/src/userprog/build/tests/userprog$ objdump -d do-nothing | less
```

```assembly
080488e8 <_start>:
 80488e8:       55                      push   %ebp
 80488e9:       89 e5                   mov    %esp,%ebp
 80488eb:       83 ec 18                sub    $0x18,%esp
 80488ee:       8b 45 0c                mov    0xc(%ebp),%eax
 80488f1:       89 44 24 04             mov    %eax,0x4(%esp)
 80488f5:       8b 45 08                mov    0x8(%ebp),%eax
 80488f8:       89 04 24                mov    %eax,(%esp)
 80488fb:       e8 94 f7 ff ff          call   8048094 <main>
 8048900:       89 04 24                mov    %eax,(%esp)
 8048903:       e8 d3 21 00 00          call   804aadb <exit>
```

**Explanation:**

From the disassembled code you provided, it looks like the function the program was in when it crashed was `_start`.

The instruction that resulted in the crash appears to be at address `0x80488ee`:

```assembly
80488ee:       8b 45 0c                mov    0xc(%ebp),%eax    ;argv
```

This instruction is attempting to move the value at the memory location given by `(%ebp + 0xc)` into the `eax` register. Here, `(%ebp + 0xc)` is intended to represent the location of the second argument passed to `_start`, which should be `argv`, the pointer to the array of command-line argument strings.

It appears that there is a problem with this memory access, perhaps because the memory location at `(%ebp + 0xc)` is not mapped to a valid memory page or because the program doesn't have the correct permissions to access that location. This causes the page fault and leads to the segmentation fault.

It's also possible that the issue arises because the `_start` function is improperly handling its arguments. In a typical C environment, the operating system's kernel sets up the stack so that when `_start` is called, it can access `argc` (the count of command-line arguments) and `argv` (the array of command-line arguments). If Pintos is not correctly setting up the stack or if `_start` is not correctly accessing these values, that could lead to this sort of issue. 

#### Step through the crash



## MIT 6.S081

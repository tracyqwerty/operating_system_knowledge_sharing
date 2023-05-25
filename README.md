# Operating System Knowledge Sharing
OS self-learning material. Powered by ChatGPT4 >w<

## UCB CS162

Lectures: https://www.youtube.com/watch?v=4FpG1DcvHzc&list=PLMcZASLnrVs5qyBC-dc4bFRtC8ROPYmS0&index=2

Website: https://inst.eecs.berkeley.edu/~cs162/fa22/

BaseCode: https://github.com/Berkeley-CS162

### Hw1: Intro

Basic usage of c, combined with I/O, editor, gdb and other useful tools.

Personally I would recommend:

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

## MIT 6.S081

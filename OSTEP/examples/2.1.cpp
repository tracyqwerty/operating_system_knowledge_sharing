#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// on bash, run: gcc 2.1.c -Wall -o 2.1.out
// run one isntance: ./2.1.out "ABCDEF"
// run several at once: ./2.1.out "A" & ./2.1.out "BCDEF" &  ./2.1.out "GHIJKL"
// argument count & argument vector

int main(int argc, char *argv[]) {
  // printf("argc: %d\n", argc);
  if (argc != 2) {
    fprintf(stderr, "usage: ./2.1.out <string>\n");
    return 1;
  }

  char *str = argv[1];
  while (*str != '\0') {
    Spin(1);
    printf("%s\n", str);
    // str += sizeof(char);
  }
}

/*

question:
tracyqwerty@tracyqwertydezhuantou examples % ./2.1.out "AAAA" & ./2.1.out
"BCDEF" &  ./2.1.out "GHIJKL" [1] 43283 [2] 43284 AAAA
1
AAA
1
AA
1
A
1
[1]  - done       ./2.1.out "AAAA"
BCDEF
1
CDEF
1
DEF
1
EF
1
F
1
[2]  + done       ./2.1.out "BCDEF"
GHIJKL
1
HIJKL
1
IJKL
1
JKL
1
KL
1
L
1

why there are just 2 threads. i suppose there should be three

Answer:
The output you are seeing is actually from three separate processes, not
threads. Each invocation of `./2.1.out` starts a new process. In this case,
you've started three processes by running `./2.1.out "AAAA"`, `./2.1.out
"BCDEF"`, and `./2.1.out "GHIJKL"`.

The "&" character in the command line tells the shell to run the command in the
background. So each of these commands is running simultaneously in a separate
process. The shell also assigns a job number (like `[1]` and `[2]`) to each
background job.

As for why you're seeing the output intermingled like that, it's because output
to the terminal is not synchronized between different processes. So one process
may output some text, then another process may output some text, and so on. It's
also important to note that the order in which the processes are scheduled and
run is determined by the operating system's scheduler, so it's generally not
predictable.

In the end, you're seeing output from all three processes. The prompt `[1] -
done` and `[2] + done` means that the corresponding background jobs have
completed. The last process didn't get a job number printed because the shell
only prints job numbers when there's more than one active job.




There are a few different ways to halt (or stop, or terminate) programs that are
running in the background in a Unix-like terminal.

1. Use the `kill` command: To use the `kill` command, you first need to know the
process ID (PID) of the process you want to stop. You can find this using the
`ps` command, or, if you've just started the process, the shell will have
printed a message like `[1] 1234` where `1234` is the PID. Once you know the
PID, you can use the `kill` command to send a signal to the process, like this:
`kill 1234`. By default, `kill` sends the "TERM" (terminate) signal, which asks
the process to clean up and exit. If the process doesn't respond to the TERM
signal, you can send the "KILL" signal, which forces the process to stop
immediately: `kill -KILL 1234` or `kill -9 1234`.
2. Use the `fg` command: The `fg` command brings a background job to the
foreground. After you've brought a job to the foreground, you can use Ctrl+C to
send it the "INT" (interrupt) signal, which will usually cause the process to
stop. For example, if you've started a job in the background and the shell has
printed `[1] 1234`, then you can use the command `fg 1` to bring that job to the
foreground.
3. Use the `killall` command: If you have multiple instances of a program
running and you want to stop all of them, you can use the `killall` command
followed by the name of the program. For example, `killall myprogram` will send
the TERM signal to all running instances of `myprogram`.

Note: These are all ways to terminate a program, which means that the program
will stop running. If you want to pause a program (so you can continue it
later), you can use the "STOP" signal (`kill -STOP 1234`) and then later use the
"CONT" signal (`kill -CONT 1234`) to continue it.

*/
---
title: "Debugging in command line, Visual Studio, and Visual Studio Code"
keywords: VSCode
tags: [debug,visual studio,vscode]
permalink:  debugging.html
sidebar: home_sidebar
---

Using debugging is essential to catch elusive bugs! This page explain how to debug in command line, Visual Studio, and VSCode.

To debug

# Debugging in command line

## Run a program

The program used to debug c++ code is called gdb. You can run `gdb -h` to get summary of the command usage and on linux you can run `man gdb` to access the manual entry for gdb.
- Open a terminal (Powershell on Windows, any on Linux and MacOS)
- run 'gdb <your_program>`

  You will enter an interactive debugging session:
```bash
GNU gdb (Ubuntu 12.1-0ubuntu1~22.04.2) 12.1
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from cpg_test...
(gdb) 
```
- To run the program type `run`, to run with arguments type `run arg1 arg2 ...`
- The program will run until it finishes, if the program crashes you will be able to inspect the call stack and the variables.
- To inspect the call stack type: `backtrace` or `bt`, then to navigate the call stack type `up` or `down`
- To inspect a variable type `print <variable name>` or `explore <variable name>`

You can also directly start `gdb` with arguments with `gdb --args <your_program> arg1 arg2 ...`

## Set breakpoints

- To add a breakpoint, before running the program, type `break <line number>` or `break <file name:line number>.
- After running the program, the execution will pause at the set breakpoint. You can inspect the call stack and variables with same commands described in the previous section.
- To continue the execution simply type `continue`
- To continue the execution until a certain line, type `until <line number>`. 
- After a break, you can also execute the program step by step by typing `step`

**While debugging always pay attention in which file you currently  are.**

## Navigating the help menu in gdb's interactive session.


In the interactive session, you can type `help` to get the list of command classes.
```bash
List of classes of commands:

aliases -- User-defined aliases of other commands.
breakpoints -- Making program stop at certain points.
data -- Examining data.
files -- Specifying and examining files.
internals -- Maintenance commands.
obscure -- Obscure features.
running -- Running the program.
stack -- Examining the stack.
status -- Status inquiries.
support -- Support facilities.
text-user-interface -- TUI is the GDB text based interface.
tracepoints -- Tracing of program execution without stopping the program.
user-defined -- User-defined commands.
```
For instance, if you type `help breakpoints`, you will get the list of the breakpoints commands.



# Debugging in Visual Studio

You can refer to this [page](https://learn.microsoft.com/en-us/visualstudio/debugger/getting-started-with-the-debugger-cpp?view=visualstudio)

- To start debugging, type F5 or select Debug > Start Debugging


# Debugging in VSCode


![image](assets/images/debug-session.png)
You can refer to this [page](https://code.visualstudio.com/docs/debugtest/debugging)

- Open the source file that you want to debug
- You can set breakpoints by clicking the editor margin to the lines you want the execution to stop.
- Start debugging with F5, or open the Run and Debug view (Ctrl+Shift+D) and select Run and Debug.
- If prompted, select the debugger for your language or runtime.
    Per default VSCode will run the active file. You can create a launch.json file in .vscode folder to create custom configuration such as this one:
    ```json
    {
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "/home/leni/git/AREFramework/build/simulation/are-client",
            "args": ["/home/leni/git/AREFramework/experiments/me2im/parameters.csv", "10000", "1"],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                },
                {
                    "description": "Set Disassembly Flavor to Intel",
                    "text": "-gdb-set disassembly-flavor intel",
                    "ignoreFailures": true
                }
            ]
            }
    ]
    }
    ```



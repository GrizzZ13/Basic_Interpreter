# Mini Basic Interpreter

### Author : Wang Haotian, SJTU

## Commands

You can input `HELP` to get support

Legal commands are as follows:

```
HELP : Get help
RUN  : Run the code in buffer
LOAD : Load code from existing txt file
LIST : No longer supported
CLEAR: Clear buffer, global variables, results and syntax tree
QUIT : Quit mini basic interpreter
INPUT: Input variable
LET  : Assignment statement
PRINT: Print a constant number, a defined variable or a legal expression
```

## Statement

Legal statements are as follows:

```
10 INPUT m
20 LET m=10
30 IF m>=10 Then 50
40 END
50 PRINT m
60 GOTO 80
70 REM This line will be never executed
80 END
```

## Expression

By using stacks to construct expression tree, this basic interpreter can do some simple integer calculations but overflow exception isn't handled yet. This interpreter can do negative number calculation.

## Operands

Legal operands are as follows:

```
=  : assignment
+  : add
-  : subtract
*  : multiply
/  : divide
** : power
== : equal
>= : greater equal
<= : less equal
```

## Bugs

There may be some bugs, and I'll be thankful if anyone can figure them out.
# Overview
Shell, tokenizer, parser and interpreter for a custom programming language, loosely based on BASIC.
Not useful as a programming language, full of bugs, just an exercise. My next programming language project will be more solid now that I've grasped the basics through trial and error. For this to be usable it would require a major rewrite.

# Setup
The makefile uses clang, but the code is compatible with gcc, simply switch the value of the CC variable in the Makefile to be equal to gcc instead of clang.
To compile the project, simply manouver into the main directory on your computer and use make:

```
make
```

Then, to enter the shell (only supported operation mode at the moment, no file inputs yet) type in

```
./bin/exe
```

# How it works
Since this is a relatively big project for one person, I built the whole thing to work on simple expressions and then expanded the ability in steps.
Quick note: the hash symbol ('#') signifies end of file for the shell. It tells the program when to stop reading user input.


## Arithmetic
I started by adding some arithmetic expressions and making sure the precedence was all right.

<img width="287" alt="Screenshot 2022-02-06 at 18 16 32" src="https://user-images.githubusercontent.com/81037885/152692562-ce881a29-e980-422f-b172-f32e0f5ed96d.png">

Added some parentheses.

<img width="288" alt="Screenshot 2022-02-06 at 18 18 03" src="https://user-images.githubusercontent.com/81037885/152692604-dd30515b-0408-447b-9ab4-0ffd5f20a6c1.png">

Also works with the unary minus operator.

<img width="288" alt="Screenshot 2022-02-06 at 18 22 18" src="https://user-images.githubusercontent.com/81037885/152692916-6722c654-b477-41cb-92d7-06bbf0e9c2ed.png">

I then added some logical operators

<img width="286" alt="Screenshot 2022-02-06 at 18 19 28" src="https://user-images.githubusercontent.com/81037885/152692661-1df2308f-271d-4574-89e2-293e66216a1c.png">

note: true, false are built-in keywords, signifying 1 and 0 respectively

And with those logical operators came the comparison operators. Those return either 1 or 0 so they can be used in expressions, like so:

<img width="288" alt="Screenshot 2022-02-06 at 18 24 59" src="https://user-images.githubusercontent.com/81037885/152693020-038cfe63-039e-447e-b828-e970da269ae4.png">

## Typing
Everything within the language is an integer or a float and the interpreter can switch (one-way) between them and can tell them apart when it needs to. For instance:

<img width="288" alt="Screenshot 2022-02-06 at 18 31 18" src="https://user-images.githubusercontent.com/81037885/152693289-e286e013-c240-4618-ade6-44032d818f84.png">

However note that integer division is still possible (and default):

<img width="288" alt="Screenshot 2022-02-06 at 18 32 07" src="https://user-images.githubusercontent.com/81037885/152693316-fdec3c94-8aa9-45e6-9742-7bc9f79d820a.png">

And an error wil be thrown upon encountering a float in a mod equation:

<img width="385" alt="Screenshot 2022-02-06 at 18 34 49" src="https://user-images.githubusercontent.com/81037885/152693411-7d2ce2a0-d6db-4816-af78-974a76d005b1.png">

## Variables
A variable is declared with the var keyword and accessed via the variable name. All variables are mutable and must be initialized at declaration.

<img width="288" alt="Screenshot 2022-02-06 at 18 38 06" src="https://user-images.githubusercontent.com/81037885/152693536-ac05512b-c557-4d15-98e5-34d5f2f4b79e.png">

Variables are stored in variable tables at runtime which belong to a given context, meaning a variable within a function cannot be accessed outside of that function. Same goes for if-else statements and loops (more on that later).
## If-else statements
An if-statement is started with the if keyword, followed by a condition, a 'then' keyword and then some statements in brackets (optional if the entire if-statement is inline). An elif keyword can be used next (also followed by the then keyword) and an else keyword. 

<img width="288" alt="Screenshot 2022-02-06 at 18 45 35" src="https://user-images.githubusercontent.com/81037885/152693877-7276bb0e-e9b7-45d7-a417-9ce21b9a5d82.png">

An inline if-statement can be used without brackets, permitting only one expression after each 'then' keyword.

## Loops
Only one type of loop is permitted: the while loop. It must be supplied with a condition and a statement to execute if that condition is true.

<img width="288" alt="Screenshot 2022-02-06 at 18 49 40" src="https://user-images.githubusercontent.com/81037885/152694053-01ae0f19-d08d-421b-9b3a-4b6c14a95e18.png">

## Functions
Functions are declared with a 'func' keyword, function name, arguments in parentheses, followed by an arrow and some statements in braces.

<img width="288" alt="Screenshot 2022-02-06 at 18 52 23" src="https://user-images.githubusercontent.com/81037885/152694165-12a07942-075e-4b7a-9c8a-3b0c3f417da5.png">

## Error management
The error management system is divided into two main parts: the warning section and the error section. Warnings show up without stopping the program, while errors result in relaunching the shell, effectively never finishing the task which led to the error.
Each stage of interpreting has its own error and warning types. Below are some common examples.'

#### Lexer warning

<img width="288" alt="Screenshot 2022-02-06 at 17 28 26" src="https://user-images.githubusercontent.com/81037885/152690802-8cb8a8a3-cd08-442d-8c06-146c3b22e288.png">

#### Lexer error

<img width="288" alt="Screenshot 2022-02-06 at 17 46 17" src="https://user-images.githubusercontent.com/81037885/152691475-3053cdaf-d031-4609-a074-11922a901cc3.png">

#### Parser error

<img width="286" alt="Screenshot 2022-02-06 at 17 48 45" src="https://user-images.githubusercontent.com/81037885/152691573-1f307ae2-3002-44b7-8253-2c46fd624758.png">

### Runtime error, traceback
In this example, we get a runtime division by zero error.

<img width="287" alt="Screenshot 2022-02-06 at 17 52 25" src="https://user-images.githubusercontent.com/81037885/152691691-82490756-4b91-4a0e-90d4-b729d0d1c5d5.png">

The traceback feature gets more sophisticated than that though.

<img width="287" alt="Screenshot 2022-02-06 at 18 07 38" src="https://user-images.githubusercontent.com/81037885/152692208-bb4731eb-1a52-4d18-b791-54ba30c29119.png">

 
And that's pretty much the whole thing. Feel free to play around with it, though I consider the project to be over since the code would require (and isn't worth) a rewrite for any substantial improvement. It's served its educational/entertainment purpose, now I'm on to do another one, hopefully better this time.

# The Silly Game Console
*The Silly Game Console* is a ~~(fictional)~~ gaming console which released in Japan in 19XX, and internationnally the following year.
This machine is not very known nor remembered today, mainly due to its eccentric design choices:
- 7-bit registers
- separate game and graphics cartridges, which needed to be bought separately
- unusual controller shape

Few games were ever released for this console.  
In cause, the instruction set of the *The Silly Game Console*'s processor is very unusual, scaring a lot of developpers away.  


# Registers
`x` : 7-bit register used for all memory related operations (`ldx`, `read`, `write`)  
`y` : 7-bit register, only accessible via `swapXY`  
`a` : 7-bit register used for all arithmetic operations  
`d` : 14-bit register used as the address for all memoryh related operations  
`pc` : 14-bit register serving as program counter, not directly accessible. Starts with the value `$2000`  
`sp` : 14-bit register serving as stack pointer (the stack is stored in ram from $1EFF downwards)  

# Memory Map
```
RAM : $0000 -> $1EFF
ROM : $2000 -> 3FFFF
```
## Special Addresses
`$1F80 - $1F8F | RW` : Place in which to store Sprite information (x, y, texture_index, color_palette)  
`$1FF0 | R_` : Force the console to render the game, which then waits a minimum of 1/5 seconds  
`$1FFE | RW` : Place to store the number of sprites that need to be rendered  
`$1FFF | R_` : Accesses the controller's button status `U D L R A B S`  

# Instructions
|Instruction|OpCode|Length|Effect|Note|
|--|--|--|--|--|
| `NOP` | 0 | 1 |  |  |
| `READ` | 1 | 1 | `x = mem[d]` |  |
| `WRITE` | 2 | 1 | `mem[d] = x` |  |
| `SAVE` | 3 | 3 | writes in little endian `d` at `mem[operand]` |  |
| `RESTORE` | 4 | 3 | restores `d` to the little endian value at `mem[operand]` |  |
| `INCD` | 5 | 1 | `d++` |  |
| `DECD` | 6 | 1 | `d--` |  |
| `LDD` | 7 | 3 | `d = operand` |  |
| `INC` | 8 | 1 | `a++` |  |
| `DEC` | 9 | 1 | `a--` |  |
| `LDX` | 10 | 2 | `x = operand` |  |
| `SWAPXY` | 11 | 1 | `x <-> y` |  |
| `DUPEXY` | 12 | 1 | `y = x` |  |
| `SWAPXA` | 13 | 1 | `x <-> a` |  |
| `DUPEXA` | 14 | 1 | `a = x` |  |
| `PUSH` | 15 | 1 | pushes `x` on stack |  |
| `POP` | 16 | 1 | pops from stack into `x` |  |
| `LEAP` | 17 | 2 | `pc = pc + operand` | `operand` is in bytes, not in instructions (ex: `LDD` counts as 3, not 1) |
| `CALL` | 18 | 3 | pushes `pc` onto the stack (little endian), then `pc = operand` | `d` is not preserved accross calls |
| `RET` | 19 | 1 | `pc = pop()` (little endian) |  |
| `ADD` | 20 | 1 | `a += x` |  |
| `SUB` | 21 | 1 | `a -= x` |  |
| `MUL` | 22 | 1 | `a *= x` |  |
| `MOD` | 23 | 1 | `a %= x` |  |
| `AND` | 24 | 1 | `a &= x` |  |
| `XOR` | 25 | 1 | `a ^= x` |  |
| `OR` | 26 | 1 | `a \|= x` |  |
| `NOT` | 27 | 1 | `a = !a` | /!\ not bitwise |

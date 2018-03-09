## Exploits in C Project Proposal
Kai Levy and Patrick Huston | Software Systems Spring 2018
### Goal

In this project, we hope to explore the common methods of software exploitation in C. In specific, we want to understand the basis of how it works, what makes software vulnerable, and the steps that can be taken to ensure safety from a developer standpoint. 

### Learning Goals

  - Learn about real security vulnerabilities that can occur in common programs, how to exploit them, and how to avoid them. 
  - Research and learn about vulnerabilities to real software - understanding what the issue was, how it was exploited, and how it was fixed.
  - Develop simple and effective C programs

### Exercises Completed
So far, we've completed exercises that have allowed us to execute arbitrary code through vulnerabilities.  
1. Walked through a number of tutorials on common exploits in C. These exercises are from the [Protostar series](https://exploit-exercises.com/protostar/) of tutorials. The topics we've covered so far include:
  - Buffer overflows/stack smashing
  - String formatting exploits
  - Heap overflows
2. Researched C implementations of common Unix command line tools. Our next steps (outlined below) will focus on these tools.

### Next Steps
Now that we've gained a solid background of common exploits, we plan on shifting our focus to implementation. Our goal is to implement simple versions of common Unix tools (e.g. `cat`, `head`, `tail`) with vulnerabilities embedded. Next, we'll write scripts to exploit these implementations and document our process. Finally, we'll patch the programs and demonstrate what it takes to write safe C code. 

### Resources
Tutorials on writing exploits in C:
https://www.corelan.be/index.php/2009/07/19/exploit-writing-tutorial-part-1-stack-based-overflows/
http://insecure.org/stf/smashstack.html
We would additionally be interested in further resources recommended by a professor.

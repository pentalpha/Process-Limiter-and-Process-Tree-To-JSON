Process limiter:
A small header who enables your program to control the maximum number of child
processes that can be created. Any process who exceeds that maximum receives a
SIGTERM.

void limitProcs(int n):
  n   Maximum number of child processes. Does not include the process created by
      "limitProcs" itself, or the father process who called limitProcs.

Be aware that the "process cleaning" is linear, so if your program is creating
new processes unstoppably (A.K.A. fork-bomb), limitProcs may not be capable
of handling everything.

To use: include processLimiter.h
--------------------------------------- 
PsJSON:
This small program efficiently outputs all the current processes being run on
the machine in JSON format.

To make: make psjson

--------------------------------------
Anti-bomb:
An executable made to easily change the system's process limits for a user.

Usage:
anti-bomb [user or group] [nProcesses]
  Limit processes to nProcesses permanently in user or group.
anti-bomb restore
  Restore old configuration of process limits.

To make: make anti-bomb

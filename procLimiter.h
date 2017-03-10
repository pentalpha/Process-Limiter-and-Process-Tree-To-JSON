/*
Pitágoras Alves & André Winston, UFRN, March 2016

A small header who enables your program to control the maximum number of child
processes that can be created. Any process who exceeds that maximum receives a
SIGTERM.

void limitProcs(int n):
  n   Maximum number of child processes. Does not include the process created by
      "limitProcs" itself, or the father process who called limitProcs.

Be aware that the "process cleaning" is linear, so if your program is creating
new processes unstoppably (A.K.A. fork-bomb), limitProcs may not be capable
of handling everything.
*/

#ifndef _PROCLIMITER_
#define _PROCLIMITER_

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <set>
#include <signal.h>
#include <sys/wait.h>
#include "tinydir.h"
#include <sys/time.h>
#include <sys/resource.h>

typedef tinydir_dir Dir;

using namespace std;

int* myLimit;
set<int> childs;
set<int> childsKilled;
int monitorID;

void limitProcs(int n);
void startLimiting(int father);
void scanProcsForChildren(int father);
void addChildToList(int newID);
bool fatherProcExists(pid_t pid);

void limitProcs(int n){
  myLimit = new int(n);
  int father = getpid();
  int pid = fork();
  if(pid == 0){
    monitorID = getpid();
    startLimiting(father);
  }else{
    return;
  }
}

void startLimiting(int father){
  while(fatherProcExists(father)){
      scanProcsForChildren(father);
  }
  exit(0);
}

void scanProcsForChildren(int father){
  tinydir_dir dir;
  tinydir_open(&dir, "/proc");

  while(dir.has_next){
    tinydir_file file;
    tinydir_readfile(&dir, &file);
    if(file.is_dir){
      string fileName = file.name;
      try{
        int idFound = stoi(fileName);
        int group = getpgid(idFound);
        if(group == father
          && idFound != father
          && idFound != monitorID){
            if(childs.count(idFound) == 0){
              addChildToList(idFound);
            }
        }
      }catch(...){

      }
    }
    tinydir_next(&dir);
  }

  tinydir_close(&dir);
}

void addChildToList(int newID){
  if(childsKilled.count(newID) > 0){
    return;
  }

  if(childs.size() < *myLimit){
    cout << "Welcome " << newID << "!!!" << endl;
    childs.insert(newID);
  }else{
    kill(newID, SIGTERM);
    cout << "[" << newID << "] Killed" << endl;
    childsKilled.insert(newID);
  }
}

bool fatherProcExists(pid_t pid){
  int exists = false;
  if(getppid() == pid){
    exists = true;
  }
  return exists;
}

#endif

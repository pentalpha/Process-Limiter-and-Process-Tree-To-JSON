/*
Pitágoras Alves & André Winston, UFRN, March 2017.

An executable made to easily change the system's process limits for a user.

Usage:
anti-bomb [user or group] [nProcesses]
  Limit processes to nProcesses permanently in user or group.
anti-bomb restore
  Restore old configuration of process limits.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include "run.h"

using namespace std;

string confPath = "/etc/security/limits.conf";
string confPathTemp = "/etc/security/limits.conf.temp";
string confPathBackup = "/etc/security/limits.conf.old";

void replaceLimits(string user, string nProc);
bool replaceConfigLine(string user, string nProc);
void restore();
void printUsage();

int main(int argc, char const *argv[]) {
  if(argc == 2){
    string restoreArg(argv[1]);
    if(restoreArg == "restore"){
      restore();
    }else{
      printUsage();
    }
  }else if(argc == 3){
    string nProc(argv[2]);
    string user(argv[1]);
    replaceLimits(user, nProc);
  }else{
    printUsage();
  }

  return 0;
}

void restore(){
  vector<string> commands;
  commands.push_back("sudo rm -f /etc/security/limits.conf");
  commands.push_back("sudo mv /etc/security/limits.conf.old /etc/security/limits.conf");
  runWhileSilent(commands);
}

void printUsage(){
  cout << "Usage:\nanti-bomb [user or group] [nProcesses]" << endl
        << "\t Limit processes to nProcesses permanently in user or group." << endl
        << "anti-bomb restore" << endl
        << "\t Restore old configuration of process limits." << endl;
}

void replaceLimits(string user, string nProc){
  if(replaceConfigLine(user, nProc)){
    vector<string> commands;
    commands.push_back("sudo rm -f /etc/security/limits.conf.old");
    commands.push_back("sudo mv /etc/security/limits.conf /etc/security/limits.conf.old");
    commands.push_back("sudo rm -f /etc/security/limits.conf");
    commands.push_back("sudo mv /etc/security/limits.conf.temp /etc/security/limits.conf");
    commands.push_back("sudo rm -f /etc/security/limits.conf.temp");
    runWhileSilent(commands);
  }
}

bool replaceConfigLine(string user, string nProc){
  ifstream filein(confPath.c_str()); //File to read from
  ofstream fileout(confPathTemp.c_str()); //Temporary file
  if(!filein)
  {
      cout << "Error opening /etc/security/limits.conf" << endl;
      return false;
  }else if (!fileout){
    cout << "Error opening /etc/security/limits.conf.temp" << endl;
    return false;
  }

  string strTemp;
  while(!filein.eof())
  {
    getline(filein, strTemp);
    if(strTemp.size() > 0){
      if(strTemp.at(0) == '#'){
        //cout << "comment, ignoring: " << strTemp << endl;
        fileout << strTemp << endl;
      }else{
        size_t pos = strTemp.find(user);
        if(pos == string::npos){
          //cout << "config on another user, ignoring: " << strTemp << endl;
          fileout << strTemp << endl;
        }else{
          pos = strTemp.find("nproc");
          if(pos == string::npos){
            //cout << "another type of config, ignoring: " << strTemp << endl;
            fileout << strTemp << endl;
          }
        }
      }
    }else {
      fileout << endl;
    }

  }
  string newLine = user;
  newLine += "\t-\tnproc\t";
  newLine += nProc;
  //cout << "new config line: " << newLine << endl;
  fileout << newLine << endl;
  return true;
}

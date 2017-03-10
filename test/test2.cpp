/*
 * Universidade Federal do Rio Grande do Norte
 * Instituto Metropole Digital
 *
 * copyright: Ivanovitch Silva
 * e-mail: ivan@imd.ufrn.br
 *
 * Tópico: manipulação de processos
 * Programa: informar o pid do processo e do seu respectivo pai.
 * Conferir a respostar digitando no terminal "ps -c $pid"  ou "ps -c $ppid"
 */


#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <errno.h>
#include "../procLimiter.h"

using namespace std;

int main(int argc, char *argv[])
{
    limitProcs(3);

    pid_t fatherID;

    //Identificador do processo
    pid_t id;

    //Obtendo o identificador do pai
    fatherID = getpid();
    cout << "Initial process: " << fatherID << "\n";
    pid_t forkRes = fork();

    if(forkRes == 0){
      cout << "[" << getpid() << "] in " << getpgid(getpid()) << " Started" << endl;
      sleep(6);
      cout << "[" << getpid() << "] Ending" << endl;
      return 0;
    }else{
      sleep(1);
      forkRes = fork();
      if(forkRes == 0){
        cout << "[" << getpid() << "] in " << getpgid(getpid()) << " Started" << endl;
        sleep(6);
        cout << "[" << getpid() << "] Ending" << endl;
        return 0;
      }else{
        sleep(1);
        forkRes = fork();
        if(forkRes == 0){
          cout << "[" << getpid() << "] in " << getpgid(getpid()) << " Started" << endl;
          sleep(6);
          cout << "[" << getpid() << "] Ending" << endl;
          return 0;
        }else{
          sleep(1);
          forkRes = fork();
          if(forkRes == 0){
            cout << "[" << getpid() << "] in " << getpgid(getpid()) << " Started" << endl;
            sleep(6);
            cout << "[" << getpid() << "] Ending" << endl;
            return 0;
          }else{
            sleep(1);
            forkRes = fork();
            if(forkRes == 0){
              cout << "[" << getpid() << "] in " << getpgid(getpid()) << " Started" << endl;
              sleep(6);
              cout << "[" << getpid() << "] Ending" << endl;
              return 0;
            }else{
              sleep(11);
              cout << "[" << getpid() << "](Father) in " << getpgid(getpid()) << " Ending" << endl;
            }
          }
        }
      }
    }
    return 0;
}

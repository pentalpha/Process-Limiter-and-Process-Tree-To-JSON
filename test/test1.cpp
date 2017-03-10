#include <unistd.h>
#include <iostream>
#include "../procLimiter.h"

using namespace std;

int main(int argc, char const *argv[]) {
  limitProcs(4);
  while(true){
    cout << "[" << getppid() << "] Started" << endl;
    sleep(2);
    fork();
  }
  return 0;
}

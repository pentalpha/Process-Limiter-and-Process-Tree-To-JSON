/*
André Winston & Pitágoras Alves, UFRN, March 2017

This small program efficiently outputs all the current processes being run on
the machine in JSON format.
*/

#include <bits/stdc++.h>
using namespace std;

string run(const char* command){
  int bufferSize = 128;
  char buff[bufferSize];
  string output = "";
  FILE *procStream = popen(command, "r");

  if(procStream == NULL){
    throw std::runtime_error("Could not get process output");
  }else{
    try{
      while (!feof(procStream)){
          if (fgets(buff, bufferSize, procStream) != NULL){
              output += buff;
          }
      }
    }catch(...){
      pclose(procStream);
      throw std::runtime_error("Error while getting output of process");
    }
    pclose(procStream);
    return output;
  }

}

struct processo{
	string nome;
	string pid;
	vector<processo> filhos;
};

map<string, vector<string> > arvore;
map<string, bool> vis;
string pgrep = "pgrep -P";
string pgrep1 = "pgrep -P";

string getname(string pid){
	if(pid == "0") return "\n";
	string str = "ps -p " + pid + " -o comm=";
	return run(str.c_str());
}

string tabs(int k){
	string a = "";
	for(int i = 0; i < k; i++) a += "\t";
	return a;
}

void printtabs(int k){
	for(int i = 0; i < k; i++) printf("\t");
}

processo dfs(string v, int h, bool temprox){
	processo p;
	p.pid = v;
	p.nome = getname(v);
	//cout << "comando = " << pgrep.append(v).c_str() << endl;
	printtabs(h);
	printf("\"%s\": {\n", p.pid.c_str());
	//cout << tabs(h) << "\""<<p.nome.substr(0, p.nome.size()-1)<<"\": {\n";
	printtabs(h+1);
	printf("\"nome\": \"%s\",\n", p.nome.substr(0, p.nome.size()-1).c_str());
	//cout << tabs(h+1) << "\"pid\": " << p.pid << ",\n";
	printtabs(h+1);
	printf("\"filhos\": {\n");
	//cout << tabs(h+1) << "\"filhos\": {\n";
	vis[v] = true;
	stringstream ss;
	ss.str(run(pgrep.append(v).c_str()));
	pgrep = pgrep1;
	string filho;
	vector<string> aux;
	bool read = (ss>>filho);
	while(read){
		string prox;
		read = (ss>>prox);
		p.filhos.push_back(dfs(filho, h+2, read));
		if(!read) break;
		filho = prox;
	}
	printtabs(h+1);
	printf("}\n");
	//cout << tabs(h+1) << "}\n";
	printtabs(h);
	printf("}");
	//cout << tabs(h) << "}";
	if(temprox) puts(",");
	//cout << endl;
	puts("");
	return p;
}

int main(){
	cout << "{\n";
	string v = "0";
	processo p = dfs(v, 1, 0);
	cout << "}\n";
}

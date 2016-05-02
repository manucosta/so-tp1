#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
  // MFQ recibe los quantums por parÃ¡metro
  std::queue<int> vacio;
  for(int i = 0; i < argn.size(); i++){
	  def_quantum.push_back(argn[i]);
	  vq.push_back(vacio);
  }
  cur_pri = 0;
  quantum = def_quantum[cur_pri];
  n = vq.size();
}

void SchedNoMistery::load(int pid) {
	vq[0].push(pid);
  unblock_to.push_back(0);
}

void SchedNoMistery::unblock(int pid) {
  int pri = unblock_to[pid];
  vq[pri].push(pid);
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
	switch(m) {
    case TICK:
      if(current_pid(cpu) == IDLE_TASK){  //separamos esto porque no queremos encolar idle
        return next();
      }else if(quantum == 1) { //uso todo su quantum, sacarlo y poner otro de la cola.
        if(cur_pri < n-1){
			   vq[cur_pri+1].push(current_pid(cpu));
        }else{
			   vq[cur_pri].push(current_pid(cpu));
		    }
        return next();
      } else { //no uso todo su quantum, restarle 1 al quantum
        quantum--;
        return current_pid(cpu);
      }
    
    case BLOCK:
      unblock_to[current_pid(cpu)] = (cur_pri == 0 ? 0 : cur_pri - 1);
      return next();
    
    case EXIT:
      return next();
  }
  cerr << "Motivo inválido para invocar a la función tick" << endl;
  return -1;
}

int SchedNoMistery::next(void) {
  // Elijo el nuevo pid
  for(int i = 0; i < n; i++){
	  if(!vq[i].empty()){
		  quantum = def_quantum[i];
		  cur_pri = i;
		  int pid = vq[i].front();
		  vq[i].pop();
		  return pid;
	  }
  }
  return IDLE_TASK;
}

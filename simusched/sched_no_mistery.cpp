#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
  // MFQ recibe los quantums por parÃ¡metro
  std::queue<int> vacio;
  for(int i = 0; i < (int)argn.size(); i++){ //Por default viene el parametro 1 de la primera cola
	  def_quantum.push_back(argn[i]); //Agrego cada quantum de las colas en en def_quantum
	  vq.push_back(vacio); //Genero una cola vacia para poder accederla más tarde
  }
  cur_pri = 0; //la primera prioridad es la de quantum 1
  quantum = def_quantum[cur_pri];
  n = vq.size(); //para usar n en los for
}

void SchedNoMistery::load(int pid) { //solo hago load cuando entra por primera vez
	vq[0].push(pid); //Lo agrego a la primera cola, la de más prioridad
	unblock_to.push_back(0); //genero el espacio para cuando se bloquee
}

void SchedNoMistery::unblock(int pid) {
  int pri = unblock_to[pid]; //Obtengo la prioridad de la pocision de la tarea en unblock_to
  vq[pri].push(pid); //Lo pongo en la cola
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
	switch(m) {
    case TICK:
      if(current_pid(cpu) == IDLE_TASK){  //separamos esto porque no queremos encolar idle
        return next();
      }else if(quantum == 1) { //uso todo su quantum, sacarlo y poner otro de la cola.
        if(cur_pri < n-1){ //Como se termino el quantum lo cambio a la siguente cola de prioridad
			   vq[cur_pri+1].push(current_pid(cpu));
        }else{ //Como esta en la ultima se mantiene
			   vq[cur_pri].push(current_pid(cpu));
		    }
        return next();
      } else { //no uso todo su quantum, restarle 1 al quantum
        quantum--;
        return current_pid(cpu);
      }
    
    case BLOCK:
      unblock_to[current_pid(cpu)] = (cur_pri == 0 ? 0 : cur_pri - 1); // agrego en la posición del la tarea, segun su pid, la prioridad que le toca
      return next();
    
    case EXIT:
      return next();
  }
  cerr << "Motivo inválido para invocar a la función tick" << endl;
  return -1;
}

int SchedNoMistery::next(void) {
  // Elijo el nuevo pid
  for(int i = 0; i < n; i++){ //Siempre reviso desde la de prioridad más alta hasta la de más baja
	  if(!vq[i].empty()){ //Cuando encuentro una cola con tareas busca la primera de la cola
		  quantum = def_quantum[i]; //renuevo el quantum
		  cur_pri = i; //actualizo la prioridad
		  int pid = vq[i].front();
		  vq[i].pop();
		  return pid;
	  }
  }
  return IDLE_TASK;
}

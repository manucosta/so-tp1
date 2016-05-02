#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
  // Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
  // Cuando se llama para graficar, no tenes que volver a poner los nucleos 
  // en la parte de los parametros del scheduler, solo le pasas los quantums. La cant
  // de nucleos la saca del segundo parametro, dsp del nombre del lote a utilizar.
  cant_cores = argn[0];
  for(int i = 1; i <= cant_cores; i++) {
    quantum_original_cpu.push_back(argn[i]); //lleno el array de cpu_quantum con los quantums de cada core.
    quantum_restante_cpu.push_back(argn[i]); //le pongo todo el quantum a cada cpu.
  }
}

SchedRR::~SchedRR() {}

void SchedRR::load(int pid) {
  cola_procesos.push(pid);
}

void SchedRR::unblock(int pid) {
  load(pid); // si se desbloquea, lo vuelvo a cargar.
}

int SchedRR::tick(int cpu, const enum Motivo m) {
  switch(m) {
    case TICK:
      if(current_pid(cpu) == IDLE_TASK) {
        //separamos esto porque no queremos encolar idle, ni tiene una duración determinada        
        return next(cpu);
      }else if(quantum_restante_cpu[cpu] == 1) { 
        //uso todo su quantum, sacarlo y poner otro de la cola.
        cola_procesos.push(current_pid(cpu)); //encolo al proceso pues todavía no terminó
        return next(cpu);
      } else { //no uso todo su quantum, restarle 1 al quantum
        quantum_restante_cpu[cpu]--;
        return current_pid(cpu);
      }
    
    case BLOCK:
      return next(cpu);
    
    case EXIT:
      return next(cpu);
  }
  cerr << "Motivo inválido para invocar a la función tick" << endl;
  return -1;
}

int SchedRR::next(int cpu) {
  int siguiente = IDLE_TASK;
  if(!cola_procesos.empty()) {
    quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];
    siguiente = cola_procesos.front();
    cola_procesos.pop();
  }

  return siguiente;
}
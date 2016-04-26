#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

//es para testear, dsp borrar!
void SchedRR::mostrarEstructura() {
  /*for(int i = 0; i < cant_cores; i++) {
    cout << "quantum original cpu " << i << ": " << quantum_original_cpu[i] << endl;
    cout << "quantum restante cpu " << i << ": " << quantum_restante_cpu[i] << endl;
    cout << "pid_actual cpu " << i << ": " << pid_actual_cpu[i] << endl;
  }*/
}

SchedRR::SchedRR(vector<int> argn) {
  // Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
  // (Eze) Cuando llamas para graficar, no tenes que volver a poner los nucleos 
  // en la parte de los parametros del scheduler, solo le pasas los quantums. la cant
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
  int siguiente;
  switch(m) {
    case TICK:
      if(current_pid(cpu) == IDLE_TASK){  //separamos esto porque no queremos encolar idle
        quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];
        siguiente = next(cpu);
      }else if(quantum_restante_cpu[cpu] == 1) { //uso todo su quantum, sacarlo y poner otro de la cola.
        cola_procesos.push(current_pid(cpu)); //encolo al proceso pues todavía no terminó
        quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];
        siguiente = next(cpu);
      } else { //no uso todo su quantum, restarle 1 al quantum
        siguiente = current_pid(cpu);
        quantum_restante_cpu[cpu]--;
      }
      break;
    
    case BLOCK:
      quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];  
      siguiente = next(cpu);
      break;
    
    case EXIT:
      quantum_restante_cpu[cpu] = quantum_original_cpu[cpu]; //recargo el quantum
      siguiente = next(cpu);
      break;
  }
  return siguiente;
}

int SchedRR::next(int cpu) {
  int siguiente = IDLE_TASK;
  if(!cola_procesos.empty()) {
    siguiente = cola_procesos.front();
    cola_procesos.pop();
  }

  return siguiente;
}
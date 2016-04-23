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
	int siguiente = current_pid(cpu);
	switch(m) {
		case TICK:
			if(current_pid(cpu) == IDLE_TASK){	//separamos esto porque no queremos encolar idle
				if(!cola_procesos.empty()) {
					quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];
					siguiente = cola_procesos.front(); //notar que por lo anterior la cola esta vacia
					cola_procesos.pop();
				}
			}else if(quantum_restante_cpu[cpu] == 1) { //uso todo su quantum, sacarlo y poner otro de la cola.
				quantum_restante_cpu[cpu] = quantum_original_cpu[cpu]; //recargo el quantum
				cola_procesos.push(current_pid(cpu)); //encolo al proceso
				siguiente = cola_procesos.front(); //notar que por lo anterior la cola esta vacia
				cola_procesos.pop();
			} else { //no uso todo su quantum, restarle 1 al quantum
				quantum_restante_cpu[cpu]--;
			}
			break;
		
		case BLOCK:
			quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];	
			if(!cola_procesos.empty()) { //si hay una tarea en la cola de procesos, asignarsela a esta cpu y quitarla de la cola
				siguiente = cola_procesos.front();
				cola_procesos.pop();
			} else {
				siguiente = IDLE_TASK;
			}
			break;
		
		case EXIT:
			quantum_restante_cpu[cpu] = quantum_original_cpu[cpu]; //recargo el quantum
			if(!cola_procesos.empty()) { //si hay una tarea en la cola de procesos, asignarsela a esta cpu y quitarla de la cola
				siguiente = cola_procesos.front();
				cola_procesos.pop();
			} else {
				siguiente = IDLE_TASK;
			}
			break;
	}
	return siguiente;
}

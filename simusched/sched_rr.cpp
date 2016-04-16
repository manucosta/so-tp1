#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cant_cores = argn[0];
	for(int i = 0; i < cant_cores; i++) {
		quantum_original_cpu.pushback(argn[i+1]); //lleno el array de cpu_quantum con los quantums de cada core.
		quantum_restante_cpu.pushback(argn[i+1]); //le pongo todo el quantum a cada cpu.
		pid_actual_cpu.pushback(IDLE_TASK); //le pongo IDLE_TASK a cada cpu, porque inicialmente ninguna cpu ejecuta ningun proceso.
	}
	//poner algo sobre cola_procesos? inicializarla con algo? no se.
//<<<REMOVE>>>
//<<<REMOVE END>>>
}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {
//<<<REMOVE>>>
//<<<REMOVE END>>>
	//si hay un cpu vacio, meterle a esa cpu este proceso. si no hay cpu vacio, encolar.
	bool lo_asigne = false;
	for(int i = 0; i < cant_cores; i++) {
		if(pid_actual_cpu[i] == IDLE_TASK) {
			pid_actual_cpu[i] = pid;
			lo_asigne = true;
		}
	}
	if(!lo_asigne) {
		cola_procesos.push(pid); // si no lo asigne, porque no hay cpu libre, lo encolo.
	}
}

void SchedRR::unblock(int pid) {
//<<<REMOVE>>>
//<<<REMOVE END>>>
	//Si se desbloqueo, meterlo a una cpu si hay alguna libre (y si no hay nada en la cola, aunque creo q no puede pasar q haya cpu libre y alguien en la cola, porq automaticamente deberia meter al proceso en alguna cpu), y sino encolar.
	load(pid); // si se desbloquea, lo vuelvo a cargar.
}

int SchedRR::tick(int cpu, const enum Motivo m) {
//<<<REMOVE>>>
//<<<REMOVE END>>>
	switch(m) {
		case TICK:
			if(quantum_restante_cpu[cpu] == 1) { //uso todo su quantum, sacarlo y poner otro de la cola.
				quantum_restante_cpu[cpu] = quantum_original_cpu[cpu]; //recargo el quantum
				cola_procesos.push(pid_actual_cpu[cpu]); //encolo al proceso
				pid_actual_cpu[cpu] = cola_procesos.pop(); //agarro al proximo de la cola y lo pongo a ejecutar en este cpu (le puede volver a tocar al anterior si no hay nadie mas en la cola.
			} else { //no uso todo su quantum, restarle 1 al quantum
				quantum_restante_cpu[cpu]--;
			}
			/*int j = cpu;
			for(int i = 0; i < cant_cores; i++) {
				j = (j+1)%cant_cores;
				if(pid_actual_cpu[j] != IDLE_TASK) {
					return pid_actual_cpu[j]; //devuelvo el pid de la siguiente cpu que tenga un proceso ejecutandose.
				}
			}*/
			return pid_actual_cpu[cpu]; //devuelvo el pid de esa cpu
		case BLOCK:
			//si se bloqueo el proceso, sacarlo de la cpu y ni ponerlo en la cola de ready, hay q esperar que se desbloquee para eso.
			//si se bloqueo ejecutar idle hasta que se termine el quantum? o ya directamente poner otro proceso?
			//asumo que si una tarea se bloqueo, no me quedo haciendo idle hasta que termine el quantum, sino que busco otra tarea para ejecutar y recargo el quantum (preguntar si esta bien asumir y hacer eso)
			quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];
			pid_actual_cpu[cpu] = IDLE_TASK;
			if(!cola_procesos.empty()) { //si hay una tarea en la cola de procesos, asignarsela a esta cpu y quitarla de la cola
				pid_actual_cpu[cpu] = cola_procesos.front();
				cola_procesos.pop();
			}
			return pid_actual_cpu[cpu];
			//ocurre que si un proces ose bloquea, cuando vuelve puede alterar el orden de ejecucion de las tareas.
			//si venia, p1, p2, p3, p4, p1, p2, p3, p4, p1, y ahora p1 se bloquea, sigue:
			//p2, p3, p4, p2, p3, y si ahora vuelve p1, sigue: p4, p2, p1, p3, p4, p2, p1, p3, etc... CREO QUE PASA ESO
			//si pasa eso esta mal?... no se..
		case EXIT:
			quantum_restante_cpu[cpu] = quantum_original_cpu[cpu]; //recargo el quantum
			pid_actual_cpu[cpu] = IDLE_TASK;
			if(!cola_procesos.empty()) { //si hay una tarea en la cola de procesos, asignarsela a esta cpu y quitarla de la cola
				pid_actual_cpu[cpu] = cola_procesos.front();
				cola_procesos.pop();
			}
			return pid_actual_cpu[cpu];
	}
}

//<<<REMOVE>>>

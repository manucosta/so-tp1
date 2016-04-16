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
		pid_actual_cpu.pushback(-1); //le pongo -1 a cada cpu, porque inicialmente ninguna cpu ejecuta ningun proceso.
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
		if(pid_actual_cpu[i] == -1) {
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
			int j = cpu;
			for(int i = 0; i < cant_cores; i++) {
				j = (j+1)%cant_cores;
				if(pid_actual_cpu[j] != -1) {
					return pid_actual_cpu[j]; //devuelvo el pid de la siguiente cpu que tenga un proceso ejecutandose.
				}
			}
		case BLOCK:
			break;
		case EXIT:
			break;
	}
}

//<<<REMOVE>>>

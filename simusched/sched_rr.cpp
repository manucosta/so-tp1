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
	//cantTareasTerminadas= 0;
	//cout << "Empezo todo..." << endl;
	cant_cores = argn[0];
	for(int i = 1; i <= cant_cores; i++) {
		quantum_original_cpu.push_back(argn[i]); //lleno el array de cpu_quantum con los quantums de cada core.
		quantum_restante_cpu.push_back(argn[i]); //le pongo todo el quantum a cada cpu.
		pid_actual_cpu.push_back(IDLE_TASK); //le pongo IDLE_TASK a cada cpu, porque inicialmente ninguna cpu ejecuta ningun proceso.
		//cout << "arg "  << (i) << ": " << argn[i] << endl;
	}
	//cout << "Cargué la estructura interna" << endl;
	mostrarEstructura();
	
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
	//cout << "Llego un nuevo proceso. pid = " << pid << endl;
	
	bool lo_asigne = false;
	for(int i = 0; i < cant_cores; i++) {
		if(pid_actual_cpu[i] == IDLE_TASK) {
			pid_actual_cpu[i] = pid;
			lo_asigne = true;
			//cout << "Asigne al proceso a la cpu " << i << endl;
			break;
		}
	}
	if(!lo_asigne) {
		cola_procesos.push(pid); // si no lo asigne, porque no hay cpu libre, lo encolo.
		//cout << "No lo asigne, lo encolo" << endl;
	}
	
	mostrarEstructura();
}

void SchedRR::unblock(int pid) {
//<<<REMOVE>>>
//<<<REMOVE END>>>
	//Si se desbloqueo, meterlo a una cpu si hay alguna libre (y si no hay nada en la cola, aunque creo q no puede pasar q haya cpu libre y alguien en la cola, porq automaticamente deberia meter al proceso en alguna cpu), y sino encolar.
	//cout << "Se desbloqueo el proceso con pid = " << pid << endl;
	//cout << "Omitir el siguiente 'Llego un nuevo...' ya que llego el proceso que se acaba de desbloquear" << endl;
	load(pid); // si se desbloquea, lo vuelvo a cargar.
	//mostrarEstructura(); //no lo pongo porq en el load ya esta el mostrarEstructura, para no mostrarlo dos veces
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	//cout << "Llego un tick de la cpu " << cpu << endl;
	//cout << "Se terminaron: " << cantTareasTerminadas << " tareas"<< endl; 
	//mostrarEstructura();
//<<<REMOVE>>>
//<<<REMOVE END>>>
	switch(m) {
		case TICK:
			//cout << "Llego un tick posta" << endl;
			
			if(quantum_restante_cpu[cpu] == 1) { //uso todo su quantum, sacarlo y poner otro de la cola.
				quantum_restante_cpu[cpu] = quantum_original_cpu[cpu]; //recargo el quantum
				cola_procesos.push(pid_actual_cpu[cpu]); //encolo al proceso	
				pid_actual_cpu[cpu] = cola_procesos.front(); //agarro al proximo de la cola y lo pongo a ejecutar en este cpu (le puede volver a tocar al anterior si no hay nadie mas en la cola.
				cola_procesos.pop();
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
			//cout << "Retorno pid_actual: " << pid_actual_cpu[cpu] << endl;
			mostrarEstructura();
			return pid_actual_cpu[cpu]; //devuelvo el pid de esa cpu
		case BLOCK:
			//cout << "Llego un tick block" << endl;
			//si se bloqueo el proceso, sacarlo de la cpu y ni ponerlo en la cola de ready, hay q esperar que se desbloquee para eso.
			//si se bloqueo ejecutar idle hasta que se termine el quantum? o ya directamente poner otro proceso?
			//asumo que si una tarea se bloqueo, no me quedo haciendo idle hasta que termine el quantum, sino que busco otra tarea para ejecutar y recargo el quantum (preguntar si esta bien asumir y hacer eso)
			quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];
			pid_actual_cpu[cpu] = IDLE_TASK;
			if(!cola_procesos.empty()) { //si hay una tarea en la cola de procesos, asignarsela a esta cpu y quitarla de la cola
				pid_actual_cpu[cpu] = cola_procesos.front();
				cola_procesos.pop();
			}
			mostrarEstructura();
			return pid_actual_cpu[cpu];
			//ocurre que si un proces ose bloquea, cuando vuelve puede alterar el orden de ejecucion de las tareas.
			//si venia, p1, p2, p3, p4, p1, p2, p3, p4, p1, y ahora p1 se bloquea, sigue:
			//p2, p3, p4, p2, p3, y si ahora vuelve p1, sigue: p4, p2, p1, p3, p4, p2, p1, p3, etc... CREO QUE PASA ESO
			//si pasa eso esta mal?... no se..
		case EXIT:
			//cout << "Llego un tick exit" << endl;
			quantum_restante_cpu[cpu] = quantum_original_cpu[cpu]; //recargo el quantum
			pid_actual_cpu[cpu] = IDLE_TASK;
			if(!cola_procesos.empty()) { //si hay una tarea en la cola de procesos, asignarsela a esta cpu y quitarla de la cola
				pid_actual_cpu[cpu] = cola_procesos.front();
				cola_procesos.pop();
			}
			//cantTareasTerminadas++;
			mostrarEstructura();
			return pid_actual_cpu[cpu];
		default:
			//cout << "putos..." << endl;
			break;
	}
	return 0;
}

//<<<REMOVE>>>

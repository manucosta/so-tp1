#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cant_cores = argn[0];
	for(int i = 1; i <= cant_cores; i++) {
		quantum_original_cpu.push_back(argn[i]); //lleno el array de cpu_quantum con los quantums de cada core.
		quantum_restante_cpu.push_back(argn[i]); //le pongo todo el quantum a cada cpu.
		cant_procesos_cpu.push_back(0);
		cola_procesos_cpu.push_back(std::queue<int>());
	}
//<<<REMOVE>>>
//<<<REMOVE END>>>
}

SchedRR2::~SchedRR2() {

}


void SchedRR2::load(int pid) {
	//cout << "entro al load" << endl;
	if(procesos_por_nucleo.count(pid)) { //si existe en el diccionario, es porque ya tenia asignado una cpu
		int cpu_perteneciente = procesos_por_nucleo.find(pid)->second; //me guardo a que cpu pertenece el proceso con id = pid
		cola_procesos_cpu[cpu_perteneciente].push(pid); //agrego el proceso a la cola de la cpu perteneciente
	} else {
		//cout << "entre al else" << endl;
		int indice_cpu_menor = 0;
		for(int i = 0; i < cant_cores; i++) {
			//cout << "entre al for" << endl;
			if(cant_procesos_cpu[i] < cant_procesos_cpu[indice_cpu_menor]) {
				indice_cpu_menor = i;
				break;
			}
		}
		//cout << "sali del for" << endl;
		//hasta aca tengo el indice de la cpu que tiene menos procesos
		cant_procesos_cpu[indice_cpu_menor]++; //ahora esa cpu tiene un proceso mas
		//cout << "hizo el 1" << endl;
		procesos_por_nucleo.insert( std::pair<int,int>(pid, indice_cpu_menor) ); //agrego la conexion pid-cpu
		//cout << "hizo el 2" << endl;
		cola_procesos_cpu[indice_cpu_menor].push(pid); //pusheo el proceso a la cola de su cpu.
	}
	//cout << "termine el load" << endl;
}

void SchedRR2::unblock(int pid) {
	load(pid);
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	switch(m) {
		case TICK:
			//si era el ultimo tick encolarlo y agarrar otro. sino restar quantum.
			if(current_pid(cpu) == IDLE_TASK) {  //separamos esto porque no queremos encolar idle
				return next(cpu);
			} else if(quantum_restante_cpu[cpu] == 1) { //uso todo su quantum, sacarlo y poner otro de la cola.
				cola_procesos_cpu[cpu].push(current_pid(cpu)); //encolo al proceso pues todavía no terminó
				return next(cpu);
			} else { //no uso todo su quantum, restarle 1 al quantum
				quantum_restante_cpu[cpu]--;
				return current_pid(cpu);
			}
		case BLOCK:
			return next(cpu);
		case EXIT:
			cant_procesos_cpu[cpu]--; //ahora hay un proceso menos en la cpu
			procesos_por_nucleo.erase(current_pid(cpu)); //borro del diccionario el pid del proceso que termino.
			return next(cpu);
	}
	cerr << "Motivo inválido para invocar a la función tick" << endl;
	return -1;
}

int SchedRR2::next(int cpu) {
  int siguiente = IDLE_TASK;
  if(!cola_procesos_cpu[cpu].empty()) {
    quantum_restante_cpu[cpu] = quantum_original_cpu[cpu];
    siguiente = cola_procesos_cpu[cpu].front();
    cola_procesos_cpu[cpu].pop();
  }
  return siguiente;
}

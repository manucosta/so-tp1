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
		cant_procesos_cpu[i] = 0;
	}
//<<<REMOVE>>>
//<<<REMOVE END>>>
}

SchedRR2::~SchedRR2() {

}


void SchedRR2::load(int pid) {
//<<<REMOVE>>>
//<<<REMOVE END>>>
	int indice_cpu_menor = 0;
	for(int i = 0; i < cant_cores; i++) {
		if(cant_procesos_cpu[i] < cant_procesos_cpu[indice_cpu_menor]) {
			indice_cpu_menor = i;
			break;
		}
	}
	//hasta aca tengo el indice de la cpu que tiene menos procesos
	cant_procesos_cpu[indice_cpu_menor]++; //ahora esa cpu tiene un proceso mas
	procesos_por_nucleo.insert( std::pair<int,int>(pid, indice_cpu_menor) ); //agrego la conexion pid-cpu
	cola_procesos_cpu[indice_cpu_menor].push(pid); //pusheo el proceso a la cola de su cpu.
}

void SchedRR2::unblock(int pid) {
//<<<REMOVE>>>
//<<<REMOVE END>>>
	int cpu_perteneciente = procesos_por_nucleo.find(pid)->second; //me guardo a que cpu pertenece el proceso con id = pid
	cola_procesos_cpu[cpu_perteneciente].push(pid); //agrego el proceso a la cola de la cpu perteneciente
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
//<<<REMOVE>>>
//<<<REMOVE END>>>
	switch(m) {
		case TICK:
			//si era el ultimo tick encolarlo y agarrar otro. sino restar quantum.
		case BLOCK:
			//poner nuevo pid, recargar quantum
		case EXIT:
			cant_procesos_cpu[cpu]--; //ahora hay un proceso menos en la cpu
			procesos_por_nucleo.erase(current_pid(cpu)); //borro del diccionario el pid del proceso que termino.
			//falta retornar
	}
return 0;
}

int SchedRR2::next(int cpu) {
//<<<REMOVE>>>
//<<<REMOVE END>>>
return 0;
}

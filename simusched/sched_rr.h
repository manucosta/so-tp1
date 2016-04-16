#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
//<<<REMOVE>>>
		std::queue<int> q;
		std::vector<int> cpu_quantum;
		std::vector<int> def_quantum;
		int next(int cpu);
//<<<REMOVE END>>>
		int cant_cores;
		std::queue<int> cola_procesos; //la cola donde se van poniendo los procesos que no se estan ejecutando. se guarda el pid de cada proceso.
		std::vector<int> quantum_original_cpu; //te dice cuantos ciclos originalmente tiene cada cpu
		std::vector<int> quantum_restante_cpu; //te dice cuantos ciclos le quedan al proceso por cada cpu
		std::vector<int> pid_actual_cpu; //te dice que pid esta ejecutando en cada cpu. -1 si no hay ningun proceso en esa cpu.
};

#endif

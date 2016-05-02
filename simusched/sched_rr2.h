#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include <map>
#include "basesched.h"

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
    ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int cant_cores; //guarda la cantidad de cores que hay.
		std::vector< std::queue<int> > cola_procesos_cpu; //indexado por cpu, tiene la cola de procesos para cada cpu
		std::vector<int> quantum_original_cpu; //quantum original de cada cpu
		std::vector<int> quantum_restante_cpu; //quantum que le queda al proceso antes de ser desalojado, por cpu
		std::vector<int> cant_procesos_cpu; //cantidad de procesos que hay en cada cpu (suma los ready+bloqueados+ejecutando)
		std::map<int,int> procesos_por_nucleo; //un dicc. dado un pid, devuelve la cpu a la que pertenece.

		int next(int cpu);
};

#endif

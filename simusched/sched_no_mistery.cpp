#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
  // MFQ recibe los quantums por parÃ¡metro
  def_quantum.push_back(1);
  for(int i = 0; i < argn.size(); i++){
	  def_quantum.push_back(argn[i]);
  }
  cur_pri = 0;
  quantum = def_quantum[cur_pri];
  n = 0;
}

void SchedNoMistery::load(int pid) {
}

void SchedNoMistery::unblock(int pid) {
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
}

int SchedNoMistery::next(void) {
  // Elijo el nuevo pid
}

#include "tasks.h"
#include <stdlib.h> /* srand, rand */
#include <iostream> /* cerr*/
#include <time.h>  

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
  uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
  uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
  uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
  for(int i = 0; i < (int)params.size(); i++) {
    if (i % 2 == 0) uso_CPU(pid, params[i]);
    else uso_IO(pid, params[i]);
  }
}

void TaskConsola(int pid, vector<int> params) { // params: n, bmin, bmax.
  int bmin = params[1];
  int bmax = params[2];
  int nro_random;
  srand(time(NULL)); //Seteamos un seed aleatorio en cada llamada a TaskConsola
  for(int i = 0; i < params[0]; i++) {
    nro_random = rand() % (bmax - bmin + 1) + bmin; //nro_random entre bmin y bmax, 
                                                    //ambos inclusive
    uso_IO(pid, nro_random); //Realizo una llamada bloqueante con duración nro_random.
  }
}

void TaskBatch(int pid, vector<int> params) { // params: total_cpu, cant_bloqueos
  int total_cpu = params[0];
  int cant_bloqueos = params[1];
  int tiempo_disponible = total_cpu - cant_bloqueos; //tiempo_disponible indica cuanto tiempo se va a tener que usar la CPU. El tiempo que tarda lanzar cada llamada bloqueante es 1.
  if(tiempo_disponible < 0) { 
    uso_CPU(pid, params[0]);
    cerr << "La cantidad de llamadas bloqueantes no puede ser mayor al tiempo total\n" << endl;
  } else { //Si puedo ejecutar todas las llamadas bloqueantes con el tiempo de cpu que dispongo, entonces...
    srand(time(NULL));
    for(int i = 0; i < cant_bloqueos; i++) { // Recorro todas las llamadas bloqueantes
      int cuanto_cpu_uso = rand()%(tiempo_disponible+1); //Elijo cuanto tiempo uso la cpu antes de realizar la llamada bloqueante entre 0 y tiempo_disponible milisegundos (inclusive ambos).
      uso_CPU(pid, cuanto_cpu_uso);
      tiempo_disponible-=cuanto_cpu_uso; // Actualizo tiempo_disponible quitandole el tiempo que ya se uso el cpu
      uso_IO(pid, 2);
    }
    uso_CPU(pid, tiempo_disponible); //Uso lo que queda de tiempo en el CPU
  }
}



void tasks_init(void) {
  /* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
   * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
   * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
  register_task(TaskCPU, 1);
  register_task(TaskIO, 2);
  register_task(TaskAlterno, -1);
  register_task(TaskConsola, 3);
  register_task(TaskBatch, 2);
}

#include "tasks.h"
#include <stdlib.h> /* srand, rand */
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
	int nro_random;
	srand(53); //seed = 53.. dsp usar de seed el time del sistema, o no se...
	for(int i = 0; i < params[0]; i++) { // params[0] = n. Se ejecuta el for n veces. Por lo tanto calcula n números random y realiza n llamadas bloqueantes, cada una con un nuevo número random.
		nro_random = rand()%(params[2]-params[1]+1) + params[1]; //Obtengo un numero random entre params[1] = bmin y params[2] = bmax inclusive ambos.
		uso_IO(pid, nro_random); //Realizo una llamada bloqueante con duración nro_random.
	}
	/*
	 
	 Para el informe:
	 
	 Ej 1)
	 Para crear la tarea TaskConsola, creamos un nuevo método en el archivo 'task.cpp' llamado 'TaskConsola'.
	 La tarea genera un número pseudoaleatorio m y luego realiza una llamada bloqueante IO de m milisegundos.
	 Esto último lo hace en una estructura repetitiva n veces, por lo tanto, genera n números pseudoaleatorios
	 y n llamadas bloqueantes IO.
	  
	 Lote:
		TaskConsola 5 4 10
		*2 TaskCPU 25
	 
	 Ej 2)
	 Lote:
		TaskCPU 500
		TaskConsola 10 1 4
		TaskConsola 20 1 4
		TaskConsola 30 1 4
	 Asumo que si se hace una llamada bloqueante, el tiempo minimo seria 1, no tiene sentido realizar una llamada bloqueante de tiempo 0
	 No entendi bien la consigna, pero parece que hay 3 procesos. uno que realiza 10 llamadas bloqueantes,
	 otro que realiza 20 llamadas bloqueantes, y otro que realiza 30 llamadas bloqueantes. Eso fue lo que
	 entendi y en base a eso hice el lote.
	 
	*/
}

void TaskBatch(int pid, vector<int> params) { // params: total_cpu, cant_bloqueos
	srand(20); //uso seed = 20;
	int tiempo_disponible = params[0] - params[1]; //params[0] = total_cpu. params[1] = cant_bloqueos. tiempo_disponible indica cuanto tiempo se va a tener que usar la CPU. Sería el total_cpu menos el tiempo que tarda lanzar cada llamada bloqueante * cant_bloqueos. El tiempo que tarda lanzar cada llamada bloqueante es 1, asi que queda 1*cant_bloqueos = cant_bloqueos.
	if(tiempo_disponible < 0) { // Si tiempo disponible < 0, significa que no me da el tiempo para ejecutar todas las llamadas bloqueantes que debo, asi que simplemente uso todo el CPU (lo decidi YO - eze).
		uso_CPU(pid, params[0]);
	} else { //Si puedo ejecutar todas las llamadas bloqueantes con el tiempo de cpu que dispongo, entonces...
		for(int i = 0; i < params[1]; i++) { // Recorro todas las llamadas bloqueantes
			int cuanto_cpu_uso = rand()%(tiempo_disponible+1); //Elijo cuanto tiempo uso la cpu antes de realizar la llamada bloqueante entre 0 y tiempo_disponible milisegundos (inclusive ambos).
			if(cuanto_cpu_uso > 0) { // Si se usa por más de 0 milisegundos, lo uso.
				uso_CPU(pid, cuanto_cpu_uso);
			}
			//sacar el if si me dicen que puedo ejecutar uso_CPU(pid, 0) y sería lo mismo que no se use el cpu. si no se puede usar la cpu por 0 milisegundos, dejar el if.
			// Después de usar la cpu ese tiempo, hago la llamada bloqueante.
			uso_IO(pid, 2); // o 4, no esta claro cuanto se bloquea en el enunciado.
			tiempo_disponible-=cuanto_cpu_uso; // Actualizo tiempo_disponible quitandole el tiempo que ya se uso el cpu (no le quito el tiempo de la llamada bloqueante porque se lo quite al principio del algoritmo.
		}
	}
	
	/*
	 
	 Lote:
		TaskBatch 20 5
		TaskBatch 5 2
		TaskBatch 10 10
		TaskBatch 15 1
	 
	 
	*/
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

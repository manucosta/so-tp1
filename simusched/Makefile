export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_no_mistery.o
MAIN=simusched

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS) sched_mistery.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
 sched_rr.h sched_rr2.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

ejercicio1:
	./simusched lotes/loteEj1.tsk 2 2 0 SchedFCFS | ./graphsched.py >> img/imgEj1-1.png

ejercicio2:
	./simusched lotes/loteEj2.tsk 1 5 2 SchedFCFS | ./graphsched.py >> img/imgEj2-1.png
	./simusched lotes/loteEj2.tsk 1 5 2 SchedFCFS >> data/data2-1.out
	./simusched lotes/loteEj2.tsk 2 5 2 SchedFCFS | ./graphsched.py >> img/imgEj2-2.png
	./simusched lotes/loteEj2.tsk 2 5 2 SchedFCFS >> data/data2-2.out
	./simusched lotes/loteEj2.tsk 4 5 2 SchedFCFS | ./graphsched.py >> img/imgEj2-3.png
	./simusched lotes/loteEj2.tsk 4 5 2 SchedFCFS >> data/data2-3.out

ejercicio3:
	./simusched lotes/loteEj3.tsk 1 5 0 SchedFCFS | ./graphsched.py >> img/imgEj3.png
	./simusched lotes/loteEj3.tsk 1 5 0 SchedFCFS >> data/data3.out

ejercicio5:
	./simusched lotes/loteEj5.tsk 1 2 0 SchedRR 2 | ./graphsched.py >> img/imgEj5-1.png
	./simusched lotes/loteEj5.tsk 1 2 0 SchedRR 2 >> data/data5-1.out
	./simusched lotes/loteEj5.tsk 1 2 0 SchedRR 10 | ./graphsched.py >> img/imgEj5-2.png
	./simusched lotes/loteEj5.tsk 1 2 0 SchedRR 10 >> data/data5-2.out
	./simusched lotes/loteEj5.tsk 1 2 0 SchedRR 30 | ./graphsched.py >> img/imgEj5-3.png
	./simusched lotes/loteEj5.tsk 1 2 0 SchedRR 30 >> data/data5-3.out

ejercicios: new ejercicio1 ejercicio2 ejercicio3 ejercicio5

clean:
	rm -f $(OBJS) $(MAIN)
	rm -f img/*.png data/*.out

new: clean all

export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_mistery.o sched_no_mistery.o
MAIN=simusched

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS)
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
	./simusched lotes/loteEj1-1.tsk 1 1 0 SchedFCFS | ./graphsched.py >> imgEj1-1
ejercicio2:
	./simusched lotes/loteEj2-1.tsk 1 5 2 SchedFCFS | ./graphsched.py >> imgEj2-1
	./simusched lotes/loteEj2-1.tsk 2 5 2 SchedFCFS | ./graphsched.py >> imgEj2-2
	./simusched lotes/loteEj2-1.tsk 4 5 2 SchedFCFS | ./graphsched.py >> imgEj2-3

clean:
	rm -f $(OBJS) $(MAIN)

new: clean all

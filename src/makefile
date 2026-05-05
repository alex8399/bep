CC = g++
NVCC = /usr/local/cuda-13.2/bin/nvcc

CFLAGS = -Iinc -O2 -std=c++17
OPENMPFLAGS = -fopenmp
STDTHREADFLAGS = -pthread
CUDAFLAGS = -L/usr/local/cuda-13.2/lib64 -lcudart -Wl,-rpath,/usr/local/cuda-13.2/lib64
GTESTGLAGS = -L/usr/lib -lgtest -lgtest_main

BLD_DIR = bld
TARGET_DIR = ${BLD_DIR}
LIB_DIR = lib
INC_DIR = inc
TST_DIR = tst


clean:
	rm -rf ${BLD_DIR}/*.o ${BLD_DIR}/*.exe ${BLD_DIR}/*.out ${TST_DIR}/tmp/*.tmp

build: \
	${TARGET_DIR}/experiment.exe \
	${TARGET_DIR}/tst_experiment.exe


# Main files

${TARGET_DIR}/experiment.exe: \
	${BLD_DIR}/base_boruvka_mst_solver.o \
	${BLD_DIR}/experiment.o \
	${BLD_DIR}/file_facilities.o \
	${BLD_DIR}/main.o \
	${BLD_DIR}/sequential_boruvka_mst_solver.o \
	${BLD_DIR}/openmp_boruvka_mst_solver.o \
	${BLD_DIR}/stdthread_boruvka_mst_solver.o \
	${BLD_DIR}/cuda_boruvka_mst_solver.o \
	${BLD_DIR}/timer.o
	${CC} $^ ${CUDAFLAGS} ${OPENMPFLAGS} ${STDTHREADFLAGS} -o $@

${BLD_DIR}/base_boruvka_mst_solver.o: ${LIB_DIR}/base_boruvka_mst_solver.cpp
	${CC} ${CFLAGS} -c $< -o $@

${BLD_DIR}/experiment.o: ${LIB_DIR}/experiment.cpp
	${CC} ${CFLAGS} -c $< -o $@

${BLD_DIR}/file_facilities.o: ${LIB_DIR}/file_facilities.cpp
	${CC} ${CFLAGS} -c $< -o $@

${BLD_DIR}/main.o: ${LIB_DIR}/main.cpp
	${CC} ${CFLAGS} -c $< -o $@

${BLD_DIR}/sequential_boruvka_mst_solver.o: ${LIB_DIR}/sequential_boruvka_mst_solver.cpp
	${CC} ${CFLAGS} -c $< -o $@

${BLD_DIR}/openmp_boruvka_mst_solver.o: ${LIB_DIR}/openmp_boruvka_mst_solver.cpp
	${CC} ${CFLAGS} ${OPENMPFLAGS} -c $< -o $@

${BLD_DIR}/stdthread_boruvka_mst_solver.o: ${LIB_DIR}/stdthread_boruvka_mst_solver.cpp
	${CC} ${CFLAGS} ${STDTHREADFLAGS} -c $< -o $@

${BLD_DIR}/cuda_boruvka_mst_solver.o: ${LIB_DIR}/cuda_boruvka_mst_solver.cu
	${NVCC} ${CFLAGS} -c lib/cuda_boruvka_mst_solver.cu -o bld/cuda_boruvka_mst_solver.o

${BLD_DIR}/timer.o: ${LIB_DIR}/timer.cpp
	${CC} ${CFLAGS} -c $< -o $@


# Test files

${TARGET_DIR}/tst_experiment.exe: \
	${BLD_DIR}/tst_boruvka_mst_solver.o \
	${BLD_DIR}/tst_file_facilities.o \
	${BLD_DIR}/tst_timer.o \
	${BLD_DIR}/base_boruvka_mst_solver.o \
	${BLD_DIR}/experiment.o \
	${BLD_DIR}/file_facilities.o \
	${BLD_DIR}/sequential_boruvka_mst_solver.o \
	${BLD_DIR}/openmp_boruvka_mst_solver.o \
	${BLD_DIR}/stdthread_boruvka_mst_solver.o \
	${BLD_DIR}/cuda_boruvka_mst_solver.o \
	${BLD_DIR}/timer.o
	${CC} $^ ${CUDAFLAGS} ${OPENMPFLAGS} ${STDTHREADFLAGS} ${GTESTGLAGS}  -o $@

${BLD_DIR}/tst_boruvka_mst_solver.o: ${TST_DIR}/tst_boruvka_mst_solver.cpp
	${CC} ${CFLAGS} -c $< -o $@

${BLD_DIR}/tst_file_facilities.o: ${TST_DIR}/tst_file_facilities.cpp
	${CC} ${CFLAGS} -c $< -o $@

${BLD_DIR}/tst_timer.o: ${TST_DIR}/tst_timer.cpp
	${CC} ${CFLAGS} -c $< -o $@


.PHONY: clean build

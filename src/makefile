CXX = D:\Programs\MSY32\ucrt64\bin\g++.exe -DGCC
NVCC = D:\Programs\Nvidia-toolkit\bin\nvcc.exe -DNVCC

LIB_DIR = lib
INC_DIR = inc
BLD_DIR = bld
TST_DIR = tst
UTILS_DIR = utils
NVIDIA_TOOLKIT_DIR = D:\Programs\Nvidia-toolkit

CXXFLAGS = -I $(INC_DIR) -O2 -march=native -DNDEBUG -std=c++20
NVCCFLAGS = -I $(INC_DIR) -O2 -std=c++20 -arch=sm_75 -Xcompiler="/O2 /std:c++20 /DNDEBUG /arch:AVX2"

OPENMPFLAGS = -fopenmp
STDTHREADFLAGS = -pthread
OPENCLFLAGS = -I "$(NVIDIA_TOOLKIT_DIR)\include" -lOpenCL -L "$(NVIDIA_TOOLKIT_DIR)\lib\x64"
CUDAFLAGS = -L"$(NVIDIA_TOOLKIT_DIR)\lib\x64" -lcudart

GTESTFLAGS = -lgtest -lgtest_main -pthread

.PHONY: build clean opencl_info run_tests opencl_info

run_tests:
	$(BLD_DIR)/tst_experiment.exe
	$(BLD_DIR)/tst_experiment_nvcc.exe

clean:
	del /Q $(BLD_DIR)\*.o $(BLD_DIR)\*.exe $(BLD_DIR)\*.out $(TST_DIR)\tmp\*.tmp

build: \
	$(BLD_DIR)/experiment.exe \
	$(BLD_DIR)/experiment_nvcc.exe \
	$(BLD_DIR)/tst_experiment.exe \
	$(BLD_DIR)/tst_experiment_nvcc.exe

opencl_info: $(BLD_DIR)/opencl_info.exe
	$(BLD_DIR)/opencl_info.exe

$(BLD_DIR)/opencl_info.exe: $(UTILS_DIR)/opencl_info.cpp
	$(CXX) $(CXXFLAGS) $< $(OPENCLFLAGS) -o $@

$(BLD_DIR)/experiment.exe: \
	$(BLD_DIR)/base_boruvka_mst_solver.o \
	$(BLD_DIR)/experiment.o \
	$(BLD_DIR)/file_facilities.o \
	$(BLD_DIR)/main.o \
	$(BLD_DIR)/timer.o \
	$(BLD_DIR)/sequential_boruvka_mst_solver_findpivotsafely.o \
	$(BLD_DIR)/sequential_boruvka_mst_solver_findpivot.o \
	$(BLD_DIR)/openmp_boruvka_mst_solver_mapreduce.o \
	$(BLD_DIR)/openmp_boruvka_mst_solver_mutex.o \
	$(BLD_DIR)/stdthread_boruvka_mst_solver_mapreduce.o \
	$(BLD_DIR)/stdthread_boruvka_mst_solver_mutex.o \
	$(BLD_DIR)/opencl_boruvka_mst_solver.o
	$(CXX) $^ $(OPENMPFLAGS) $(STDTHREADFLAGS) $(OPENCLFLAGS) -o $@

$(BLD_DIR)/base_boruvka_mst_solver.o: $(LIB_DIR)/base_boruvka_mst_solver.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/experiment.o: $(LIB_DIR)/experiment.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/file_facilities.o: $(LIB_DIR)/file_facilities.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/timer.o: $(LIB_DIR)/timer.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/main.o: $(LIB_DIR)/main.cpp
	$(CXX) $(OPENCLFLAGS) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/sequential_boruvka_mst_solver_findpivot.o: $(LIB_DIR)/sequential_boruvka_mst_solver_findpivot.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/sequential_boruvka_mst_solver_findpivotsafely.o: $(LIB_DIR)/sequential_boruvka_mst_solver_findpivotsafely.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/openmp_boruvka_mst_solver_mapreduce.o: $(LIB_DIR)/openmp_boruvka_mst_solver_mapreduce.cpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAGS) -c $< -o $@

$(BLD_DIR)/openmp_boruvka_mst_solver_mutex.o: $(LIB_DIR)/openmp_boruvka_mst_solver_mutex.cpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAGS) -c $< -o $@

$(BLD_DIR)/stdthread_boruvka_mst_solver_mapreduce.o: $(LIB_DIR)/stdthread_boruvka_mst_solver_mapreduce.cpp
	$(CXX) $(CXXFLAGS) $(STDTHREADFLAGS) -c $< -o $@

$(BLD_DIR)/stdthread_boruvka_mst_solver_mutex.o: $(LIB_DIR)/stdthread_boruvka_mst_solver_mutex.cpp
	$(CXX) $(CXXFLAGS) $(STDTHREADFLAGS) -c $< -o $@

$(BLD_DIR)/opencl_boruvka_mst_solver.o: $(LIB_DIR)/opencl_boruvka_mst_solver.cpp
	$(CXX) $(CXXFLAGS) -c $< $(OPENCLFLAGS) -o $@

$(BLD_DIR)/tst_experiment.exe: \
	$(BLD_DIR)/tst_boruvka_mst_solver.o \
	$(BLD_DIR)/tst_file_facilities.o \
	$(BLD_DIR)/tst_timer.o \
	$(BLD_DIR)/base_boruvka_mst_solver.o \
	$(BLD_DIR)/experiment.o \
	$(BLD_DIR)/file_facilities.o \
	$(BLD_DIR)/timer.o \
	$(BLD_DIR)/sequential_boruvka_mst_solver_findpivotsafely.o \
	$(BLD_DIR)/sequential_boruvka_mst_solver_findpivot.o \
	$(BLD_DIR)/openmp_boruvka_mst_solver_mapreduce.o \
	$(BLD_DIR)/openmp_boruvka_mst_solver_mutex.o \
	$(BLD_DIR)/stdthread_boruvka_mst_solver_mapreduce.o \
	$(BLD_DIR)/stdthread_boruvka_mst_solver_mutex.o \
	$(BLD_DIR)/opencl_boruvka_mst_solver.o
	$(CXX) $^ $(OPENMPFLAGS) $(STDTHREADFLAGS) $(OPENCLFLAGS) $(GTESTFLAGS) -o $@

$(BLD_DIR)/tst_boruvka_mst_solver.o: $(TST_DIR)/tst_boruvka_mst_solver.cpp
	$(CXX) $(CXXFLAGS) -c $< $(OPENCLFLAGS) -o $@

$(BLD_DIR)/tst_file_facilities.o: $(TST_DIR)/tst_file_facilities.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/tst_timer.o: $(TST_DIR)/tst_timer.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/experiment_nvcc.exe: \
	$(BLD_DIR)/base_boruvka_mst_solver_nvcc.o \
	$(BLD_DIR)/experiment_nvcc.o \
	$(BLD_DIR)/file_facilities_nvcc.o \
	$(BLD_DIR)/main_nvcc.o \
	$(BLD_DIR)/timer_nvcc.o \
	$(BLD_DIR)/cuda_boruvka_mst_solver_nvcc.o
	$(NVCC) $^ $(CUDAFLAGS) -o $@

$(BLD_DIR)/cuda_boruvka_mst_solver_nvcc.o: $(LIB_DIR)/cuda_boruvka_mst_solver.cu
	$(NVCC) $(NVCCFLAGS) $(CUDAFLAGS) -c $< -o $@

$(BLD_DIR)/base_boruvka_mst_solver_nvcc.o: $(LIB_DIR)/base_boruvka_mst_solver.cpp
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

$(BLD_DIR)/experiment_nvcc.o: $(LIB_DIR)/experiment.cpp
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

$(BLD_DIR)/file_facilities_nvcc.o: $(LIB_DIR)/file_facilities.cpp
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

$(BLD_DIR)/timer_nvcc.o: $(LIB_DIR)/timer.cpp
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

$(BLD_DIR)/main_nvcc.o: $(LIB_DIR)/main.cpp
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

$(BLD_DIR)/tst_experiment_nvcc.exe: \
	$(BLD_DIR)/tst_boruvka_mst_solver_nvcc.o \
	$(BLD_DIR)/base_boruvka_mst_solver_nvcc.o \
	$(BLD_DIR)/experiment_nvcc.o \
	$(BLD_DIR)/file_facilities_nvcc.o \
	$(BLD_DIR)/timer_nvcc.o \
	$(BLD_DIR)/cuda_boruvka_mst_solver_nvcc.o
	$(NVCC) $^ $(CUDAFLAGS) -o $@

$(BLD_DIR)/tst_boruvka_mst_solver_nvcc.o: $(TST_DIR)/tst_boruvka_mst_solver_nvcc.cpp
	$(NVCC) $(NVCCFLAGS) $(CUDAFLAGS) -c $< -o $@
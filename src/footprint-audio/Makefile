NVCC        = nvcc
CC          = g++


NVCC_FLAGS  = -I/usr/local/cuda/include -gencode=arch=compute_60,code=\"sm_60\" -lcuda -lcudart -lcufft
CC_FLAGS    = -std=c++11


ifdef dbg
	NVCC_FLAGS  += -g -G
else
	NVCC_FLAGS  += -O2
endif

LD_FLAGS    = -lcudart -L/usr/local/cuda/lib64
EXE	        = footprint-audio
OBJ	        = footprint-audio.o gpu_helpers.o cpu_helpers.o audiodatabase.o AudioFile.o

default: $(EXE)


gpu_helpers.o: ../common/gpu_helpers.cu
	$(NVCC) -c -o $@ ../common/gpu_helpers.cu $(NVCC_FLAGS)

cpu_helpers.o: ../common/cpu_helpers.cpp
	$(CC) -c -o $@ ../common/cpu_helpers.cpp $(CC_FLAGS)

audiodatabase.o: ../common/audiodatabase.cpp
	$(CC) -c -o $@ ../common/audiodatabase.cpp $(CC_FLAGS)	

AudioFile.o: ../common/AudioFile.cpp
	$(CC) -c -o $@ ../common/AudioFile.cpp $(CC_FLAGS)	

footprint-audio.o:  main.cpp
	$(CC) -c -o $@ main.cpp $(CC_FLAGS) 

$(EXE): $(OBJ)
	$(NVCC) $(OBJ) -o $(EXE) $(LD_FLAGS) $(NVCC_FLAGS)
	make clean_temp

clean_temp:
	rm -rf *.o

clean:
	rm -rf *.o $(EXE)
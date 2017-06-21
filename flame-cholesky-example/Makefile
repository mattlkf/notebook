SRC_PATH := .
OBJ_PATH := .
INC_PATH := .

LIB_HOME := $(HOME)
BLAS_LIB := /opt/OpenBLAS/lib/libopenblas.a

# BLAS_LIB := $(LIB_HOME)/lib/libblas.a
# LAPACK_LIB := $(LIB_HOME)/lib/liblapack.a

FLAME_HOME := $(HOME)
FLAME_INC := $(FLAME_HOME)/flame/include
FLAME_LIB := $(FLAME_HOME)/flame/lib/libflame.a

CC := g++
LINKER := $(CC)

CFLAGS := -g -O2 -Wall -I$(INC_PATH) -I$(FLAME_INC)

# LDFLAGS := -L/opt/intel/fc/em64t/10.0.026/lib

LDFLAGS += -L/usr/lib/gcc/x86_64-pc-linux-gnu/5.4.0/
# LDFLAGS += -L/usr/lib/gcc/x86_64-pc-linux-gnu/3.4.6/../../../../lib64
FINAL_LDFLAGS := -lpthread
# LDFLAGS += -lifport -lifcore -limf -lsvml -lm -lipgo -lirc -lirc_s -ldl
MYAPP_OBJS := main.o 
# file.o util.o proc.o
MYAPP_BIN := my_app

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(MYAPP_OBJS)
	$(LINKER) $(MYAPP_OBJS) $(LDFLAGS) $(FLAME_LIB) $(LAPACK_LIB) $(BLAS_LIB) $(FINAL_LDFLAGS) -o $(MYAPP_BIN)

clean:
	rm -f $(MYAPP_OBJS) $(MYAPP_BIN)
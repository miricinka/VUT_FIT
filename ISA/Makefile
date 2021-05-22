
CC=gcc

OBJS1 = isa_project_module.o

CFLAGS=-I. `net-snmp-config --cflags`

# shared library flags
DLFLAGS=-fPIC -shared


isa_project_module.so: isa_project_module.c Makefile
	$(CC) $(CFLAGS) $(DLFLAGS) -c -o isa_project_module.o isa_project_module.c
	$(CC) $(CFLAGS) $(DLFLAGS) -o isa_project_module.so isa_project_module.o

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Located in: microblaze_0/include/
#include "xbasic_types.h"
#include "xutil.h"
#include "xstatus.h"

#include "sleep.h"


#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


#include "xaxivdma.h"
#include "smart_car_demo.h"
extern smart_car_demo_t smart_car_demo;

int vga_command(int read_index);
int vga_command_sobel(int read_index);

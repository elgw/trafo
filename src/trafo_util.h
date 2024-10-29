#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

double
timespec_diff(struct timespec* end, struct timespec * start);


int
get_peakMemoryKB(size_t * VmPeak, size_t * VmHWM);

void print_peak_memory(void);

void
print_section(const char * msg);

/**
 * VampirTrace
 * http://www.tu-dresden.de/zih/vampirtrace
 *
 * Copyright (c) 2005-2008, ZIH, TU Dresden, Federal Republic of Germany
 *
 * Copyright (c) 1998-2005, Forschungszentrum Juelich, Juelich Supercomputing
 *                          Centre, Federal Republic of Germany
 *
 * See the file COPYING in the package base directory for details
 **/

#include "config.h"

#include "vt_pform.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef TIMER_PAPI_REAL_CYC
#  define TIMER_PAPI_REAL_CYC 10
#endif
#ifndef TIMER_PAPI_REAL_USEC
#  define TIMER_PAPI_REAL_USEC 11
#endif

#if TIMER != TIMER_GETHRTIME && \
    TIMER != TIMER_PAPI_REAL_CYC && \
    TIMER != TIMER_PAPI_REAL_USEC
# error Unknown timer specified! Check the timer configuration in 'config.h'.
#endif

#if TIMER == TIMER_GETHRTIME
# include <sys/time.h>
#elif TIMER == TIMER_PAPI_REAL_CYC
# include <vt_metric.h>
#elif TIMER == TIMER_PAPI_REAL_USEC
# include <vt_metric.h>
  static uint64_t vt_time_base = 0;
#endif

/* platform specific initialization */
void vt_pform_init() {
#if TIMER == TIMER_PAPI_REAL_USEC
  vt_time_base = vt_metric_real_usec();
#endif
}

/* directory of global file system  */
char* vt_pform_gdir() {
  return ".";
}

/* directory of local file system  */
char* vt_pform_ldir() {
  #ifdef PFORM_LDIR
    return PFORM_LDIR;
  #else
    return "/tmp";
  #endif
}

/* clock resolution */
uint64_t vt_pform_clockres() {
#if TIMER == TIMER_GETHRTIME
  return 1e9;
#elif TIMER == TIMER_PAPI_REAL_CYC
  return vt_metric_clckrt();
#elif TIMER == TIMER_PAPI_REAL_USEC
  return 1e6;
#endif
}

/* local or global wall-clock time */
uint64_t vt_pform_wtime() {
#if TIMER == TIMER_GETHRTIME
  return (uint64_t)gethrtime();
#elif TIMER == TIMER_PAPI_REAL_CYC
  return vt_metric_real_cyc();
#elif TIMER == TIMER_PAPI_REAL_USEC
  return vt_metric_real_usec() - vt_time_base;
#endif
}

/* unique numeric SMP-node identifier */
long vt_pform_node_id() {
  return gethostid();
}

/* unique string SMP-node identifier */
char* vt_pform_node_name() {
  static char node[64];
  char *dlmt;
  gethostname(node, 64);
  if ( (dlmt = strchr(node, '.')) != NULL ) *dlmt = '\0';
  return node;
}

/* number of CPUs */
int vt_pform_num_cpus() {
  return sysconf(_SC_NPROCESSORS_ONLN);
}

/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2006      Los Alamos National Security, LLC.  All rights
 *                         reserved. 
 * Copyright (c) 2008-2010 Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "opal_config.h"

#include <time.h>
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "opal/constants.h"
#include "opal/runtime/opal.h"
#include "opal/util/output.h"
#include "opal/util/show_help.h"
#include "opal/mca/base/mca_base_param.h"
#include "opal/threads/mutex.h"
#include "opal/mca/paffinity/base/base.h"

int opal_register_params(void)
{
    /*
     * This string is going to be used in opal/util/stacktrace.c
     */
    {
        char *string = NULL;
        int j;
        int signals[] = {
#ifdef SIGABRT
            SIGABRT,
#endif
#ifdef SIGBUS
            SIGBUS,
#endif
#ifdef SIGFPE
            SIGFPE,
#endif
#ifdef SIGSEGV
            SIGSEGV,
#endif
            -1
        };
        for (j = 0 ; signals[j] != -1 ; ++j) {
            if (j == 0) {
                asprintf(&string, "%d", signals[j]);
            } else {
                char *tmp;
                asprintf(&tmp, "%s,%d", string, signals[j]);
                free(string);
                string = tmp;
            }
        }

        mca_base_param_reg_string_name("opal", "signal", 
                                       "Comma-delimited list of integer signal numbers to Open MPI to attempt to intercept.  Upon receipt of the intercepted signal, Open MPI will display a stack trace and abort.  Open MPI will *not* replace signals if handlers are already installed by the time MPI_INIT is invoked.  Optionally append \":complain\" to any signal number in the comma-delimited list to make Open MPI complain if it detects another signal handler (and therefore does not insert its own).",
                                       false, false, string, NULL);
        free(string);
    }

#if OMPI_ENABLE_DEBUG


    mca_base_param_reg_int_name("opal", "progress_debug", 
                                "Set to non-zero to debug progress engine features",
                                false, false, 0, NULL);

    {
        int value;
        mca_base_param_reg_int_name("opal", "debug_locks",
                                    "Debug mutex usage within Open MPI.  On a "
                                    "non-threaded build, this enables integer counters and "
                                    "warning messages when double-locks are detected.",
                                    false, false, 0, &value);
        if (value) opal_mutex_check_locks = true;
    }
#endif

    /* Paffinity base also has some parameters */
    return opal_paffinity_base_register_params();
}

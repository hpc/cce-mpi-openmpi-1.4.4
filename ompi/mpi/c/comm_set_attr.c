/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2008 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2007      Cisco, Inc.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/attribute/attribute.h"
#include "ompi/memchecker.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Comm_set_attr = PMPI_Comm_set_attr
#endif

#if OMPI_PROFILING_DEFINES
#include "ompi/mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Comm_set_attr";


int MPI_Comm_set_attr(MPI_Comm comm, int comm_keyval, void *attribute_val) 
{
    int ret;

    MEMCHECKER(
        memchecker_comm(comm);
    );

    if (MPI_PARAM_CHECK) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        if (ompi_comm_invalid(comm)) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_COMM, 
                                          FUNC_NAME);
        }
    }

    OPAL_CR_ENTER_LIBRARY();

    ret = ompi_attr_set_c(COMM_ATTR, comm, &comm->c_keyhash, 
                          comm_keyval, attribute_val, false);
    OMPI_ERRHANDLER_RETURN(ret, comm, MPI_ERR_OTHER, FUNC_NAME);  
}

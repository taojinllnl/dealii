// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2010 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// creation and size of LA::MPI::Vector

#include <deal.II/base/index_set.h>

#include <deal.II/lac/generic_linear_algebra.h>

#include <iostream>
#include <vector>

#include "../tests.h"

#include "gla.h"

template <class LA>
void
test()
{
  unsigned int myid    = Utilities::MPI::this_mpi_process(MPI_COMM_WORLD);
  unsigned int numproc = Utilities::MPI::n_mpi_processes(MPI_COMM_WORLD);

  if (myid == 0)
    deallog << "numproc=" << numproc << std::endl;

  // each processor owns 2 indices and all
  // are ghosting Element 1 (the second)

  IndexSet local_active(numproc * 2);
  local_active.add_range(myid * 2, myid * 2 + 2);
  IndexSet local_relevant(numproc * 2);
  local_relevant.add_range(1, 2);


  IndexSet something(100);
  something.add_range(myid, myid + 1);
  if (myid == numproc - 1)
    something.add_range(numproc, 100);


  {
    typename LA::MPI::Vector v1;

    v1.reinit(something, MPI_COMM_WORLD);
    Assert(!v1.has_ghost_elements(), ExcInternalError());
    Assert(v1.size() == 100, ExcInternalError());

    typename LA::MPI::Vector v2(local_active, local_relevant, MPI_COMM_WORLD);
    Assert(v2.has_ghost_elements(), ExcInternalError());
    Assert(v2.size() == numproc * 2, ExcInternalError());

    v2.reinit(local_active, MPI_COMM_WORLD);
    Assert(!v2.has_ghost_elements(), ExcInternalError());
    Assert(v2.size() == numproc * 2, ExcInternalError());

    v2.reinit(local_active, local_relevant, MPI_COMM_WORLD);
    Assert(v2.has_ghost_elements(), ExcInternalError());
    Assert(v2.size() == numproc * 2, ExcInternalError());
  }

  // done
  if (myid == 0)
    deallog << "OK" << std::endl;
}



int
main(int argc, char **argv)
{
  Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);
  unsigned int myid = Utilities::MPI::this_mpi_process(MPI_COMM_WORLD);

  deallog.push(Utilities::int_to_string(myid));

  if (myid == 0)
    {
      initlog();
      deallog << std::setprecision(4);

      {
        deallog.push("PETSc");
        test<LA_PETSc>();
        deallog.pop();
        deallog.push("Trilinos");
        test<LA_Trilinos>();
        deallog.pop();
      }
    }
  else
    {
      deallog.push("PETSc");
      test<LA_PETSc>();
      deallog.pop();
      deallog.push("Trilinos");
      test<LA_Trilinos>();
      deallog.pop();
    }

  if (myid == 9999)
    test<LA_Dummy>();
}

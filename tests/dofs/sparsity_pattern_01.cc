// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2001 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// A variant of deal.II/sparsity_pattern. use the version of
// DoFTools::make_sparsity_pattern that takes two DoFHandler arguments. the
// output should be the same, of course, if we use the same DoFHandler for
// both arguments.


#include <deal.II/dofs/dof_handler.h>
#include <deal.II/dofs/dof_tools.h>

#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_system.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>

#include <deal.II/lac/affine_constraints.h>
#include <deal.II/lac/block_sparsity_pattern.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>
#include <deal.II/lac/sparsity_pattern.h>

#include "../tests.h"



bool
operator==(const BlockSparsityPattern &sp1, const BlockSparsityPattern &sp2)
{
  if (sp1.n_block_rows() != sp2.n_block_rows())
    return false;

  if (sp1.n_block_cols() != sp2.n_block_cols())
    return false;

  for (unsigned int i = 0; i < sp1.n_block_rows(); ++i)
    for (unsigned int j = 0; j < sp1.n_block_cols(); ++j)
      if (!(sp1.block(i, j) == sp2.block(i, j)))
        return false;

  return true;
}



template <int dim>
void
check_boundary(const DoFHandler<dim> &dof)
{
  std::vector<types::global_dof_index> dof_to_boundary_mapping;
  DoFTools::map_dof_to_boundary_indices(dof, dof_to_boundary_mapping);

  // first way: direct generation
  SparsityPattern sparsity_1(dof.n_boundary_dofs(),
                             dof.max_couplings_between_boundary_dofs());
  DoFTools::make_boundary_sparsity_pattern(dof,
                                           dof_to_boundary_mapping,
                                           sparsity_1);
  sparsity_1.compress();

  // second way: via a DynamicSparsityPattern
  SparsityPattern        sparsity_2;
  DynamicSparsityPattern csp(dof.n_boundary_dofs());
  DoFTools::make_boundary_sparsity_pattern(dof, dof_to_boundary_mapping, csp);
  sparsity_2.copy_from(csp);

  // the exact content of sparsity
  // patterns is checked in other
  // tests, so only make sure that
  // sparsity_[12] are equal
  deallog << "Check boundary"
          << " -- " << (sparsity_1 == sparsity_2 ? "ok" : "failed")
          << std::endl;
}



template <int dim>
void
check()
{
  Triangulation<dim> tr;
  if (dim == 2)
    GridGenerator::hyper_ball(tr, Point<dim>(), 1);
  else
    GridGenerator::hyper_cube(tr, -1, 1);
  tr.refine_global(1);
  tr.begin_active()->set_refine_flag();
  tr.execute_coarsening_and_refinement();
  tr.begin_active(2)->set_refine_flag();
  tr.execute_coarsening_and_refinement();
  if (dim == 1)
    tr.refine_global(2);

  // create a system element composed
  // of one Q1 and one Q2 element
  FESystem<dim>   element(FE_Q<dim>(1), 1, FE_Q<dim>(2), 1);
  DoFHandler<dim> dof(tr);
  dof.distribute_dofs(element);

  AffineConstraints<double> constraints;
  DoFTools::make_hanging_node_constraints(dof, constraints);
  constraints.close();

  // create sparsity pattern. note
  // that different components should
  // not couple, so use pattern
  Table<2, DoFTools::Coupling> mask(2, 2);
  mask(0, 0) = mask(1, 1) = DoFTools::always;
  mask(0, 1) = mask(1, 0) = DoFTools::none;


  //--------------- Regular sparsity pattern checks -----------------

  // first way: directly
  SparsityPattern sparsity_1(dof.n_dofs(), dof.n_dofs());
  DoFTools::make_sparsity_pattern(dof, mask, sparsity_1);
  constraints.condense(sparsity_1);
  sparsity_1.compress();

  // second way: via
  // DynamicSparsityPattern
  SparsityPattern        sparsity_2;
  DynamicSparsityPattern csp_2(dof.n_dofs());
  DoFTools::make_sparsity_pattern(dof, mask, csp_2);
  constraints.condense(csp_2);
  sparsity_2.copy_from(csp_2);


  // the exact content of sparsity
  // patterns is checked in other
  // tests, so only make sure that
  // sparsity_[12] are equal
  deallog << "Check 1:"
          << " -- " << (sparsity_1 == sparsity_2 ? "ok" : "failed")
          << std::endl;



  //--------------- Block sparsity pattern checks -----------------

  const unsigned int n  = dof.n_dofs();
  const unsigned int n1 = n / 3;
  const unsigned int n2 = n - n1;

  BlockSparsityPattern sparsity_3(2, 2);
  sparsity_3.block(0, 0).reinit(n1, n1, n);
  sparsity_3.block(1, 0).reinit(n2, n1, n);
  sparsity_3.block(0, 1).reinit(n1, n2, n);
  sparsity_3.block(1, 1).reinit(n2, n2, n);
  sparsity_3.collect_sizes();

  DoFTools::make_sparsity_pattern(dof, dof, sparsity_3);
  constraints.condense(sparsity_3);
  sparsity_3.compress();

  BlockSparsityPattern        sparsity_4;
  BlockDynamicSparsityPattern csp_4(2, 2);
  csp_4.block(0, 0).reinit(n1, n1);
  csp_4.block(1, 0).reinit(n2, n1);
  csp_4.block(0, 1).reinit(n1, n2);
  csp_4.block(1, 1).reinit(n2, n2);
  csp_4.collect_sizes();

  DoFTools::make_sparsity_pattern(dof, dof, csp_4);
  constraints.condense(csp_4);
  csp_4.compress();

  sparsity_4.copy_from(csp_4);

  deallog << "Check 2:"
          << " -- " << (sparsity_3 == sparsity_4 ? "ok" : "failed")
          << std::endl;


  //-------- Sparsity pattern checks for boundary sparsity generators ---------

  // check boundary matrices
  check_boundary(dof);
}



int
main()
{
  initlog();
  deallog << std::setprecision(2) << std::fixed;

  deallog.push("1d");
  check<1>();
  deallog.pop();
  deallog.push("2d");
  check<2>();
  deallog.pop();
  deallog.push("3d");
  check<3>();
  deallog.pop();
}

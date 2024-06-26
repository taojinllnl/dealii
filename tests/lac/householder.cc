// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2005 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


// Tests Householder class for QR-decomposition

#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/householder.h>
#include <deal.II/lac/vector.h>

#include "../tests.h"


const double rect[3][4] = {{4., 3., 2., 1.},
                           {5., 8., 1., -2.},
                           {11., 13., -4., -5}};


int
main()
{
  initlog();
  deallog << std::setprecision(3);

  FullMatrix<double>  A(4, 3, &rect[0][0]);
  Householder<double> H(A);

  Vector<double> u(4);
  Vector<double> v1(3);
  Vector<double> v2(3);

  for (unsigned int i = 0; i < u.size(); ++i)
    u(i) = i * i;
  deallog << "Distance " << H.least_squares(v1, u) << std::endl;
}

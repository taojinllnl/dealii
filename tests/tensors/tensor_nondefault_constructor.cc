// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2006 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


// Same as tensor.cc, but uses tensors based on floats instead of doubles

#include <deal.II/base/tensor.h>

#include <deal.II/lac/vector.h>

#include "../tests.h"

int
main()
{
  initlog();
  deallog << std::setprecision(3);

  float a[3][3] = {{1, 2, 3}, {3, 4, 5}, {6, 7, 8}};
  float b[3]    = {25, 31, 37};

  const unsigned int dim = 3;

  // rank 2
  {
    Tensor<2, dim, float>  t(a);
    Tensor<2, dim, double> dt(t), dt2;
    dt2 = t;
    AssertThrow(dt2 == dt, ExcInternalError());
    AssertThrow(dt == dt2, ExcInternalError());

    Tensor<2, dim, float> ft(dt), ft2;
    ft2 = dt;
    AssertThrow(ft2 == ft, ExcInternalError());
    AssertThrow(ft == ft2, ExcInternalError());

    Tensor<2, dim, std::complex<double>> ct(dt), ct2;
    ct2 = dt;
    AssertThrow(ct2 == ct, ExcInternalError());
    AssertThrow(ct == ct2, ExcInternalError());
  }

  // rank 1
  {
    Tensor<1, dim, float>  t(b);
    Tensor<1, dim, double> dt(t), dt2;
    dt2 = t;
    AssertThrow(dt2 == dt, ExcInternalError());
    AssertThrow(dt == dt2, ExcInternalError());

    Tensor<1, dim, float> ft(dt), ft2;
    ft2 = dt;
    AssertThrow(ft2 == ft, ExcInternalError());
    AssertThrow(ft == ft2, ExcInternalError());

    Tensor<1, dim, std::complex<double>> ct(dt), ct2;
    ct2 = dt;
    AssertThrow(ct2 == ct, ExcInternalError());
    AssertThrow(ct == ct2, ExcInternalError());
  }

  deallog << "OK." << std::endl;
}

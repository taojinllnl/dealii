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


// compute double contraction between two rank-4 tensors

#include <deal.II/base/symmetric_tensor.h>

#include "../tests.h"


template <int dim>
void
test()
{
  deallog << "dim=" << dim << std::endl;

  SymmetricTensor<4, dim> T, A, B;
  for (unsigned int i = 0; i < dim; ++i)
    for (unsigned int j = 0; j < dim; ++j)
      for (unsigned int k = 0; k < dim; ++k)
        for (unsigned int l = 0; l < dim; ++l)
          {
            // write some entries
            // into the tensors. may
            // be overwritten by
            // subsequent writes, but
            // who cares?
            A[i][j][k][l] = (i + 1) * (j + 1) * (l + 1) * (k + 1);
            B[i][j][k][l] = (i + 2) * (j + 3) * (l + 4) * (k + 5);
          }

  T = A * B;

  for (unsigned int i = 0; i < dim; ++i)
    for (unsigned int j = 0; j < dim; ++j)
      for (unsigned int k = 0; k < dim; ++k)
        for (unsigned int l = 0; l < dim; ++l)
          {
            deallog << (int)T[i][j][k][l] << std::endl;

            // calculate result by
            // hand
            double tmp = 0;
            for (unsigned int a = 0; a < dim; ++a)
              for (unsigned int b = 0; b < dim; ++b)
                tmp += A[i][j][a][b] * B[a][b][k][l];

            AssertThrow(std::fabs(T[i][j][k][l] - tmp) < 1e-14 * tmp,
                        ExcInternalError());
          }
}



int
main()
{
  initlog();
  deallog << std::setprecision(3);

  test<1>();
  test<2>();
  test<3>();

  deallog << "OK" << std::endl;
}

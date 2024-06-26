// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2017 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


#include <deal.II/base/numbers.h>
#include <deal.II/base/patterns.h>
#include <deal.II/base/point.h>

#include <boost/core/demangle.hpp>

#include <memory>

#include "../tests.h"

using namespace Patterns::Tools;

// Try conversion on map types

template <class T>
void
test(T t)
{
  auto p = Convert<T>::to_pattern();
  deallog << "Pattern  : " << p->description() << std::endl;
  auto s = Convert<T>::to_string(t);
  deallog << "To String: " << s << std::endl;
  deallog << "To value : " << Convert<T>::to_string(Convert<T>::to_value(s))
          << std::endl;
}

int
main()
{
  initlog();

  int           t0 = 1;
  unsigned int  t1 = 2;
  unsigned char t2 = 3;
  std::string   t3 = "Ciao";
  double        t4 = 4.0;

  std::map<unsigned int, int>           t10;
  std::map<unsigned int, unsigned int>  t11;
  std::map<unsigned int, unsigned char> t12;
  std::map<unsigned int, std::string>   t13;
  std::map<unsigned int, double>        t14;

  t10[0] = t0;
  t11[0] = t1;
  t12[0] = t2;
  t13[0] = t3;
  t14[0] = t4;

  t10[2] = t0;
  t11[2] = t1;
  t12[2] = t2;
  t13[2] = t3;
  t14[2] = t4;

  test(t10);
  test(t11);
  test(t12);
  test(t13);
  test(t14);

  return 0;
}

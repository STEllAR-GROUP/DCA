// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This file implements Modules.

#include "dca/util/modules.hpp"
#include <iostream>
#include <string>

namespace dca {
namespace util {
// dca::util::

const std::string Modules::module_list = "";

void Modules::print() {
  std::cout << "\n"
            << "********************************************************************************\n"
            << "**********                          Modules                           **********\n"
            << "********************************************************************************\n"
            << "\n"
            << Modules::module_list << std::endl;
}

std::string Modules::string() {
  return Modules::module_list;
}

}  // util
}  // dca

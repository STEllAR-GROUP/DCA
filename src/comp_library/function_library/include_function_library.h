// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Peter Staar (peter.w.j.staar@gmail.com)
//
// Description

#ifndef COMP_LIBRARY_FUNCTION_LIBRARY_INCLUDE_FUNCTION_LIBRARY_H
#define COMP_LIBRARY_FUNCTION_LIBRARY_INCLUDE_FUNCTION_LIBRARY_H

#include "comp_library/function_library/domains/domain.h"
#include "comp_library/function_library/domains/special_domains/dmn.h"
#include "comp_library/function_library/domains/special_domains/dmn_0.h"
#include "comp_library/function_library/domains/special_domains/dmn_variadic.h"

template <typename D>
using dmn_1 = dmn_variadic<D>;

template <typename D1, typename D2>
using dmn_2 = dmn_variadic<D1, D2>;

template <typename D1, typename D2, typename D3>
using dmn_3 = dmn_variadic<D1, D2, D3>;

template <typename D1, typename D2, typename D3, typename D4>
using dmn_4 = dmn_variadic<D1, D2, D3, D4>;

template <typename D1, typename D2, typename D3, typename D4, typename D5>
using dmn_5 = dmn_variadic<D1, D2, D3, D4, D5>;

template <typename D1, typename D2, typename D3, typename D4, typename D5, typename D6>
using dmn_6 = dmn_variadic<D1, D2, D3, D4, D5, D6>;

template <typename D1, typename D2, typename D3, typename D4, typename D5, typename D6, typename D7>
using dmn_7 = dmn_variadic<D1, D2, D3, D4, D5, D6, D7>;

template <typename D1, typename D2, typename D3, typename D4, typename D5, typename D6, typename D7,
          typename D8>
using dmn_8 = dmn_variadic<D1, D2, D3, D4, D5, D6, D7, D8>;

template <typename D1, typename D2, typename D3, typename D4, typename D5, typename D6, typename D7,
          typename D8, typename D9>
using dmn_9 = dmn_variadic<D1, D2, D3, D4, D5, D6, D7, D8, D9>;

template <typename D1, typename D2, typename D3, typename D4, typename D5, typename D6, typename D7,
          typename D8, typename D9, typename D10>
using dmn_10 = dmn_variadic<D1, D2, D3, D4, D5, D6, D7, D8, D9, D10>;

#include "domains/domain_type_operations.h"

#include "function.h"

#endif  // COMP_LIBRARY_FUNCTION_LIBRARY_INCLUDE_FUNCTION_LIBRARY_H

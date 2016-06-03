// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Peter Staar (peter.w.j.staar@gmail.com)
//
// This header file defines the supported IO types.

#ifndef COMP_LIBRARY_IO_LIBRARY_IO_TYPES_H
#define COMP_LIBRARY_IO_LIBRARY_IO_TYPES_H

namespace IO {
enum FORMAT { CSV, JSON, HDF5 };
typedef FORMAT format_type;
}

#endif  // COMP_LIBRARY_IO_LIBRARY_IO_TYPES_H

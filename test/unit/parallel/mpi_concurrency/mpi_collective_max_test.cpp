// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This file tests mpi_collective_max.hpp.
// It is run with 4 MPI processes.

#include "dca/parallel/mpi_concurrency/mpi_collective_max.hpp"
#include "gtest/gtest.h"
#include "dca/testing/minimalist_printer.hpp"

class MPICollectiveMaxTest : public ::testing::Test {
protected:
  MPICollectiveMaxTest() : max_interface_(grouping_) {
    grouping_.set();

    rank_ = grouping_.get_id();
    size_ = grouping_.get_Nr_threads();
  }

  dca::parallel::MPIProcessorGrouping grouping_;
  dca::parallel::MPICollectiveMax max_interface_;

  int rank_;
  int size_;
};

TEST_F(MPICollectiveMaxTest, FundamentalTypes) {
  // int
  int i = rank_;
  max_interface_.max(i);
  EXPECT_EQ(size_ - 1, i);

  // double
  double d = 42. + rank_;
  max_interface_.max(d);
  EXPECT_EQ(42. + size_ - 1, d);
}

int main(int argc, char** argv) {
  int result = 0;

  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  ::testing::InitGoogleTest(&argc, argv);

  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new dca::testing::MinimalistPrinter);
  }

  result = RUN_ALL_TESTS();

  MPI_Finalize();

  return result;
}

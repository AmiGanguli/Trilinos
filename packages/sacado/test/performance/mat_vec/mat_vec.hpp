// @HEADER
// ***********************************************************************
//
//                           Sacado Package
//                 Copyright (2006) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
// USA
// Questions? Contact David M. Gay (dmgay@sandia.gov) or Eric T. Phipps
// (etphipp@sandia.gov).
//
// ***********************************************************************
// @HEADER

#pragma once

#include "common.hpp"

template <typename ... ViewArgs>
Perf
do_time_val(const size_t m, const size_t n, const size_t nloop,
            const bool check);

template <typename FadType, typename ... ViewArgs>
Perf
do_time_fad(const size_t m, const size_t n, const size_t p, const size_t nloop,
            const bool check);

template <typename FadType, typename ... ViewArgs>
Perf
do_time_scratch(const size_t m, const size_t n, const size_t p,
                const size_t nloop, const bool check);

template <typename ... ViewArgs>
Perf
do_time_analytic(const size_t m, const size_t n, const size_t p,
                 const size_t nloop, const bool check);

template <int MaxP, typename ... ViewArgs>
Perf
do_time_analytic_sl(const size_t m, const size_t n, const size_t p,
                    const size_t nloop, const bool check);

template <int p, typename ... ViewArgs>
Perf
do_time_analytic_s(const size_t m, const size_t n,
                   const size_t nloop, const bool check);

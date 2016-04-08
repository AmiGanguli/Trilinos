/*
 * Copyright (c) 2014, Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 * the U.S. Government retains certain rights in this software.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 * 
 *     * Neither the name of Sandia Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#ifndef _ELB_LOADBAL_CONST_H_
#define _ELB_LOADBAL_CONST_H_

struct Machine_Description;
struct Problem_Description;
struct Solver_Description;
struct Sphere_Info;
template <typename INT> struct Graph_Description;
template <typename INT> struct LB_Description;
template <typename INT> struct Mesh_Description;
template <typename INT> struct Weight_Description;

template <typename INT>
int generate_loadbal(
  Machine_Description*   machine,
  Problem_Description* problem,
  Mesh_Description<INT>* mesh,
  LB_Description<INT>* lb,
  Solver_Description* solve,
  Graph_Description<INT>* graph,
  Weight_Description<INT>* weight,
  Sphere_Info* sphere,
  int argc,
  char *argv[]
);


template <typename INT>
int generate_maps(
  Machine_Description* machine,
  Problem_Description* problem,
  Mesh_Description<INT>* mesh,
  LB_Description<INT>* lb,
  Graph_Description<INT>* graph
);
#endif /* _ELB_LOADBAL_CONST_H_ */

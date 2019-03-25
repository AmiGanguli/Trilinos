/*
// @HEADER
// ***********************************************************************
//
//          Tpetra: Templated Linear Algebra Services Package
//                 Copyright (2008) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ************************************************************************
// @HEADER
*/

#include "Tpetra_TestingUtilities.hpp"
#include "Tpetra_FECrsMatrix.hpp"
#include "Tpetra_FEMultiVector.hpp"
#include "Tpetra_Details_getNumDiags.hpp"

// TODO: add test where some nodes have zero rows
// TODO: add test where non-"zero" graph is used to build matrix; if no values are added to matrix, the operator effect should be zero. This tests that matrix values are initialized properly.
// TODO: add test where dynamic profile initially has no allocation, then entries are added. this will test new view functionality.

namespace { // (anonymous)

using Tpetra::TestingUtilities::getDefaultComm;
using Tpetra::createContigMapWithNode;
using Teuchos::RCP;
using Teuchos::ArrayRCP;
using Teuchos::rcp;
using Teuchos::outArg;
using Teuchos::Comm;
using Teuchos::Array;
using Teuchos::ArrayView;
using Teuchos::tuple;
using Teuchos::NO_TRANS;
//using Teuchos::TRANS;
using Teuchos::CONJ_TRANS;
using std::endl;
typedef Tpetra::global_size_t GST;

template<class Scalar, class LO, class GO, class Node, class TOLERANCE >
bool compare_final_matrix_structure_impl(Teuchos::FancyOStream &out,Tpetra::CrsMatrix<Scalar,LO,GO,Node> & g1, Tpetra::CrsMatrix<Scalar,LO,GO,Node> & g2, TOLERANCE tol) {
  using std::endl;
 
  if (!g1.isFillComplete() || !g2.isFillComplete()) {out<<"Compare: FillComplete failed"<<endl;return false;}
  if (!g1.getRangeMap()->isSameAs(*g2.getRangeMap())) {out<<"Compare: RangeMap failed"<<endl;return false;}
  if (!g1.getRowMap()->isSameAs(*g2.getRowMap())) {out<<"Compare: RowMap failed"<<endl;return false;}
  if (!g1.getColMap()->isSameAs(*g2.getColMap())) {out<<"Compare: ColMap failed"<<endl;return false;}
  if (!g1.getDomainMap()->isSameAs(*g2.getDomainMap())) {out<<"Compare: DomainMap failed"<<endl;return false;}

  auto rowptr1 = g1.getLocalMatrix().graph.row_map;
  auto rowptr2 = g2.getLocalMatrix().graph.row_map;

  auto colind1 = g1.getLocalMatrix().graph.entries;
  auto colind2 = g2.getLocalMatrix().graph.entries;

  auto values1 = g1.getLocalMatrix().values;
  auto values2 = g2.getLocalMatrix().values;

  if (rowptr1.extent(0) != rowptr2.extent(0)) {out<<"Compare: rowptr extent failed"<<endl;return false;}      
  if (colind1.extent(0) != colind2.extent(0)) {out<<"Compare: colind extent failed"<<endl;return false;}      
  if (values1.extent(0) != values2.extent(0)) {out<<"Compare: values extent failed"<<endl;return false;}      

  bool success=true;
  TEST_COMPARE_ARRAYS(rowptr1,rowptr2);
  if (!success) {out<<"Compare: rowptr match failed"<<endl;return false;}

  TEST_COMPARE_ARRAYS(colind1,colind2);
  if (!success) {out<<"Compare: colind match failed"<<endl;return false;}

  TEST_COMPARE_FLOATING_ARRAYS(values1,values2,tol);
  if (!success) {out<<"Compare: values match failed"<<endl;return false;}

  return true;
}


template<class Scalar, class LO, class GO, class Node>
struct compare {
  static bool compare_final_matrix_structure(Teuchos::FancyOStream &out,Tpetra::CrsMatrix<Scalar,LO,GO,Node> & g1, Tpetra::CrsMatrix<Scalar,LO,GO,Node> & g2){ 
    typedef typename Teuchos::ScalarTraits<Scalar>::magnitudeType Mag;
    double errorTolSlack = 1.0e+2;
    const Mag tol = errorTolSlack * Teuchos::ScalarTraits<Scalar>::eps();
    return compare_final_matrix_structure_impl(out,g1,g2,tol);
  }
};


template<class LO, class GO, class Node>
struct compare<int,LO,GO,Node> {
  static bool compare_final_matrix_structure(Teuchos::FancyOStream &out,Tpetra::CrsMatrix<int,LO,GO,Node> & g1, Tpetra::CrsMatrix<int,LO,GO,Node> & g2) {
    return compare_final_matrix_structure_impl(out,g1,g2,0);
  }
};

template<class LO, class GO, class Node>
struct compare<long long,LO,GO,Node> {
  static bool compare_final_matrix_structure(Teuchos::FancyOStream &out,Tpetra::CrsMatrix<long long,LO,GO,Node> & g1, Tpetra::CrsMatrix<long long,LO,GO,Node> & g2) {
    return compare_final_matrix_structure_impl(out,g1,g2,0);
  }
};


template<class LO, class GO, class Node>
class GraphPack {
public:
  RCP<const Tpetra::Map<LO,GO,Node> > uniqueMap;
  RCP<const Tpetra::Map<LO,GO,Node> > overlapMap;
  std::vector<std::vector<GO> > element2node;

  void print(int rank, std::ostream & out) {
    using std::endl;
    out << "["<<rank<<"] Unique Map  : ";
    for(size_t i=0; i<uniqueMap->getNodeNumElements(); i++)
      out << uniqueMap->getGlobalElement(i) << " ";
    out<<endl;      

    out << "["<<rank<<"] Overlap Map : ";
    for(size_t i=0; i<overlapMap->getNodeNumElements(); i++)
      out << overlapMap->getGlobalElement(i) << " ";
    out<<endl;

    out << "["<<rank<<"] element2node: ";
    for(size_t i=0; i<(size_t)element2node.size(); i++) {
      out <<"(";
      for(size_t j=0; j<(size_t)element2node[i].size(); j++)
        out<<element2node[i][j] << " ";
      out<<") ";
    }
    out<<endl;
  }
};


template<class LO, class GO, class Node>
void generate_fem1d_graph(size_t numLocalNodes, RCP<const Comm<int> > comm , GraphPack<LO,GO,Node> & pack) {
  const GST INVALID = Teuchos::OrdinalTraits<GST>::invalid();
  int rank    = comm->getRank();
  int numProc = comm->getSize();
  size_t numOverlapNodes = numLocalNodes; if(rank!=numProc-1) numOverlapNodes++;  if(rank!=0) numOverlapNodes++;
  size_t numLocalElements = (rank == numProc-1) ? numLocalNodes -1 : numLocalNodes;
  //  printf("CMS numOverlapNodes = %d numLocalElements = %d\n",numOverlapNodes,numLocalElements);

  pack.uniqueMap = createContigMapWithNode<LO,GO,Node>(INVALID,numLocalNodes,comm);

  Teuchos::Array<GO> overlapIndices(numOverlapNodes);
  for(size_t i=0; i<numLocalNodes; i++) {
    overlapIndices[i] = pack.uniqueMap->getGlobalElement(i);
  }
  size_t last = numLocalNodes;
  if(rank != 0)           {overlapIndices[last] = overlapIndices[0] - 1; last++;}
  if(rank != numProc -1)  {overlapIndices[last] = overlapIndices[numLocalNodes-1] + 1; last++;}

  pack.overlapMap = rcp(new Tpetra::Map<LO,GO,Node>(INVALID,overlapIndices,0,comm));

  pack.element2node.resize(numLocalElements);
  for(size_t i=0; i<numLocalElements; i++) {
    pack.element2node[i].resize(2);
    pack.element2node[i][0] = pack.uniqueMap->getGlobalElement(i);
    pack.element2node[i][1] = pack.uniqueMap->getGlobalElement(i) + 1;
  }
}

template<class Scalar>
std::vector<std::vector<Scalar> > generate_fem1d_element_values() {
  std::vector<std::vector<Scalar> > mat;
  mat.resize(2);
  mat[0].resize(2);
  mat[1].resize(2);
  mat[0][0] =  1;
  mat[0][1] = -1;
  mat[1][0] = -1;
  mat[1][1] =  1;

  return mat;
}



//
// UNIT TESTS
//


////
TEUCHOS_UNIT_TEST_TEMPLATE_4_DECL( FECrsMatrix, Assemble1D, LO, GO, Scalar, Node )
{
  typedef Tpetra::FECrsMatrix<Scalar,LO,GO,Node> FEMAT;
  typedef Tpetra::CrsMatrix<Scalar,LO,GO,Node> CMAT;
  typedef Tpetra::FECrsGraph<LO,GO,Node> FEG;

  // get a comm
  RCP<const Comm<int> > comm = getDefaultComm();
  
  // Generate a mesh
  size_t numLocal = 10;
  GraphPack<LO,GO,Node> pack;
  generate_fem1d_graph(numLocal,comm,pack);
  
  // Make the graph    
  // FIXME: We should be able to get away with 3 for StaticProfile here, but we need 4 since duplicates are
  // not being handled correctly. 
  RCP<FEG> graph = rcp(new FEG(pack.uniqueMap,pack.overlapMap,4));

  graph->beginFill();
  for(size_t i=0; i<(size_t)pack.element2node.size(); i++) {
    for(size_t j=0; j<pack.element2node[i].size(); j++) {
      GO gid_j = pack.element2node[i][j];
      for(size_t k=0; k<pack.element2node[i].size(); k++) {
        GO gid_k = pack.element2node[i][k];
        //        printf("Inserting (%d,%d)\n",gid_j,gid_k);
        graph->insertGlobalIndices(gid_j,1,&gid_k);
      }
    }
  }
  graph->endFill();


  // Generate the "local stiffness matrix"
  std::vector<std::vector<Scalar> > localValues = generate_fem1d_element_values<Scalar>();

  // Make the matrix two ways
  FEMAT mat1(graph); // Here we use graph as a FECrsGraph
  CMAT mat2(graph);  // Here we use graph as a CrsGraph in OWNED mode
  mat1.beginFill();
  for(size_t i=0; i<(size_t)pack.element2node.size(); i++) {
    for(size_t j=0; j<pack.element2node[i].size(); j++) {
      GO gid_j = pack.element2node[i][j];
      for(size_t k=0; k<pack.element2node[i].size(); k++) {
        GO gid_k = pack.element2node[i][k];
        mat1.sumIntoGlobalValues(gid_j,1,&localValues[j][k],&gid_k);
        mat2.sumIntoGlobalValues(gid_j,1,&localValues[j][k],&gid_k);
      }
    }
  }
  mat1.endFill();
  mat2.fillComplete();

  success = compare<Scalar,LO,GO,Node>::compare_final_matrix_structure(out,mat1,mat2);
  TPETRA_GLOBAL_SUCCESS_CHECK(out,comm,success)
}

//
// INSTANTIATIONS
//

#define UNIT_TEST_GROUP( SCALAR, LO, GO, NODE ) \
      TEUCHOS_UNIT_TEST_TEMPLATE_4_INSTANT( FECrsMatrix, Assemble1D, LO, GO, SCALAR, NODE )

  TPETRA_ETI_MANGLING_TYPEDEFS()

  TPETRA_INSTANTIATE_SLGN( UNIT_TEST_GROUP )

} // end namespace (anonymous)



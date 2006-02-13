//@HEADER
// ************************************************************************
// 
//                 Kokkos: A Fast Kernel Package
//              Copyright (2004) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
// 
// ************************************************************************
//@HEADER

#include "Kokkos_OskiMultiVector.hpp"
#include "Kokkos_OskiVector.hpp"
#include "Kokkos_OskiMatrix.hpp"
#include "Kokkos_OskiSparseMultiply.hpp"
#include "Kokkos_Time.hpp"
#include "Kokkos_Flops.hpp"
#include "Kokkos_Version.hpp"
#include "GenerateOskiProblem.hpp"

using namespace std;
using namespace Kokkos;

#define OTYPE int
#define STYPE double
#define MULTCLASS OskiSparseMultiply

template<typename TYPE>
int PrintTestResults(string, TYPE, TYPE, bool);

int ReturnCodeCheck(string, int, int, bool);

/*

template<typename OrdinalType, typename ScalarType>
void GenerateHbProblem(bool generateClassicHbMatrix, bool isRowOriented, bool hasImplicitUnitDiagonal,
		       OrdinalType nx, OrdinalType ny, OrdinalType npoints, 
		       OrdinalType * xoff, OrdinalType * yoff, OrdinalType nrhs,
		       Kokkos::CisMatrix<OrdinalType, ScalarType> *& A, 
		       Kokkos::MultiVector<OrdinalType, ScalarType> *& x, 
		       Kokkos::MultiVector<OrdinalType, ScalarType> *& b,
		       Kokkos::MultiVector<OrdinalType, ScalarType> *&xexact,
		       OrdinalType & numEntries);
*/
typedef MultiVector<OTYPE, STYPE> OMultiVector;
typedef MultiVector<OTYPE, STYPE> OVector;
typedef CisMatrix<OTYPE, STYPE> OMatrix;

int main(int argc, char* argv[]) 
{

  bool verbose = 0;
  if (argc>1) if (argv[1][0]=='-' && argv[1][1]=='v') verbose = true;

  if (verbose)
	cout << Kokkos::Kokkos_Version() << endl << endl;

  int numberFailedTests = 0;
  string testName = "";

  OMatrix * A;
  OVector * x;
  OVector * b;
  OVector * xexact;
  OMultiVector * xm;
  OMultiVector * bm;
  OMultiVector * xexactm;
  OTYPE nx = 100;
  OTYPE ny = nx;
  OTYPE npoints = 11;
 
  OTYPE xoff[] = {-2, -1, -1,  0,  1, -1,  0,  1,  0, 1, 2};
  OTYPE yoff[] = {-1, -2, -1, -1, -1,  0,  0,  0,  1, 2, 1};

  OTYPE numEquations = nx*ny;
  OTYPE numEntries; 
    if (verbose) cout << "Size of Ordinal Type (in bytes) = " << sizeof(OTYPE) << endl
		      << "Size of Scalar  Type (in bytes) = " << sizeof(STYPE) << endl;

  {
    //bool generateClassicHbMatrix = true;
    bool isRowOriented = true;
    bool hasImplicitUnitDiagonal = false;
    OTYPE nrhs = 1;
    KokkosTest::GenerateOskiProblem<OTYPE, STYPE>
      problem(isRowOriented,hasImplicitUnitDiagonal,
	      nx, ny, npoints, xoff, yoff, nrhs, A, x, b, xexact, numEntries);
    if (verbose) cout<<endl<<"********** CHECKING OskiSparseMultiply **********" << " Dim = " << numEquations <<endl<<endl;
    
    // Check output objects
    if (verbose) cout <<"Checking Attribute accessors .......";
    if ( A->getNumRows()!=numEquations || A->getNumCols()!=numEquations || 
	 A->getIsRowOriented()!=isRowOriented ||A->getNumEntries()!=numEntries) {
      if (verbose) cout << "unsuccessful."<<endl;
      numberFailedTests++;
    } else {
      if (verbose) cout << "successful."<<endl;
    }
    if (verbose) cout <<"Checking if attribute set/check is working.......";

    Kokkos::OskiMatrix<OTYPE, STYPE> * HbA = dynamic_cast<Kokkos::OskiMatrix<OTYPE, STYPE> *>(A);
//    HbA->setHasDiagonalEntries(false); // Invalid, but testing it
//    assert(HbA->checkStructure()==-3);
//    HbA->setIsLowerTriangular(true); // Invalid, but testing it
//    assert(HbA->checkStructure()==-2);
//    HbA->setIsUpperTriangular(true); // Invalid, but testing it
//    assert(HbA->checkStructure()==-1);
    assert(HbA->checkStructure()==0);
    if (verbose) cout << "successful."<<endl;
    
    Kokkos::MULTCLASS<OTYPE, STYPE> opA;
    opA.initializeStructure(*A, true);
    opA.initializeValues(*A, true);
    
    Kokkos::Flops counter;
    opA.setFlopCounter(counter);
    Kokkos::Time timer;

    for (int ii=0; ii<20; ii++)
      opA.apply(*xexact, *x); // Use x for results

    double opAtime = timer.elapsedTime();
    double opAflops = opA.getFlops();

    double mflops = opAflops/opAtime/1000000.0;
    
    STYPE ** bvtmp = b->getValues();
    STYPE ** xvtmp = x->getValues();
    STYPE * bv = bvtmp[0];
    STYPE * xv = xvtmp[0];
    STYPE sum = 0.0;
    for (OTYPE i=0; i<numEquations; i++) sum += xv[i] - bv[i];
    if (verbose) cout << "Difference between exact and computed = " << sum << endl;
    if (verbose) cout << "MFLOPS = " << mflops << endl;
  }
/*  {
    bool generateClassicHbMatrix = false;
    bool isRowOriented = false;
    bool hasImplicitUnitDiagonal = false;
    OTYPE nrhs = 10;
    
    KokkosTest::GenerateHbProblem<OTYPE, STYPE>
      problem(generateClassicHbMatrix, isRowOriented, hasImplicitUnitDiagonal, 
	      nx, ny, npoints, xoff, yoff, nrhs, A, xm, bm, xexactm, numEntries);
    
    if (verbose) cout<<endl<<"********** CHECKING KOKKOS  Generalized HbMatrix **********"<<endl<<endl;
    
    // Check output objects
    if (verbose) cout <<"Checking Attribute accessors ";
    if ( A->getNumRows()!=numEquations || A->getNumCols()!=numEquations || 
	 A->getIsRowOriented()!=isRowOriented ||A->getNumEntries()!=numEntries) {
      if (verbose) cout << "unsuccessful."<<endl;
      numberFailedTests++;
    } else {
      if (verbose) cout << "successful."<<endl;
    }
    Kokkos::MULTCLASS<OTYPE, STYPE> opA;
    opA.initializeStructure(*A, true);
    opA.initializeValues(*A, true);
    
    Kokkos::Flops counter;
    opA.setFlopCounter(counter);
    Kokkos::Time timer;

    for (int ii=0; ii<20; ii++)
      opA.apply(*xexactm, *xm); // Use x for results

    double opAtime = timer.elapsedTime();
    double opAflops = opA.getFlops();

    double mflops = opAflops/opAtime/1000000.0;
    
    STYPE * bv = bm->getValues(0);
    STYPE * xv = xm->getValues(0);
    STYPE sum = 0.0;
    for (OTYPE i=0; i<numEquations; i++) sum += xv[i] - bv[i];
    if (verbose) cout << "Difference between exact and computed = " << sum << endl;
    if (verbose) cout << "MFLOPS = " << mflops << endl;
  }*/
  //
  // If a test failed output the number of failed tests.
  //
  if(numberFailedTests > 0) cout << "Number of failed tests: " << numberFailedTests << endl;

 return 0;
}  

template<typename TYPE>
int PrintTestResults(string testName, TYPE calculatedResult, TYPE expectedResult, bool verbose)
{
  int result;
  if(calculatedResult == expectedResult)
    {
      if(verbose) cout << testName << " successful." << endl;
      result = 0;
    }
  else
    {
      if(verbose) cout << testName << " unsuccessful." << endl;
      result = 1;
    }
  return result;
}

int ReturnCodeCheck(string testName, int returnCode, int expectedResult, bool verbose)
{
  int result;
  if(expectedResult == 0)
    {
      if(returnCode == 0)
	{
	  if(verbose) cout << testName << " test successful." << endl;
	  result = 0;
	}
      else
	{
	  if(verbose) cout << testName << " test unsuccessful. Return code was " << returnCode << "." << endl;
	  result = 1;
	}
    }
  else
    {
      if(returnCode != 0)
	{
	  if(verbose) cout << testName << " test successful -- failed as expected." << endl;
	  result = 0;
	}
      else
	{
	  if(verbose) cout << testName << " test unsuccessful -- did not fail as expected. Return code was " << returnCode << "." << endl;
	  result = 1;
	}
    }
  return result;
}

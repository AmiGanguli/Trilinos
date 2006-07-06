// @HEADER
// ***********************************************************************
// 
//                    Teuchos: Common Tools Package
//                 Copyright (2004) Sandia Corporation
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
// ***********************************************************************
// @HEADER

#include "Teuchos_ArrayRefCountPtr.hpp"
#include "Teuchos_GlobalMPISession.hpp"
#include "Teuchos_CommandLineProcessor.hpp"
#include "Teuchos_VerboseObject.hpp"
#include "Teuchos_StandardCatchMacros.hpp"
#include "Teuchos_Version.hpp"

//#define SHOW_COMPILE_FAILURE_1;

template<class T>
void test_ArrayRefCountPtr_iterators(
  const Teuchos::ArrayRefCountPtr<T>   &ptr
  ,const bool                          verbose
  ,Teuchos::FancyOStream               &out_arg
  )
{

  using Teuchos::ArrayRefCountPtr;
  using Teuchos::null;
  using Teuchos::arcp;
  using Teuchos::arcp_reinterpret_cast;

  Teuchos::RefCountPtr<Teuchos::FancyOStream>
    out = Teuchos::rcp(&out_arg,false);
  
  if(verbose)
    *out << "\nTesting iterators and accessors for ptr = " << ptr <<"\n";

  Teuchos::OSTab tab(out);

  const int dim = ptr.dim();
  
  if(1) {
    if(verbose)
      *out << "\nChecking ++itr:\n";
    ArrayRefCountPtr<T> itr = ptr;
    for( int i = 0; i < dim; ++i, ++itr )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking itr++:\n";
    ArrayRefCountPtr<T> itr = ptr;
    for( int i = 0; i < dim; ++i, itr++ )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking itr+=1:\n";
    ArrayRefCountPtr<T> itr = ptr;
    for( int i = 0; i < dim; ++i, itr+=1 )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking itr-=(-1):\n";
    ArrayRefCountPtr<T> itr = ptr;
    for( int i = 0; i < dim; ++i, itr-=(-1) )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking itr=itr+1:\n";
    ArrayRefCountPtr<T> itr = ptr;
    for( int i = 0; i < dim; ++i, itr=itr+1 )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking itr=itr-(-1):\n";
    ArrayRefCountPtr<T> itr = ptr;
    for( int i = 0; i < dim; ++i, itr=itr-(-1) )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking iterator ++itr:\n";
    typename ArrayRefCountPtr<T>::const_iterator itr = ptr.begin();
    for( int i = 0; i < dim; ++i, ++itr )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking iterator itr++:\n";
    typename ArrayRefCountPtr<T>::const_iterator itr = ptr.begin();
    for( int i = 0; i < dim; ++i, itr++ )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking iterator itr+=1:\n";
    typename ArrayRefCountPtr<T>::const_iterator itr = ptr.begin();
    for( int i = 0; i < dim; ++i, itr+=1 )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking iterator itr-=(-1):\n";
    typename ArrayRefCountPtr<T>::const_iterator itr = ptr.begin();
    for( int i = 0; i < dim; ++i, itr-=(-1) )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking iterator itr=itr+1:\n";
    typename ArrayRefCountPtr<T>::const_iterator itr = ptr.begin();
    for( int i = 0; i < dim; ++i, itr=itr+1 )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }
  
  if(1) {
    if(verbose)
      *out << "\nChecking iterator itr=itr-(-1):\n";
    typename ArrayRefCountPtr<T>::const_iterator itr = ptr.begin();
    for( int i = 0; i < dim; ++i, itr=itr-(-1) )
      TEST_FOR_EXCEPT( !(*itr == ptr[i]) );
  }

}

template<class T>
void test_ArrayRefCountPtr(
  const Teuchos::ArrayRefCountPtr<T>   &ptr
  ,const bool                          verbose
  ,Teuchos::FancyOStream               &out_arg
  )
{

  using Teuchos::ArrayRefCountPtr;
  using Teuchos::null;
  using Teuchos::arcp;
  using Teuchos::arcp_reinterpret_cast;

  Teuchos::RefCountPtr<Teuchos::FancyOStream>
    out = Teuchos::rcp(&out_arg,false);
  
  if(verbose)
    *out << "\nTesting ptr = " << ptr <<"\n";

  Teuchos::OSTab tab(out);

  const int dim = ptr.dim();
  
  if(1) {
    if(verbose)
      *out << "\nInitializing data ...\n";
    for( int i = 0; i < dim; ++i )
      ptr[i] = i;
  }

  TEST_FOR_EXCEPT( !(&*ptr == ptr.get()) )

  test_ArrayRefCountPtr_iterators(ptr,verbose,out_arg);

}

int main( int argc, char* argv[] ) {

  using Teuchos::CommandLineProcessor;
  using Teuchos::ArrayRefCountPtr;
  using Teuchos::null;
  using Teuchos::arcp;
  using Teuchos::arcp_reinterpret_cast;
	
	bool success = true, verbose = true;
  
  Teuchos::GlobalMPISession mpiSession(&argc,&argv);
  //const int procRank = Teuchos::GlobalMPISession::getRank();
  
  Teuchos::RefCountPtr<Teuchos::FancyOStream>
    out = Teuchos::VerboseObjectBase::getDefaultOStream();
  
	try {

		// Read options from the commandline
    int   num_ints = 10;
    int   num_doubles = 10;
    CommandLineProcessor  clp(false); // Don't throw exceptions
    clp.setOption( "num-ints", &num_ints, "Number of ints to allocate space for" );
    clp.setOption( "num-doubles", &num_doubles, "Number of doubles to allocate space for" );
		clp.setOption( "verbose", "quiet", &verbose, "Set if output is printed or not." );
		CommandLineProcessor::EParseCommandLineReturn parse_return = clp.parse(argc,argv);
		if( parse_return != CommandLineProcessor::PARSE_SUCCESSFUL ) {
			*out << "\nEnd Result: TEST FAILED" << endl;
			return parse_return;
		}

    const int sizeOfDouble = sizeof(double);
    const int sizeOfInt = sizeof(int);

    const int total_bytes = num_doubles*sizeOfDouble + num_ints*sizeOfInt;

    if(verbose)
      *out << std::endl << Teuchos::Teuchos_Version() << std::endl;
    
		if(verbose)
			*out << "\nTesting basic ArrayRefCountPtr functionality ...\n";

    ArrayRefCountPtr<char>
      char_ptr1 = arcp<char>(total_bytes);

    if(verbose)
      *out << "\nchar_ptr1 = " << char_ptr1 << "\n";

    TEST_FOR_EXCEPT( !(char_ptr1.dim() == total_bytes) );
    TEST_FOR_EXCEPT( !(char_ptr1.lowerOffset() == 0) );
    TEST_FOR_EXCEPT( !(char_ptr1.upperOffset() == total_bytes-1) );
    TEST_FOR_EXCEPT( !(char_ptr1.count() == 1) );

    ArrayRefCountPtr<char>
      char_ptr2 = null;

    if(verbose)
      *out << "\nchar_ptr2 = " << char_ptr2 << "\n";

    TEST_FOR_EXCEPT( !(char_ptr2.dim() == 0) );
    TEST_FOR_EXCEPT( !(char_ptr2.get() == NULL) );
    TEST_FOR_EXCEPT( !(char_ptr2.count() == 0) );

    ArrayRefCountPtr<char>
      char_ptr2b(char_ptr1); // excplicitly test copy constructor

    if(verbose)
      *out << "\nchar_ptr2b = " << char_ptr2b << "\n";

    TEST_FOR_EXCEPT( !(char_ptr2b.dim() == total_bytes) );
    TEST_FOR_EXCEPT( !(char_ptr2b.lowerOffset() == 0) );
    TEST_FOR_EXCEPT( !(char_ptr2b.upperOffset() == total_bytes-1) );
    TEST_FOR_EXCEPT( !(char_ptr2b.count() == 2) );

    char_ptr2b = null;

    TEST_FOR_EXCEPT( !(char_ptr2b.dim() == 0) );
    TEST_FOR_EXCEPT( !(char_ptr2.get() == NULL) );
    TEST_FOR_EXCEPT( !(char_ptr2b.count() == 0) );
    TEST_FOR_EXCEPT( !(char_ptr1.count() == 1) );

    ArrayRefCountPtr<char>
      char_ptr3 = char_ptr1.subview(total_bytes/2,total_bytes-1);

    TEST_FOR_EXCEPT( !(char_ptr1.count() == 2) );
    TEST_FOR_EXCEPT( !(char_ptr3.count() == 2) );
    TEST_FOR_EXCEPT( !(char_ptr3.lowerOffset() == 0) );
    TEST_FOR_EXCEPT( !(char_ptr3.upperOffset() == total_bytes/2-1) );

    if(verbose)
      *out << "\nchar_ptr3 = " << char_ptr3 << "\n";

    if(verbose)
      *out << "\nBreak up char_ptr1 into views of double and int data\n";

    int offset = 0;
    
    ArrayRefCountPtr<double>
      double_ptr1 = arcp_reinterpret_cast<double>(char_ptr1.subview(offset,offset+sizeOfDouble*num_doubles-1));

    if(verbose)
      *out
        << "\ndouble_ptr1 = " << double_ptr1 << "\n";

    TEST_FOR_EXCEPT( !(char_ptr1.count() == 3) );
    TEST_FOR_EXCEPT( !(double_ptr1.count() == 3) );
    TEST_FOR_EXCEPT( !(double_ptr1.dim() == num_doubles) );

    test_ArrayRefCountPtr(double_ptr1,verbose,*out);
    
    offset += sizeOfDouble*num_doubles;

    ArrayRefCountPtr<int>
      int_ptr1 = arcp_reinterpret_cast<int>(char_ptr1.subview(offset,offset+sizeOfInt*num_ints-1));

    if(verbose)
      *out
        << "\nint_ptr1 = " << int_ptr1 << "\n";

    TEST_FOR_EXCEPT( !(char_ptr1.count() == 4) );
    TEST_FOR_EXCEPT( !(int_ptr1.count() == 4) );
    TEST_FOR_EXCEPT( !(int_ptr1.dim() == num_ints) );

    test_ArrayRefCountPtr(int_ptr1,verbose,*out);

    if(verbose)
      *out << "\nCreating a constant view of data\n";
    
    ArrayRefCountPtr<const double>
      double_ptr2 = double_ptr1.getConst();

    test_ArrayRefCountPtr_iterators(double_ptr2,verbose,*out);

#ifdef SHOW_COMPILE_FAILURE_1
    // This will not compile since this function tries to use operator[] to
    // change data but it can't since it returns a reference to a const
    // double!
    test_ArrayRefCountPtr(double_ptr2,verbose,*out);
#endif
    
    // ToDo: Fill in the rest of the tests!
    
		if(verbose)
			*out << "\nAll tests for ArrayRefCountPtr seem to check out!\n";
    
	}
  TEUCHOS_STANDARD_CATCH_STATEMENTS(verbose,std::cerr,success);
  
  if(success)
    *out << "\nEnd Result: TEST PASSED" << std::endl;	
  
  return ( success ? 0 : 1 );
  
}

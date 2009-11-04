#ifndef __tBlockedEpetraOperator_hpp__
#define __tBlockedEpetraOperator_hpp__

// Teuchos includes
#include "Teuchos_RCP.hpp"

// Epetra includes
#include "Epetra_Map.h"
#include "Epetra_CrsMatrix.h"
#include "Epetra_Vector.h"

#include <string>

#include "Test_Utils.hpp"

namespace PB {
namespace Test {

class tBlockedEpetraOperator : public UnitTest {
public:
   virtual ~tBlockedEpetraOperator() {}

   virtual void initializeTest();
   virtual int runTest(int verbosity,std::ostream & stdstrm,std::ostream & failstrm,int & totalrun);
   virtual bool isParallel() const { return true; }

   bool test_vector_constr(int verbosity,std::ostream & os);
   bool test_reorder(int verbosity,std::ostream & os,int total);

protected:
   void buildBlockGIDs(std::vector<std::vector<int> > & blocks,const Epetra_Map & map) const;

   double tolerance_;
};

} // end namespace Tests
} // end namespace PB

#endif

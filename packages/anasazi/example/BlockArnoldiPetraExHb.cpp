#include "AnasaziPetraInterface.hpp"
#include "AnasaziBlockArnoldi.hpp"
#include "AnasaziCommon.hpp"
#include "Epetra_CrsMatrix.h"

#include "Trilinos_Util.h"
#include "Util.h"
//
//
#ifdef EPETRA_MPI
#include "Epetra_MpiComm.h"
#include <mpi.h>
#else
#include "Epetra_SerialComm.h"
#endif
#include "Epetra_Map.h"

int main(int argc, char *argv[]) {
	//
	int i, j;
	int n_nonzeros, N_update;
	int *bindx=0, *update=0, *col_inds=0;
	double *val=0, *row_vals=0;
	
#ifdef EPETRA_MPI	
	// Initialize MPI	
	MPI_Init(&argc,&argv); 	
	Epetra_MpiComm Comm( MPI_COMM_WORLD );	
#else	
	Epetra_SerialComm Comm;	
#endif
	
	int MyPID = Comm.MyPID();
	int NumProc = Comm.NumProc();
	
	bool verbose = (MyPID==0);
	//
    	if(argc < 2 && verbose) {
     	cerr << "Usage: " << argv[0] 
	 << " HB_filename " << endl
	 << "where:" << endl
	 << "HB_filename        - filename and path of a Harwell-Boeing data set" << endl
	 << endl;
    	return(1);
	}
	//
	//**********************************************************************
	//******************Set up the problem to be solved*********************
	//**********************************************************************
    	//
    	int NumGlobalElements;  // total # of rows in matrix
	//
	// *****Read in matrix from HB file******
	//
	read_hb_matrix(argv[1], MyPID, &NumGlobalElements, &n_nonzeros, &val, 
		                    &bindx);
	//
	// *****Distribute data among processors*****
	//
	distrib_msr_matrix(Comm, &NumGlobalElements, &n_nonzeros, &N_update,
		                             &update, &val, &bindx);
	//
	// *****Construct the matrix*****
	//
	int NumMyElements = N_update; // # local rows of matrix on processor
	//
    	// Create an integer vector NumNz that is used to build the Petra Matrix.
	// NumNz[i] is the Number of OFF-DIAGONAL term for the ith global equation 
	// on this processor
	//
	int * NumNz = new int[NumMyElements];
	for (i=0; i<NumMyElements; i++) {
		NumNz[i] = bindx[i+1] - bindx[i] + 1;
	}
	//
	Epetra_Map Map(NumGlobalElements, NumMyElements, update, 0, Comm);
	//
	// Create a Epetra_Matrix
	//
	Epetra_CrsMatrix A(Copy, Map, NumNz);
	//
	// Add rows one-at-a-time
	//
	int NumEntries;
	for (i=0; i<NumMyElements; i++) {
		row_vals = val + bindx[i];
		col_inds = bindx + bindx[i];
		NumEntries = bindx[i+1] - bindx[i];
		assert(A.InsertGlobalValues(update[i], NumEntries, row_vals, col_inds)==0);
		assert(A.InsertGlobalValues(update[i], 1, val+i, update+i)==0);
	}
	//
	// Finish up
	//
	assert(A.TransformToLocal()==0);
	A.SetTracebackMode(1); // Shutdown Epetra Warning tracebacks
	//
        //************************************
        // Start the block Arnoldi iteration
        //***********************************         
	//
        //  Variables used for the Block Arnoldi Method
        // 
        int block = 3;
        int length = 8;
        int nev = 10;
        double tol = 1.0e-8;
        string which="LM";
        int step = 1;
        int restarts = 10;
	//
        // create a PetraAnasaziVec. Note that the decision to make a view or
        // or copy is determined by the petra constructor called by AnasaziPetraVec.
        // This is possible because I pass in arguements needed by petra.
        AnasaziPetraVec<double> ivec(Map, block);
        ivec.MvRandom();
        // call the ctor that calls the petra ctor for a matrix
        AnasaziPetraMat<double> Amat(A);
        AnasaziEigenproblem<double> MyProblem(&Amat, &ivec);
	//
	//  Initialize the Block Arnoldi solver
	//
        Anasazi::BlockArnoldi<double> MyBlockArnoldi(MyProblem, tol, nev, length, block,
                                         which, step, restarts);

	//MyBlockArnoldi.setSymmetric(true);
        MyBlockArnoldi.setDebugLevel(0);

#ifdef UNIX
        Epetra_Time & timer = *new Epetra_Time(Comm);
#endif

        // iterate a few steps (if you wish)
        //MyBlockArnoldi.iterate(5);

        // solve the problem to the specified tolerances or length
        MyBlockArnoldi.solve();
#ifdef UNIX
        double elapsed_time = timer.ElapsedTime();
        double total_flops = A.Flops(); 
        double MFLOPs = total_flops/elapsed_time/1000000.0;
#endif

        // obtain results directly
        double* resids = MyBlockArnoldi.getResiduals();
        double* evals = MyBlockArnoldi.getEvals();
        
        AnasaziPetraVec<double> evecs(Map, nev);
        MyBlockArnoldi.getEvecs( evecs );
	
	
        // output results to screen
        //MyBlockArnoldi.currentStatus();

// Release all objects  

  delete [] NumNz;
	
  return 0;
  //
} // end test_bl_pgmrs_hb.cpp

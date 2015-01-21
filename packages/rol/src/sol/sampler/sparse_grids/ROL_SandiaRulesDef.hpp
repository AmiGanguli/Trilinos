// @HEADER
// ************************************************************************
//
//               Rapid Optimization Library (ROL) Package
//                 Copyright (2014) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
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
// Questions? Contact lead developers:
//              Drew Kouri   (dpkouri@sandia.gov) and
//              Denis Ridzal (dridzal@sandia.gov)
//
// ************************************************************************
// @HEADER


namespace ROL {

//**************************************************************************80
void SandiaRules::binary_vector_next ( int n, int bvec[] )
//**************************************************************************80
//
//  Purpose:
//
//    BINARY_VECTOR_NEXT generates the next binary vector.
//
//  Discussion:
//
//    A binary vector is a vector whose entries are 0 or 1.
//
//    The user inputs an initial zero vector to start.  The program returns
//    the "next" vector.
//
//    The vectors are produced in the order:
//
//    ( 0, 0, 0, ..., 0 )
//    ( 1, 0, 0, ..., 0 )
//    ( 0, 1, 0, ..., 0 )
//    ( 1, 1, 0, ..., 0 )
//    ( 0, 0, 1, ..., 0 )
//    ( 1, 0, 1, ..., 0 )
//               ...
//    ( 1, 1, 1, ..., 1)
//
//    and the "next" vector after (1,1,...,1) is (0,0,...,0).  That is,
//    we allow wrap around.
//
//  Example:
//
//    N = 3
//
//    Input      Output
//    -----      ------
//    0 0 0  =>  1 0 0
//    1 0 0  =>  0 1 0
//    0 1 0  =>  1 1 0
//    1 1 0  =>  0 0 1
//    0 0 1  =>  1 0 1
//    1 0 1  =>  0 1 1
//    0 1 1  =>  1 1 1
//    1 1 1  =>  0 0 0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 September 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of the vectors.
//
//    Input/output, int BVEC[N], on output, the successor
//    to the input vector.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    if ( bvec[i] == 1 )
    {
      bvec[i] = 0;
    }
    else
    {
      bvec[i] = 1;
      break;
    }
  }
  return;
}


//**************************************************************************80
void SandiaRules::ccn_compute ( int n, double x[], double w[] )
//**************************************************************************80
//
//  Purpose:
//
//    CCN_COMPUTE computes a nested Clenshaw Curtis quadrature rule.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  ccn_compute_points ( n, x );
  ccn_compute_weights ( n, w );

  return;
}


//**************************************************************************80
void SandiaRules::ccn_compute_np ( int n, int np, double p[], double x[], double w[] )
//**************************************************************************80
//
//  Purpose:
//
//    CCN_COMPUTE_NP computes a nested Clenshaw Curtis quadrature rule.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  ccn_compute_points ( n, x );
  ccn_compute_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::ccn_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    CCN_COMPUTE_POINTS: compute nested Clenshaw Curtis points.
//
//  Discussion:
//
//    We want to compute the following sequence:
//
//    1/2,
//    0, 1
//    1/4, 3/4
//    1/8, 3/8, 5/8, 7/8,
//    1/16, 3/16, 5/16, 7/16, 9/16, 11/16, 13/16, 15/16, and so on.
//
//    But we would prefer that the numbers in each row be regrouped in pairs
//    that are symmetric about 1/2, with the number above 1/2 coming first.
//    Thus, the last row might become:
//    (9/16, 7/16), (11/16, 5/16), ..., (15/16, 1/16).
//
//    Once we have our sequence, we apply the Chebyshev transformation
//    which maps [0,1] to [-1,+1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    06 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements to compute.
//
//    Output, double X[N], the elements of the sequence.
//
{
  int d;
  int i;
  int k;
  int m;
  double pi = 3.141592653589793;
  int td;
  int tu;
//
//  Handle first three entries specially.
//
  if ( 1 <= n )
  {
    x[0] = 0.5;
  }

  if ( 2 <= n )
  {
    x[1] = 1.0;
  }

  if ( 3 <= n )
  {
    x[2] = 0.0;
  }

  m = 3;
  d = 2;

  while ( m < n )
  {
    tu = d + 1;
    td = d - 1;

    k = i4_min ( d, n - m );

    for ( i = 1; i <= k; i++ )
    {
      if ( ( i % 2 ) == 1 )
      {
        x[m+i-1] = tu / 2.0 / ( double ) ( k );
        tu = tu + 2;
      }
      else
      {
        x[m+i-1] = td / 2.0 / ( double ) ( k );
        td = td - 2;
      }
    }
    m = m + k;
    d = d * 2;
  }
//
//  Apply the Chebyshev transformation.
//
  for ( i = 0; i < n; i++ )
  {
    x[i] = std::cos ( x[i] * pi );
  }
  x[0] = 0.0;

  if ( 2 <= n )
  {
    x[1] = -1.0;
  }

  if ( 3 <= n )
  {
    x[2] = +1.0;
  }

  return;
}
//**************************************************************************80

void SandiaRules::ccn_compute_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    CCN_COMPUTE_POINTS_NP: nested Clenshaw Curtis quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  ccn_compute_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::ccn_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CCN_COMPUTE_WEIGHTS: weights for nested Clenshaw Curtis rule.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the rule.
//
//    Output, double W[N], the weights.
//
{
  double *x;
  double x_max;
  double x_min;

  x = new double[n];

  ccn_compute_points ( n, x );
//
//  Get the weights.
//
  x_min = -1.0;
  x_max = +1.0;

  nc_compute ( n, x_min, x_max, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::ccn_compute_weights_np ( int n, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CCN_COMPUTE_WEIGHTS_NP: nested Clenshaw Curtis quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  ccn_compute_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev1_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV1_COMPUTE computes a Chebyshev type 1 quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) F(X) / sqrt ( 1 - x^2 ) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  int i;
  double pi = 3.141592653589793;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CHEBYSHEV1_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    w[i] = pi / ( double ) ( n );
  }
  for ( i = 0; i < n; i++ )
  {
    x[i] = std::cos ( pi * ( double ) ( 2 * n - 1 - 2 * i )
                         / ( double ) ( 2 * n ) );
  }
  if ( ( n % 2 ) == 1 )
  {
    x[(n-1)/2] = 0.0;
  }

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev1_compute_np ( int n, int np, double p[], double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV1_COMPUTE_NP computes a Chebyshev type 1 quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) F(X) / sqrt ( 1 - x^2 ) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  chebyshev1_compute ( n, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev1_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV1_COMPUTE_POINTS computes Chebyshev type 1 quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
{
  int i;
  double pi = 3.141592653589793;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CHEBYSHEV1_COMPUTE_POINTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    x[i] =  std::cos ( pi * ( double ) ( 2 * n - 1 - 2 * i )
                          / ( double ) ( 2 * n ) );
  }
  if ( ( n % 2 ) == 1 )
  {
    x[(n-1)/2] = 0.0;
  }

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev1_compute_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV1_COMPUTE_POINTS_NP computes Chebyshev type 1 quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  chebyshev1_compute_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev1_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV1_COMPUTE_WEIGHTS computes Chebyshev type 1 quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double W[N], the weights.
//
{
  int i;
  double pi = 3.141592653589793;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CHEBYSHEV1_COMPUTE_WEIGHTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    w[i] = pi / ( double ) ( n );
  }

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev1_compute_weights_np ( int n, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV1_COMPUTE_WEIGHTS_NP: Chebyshev type 1 quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  chebyshev1_compute_weights ( n, w );

  return;
}
//**************************************************************************80

double SandiaRules::chebyshev1_integral ( int expon )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV1_INTEGRAL evaluates a monomial Chebyshev type 1 integral.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -1 <= x <= +1 ) x^n / sqrt ( 1 - x^2 ) dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    26 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int EXPON, the exponent.
//
//    Output, double CHEBYSHEV1_INTEGRAL, the value of the exact integral.
//
{
  double bot;
  double exact;
  int i;
  double pi = 3.141592653589793;
  double top;
//
//  Get the exact value of the integral.
//
  if ( ( expon % 2 ) == 0 )
  {
    top = 1;
    bot = 1;
    for ( i = 2; i <= expon; i = i + 2 )
    {
      top = top * ( i - 1 );
      bot = bot *   i;
    }

    exact = pi * ( double ) ( top ) / ( double ) ( bot );
  }
  else
  {
    exact = 0.0;
  }

  return exact;
}
//**************************************************************************80

void SandiaRules::chebyshev2_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV2_COMPUTE computes a Chebyshev type 2 quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -1 <= x <= 1 ) f(x)  sqrt ( 1 - x^2 )  dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double angle;
  int i;
  double pi = 3.141592653589793;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CHEBYSHEV2_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    angle = pi * ( double ) ( n - i ) / ( double ) ( n + 1 );
    w[i] = pi / ( double ) ( n + 1 ) * std::pow ( std::sin ( angle ), 2 );
    x[i] = std::cos ( angle );
  }

  if ( ( n % 2 ) == 1 )
  {
    x[(n-1)/2] = 0.0;
  }

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev2_compute_np ( int n, int np, double p[], double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV2_COMPUTE_NP computes a Chebyshev type 2 quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) F(X)  sqrt ( 1 - x^2 )  dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  chebyshev2_compute ( n, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev2_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV2_COMPUTE_POINTS computes Chebyshev type 2 quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
{
  double angle;
  int i;
  double pi = 3.141592653589793;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CHEBYSHEV2_COMPUTE_POINTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    angle = pi * ( double ) ( n - i ) / ( double ) ( n + 1 );
    x[i] =  std::cos ( angle );
  }

  if ( ( n % 2 ) == 1 )
  {
    x[(n-1)/2] = 0.0;
  }

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev2_compute_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV2_COMPUTE_POINTS_NP computes Chebyshev type 2 quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  chebyshev2_compute_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev2_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV2_COMPUTE_WEIGHTS computes Chebyshev type 2 quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double W[N], the weights.
//
{
  double angle;
  int i;
  double pi = 3.141592653589793;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CHEBYSHEV2_COMPUTE_WEIGHTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    angle = pi * ( double ) ( n - i ) / ( double ) ( n + 1 );
    w[i] = pi / ( double ) ( n + 1 ) * std::pow ( std::sin ( angle ), 2 );
  }

  return;
}
//**************************************************************************80

void SandiaRules::chebyshev2_compute_weights_np ( int n, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV2_COMPUTE_WEIGHTS_NP: Chebyshev type 2 quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  chebyshev2_compute_weights ( n, w );

  return;
}
//**************************************************************************80

double SandiaRules::chebyshev2_integral ( int expon )

//**************************************************************************80
//
//  Purpose:
//
//    CHEBYSHEV2_INTEGRAL evaluates a monomial Chebyshev type 2 integral.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -1 <= x <= +1 ) x^n * sqrt ( 1 - x^2 ) dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    26 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int EXPON, the exponent.
//
//    Output, double CHEBYSHEV2_INTEGRAL, the value of the exact integral.
//
{
  double bot;
  double exact;
  int i;
  double pi = 3.141592653589793;
  double top;
//
//  Get the exact value of the integral.
//
  if ( ( expon % 2 ) == 0 )
  {
    top = 1;
    bot = 1;
    for ( i = 2; i <= expon; i = i + 2 )
    {
      top = top * ( i - 1 );
      bot = bot *   i;
    }

	bot = bot * ( double ) ( expon + 2 );

    exact = pi * ( double ) ( top ) / ( double ) ( bot );
  }
  else
  {
    exact = 0.0;
  }
  return exact;
}
//**************************************************************************80

void SandiaRules::clenshaw_curtis_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CLENSHAW_CURTIS_COMPUTE computes a Clenshaw Curtis quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 March 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double b;
  int i;
  int j;
  double pi = 3.141592653589793;
  double theta;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CLENSHAW_CURTIS_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }
  else if ( n == 1 )
  {
    x[0] = 0.0;
    w[0] = 2.0;
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      x[i] =  std::cos ( ( double ) ( n - 1 - i ) * pi
                       / ( double ) ( n - 1     ) );
    }
    x[0] = -1.0;
    if ( ( n % 2 ) == 1 )
    {
      x[(n-1)/2] = 0.0;
    }
    x[n-1] = +1.0;

    for ( i = 0; i < n; i++ )
    {
      theta = ( double ) ( i ) * pi / ( double ) ( n - 1 );

      w[i] = 1.0;

      for ( j = 1; j <= ( n - 1 ) / 2; j++ )
      {
        if ( 2 * j == ( n - 1 ) )
        {
          b = 1.0;
        }
        else
        {
          b = 2.0;
        }

        w[i] = w[i] - b *  std::cos ( 2.0 * ( double ) ( j ) * theta )
          / ( double ) ( 4 * j * j - 1 );
      }
    }

    w[0] = w[0] / ( double ) ( n - 1 );
    for ( i = 1; i < n - 1; i++ )
    {
      w[i] = 2.0 * w[i] / ( double ) ( n - 1 );
    }
    w[n-1] = w[n-1] / ( double ) ( n - 1 );
  }

  return;
}
//**************************************************************************80

void SandiaRules::clenshaw_curtis_compute_np ( int n, int np, double p[], double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CLENSHAW_CURTIS_COMPUTE_NP computes a Clenshaw Curtis quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  clenshaw_curtis_compute ( n, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::clenshaw_curtis_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    CLENSHAW_CURTIS_COMPUTE_POINTS computes Clenshaw Curtis quadrature points.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    This rule is defined on [-1,1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
{
  int index;
  double pi = 3.141592653589793;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CLENSHAW_CURTIS_COMPUTE_POINTS - Fatal error!\n";
    std::cerr << "  N < 1.\n";
    std::exit ( 1 );
  }
  else if ( n == 1 )
  {
    x[0] = 0.0;
  }
  else
  {
    for ( index = 1; index <= n; index++ )
    {
      x[index-1] =  std::cos ( ( double ) ( n - index ) * pi
                             / ( double ) ( n - 1     ) );
    }
    x[0] = -1.0;
    if ( ( n % 2 ) == 1 )
    {
      x[(n-1)/2] = 0.0;
    }
    x[n-1] = +1.0;
  }
  return;
}
//**************************************************************************80

void SandiaRules::clenshaw_curtis_compute_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    CLENSHAW_CURTIS_COMPUTE_POINTS_NP: Clenshaw Curtis quadrature points.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    This rule is defined on [-1,1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  clenshaw_curtis_compute_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::clenshaw_curtis_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CLENSHAW_CURTIS_COMPUTE_WEIGHTS computes Clenshaw Curtis quadrature weights.
//
//  Discussion:
//
//    The user must preallocate space for the output array W.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Charles Clenshaw, Alan Curtis,
//    A Method for Numerical Integration on an Automatic Computer,
//    Numerische Mathematik,
//    Volume 2, Number 1, December 1960, pages 197-205.
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double W[N], the weights.
//
{
  double b;
  int i;
  int j;
  double pi = 3.141592653589793;
  double theta;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "CLENSHAW_CURTIS_COMPUTE_WEIGHTS - Fatal error!\n";
    std::cerr << "  N < 1.\n";
    std::exit ( 1 );
  }
  else if ( n == 1 )
  {
    w[0] = 2.0;
    return;
  }

  for ( i = 1; i <= n; i++ )
  {
    theta = ( double ) ( i - 1 ) * pi / ( double ) ( n - 1 );

    w[i-1] = 1.0;

    for ( j = 1; j <= ( n - 1 ) / 2; j++ )
    {
      if ( 2 * j == ( n - 1 ) )
      {
        b = 1.0;
      }
      else
      {
        b = 2.0;
      }

      w[i-1] = w[i-1] - b *  std::cos ( 2.0 * ( double ) ( j ) * theta )
           / ( double ) ( 4 * j * j - 1 );
    }
  }

  w[0] = w[0] / ( double ) ( n - 1 );
  for ( i = 1; i < n - 1; i++ )
  {
    w[i] = 2.0 * w[i] / ( double ) ( n - 1 );
  }
  w[n-1] = w[n-1] / ( double ) ( n - 1 );

  return;
}
//**************************************************************************80

void SandiaRules::clenshaw_curtis_compute_weights_np ( int n, int np, double p[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    CLENSHAW_CURTIS_COMPUTE_WEIGHTS_NP: Clenshaw Curtis quadrature weights.
//
//  Discussion:
//
//    The user must preallocate space for the output array W.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Charles Clenshaw, Alan Curtis,
//    A Method for Numerical Integration on an Automatic Computer,
//    Numerische Mathematik,
//    Volume 2, Number 1, December 1960, pages 197-205.
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  clenshaw_curtis_compute_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::comp_next ( int n, int k, int a[], bool *more, int *h, int *t )

//**************************************************************************80
//
//  Purpose:
//
//    COMP_NEXT computes the compositions of the integer N into K parts.
//
//  Discussion:
//
//    A composition of the integer N into K parts is an ordered sequence
//    of K nonnegative integers which sum to N.  The compositions (1,2,1)
//    and (1,1,2) are considered to be distinct.
//
//    The routine computes one composition on each call until there are no more.
//    For instance, one composition of 6 into 3 parts is
//    3+2+1, another would be 6+0+0.
//
//    On the first call to this routine, set MORE = FALSE.  The routine
//    will compute the first element in the sequence of compositions, and
//    return it, as well as setting MORE = TRUE.  If more compositions
//    are desired, call again, and again.  Each time, the routine will
//    return with a new composition.
//
//    However, when the LAST composition in the sequence is computed
//    and returned, the routine will reset MORE to FALSE, signaling that
//    the end of the sequence has been reached.
//
//    This routine originally used a SAVE statement to maintain the
//    variables H and T.  I have decided that it is safer
//    to pass these variables as arguments, even though the user should
//    never alter them.  This allows this routine to safely shuffle
//    between several ongoing calculations.
//
//
//    There are 28 compositions of 6 into three parts.  This routine will
//    produce those compositions in the following order:
//
//     I         A
//     -     ---------
//     1     6   0   0
//     2     5   1   0
//     3     4   2   0
//     4     3   3   0
//     5     2   4   0
//     6     1   5   0
//     7     0   6   0
//     8     5   0   1
//     9     4   1   1
//    10     3   2   1
//    11     2   3   1
//    12     1   4   1
//    13     0   5   1
//    14     4   0   2
//    15     3   1   2
//    16     2   2   2
//    17     1   3   2
//    18     0   4   2
//    19     3   0   3
//    20     2   1   3
//    21     1   2   3
//    22     0   3   3
//    23     2   0   4
//    24     1   1   4
//    25     0   2   4
//    26     1   0   5
//    27     0   1   5
//    28     0   0   6
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 July 2008
//
//  Author:
//
//    Original FORTRAN77 version by Albert Nijenhuis, Herbert Wilf.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms for Computers and Calculators,
//    Second Edition,
//    Academic Press, 1978,
//    ISBN: 0-12-519260-6,
//    LC: QA164.N54.
//
//  Parameters:
//
//    Input, int N, the integer whose compositions are desired.
//
//    Input, int K, the number of parts in the composition.
//
//    Input/output, int A[K], the parts of the composition.
//
//    Input/output, bool *MORE.
//    Set MORE = FALSE on first call.  It will be reset to TRUE on return
//    with a new composition.  Each new call returns another composition until
//    MORE is set to FALSE when the last composition has been computed
//    and returned.
//
//    Input/output, int *H, *T, two internal parameters needed for the
//    computation.  The user should allocate space for these in the calling
//    program, include them in the calling sequence, but never alter them!
//
{
  int i;

  if ( !( *more ) )
  {
    *t = n;
    *h = 0;
    a[0] = n;
    for ( i = 1; i < k; i++ )
    {
       a[i] = 0;
    }
  }
  else
  {
    if ( 1 < *t )
    {
      *h = 0;
    }
    *h = *h + 1;
    *t = a[*h-1];
    a[*h-1] = 0;
    a[0] = *t - 1;
    a[*h] = a[*h] + 1;
  }

  *more = ( a[k-1] != n );

  return;
}
//**************************************************************************80

double SandiaRules::cpu_time ( )

//**************************************************************************80
//
//  Purpose:
//
//    CPU_TIME reports the elapsed CPU time.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    26 July 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double CPU_TIME, the current total elapsed CPU time in second.
//
{
  double value;

  value = ( double ) std::clock ( ) / ( double ) CLOCKS_PER_SEC;

  return value;
}
//**************************************************************************80

void SandiaRules::dif_deriv ( int nd, double xd[], double yd[], int *ndp, double xdp[], 
  double ydp[] )

//**************************************************************************80
//
//  Purpose:
//
//    DIF_DERIV computes the derivative of a polynomial in divided difference form.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 June 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Carl deBoor,
//    A Practical Guide to Splines,
//    Springer, 2001,
//    ISBN: 0387953663,
//    LC: QA1.A647.v27.
//
//  Parameters:
//
//    Input, int ND, the size of the input table.
//
//    Input, double XD[ND], the abscissas for the divided
//    difference table.
//
//    Input, double YD[ND], the divided difference table.
//
//    Output, int *NDP, the size of the output table, which is ND-1.
//
//    Input, double XDP[NP], the abscissas for the divided
//    difference table for the derivative.
//
//    Output, double YDP[NDP], the divided difference
//    table for the derivative.
//
{
  int i;
  double *xd_temp;
  double *yd_temp;
//
//  Using a temporary copy of the difference table, shift the
//  abscissas to zero.
//
  xd_temp = new double[nd];
  yd_temp = new double[nd];

  for ( i = 0; i < nd; i++ )
  {
    xd_temp[i] = xd[i];
  }
  for ( i = 0; i < nd; i++ )
  {
    yd_temp[i] = yd[i];
  }

  dif_shift_zero ( nd, xd_temp, yd_temp );
//
//  Construct the derivative.
//
  *ndp = nd - 1;

  for ( i = 0; i < *ndp; i++ )
  {
    xdp[i] = 0.0;
  }

  for ( i = 0; i < *ndp; i++ )
  {
    ydp[i] = ( double ) ( i + 1 ) * yd_temp[i+1];
  }

  delete [] xd_temp;
  delete [] yd_temp;

  return;
}
//**************************************************************************80

void SandiaRules::dif_shift_x ( int nd, double xd[], double yd[], double xv )

//**************************************************************************80
//
//  Purpose:
//
//    DIF_SHIFT_X replaces one abscissa of a divided difference table with a new one.
//
//  Discussion:
//
//    This routine shifts the representation of a divided difference polynomial by
//    dropping the last X value in XD, and adding a new X value to the
//    beginning of the Xd array, suitably modifying the coefficients stored
//    in YD.
//
//    The representation of the polynomial is changed, but the polynomial itself
//    should be identical.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 June 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Carl deBoor,
//    A Practical Guide to Splines,
//    Springer, 2001,
//    ISBN: 0387953663,
//    LC: QA1.A647.v27.
//
//  Parameters:
//
//    Input, int ND, the number of divided difference coefficients, and
//    the number of entries in XD.
//
//    Input/output, double XD[ND], the X values used in the representation of
//    the divided difference polynomial.  After a call to this routine, the 
//    last entry of XD has been dropped, the other
//    entries have shifted up one index, and XV has been inserted at the
//    beginning of the array.
//
//    Input/output, double YD[ND], the divided difference coefficients
//    corresponding to the XD array.  On output, this array has been
//    adjusted.
//
//    Input, double XV, a new X value which is to be used in the representation
//    of the polynomial.  On output, XD[0] equals XV and the representation
//    of the polynomial has been suitably changed.
//    Note that XV does not have to be distinct from any of the original XD
//    values.
//
{
  int i;
//
//  Recompute the divided difference coefficients.
//
  for ( i = nd - 2; 0 <= i; i-- )
  {
    yd[i] = yd[i] + ( xv - xd[i] ) * yd[i+1];
  }
//
//  Shift the X values up one position and insert XV.
//
  for ( i = nd - 1; 0 < i; i-- )
  {
    xd[i] = xd[i-1];
  }

  xd[0] = xv;

  return;
}
//**************************************************************************80

void SandiaRules::dif_shift_zero ( int nd, double xd[], double yd[] )

//**************************************************************************80
//
//  Purpose:
//
//    DIF_SHIFT_ZERO shifts a divided difference table so that all abscissas are zero.
//
//  Discussion:
//
//    When the abscissas are changed, the coefficients naturally
//    must also be changed.
//
//    The resulting pair (XD, YD) still represents the
//    same polynomial, but the entries in YD are now the
//    standard polynomial coefficients.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 June 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Carl deBoor,
//    A Practical Guide to Splines,
//    Springer, 2001,
//    ISBN: 0387953663,
//    LC: QA1.A647.v27.
//
//  Parameters:
//
//    Input, int ND, the length of the XD and YD arrays.
//
//    Input/output, double XD[ND], the X values that correspond to the
//    divided difference table.  On output, XD contains only zeroes.
//
//    Input/output, double YD[ND], the divided difference table
//    for the polynomial.  On output, YD is also
//    the coefficient array for the standard representation
//    of the polynomial.
//
{
  int i;
  double xv;

  xv = 0.0;

  for ( i = 1; i <= nd; i++ )
  {
    dif_shift_x ( nd, xd, yd, xv );
  }

  return;
}
//**************************************************************************80

void SandiaRules::dif_to_r8poly ( int nd, double xd[], double yd[], double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    DIF_TO_R8POLY converts a divided difference table to a standard polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    21 February 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Carl deBoor,
//    A Practical Guide to Splines,
//    Springer, 2001,
//    ISBN: 0387953663,
//    LC: QA1.A647.v27.
//
//  Parameters:
//
//    Input, int ND, the number of coefficients, and abscissas.
//
//    Input, double XD[ND], the X values used in the divided difference
//    representation of the polynomial.
//
//    Input, double YD[ND], the divided difference table.
//
//    Output, double C[ND], the standard form polyomial coefficients.
//    C[0] is the constant term, and C[ND-1] is the coefficient
//    of X^(ND-1).
//
{
  int i;
  int j;

  for ( i = 0; i < nd; i++ )
  {
    c[i] = yd[i];
  }
//
//  Recompute the divided difference coefficients.
//
  for ( j = 1; j <= nd - 1; j++ )
  {
    for ( i = 1; i <= nd - j; i++ )
    {
      c[nd-i-1] = c[nd-i-1] - xd[nd-i-j] * c[nd-i];
    }
  }

  return;
}
//**************************************************************************80

void SandiaRules::fejer2_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    FEJER2_COMPUTE computes a Fejer type 2 rule.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    The rule is defined on [-1,1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  int i;
  int j;
  double p;
  double pi = 3.141592653589793;
  double theta;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "FEJER2_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }
  else if ( n == 1 )
  {
    x[0] = 0.0;
    w[0] = 2.0;
    return;
  }

  for ( i = 0; i < n; i++ )
  {
    x[i] =  std::cos ( ( double ) ( n - i ) * pi
                     / ( double ) ( n + 1 ) );
  }
  if ( ( n % 2 ) == 1 )
  {
    x[(n-1)/2] = 0.0;
  }

  if ( n == 2 )
  {
    w[0] = 1.0;
    w[1] = 1.0;
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      theta = ( double ) ( n - i ) * pi
            / ( double ) ( n + 1 );

      w[i] = 1.0;

      for ( j = 1; j <= ( ( n - 1 ) / 2 ); j++ )
      {
        w[i] = w[i] - 2.0 *  std::cos ( 2.0 * ( double ) ( j ) * theta )
          / ( double ) ( 4 * j * j - 1 );
      }
      p = 2.0 * ( double ) ( ( ( n + 1 ) / 2 ) ) - 1.0;
      w[i] = w[i] -  std::cos ( ( p + 1.0 ) * theta ) / p;
    }
    for ( i = 0; i < n; i++ )
    {
      w[i] = 2.0 * w[i] / ( double ) ( n + 1 );
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::fejer2_compute_np ( int n, int np, double p[], double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    FEJER2_COMPUTE_NP computes a Fejer type 2 rule.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    The rule is defined on [-1,1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  fejer2_compute ( n, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::fejer2_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    FEJER2_COMPUTE_POINTS computes Fejer type 2 quadrature points.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    The rule is defined on [-1,1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
{
  int i;
  double pi = 3.141592653589793;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "FEJER2_COMPUTE_POINTS - Fatal error!\n";
    std::cerr << "  N < 1.\n";
    std::exit ( 1 );
  }
  else if ( n == 1 )
  {
    x[0] = 0.0;
  }
  else
  {
    for ( i = 1; i <= n; i++ )
    {
      x[i-1] =  std::cos ( ( double ) ( n + 1 - i ) * pi
                         / ( double ) ( n + 1 ) );
    }
    if ( ( n % 2 ) == 1 )
    {
      x[(n-1)/2] = 0.0;
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::fejer2_compute_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    FEJER2_COMPUTE_POINTS_NP computes Fejer type 2 quadrature points.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    The rule is defined on [-1,1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  fejer2_compute_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::fejer2_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    FEJER2_COMPUTE_WEIGHTS computes Fejer type 2 quadrature weights.
//
//  Discussion:
//
//    The user must preallocate space for the output array W.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//    Walter Gautschi,
//    Numerical Quadrature in the Presence of a Singularity,
//    SIAM Journal on Numerical Analysis,
//    Volume 4, Number 3, 1967, pages 357-362.
//
//    Joerg Waldvogel,
//    Fast Construction of the Fejer and Clenshaw-Curtis Quadrature Rules,
//    BIT Numerical Mathematics,
//    Volume 43, Number 1, 2003, pages 1-18.
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double W[N], the weights.
//
{
  int i;
  int j;
  double p;
  double pi = 3.141592653589793;
  double theta;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "FEJER2_COMPUTE_WEIGHTS - Fatal error!\n";
    std::cerr << "  N < 1.\n";
    std::exit ( 1 );
  }
  else if ( n == 1 )
  {
    w[0] = 2.0;
  }
  else if ( n == 2 )
  {
    w[0] = 1.0;
    w[1] = 1.0;
  }
  else
  {
    for ( i = 1; i <= n; i++ )
    {
      theta = ( double ) ( n + 1 - i ) * pi
            / ( double ) ( n + 1 );

      w[i-1] = 1.0;

      for ( j = 1; j <= ( ( n - 1 ) / 2 ); j++ )
      {
        w[i-1] = w[i-1] - 2.0 *  std::cos ( 2.0 * ( double ) ( j ) * theta )
          / ( double ) ( 4 * j * j - 1 );
      }
      p = 2.0 * ( double ) ( ( ( n + 1 ) / 2 ) ) - 1.0;
      w[i-1] = w[i-1] -  std::cos ( ( p + 1.0 ) * theta ) / p;
    }
    for ( i = 0; i < n; i++ )
    {
      w[i] = 2.0 * w[i] / ( double ) ( n + 1 );
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::fejer2_compute_weights_np ( int n, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    FEJER2_COMPUTE_WEIGHTS_NP computes Fejer type 2 quadrature weights.
//
//  Discussion:
//
//    The user must preallocate space for the output array W.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//    Walter Gautschi,
//    Numerical Quadrature in the Presence of a Singularity,
//    SIAM Journal on Numerical Analysis,
//    Volume 4, Number 3, 1967, pages 357-362.
//
//    Joerg Waldvogel,
//    Fast Construction of the Fejer and Clenshaw-Curtis Quadrature Rules,
//    BIT Numerical Mathematics,
//    Volume 43, Number 1, 2003, pages 1-18.
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  fejer2_compute_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::gegenbauer_compute ( int order, double alpha, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_COMPUTE computes a Gegenbauer quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) (1-X^2)^ALPHA * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//    Thanks to Janiki Raman for pointing out a problem in an earlier
//    version of the code that occurred when ALPHA was -0.5.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, double ALPHA, the exponent of (1-X^2).  -1.0 < ALPHA is required.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double an;
  double *c;
  double cc;
  double delta;
  double dp2;
  int i;
  double p1;
  double prod;
  double r1;
  double r2;
  double r3;
  double temp;
  double x0;
//
//  Check ORDER.
//
  if ( order < 1 )
  {
    std::cerr << "\n";
    std::cerr << "GEGENBAUER_COMPUTE - Fatal error!\n";
    std::cerr << "  1 <= ORDER is required.\n";
    std::exit ( 1 );
  }
  c = new double[order];
//
//  Check ALPHA.
//
  if ( alpha <= -1.0 )
  {
    std::cerr << "\n";
    std::cerr << "GEGENBAUER_COMPUTE - Fatal error!\n";
    std::cerr << "  -1.0 < ALPHA is required.\n";
    std::exit ( 1 );
  }
//
//  Set the recursion coefficients.
//
  c[0] = 0.0;
  if ( 2 <= order )
  {
    c[1] = 1.0 / ( 2.0 * alpha + 3.0 );
  }

  for ( i = 3; i <= order; i++ )
  {
    c[i-1] = ( double ) ( i - 1 )
          * ( alpha + alpha + ( double ) ( i - 1 ) ) /
          ( ( alpha + alpha + ( double ) ( 2 * i - 1 ) )
          * ( alpha + alpha + ( double ) ( 2 * i - 3 ) ) );
  }

  delta = r8_gamma ( alpha         + 1.0 )
        * r8_gamma (         alpha + 1.0 )
        / r8_gamma ( alpha + alpha + 2.0 );

  prod = 1.0;
  for ( i = 2; i <= order; i++ )
  {
    prod = prod * c[i-1];
  }
  cc = delta * std::pow ( 2.0, alpha + alpha + 1.0 ) * prod;

  for ( i = 1; i <= order; i++ )
  {
    if ( i == 1 )
    {
      an = alpha / ( double ) ( order );

      r1 = ( 1.0 + alpha )
        * ( 2.78 / ( 4.0 + ( double ) ( order * order ) )
        + 0.768 * an / ( double ) ( order ) );

      r2 = 1.0 + 2.44 * an + 1.282 * an * an;

      x0 = ( r2 - r1 ) / r2;
    }
    else if ( i == 2 )
    {
      r1 = ( 4.1 + alpha ) /
        ( ( 1.0 + alpha ) * ( 1.0 + 0.156 * alpha ) );

      r2 = 1.0 + 0.06 * ( ( double ) ( order ) - 8.0 ) *
        ( 1.0 + 0.12 * alpha ) / ( double ) ( order );

      r3 = 1.0 + 0.012 * alpha *
        ( 1.0 + 0.25 * r8_abs ( alpha ) ) / ( double ) ( order );

      x0 = x0 - r1 * r2 * r3 * ( 1.0 - x0 );
    }
    else if ( i == 3 )
    {
      r1 = ( 1.67 + 0.28 * alpha ) / ( 1.0 + 0.37 * alpha );

      r2 = 1.0 + 0.22 * ( ( double ) ( order ) - 8.0 )
        / ( double ) ( order );

      r3 = 1.0 + 8.0 * alpha /
        ( ( 6.28 + alpha ) * ( double ) ( order * order ) );

      x0 = x0 - r1 * r2 * r3 * ( x[0] - x0 );
    }
    else if ( i < order - 1 )
    {
      x0 = 3.0 * x[i-2] - 3.0 * x[i-3] + x[i-4];
    }
    else if ( i == order - 1 )
    {
      r1 = ( 1.0 + 0.235 * alpha ) / ( 0.766 + 0.119 * alpha );

      r2 = 1.0 / ( 1.0 + 0.639
        * ( ( double ) ( order ) - 4.0 )
        / ( 1.0 + 0.71 * ( ( double ) ( order ) - 4.0 ) ) );

      r3 = 1.0 / ( 1.0 + 20.0 * alpha / ( ( 7.5 + alpha ) *
        ( double ) ( order * order ) ) );

      x0 = x0 + r1 * r2 * r3 * ( x0 - x[i-3] );
    }
    else if ( i == order )
    {
      r1 = ( 1.0 + 0.37 * alpha ) / ( 1.67 + 0.28 * alpha );

      r2 = 1.0 /
        ( 1.0 + 0.22 * ( ( double ) ( order ) - 8.0 )
        / ( double ) ( order ) );

      r3 = 1.0 / ( 1.0 + 8.0 * alpha /
        ( ( 6.28 + alpha ) * ( double ) ( order * order ) ) );

      x0 = x0 + r1 * r2 * r3 * ( x0 - x[i-3] );
    }

    gegenbauer_root ( &x0, order, alpha, &dp2, &p1, c );

    x[i-1] = x0;
    w[i-1] = cc / ( dp2 * p1 );
  }
//
//  Reverse the order of the values.
//
  for ( i = 1; i <= order/2; i++ )
  {
    temp       = x[i-1];
    x[i-1]     = x[order-i];
    x[order-i] = temp;
  }

  for ( i = 1; i <=order/2; i++ )
  {
    temp       = w[i-1];
    w[i-1]     = w[order-i];
    w[order-i] = temp;
  }

  delete [] c;

  return;
}
//**************************************************************************80

void SandiaRules::gegenbauer_compute_np ( int order, int np, double p[], double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_COMPUTE_NP computes a Gegenbauer quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) (1-X^2)^ALPHA * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//    Thanks to Janiki Raman for pointing out a problem in an earlier
//    version of the code that occurred when ALPHA was -0.5.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], contains parameters.
//    P[0] = ALPHA = the exponent of (1-X^2).  -1.0 < ALPHA is required.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha;

  alpha = p[0];

  gegenbauer_compute ( order, alpha, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::gegenbauer_compute_points ( int order, double alpha, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_COMPUTE_POINTS computes Gegenbauer quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, double ALPHA, the exponent of (1-X^2).  -1.0 < ALPHA is required.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double *w;

  w = new double[order];

  gegenbauer_compute ( order, alpha, x, w );

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::gegenbauer_compute_points_np ( int order, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_COMPUTE_POINTS_NP computes Gegenbauer quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], contains parameters.
//    P[0] = ALPHA = the exponent of (1-X^2).  -1.0 < ALPHA is required.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double alpha;

  alpha = p[0];

  gegenbauer_compute_points ( order, alpha, x );

  return;
}
//**************************************************************************80

void SandiaRules::gegenbauer_compute_weights ( int order, double alpha, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_COMPUTE_WEIGHTS computes Gegenbauer quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, double ALPHA, the exponent of (1-X^2).  -1.0 < ALPHA is required.
//
//    Output, double W[ORDER], the weights.
//
{
  double *x;

  x = new double[order];

  gegenbauer_compute ( order, alpha, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::gegenbauer_compute_weights_np ( int order, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_COMPUTE_WEIGHTS_NP computes Gegenbauer quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, double P[1], contains parameters.
//    P[0] = ALPHA = the exponent of (1-X^2).  -1.0 < ALPHA is required.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha;

  alpha = p[0];

  gegenbauer_compute_weights ( order, alpha, w );

  return;
}
//**************************************************************************80

double SandiaRules::gegenbauer_integral ( int expon, double alpha )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_INTEGRAL integrates a monomial with Gegenbauer weight.
//
//  Discussion:
//
//    VALUE = Integral ( -1 <= X <= +1 ) x^EXPON (1-x^2)^ALPHA dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    26 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int EXPON, the exponent.
//
//    Input, double ALPHA, the exponent of (1-X^2) in the weight factor.
//
//    Output, double GEGENBAUER_INTEGRAL, the value of the integral.
//
{
  double arg1;
  double arg2;
  double arg3;
  double arg4;
  double c;
  double value;
  double value1;

  if ( ( expon % 2 ) == 1 )
  {
    value = 0.0;
    return value;
  }

  c = ( double ) ( expon );

  arg1 = - alpha;
  arg2 =   1.0 + c;
  arg3 =   2.0 + alpha + c;
  arg4 = - 1.0;

  value1 = r8_hyper_2f1 ( arg1, arg2, arg3, arg4 );

  value = r8_gamma ( 1.0 + c ) * 2.0
    * r8_gamma ( 1.0 + alpha  ) * value1
    / r8_gamma ( 2.0 + alpha  + c );

  return value;
}
//**************************************************************************80

void SandiaRules::gegenbauer_recur ( double *p2, double *dp2, double *p1, double x,
  int order, double alpha, double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_RECUR evaluates a Gegenbauer polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    26 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Output, double *P2, the value of J(ORDER)(X).
//
//    Output, double *DP2, the value of J'(ORDER)(X).
//
//    Output, double *P1, the value of J(ORDER-1)(X).
//
//    Input, double X, the point at which polynomials are evaluated.
//
//    Input, int ORDER, the order of the polynomial.
//
//    Input, double ALPHA, the exponents of (1-X^2).
//
//    Input, double C[ORDER], the recursion coefficients.
//
{
  double dp0;
  double dp1;
  int i;
  double p0;

  *p1 = 1.0;
  dp1 = 0.0;

  *p2 = x;
  *dp2 = 1.0;

  for ( i = 2; i <= order; i++ )
  {
    p0 = *p1;
    dp0 = dp1;

    *p1 = *p2;
    dp1 = *dp2;

    *p2 = x *  ( *p1 ) - c[i-1] * p0;
    *dp2 = x * dp1 + ( *p1 ) - c[i-1] * dp0;
  }
  return;
}
//**************************************************************************80

void SandiaRules::gegenbauer_root ( double *x, int order, double alpha, double *dp2,
  double *p1, double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEGENBAUER_ROOT improves an approximate root of a Gegenbauer polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    26 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input/output, double *X, the approximate root, which
//    should be improved on output.
//
//    Input, int ORDER, the order of the polynomial.
//
//    Input, double ALPHA, the exponents of (1-X^2).
//
//    Output, double *DP2, the value of J'(ORDER)(X).
//
//    Output, double *P1, the value of J(ORDER-1)(X).
//
//    Input, double C[ORDER], the recursion coefficients.
//
{
  double d;
  double eps;
  double p2;
  int step;
  int step_max = 10;

  eps = r8_epsilon ( );

  for ( step = 1; step <= step_max; step++ )
  {
    gegenbauer_recur ( &p2, dp2, p1, *x, order, alpha, c );

    d = p2 / ( *dp2 );
    *x = *x - d;

    if ( r8_abs ( d ) <= eps * ( r8_abs ( *x ) + 1.0 ) )
    {
      return;
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::gen_hermite_compute ( int n, double alpha, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_HERMITE_COMPUTE computes a generalized Gauss-Hermite quadrature rule.
//
//  Discussion:
//
//    The code uses an algorithm by Elhay and Kautsky.
//
//    The integral:
//
//      integral ( -oo < x < +oo ) |x|^alpha exp(-x^2) f(x) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    30 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Sylvan Elhay, Jaroslav Kautsky.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Sylvan Elhay, Jaroslav Kautsky,
//    Algorithm 655: IQPACK, FORTRAN Subroutines for the Weights of
//    Interpolatory Quadrature,
//    ACM Transactions on Mathematical Software,
//    Volume 13, Number 4, December 1987, pages 399-415.
//
//  Parameters:
//
//    Input, int N, the number of abscissas.
//
//    Input, double ALPHA, the parameter.
//    -1.0 < ALPHA.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double *bj;
  int i;
  double i_r8;
  double zemu;
//
//  Define the zero-th moment.
//
  zemu = r8_gamma ( ( alpha + 1.0 ) / 2.0 );
//
//  Define the Jacobi matrix.
//
  bj = new double[n];

  for ( i = 0; i < n; i++ )
  {
    i_r8 = ( double ) ( i + 1 );
    if ( ( i % 2 ) == 0 )
    {
      bj[i] = ( i_r8 + alpha ) / 2.0;
    }
    else
    {
      bj[i] = i_r8 / 2.0;
    }
  }

  for ( i = 0; i < n; i++ )
  {
    bj[i] = std::sqrt ( bj[i] );
  }

  for ( i = 0; i < n; i++ )
  {
    x[i] = 0.0;
  }

  w[0] = std::sqrt ( zemu );
  for ( i = 1; i < n; i++ )
  {
    w[i] = 0.0;
  }
//
//  Diagonalize the Jacobi matrix.
//
  imtqlx ( n, x, bj, w );

  for ( i = 0; i < n; i++ )
  {
    w[i] = w[i] * w[i];
  }

  delete [] bj;

  return;
}
//**************************************************************************80

void SandiaRules::gen_hermite_compute_np ( int order, int np, double p[], double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_HERMITE_COMPUTE_NP computes a Generalized Hermite quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -oo < x < +oo ) |x|^ALPHA exp(-x^2) f(x) dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], contains parameters.
//    P[0] = ALPHA, the exponent of the X factor. -1.0 < ALPHA.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha;

  alpha = p[0];

  gen_hermite_compute ( order, alpha, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::gen_hermite_compute_points ( int order, double alpha, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_HERMITE_COMPUTE_POINTS computes Generalized Hermite quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, double ALPHA, the exponent of the X factor.
//    -1.0 < ALPHA.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double *w;

  w = new double[order];

  gen_hermite_compute ( order, alpha, x, w );

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::gen_hermite_compute_points_np ( int order, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_HERMITE_COMPUTE_POINTS_NP: Generalized Hermite quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], contains parameters.
//    P[0] = ALPHA, the exponent of the X factor. -1.0 < ALPHA.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double alpha;

  alpha = p[0];

  gen_hermite_compute_points ( order, alpha, x );

  return;
}
//**************************************************************************80

void SandiaRules::gen_hermite_compute_weights ( int order, double alpha, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_HERMITE_COMPUTE_WEIGHTS computes Generalized Hermite quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, double ALPHA, the exponent of the X factor.
//    -1.0 < ALPHA.
//
//    Output, double W[ORDER], the weights.
//
{
  double *x;

  x = new double[order];

  gen_hermite_compute ( order, alpha, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::gen_hermite_compute_weights_np ( int order, int np, double p[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_HERMITE_COMPUTE_WEIGHTS_NP: Generalized Hermite quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], contains parameters.
//    P[0] = ALPHA, the exponent of the X factor. -1.0 < ALPHA.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha;

  alpha = p[0];

  gen_hermite_compute_weights ( order, alpha, w );

  return;
}
//**************************************************************************80

void SandiaRules::gen_hermite_dr_compute ( int order, double alpha, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_HERMITE_DR_COMPUTE computes a Generalized Hermite quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -oo < x < +oo ) |x|^ALPHA exp(-x^2) f(x) dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, double ALPHA, the exponent of the X factor.
//    -1.0 < ALPHA.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha_laguerre;
  double arg;
  int i;
  int order_laguerre;
  double *w_laguerre;
  double *x_laguerre;

  if ( order < 1 )
  {
    std::cerr << "\n";
    std::cerr << "GEN_HERMITE_DR_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of ORDER = " << order << "\n";
    std::exit ( 1 );
  }

  if ( order == 1 )
  {
    arg = ( alpha + 1.0 ) / 2.0;
    x[0] = 0.0;
    w[0] = r8_gamma ( arg );
    return;
  }

  if ( ( order % 2 ) == 0 )
  {
    order_laguerre = order / 2;
    alpha_laguerre = ( alpha - 1.0 ) / 2.0;
  }
  else
  {
    order_laguerre = ( order - 1 ) / 2;
    alpha_laguerre = ( alpha + 1.0 ) / 2.0;
  }

  w_laguerre = new double[order_laguerre];
  x_laguerre = new double[order_laguerre];

  gen_laguerre_ss_compute ( order_laguerre, alpha_laguerre, x_laguerre,
    w_laguerre );

  if ( ( order % 2 ) == 0 )
  {
    for ( i = 0; i < order_laguerre; i++ )
    {
      x[i] = - std::sqrt ( x_laguerre[order_laguerre-1-i] );
    }
    for ( i = 0; i < order_laguerre; i++ )
    {
      x[order_laguerre+i] = std::sqrt ( x_laguerre[i] );
	}
    for ( i = 0; i < order_laguerre; i++ )
    {
      w[i] = 0.5 * w_laguerre[order_laguerre-1-i];
    }
    for ( i = 0; i < order_laguerre; i++ )
    {
      w[order_laguerre+i] = 0.5 * w_laguerre[i];
    }
  }
  else if ( ( order % 2 ) == 1 )
  {
    for ( i = 0; i < order_laguerre; i++ )
    {
      x[i] = - std::sqrt ( x_laguerre[order_laguerre-1-i] );
    }
    x[order_laguerre] = 0.0;
    for ( i = 0; i < order_laguerre; i++ )
    {
      x[order_laguerre+1+i] = std::sqrt ( x_laguerre[i] );
	}
    for ( i = 0; i < order_laguerre; i++ )
    {
      w[i] = 0.5 * w_laguerre[order_laguerre-1-i] / x_laguerre[order_laguerre-1-i];
    }

    arg = ( alpha + 1.0 ) / 2.0;
    w[order_laguerre] = r8_gamma ( arg );
    for ( i = 0; i < order_laguerre; i++ )
    {
      w[order_laguerre] = w[order_laguerre] - w_laguerre[i] / x_laguerre[i];
    }

    for ( i = 0; i < order_laguerre; i++ )
    {
      w[order_laguerre+1+i] = 0.5 * w_laguerre[i] / x_laguerre[i];
    }
  }
  delete [] w_laguerre;
  delete [] x_laguerre;

  return;
}
//**************************************************************************80

double SandiaRules::gen_hermite_integral ( int expon, double alpha )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_HERMITE_INTEGRAL evaluates a monomial Generalized Hermite integral.
//
//  Discussion:
//
//    H(n,alpha) = Integral ( -oo < x < +oo ) x^n |x|^alpha exp(-x^2) dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int EXPON, the exponent of the monomial.
//    0 <= EXPON.
//
//    Input, double ALPHA, the exponent of |X| in the weight function.
//    -1.0 < ALPHA.
//
//    Output, double GEN_HERMITE_INTEGRAL, the value of the integral.
//
{
  double a;
  double arg;
  double value;

  if ( ( expon % 2 ) == 1 )
  {
    value = 0.0;
  }
  else
  {
    a = alpha + ( double ) ( expon );
    if ( a <= - 1.0 )
    {
      value = - r8_huge ( );
    }
    else
    {
      arg = ( a + 1.0 ) / 2.0;
      value = r8_gamma ( arg );
    }
  }
  return value;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_compute ( int n, double alpha, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_COMPUTE: generalized Gauss-Laguerre quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      integral ( 0 <= x < +oo ) exp ( - x ) * x^alpha * f(x) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    The integral:
//
//      integral ( 0 <= x < +oo ) x^alpha * f(x) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * exp ( x(i) ) * f ( x(i) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Sylvan Elhay, Jaroslav Kautsky.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Sylvan Elhay, Jaroslav Kautsky,
//    Algorithm 655: IQPACK, FORTRAN Subroutines for the Weights of
//    Interpolatory Quadrature,
//    ACM Transactions on Mathematical Software,
//    Volume 13, Number 4, December 1987, pages 399-415.
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, double ALPHA, the exponent of the X factor.
//    ALPHA must be nonnegative.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double *bj;
  int i;
  double i_r8;
  double zemu;
//
//  Define the zero-th moment.
//
  zemu = r8_gamma ( alpha + 1.0 );
//
//  Define the Jacobi matrix.
//
  bj = new double[n];

  for ( i = 0; i < n; i++ )
  {
    i_r8 = ( double ) ( i + 1 );
    bj[i] = std::sqrt ( i_r8 * ( i_r8 + alpha ) );
  }

  for ( i = 0; i < n; i++ )
  {
    i_r8 = ( double ) ( i + 1 );
    x[i] = 2.0 * i_r8 - 1.0 + alpha;
  }

  w[0] = std::sqrt ( zemu );

  for ( i = 1; i < n; i++ )
  {
    w[i] = 0.0;
  }
//
//  Diagonalize the Jacobi matrix.
//
  imtqlx ( n, x, bj, w );

  for ( i = 0; i < n; i++ )
  {
    w[i] = w[i] * w[i];
  }

  delete [] bj;

  return;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_compute_np ( int order, int np, double p[], double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_COMPUTE_NP computes a Generalized Laguerre quadrature rule.
//
//  Discussion:
//
//    In the simplest case, ALPHA is 0, and we are approximating the
//    integral from 0 to +oo of exp(-X) * F(X).  When this is so,
//    it is easy to modify the rule to approximate the integral from
//    A to +oo as well.
//
//    If ALPHA is nonzero, then there is no simple way to extend the
//    rule to approximate the integral from A to +oo.  The simplest
//    procedures would be to approximate the integral from 0 to A.
//
//    If the integral to approximate is:
//
//        Integral ( A <= X < +oo ) exp ( - X ) * F(X) dX
//      or
//        Integral ( 0 <= X < +oo ) exp ( - X ) * X^ALPHA * F(X) dX
//
//    then the quadrature rule is:
//
//      exp ( - A ) * Sum ( 1 <= I <= ORDER ) W(I) * F ( A+X(I) )
//    or
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//
//    If the integral to approximate is:
//
//        Integral ( A <= X < +oo ) F(X) dX
//      or
//        Integral ( 0 <= X < +oo ) X^ALPHA * F(X) dX
//
//    then the quadrature rule is:
//
//      exp ( - A ) * Sum ( 1 <= I <= ORDER )
//        W(I) * exp(A+X(I)) * F ( A+X(I) )
//    or
//      Sum ( 1 <= I <= ORDER ) W(I) * exp(X(I)) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, double P[1], contains parameters.
//    P[0] = ALPHA, the exponent of the X factor.
//    Set ALPHA = 0.0 for the simplest rule.
//    ALPHA must be nonnegative.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha;

  alpha = p[0];

  gen_laguerre_compute ( order, alpha, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_compute_points ( int order, double alpha, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_COMPUTE_POINTS: Generalized Laguerre quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 March 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, double ALPHA, the exponent of the X factor.
//    Set ALPHA = 0.0 for the simplest rule.
//    ALPHA must be nonnegative.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double *w;

  w = new double[order];

  gen_laguerre_compute ( order, alpha, x, w );

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_compute_points_np ( int order, int np, double p[],
  double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_COMPUTE_POINTS_NP: Generalized Laguerre quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], contains parameters.
//    P[0] = ALPHA, the exponent of the X factor.
//    Set ALPHA = 0.0 for the simplest rule.
//    ALPHA must be nonnegative.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double alpha;

  alpha = p[0];

  gen_laguerre_compute_points ( order, alpha, x );

  return;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_compute_weights ( int order, double alpha, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_COMPUTE_WEIGHTS: Generalized Laguerre quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, double ALPHA, the exponent of the X factor.
//    Set ALPHA = 0.0 for the simplest rule.
//    ALPHA must be nonnegative.
//
//    Output, double W[ORDER], the weights.
//
{
  double *x;

  x = new double[order];

  gen_laguerre_compute ( order, alpha, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_compute_weights_np ( int order, int np, double p[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_COMPUTE_WEIGHTS_NP: Generalized Laguerre quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], contains parameters.
//    P[0] = ALPHA, the exponent of the X factor.
//    Set ALPHA = 0.0 for the simplest rule.
//    ALPHA must be nonnegative.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha;

  alpha = p[0];

  gen_laguerre_compute_weights ( order, alpha, w );

  return;
}
//**************************************************************************80

double SandiaRules::gen_laguerre_integral ( int expon, double alpha )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_INTEGRAL evaluates a monomial Generalized Laguerre integral.
//
//  Discussion:
//
//    L(n,alpha) = Integral ( 0 <= x < +oo ) x^n * x^alpha exp(-x) dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    20 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int EXPON, the exponent of the monomial.
//    0 <= EXPON.
//
//    Input, double ALPHA, the exponent of X in the weight function.
//    -1.0 < ALPHA.
//
//    Output, double GEN_LAGUERRE_INTEGRAL, the value of the integral.
//
{
  double arg;
  double value;

  arg = alpha + ( double ) ( expon + 1.0 );
  value = r8_gamma ( arg );

  return value;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_ss_compute ( int order, double alpha, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_SS_COMPUTE computes a Generalized Laguerre quadrature rule.
//
//  Discussion:
//
//    In the simplest case, ALPHA is 0, and we are approximating the
//    integral from 0 to +oo of exp(-X) * F(X).  When this is so,
//    it is easy to modify the rule to approximate the integral from
//    A to +oo as well.
//
//    If ALPHA is nonzero, then there is no simple way to extend the
//    rule to approximate the integral from A to +oo.  The simplest
//    procedures would be to approximate the integral from 0 to A.
//
//    If the integral to approximate is:
//
//        Integral ( A <= X < +oo ) exp ( - X ) * F(X) dX
//      or
//        Integral ( 0 <= X < +oo ) exp ( - X ) * X^ALPHA * F(X) dX
//
//    then the quadrature rule is:
//
//      exp ( - A ) * Sum ( 1 <= I <= ORDER ) W(I) * F ( A+X(I) )
//    or
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//
//    If the integral to approximate is:
//
//        Integral ( A <= X < +oo ) F(X) dX
//      or
//        Integral ( 0 <= X < +oo ) X^ALPHA * F(X) dX
//
//    then the quadrature rule is:
//
//      exp ( - A ) * Sum ( 1 <= I <= ORDER )
//        W(I) * exp(A+X(I)) * F ( A+X(I) )
//    or
//      Sum ( 1 <= I <= ORDER ) W(I) * exp(X(I)) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, double ALPHA, the exponent of the X factor.
//    Set ALPHA = 0.0 for the simplest rule.
//    ALPHA must be nonnegative.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double *b;
  double *c;
  double cc;
  double dp2;
  int i;
  double p1;
  double prod;
  double r1;
  double r2;
  double ratio;
  double x0;

  if ( order < 1 )
  {
    std::cerr << "\n";
    std::cerr << "GEN_LAGUERRE_SS_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of ORDER = " << order << "\n";
    std::exit ( 1 );
  }

  b = new double[order];
  c = new double[order];
//
//  Set the recursion coefficients.
//
  for ( i = 0; i < order; i++ )
  {
    b[i] = ( alpha + ( double ) ( 2 * i + 1 ) );
  }

  for ( i = 0; i < order; i++ )
  {
    c[i] = ( double ) ( i ) * ( alpha + ( double ) ( i ) );
  }
  prod = 1.0;
  for ( i = 1; i < order; i++ )
  {
    prod = prod * c[i];
  }
  cc = r8_gamma ( alpha + 1.0 ) * prod;

  for ( i = 0; i < order; i++ )
  {
//
//  Compute an estimate for the root.
//
    if ( i == 0 )
    {
      x0 = ( 1.0 + alpha ) * ( 3.0+ 0.92 * alpha ) /
        ( 1.0 + 2.4 * ( double ) ( order ) + 1.8 * alpha );
    }
    else if ( i == 1 )
    {
      x0 = x0 + ( 15.0 + 6.25 * alpha ) /
        ( 1.0 + 0.9 * alpha + 2.5 * ( double ) ( order ) );
    }
    else
    {
      r1 = ( 1.0 + 2.55 * ( double ) ( i - 1 ) )
        / ( 1.9 * ( double ) ( i - 1 ) );

      r2 = 1.26 * ( double ) ( i - 1 ) * alpha /
        ( 1.0 + 3.5 * ( double ) ( i - 1 ) );

      ratio = ( r1 + r2 ) / ( 1.0 + 0.3 * alpha );

      x0 = x0 + ratio * ( x0 - x[i-2] );
    }
//
//  Use iteration to find the root.
//
    gen_laguerre_ss_root ( &x0, order, alpha, &dp2, &p1, b, c );
//
//  Set the abscissa and weight.
//
    x[i] = x0;
    w[i] = ( cc / dp2 ) / p1;
  }

  delete [] b;
  delete [] c;

  return;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_ss_recur ( double *p2, double *dp2, double *p1, double x,
  int order, double alpha, double b[], double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_SS_RECUR evaluates a Generalized Laguerre polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Output, double *P2, the value of L(ORDER)(X).
//
//    Output, double *DP2, the value of L'(ORDER)(X).
//
//    Output, double *P1, the value of L(ORDER-1)(X).
//
//    Input, double X, the point at which polynomials are evaluated.
//
//    Input, int ORDER, the order of the polynomial.
//
//    Input, double ALPHA, the exponent of the X factor in the
//    integrand.
//
//    Input, double B[ORDER], C[ORDER], the recursion coefficients.
//
{
  double dp0;
  double dp1;
  int i;
  double p0;

  *p1 = 1.0;
  dp1 = 0.0;

  *p2 = x - alpha - 1.0;
  *dp2 = 1.0;

  for ( i = 1; i < order; i++ )
  {
    p0 = *p1;
    dp0 = dp1;

    *p1 = *p2;
    dp1 = *dp2;

    *p2  = ( x - b[i] ) * ( *p1 ) - c[i] * p0;
    *dp2 = ( x - b[i] ) * dp1 + ( *p1 ) - c[i] * dp0;
  }

  return;
}
//**************************************************************************80

void SandiaRules::gen_laguerre_ss_root ( double *x, int order, double alpha, double *dp2,
  double *p1, double b[], double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    GEN_LAGUERRE_SS_ROOT improves a root of a Generalized Laguerre polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input/output, double *X, the approximate root, which
//    should be improved on output.
//
//    Input, int ORDER, the order of the polynomial.
//
//    Input, double ALPHA, the exponent of the X factor.
//
//    Output, double *DP2, the value of L'(ORDER)(X).
//
//    Output, double *P1, the value of L(ORDER-1)(X).
//
//    Input, double B[ORDER], C[ORDER], the recursion coefficients.
//
{
  double d;
  double eps;
  double p2;
  int step;
  int step_max = 10;

  eps = r8_epsilon ( );

  for ( step = 1; step <= step_max; step++ )
  {
    gen_laguerre_ss_recur ( &p2, dp2, p1, *x, order, alpha, b, c );

    d = p2 / ( *dp2 );
    *x = *x - d;

    if ( r8_abs ( d ) <= eps * ( r8_abs ( *x ) + 1.0 ) )
    {
      break;
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::hc_compute_weights_from_points ( int nhalf, double xhalf[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HC_COMPUTE_WEIGHTS_FROM_POINTS: Hermite-Cubic weights, user-supplied points.
//
//  Discussion:
//
//    An interval [A,B] has been divided by NHALF points X; at each
//    point both function and derivative information is available.
//
//    The piecewise cubic Hermite interpolant is constructed for this data.
//
//    A quadrature rule is determined for the interpolant.
//
//    There will be N=2*NHALF weights.  If the quadrature rule is to be written 
//    out, one would normally list each point twice, so that the number of points 
//    and weights are equal.  The listing of the same point value twice is an
//    implicit indication that both function and derivative values should be
//    used.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NHALF, the number of points, not counting repetitions.
//
//    Input, double XHALF[NHALF], the points, without repetition.
//
//    Output, double W[2*NHALF], the weights.  The first two weights are 
//    associated with the first point, and so on.
//
{
  int j;

  w[0+0*2] =    0.5 * ( xhalf[1] - xhalf[0] );
  w[1+0*2] = std::pow ( xhalf[1] - xhalf[0], 2 ) / 12.0;

  for ( j = 1; j < nhalf - 1; j++ )
  {
    w[0+j*2] = 0.5 * ( xhalf[j+1] - xhalf[j-1] );
    w[1+j*2] =       ( xhalf[j+1] - xhalf[j-1] ) 
                   * ( xhalf[j+1] - 2.0 * xhalf[j] + xhalf[j-1] ) / 12.0;
  }

  w[0+(nhalf-1)*2] =      0.5 * ( xhalf[nhalf-1] - xhalf[nhalf-2]   );
  w[1+(nhalf-1)*2] = - std::pow ( xhalf[nhalf-2] - xhalf[nhalf-1], 2 ) / 12.0;

  return;
}
//**************************************************************************80

void SandiaRules::hcc_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCC_COMPUTE computes a Hermite-Cubic-Chebyshev-Spacing quadrature rule.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into Chebyshev-spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  int nhalf;
  double *xhalf;

  nhalf = n / 2;
  xhalf = new double[nhalf];

  clenshaw_curtis_compute_points ( nhalf, xhalf );
  r8vec_stutter ( nhalf, xhalf, 2, x );
  hc_compute_weights_from_points ( nhalf, xhalf, w );

  delete [] xhalf;

  return;
}
//**************************************************************************80

void SandiaRules::hcc_compute_np ( int n, int np, double p[], double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCC_COMPUTE_NP computes a Hermite-Cubic-Chebyshev-Spacing quadrature rule.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into Chebyshev-spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  hcc_compute ( n, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::hcc_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCC_COMPUTE_POINTS computes Hermite-Cubic-Chebyshev-Spacing quadrature points.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into Chebyshev-spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
{
  int nhalf;
  double *xhalf;

  if ( ( n % 2 ) != 0 )
  {
    std::cerr << "\n";
    std::cerr << "HCC_COMPUTE_POINTS - Fatal error!\n";
    std::cerr << "  Order of rule N is not even.\n";
    std::exit ( 1 );
  }

  nhalf = n / 2;
  xhalf = new double[nhalf];

  clenshaw_curtis_compute_points ( nhalf, xhalf );
  r8vec_stutter ( nhalf, xhalf, 2, x );

  delete [] xhalf;

  return;
}
//**************************************************************************80

void SandiaRules::hcc_compute_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCC_COMPUTE_POINTS_NP: Hermite-Cubic-Chebyshev-Spacing quadrature points.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into Chebyshev-spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  hcc_compute_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::hcc_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCC_COMPUTE_WEIGHTS: Hermite-Cubic-Chebyshev-Spacing quadrature weights.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into Chebyshev-spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double W[N], the weights.
//
{
  int nhalf;
  double *xhalf;

  if ( ( n % 2 ) != 0 )
  {
    std::cerr << "\n";
    std::cerr << "HCC_COMPUTE_WEIGHTS - Fatal error!\n";
    std::cerr << "  Order of rule N is not even.\n";
    std::exit ( 1 );
  }

  nhalf = n / 2;
  xhalf = new double[nhalf];

  hc_compute_weights_from_points ( nhalf, xhalf, w );

  delete [] xhalf;

  return;
}
//**************************************************************************80

void SandiaRules::hcc_compute_weights_np ( int n, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCC_COMPUTE_WEIGHTS_NP: Hermite-Cubic-Chebyshev-Spacing quadrature weights.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into Chebyshev-spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  hcc_compute_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::hce_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCE_COMPUTE computes a Hermite-Cubic-Equal-Spacing quadrature rule.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into equally spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double a_high = 1.0;
  double a_low = 0.0;
  int nhalf;
  double *xhalf;

  a_low = 0.0;
  a_high = 1.0;

  nhalf = n / 2;

  xhalf = r8vec_linspace_new ( nhalf, a_low, a_high );
  r8vec_stutter ( nhalf, xhalf, 2, x );
  hc_compute_weights_from_points ( nhalf, xhalf, w );

  delete [] xhalf;

  return;
}
//**************************************************************************80

void SandiaRules::hce_compute_np ( int n, int np, double p[], double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCE_COMPUTE_NP computes a Hermite-Cubic-Equal-Spacing quadrature rule.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into equally spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  hce_compute ( n, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::hce_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCE_COMPUTE_POINTS computes Hermite-Cubic-Equal-Spacing quadrature points.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into equally spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
{
  int i;
  int j;
  int m;
  double x_value;

  if ( ( n % 2 ) != 0 )
  {
    std::cerr << "\n";
    std::cerr << "HCE_COMPUTE_POINTS - Fatal error!\n";
    std::cerr << "  Order of rule N is not even.\n";
    std::exit ( 1 );
  }
  m = n / 2;

  for ( j = 0; j < m; j++ )
  {
    x_value = ( double ) ( 2 * j + 1 - m ) / ( double ) ( m - 1 );
    for ( i = 0; i < 2; i++ )
    {
      x[i+j*2] = x_value;
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::hce_compute_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCE_COMPUTE_POINTS_NP: Hermite-Cubic-Equal-Spacing quadrature points.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into equally spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  hce_compute_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::hce_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCE_COMPUTE_WEIGHTS: Hermite-Cubic-Equal-Spacing quadrature weights.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into equally spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double W[N], the weights.
//
{
  int nhalf;
  double *xhalf;

  if ( ( n % 2 ) != 0 )
  {
    std::cerr << "\n";
    std::cerr << "HCE_COMPUTE_WEIGHTS - Fatal error!\n";
    std::cerr << "  Order of rule N is not even.\n";
    std::exit ( 1 );
  }

  nhalf = n / 2;
  xhalf = new double[nhalf];

  hc_compute_weights_from_points ( nhalf, xhalf, w );

  delete [] xhalf;

  return;
}
//**************************************************************************80

void SandiaRules::hce_compute_weights_np ( int n, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HCE_COMPUTE_WEIGHTS_NP: Hermite-Cubic-Equal-Spacing quadrature weights.
//
//  Discussion:
//
//    For the HCE rule, we assume that an interval has been divided by
//    M nodes X into equally spaced subintervals, and that at each
//    abscissa both function and derivative information is available.
//    The piecewise cubic Hermite interpolant is constructed for this data.
//    The quadrature rule uses N = 2 * M abscissas, where each node is
//    listed twice, and the weights occur in pairs, with the first multiplying
//    the function value and the second the derivative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  hce_compute_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_COMPUTE computes a Gauss-Hermite quadrature rule.
//
//  Discussion:
//
//    The code uses an algorithm by Elhay and Kautsky.
//
//    The abscissas are the zeros of the N-th order Hermite polynomial.
//
//    The integral:
//
//      integral ( -oo < x < +oo ) exp ( - x * x ) * f(x) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Sylvan Elhay, Jaroslav Kautsky.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Sylvan Elhay, Jaroslav Kautsky,
//    Algorithm 655: IQPACK, FORTRAN Subroutines for the Weights of
//    Interpolatory Quadrature,
//    ACM Transactions on Mathematical Software,
//    Volume 13, Number 4, December 1987, pages 399-415.
//
//  Parameters:
//
//    Input, int N, the number of abscissas.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double arg;
  double *bj;
  int i;
  double zemu;
//
//  Define the zero-th moment.
//
  arg = 0.5;
  zemu = r8_gamma ( arg );
//
//  Define the Jacobi matrix.
//
  bj = new double[n];

  for ( i = 0; i < n; i++ )
  {
    bj[i] = std::sqrt ( ( double ) ( i + 1 ) / 2.0 );
  }

  for ( i = 0; i < n; i++ )
  {
    x[i] = 0.0;
  }

  w[0] = std::sqrt ( zemu );
  for ( i = 1; i < n; i++ )
  {
    w[i] = 0.0;
  }
//
//  Diagonalize the Jacobi matrix.
//
  imtqlx ( n, x, bj, w );

  for ( i = 0; i < n; i++ )
  {
    w[i] = w[i] * w[i];
  }

  delete [] bj;

  return;
}
//**************************************************************************80

void SandiaRules::hermite_compute_np ( int order, int np, double p[], double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_COMPUTE_NP computes a Hermite quadrature rule.
//
//  Discussion:
//
//    The abscissas are the zeros of the N-th order Hermite polynomial.
//
//    The integral:
//
//      Integral ( -oo < X < +oo ) exp ( - X * X ) * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  hermite_compute ( order, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_compute_points ( int order, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_COMPUTE_POINTS computes Hermite quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double *w;

  w = new double[order];

  hermite_compute ( order, x, w );

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::hermite_compute_points_np ( int order, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_COMPUTE_POINTS_NP computes Hermite quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[ORDER], the abscissas.
//
{
  hermite_compute_points ( order, x );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_compute_weights ( int order, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_COMPUTE_WEIGHTS computes Hermite quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Output, double W[ORDER], the weights.
//
{
  double *x;

  x = new double[order];

  hermite_compute ( order, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::hermite_compute_weights_np ( int order, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_COMPUTE_WEIGHTS_NP computes Hermite quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[ORDER], the weights.
//
{
  hermite_compute_weights ( order, w );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_genz_keister_lookup ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_GENZ_KEISTER_LOOKUP looks up a Genz-Keister Hermite rule.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo <= x <= +oo ) f(x) exp ( - x * x ) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    A nested family of rules for the Hermite integration problem
//    was produced by Genz and Keister.  The structure of the nested
//    family was denoted by 1+2+6+10+16, that is, it comprised rules
//    of successive orders O = 1, 3, 9, 19, and 35.
//
//    The precisions of these rules are P = 1, 5, 15, 29, and 51.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 June 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Genz, Bradley Keister,
//    Fully symmetric interpolatory rules for multiple integrals
//    over infinite regions with Gaussian weight,
//    Journal of Computational and Applied Mathematics,
//    Volume 71, 1996, pages 299-309
//
//    Florian Heiss, Viktor Winschel,
//    Likelihood approximation by numerical integration on sparse grids,
//    Journal of Econometrics,
//    Volume 144, 2008, pages 62-80.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be 1, 3, 9, 19, 35, 37, 41 or 43.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  hermite_genz_keister_lookup_points ( n, x );
  hermite_genz_keister_lookup_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_genz_keister_lookup_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_GENZ_KEISTER_LOOKUP_POINTS looks up Genz-Keister Hermite abscissas.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo <= x <= +oo ) f(x) exp ( - x * x ) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    A nested family of rules for the Hermite integration problem
//    was produced by Genz and Keister.  The structure of the nested
//    family was denoted by 1+2+6+10+?, that is, it comprised rules
//    of successive orders O = 1, 3, 9, 19, and a final rule of order
//    35, 37, 41 or 43.
//
//    The precisions of these rules are P = 1, 5, 15, 29, 
//    with the final rule of precision 51, 55, 63 or 67.
//
//    Three related families begin the same way, but end with a different final
//    rule.  As a convenience, this function includes these final rules as well:
//
//    Designation  Orders       Precisions
//
//    1+2+6+10+16, 1,3,9,19,35  1,5,15,29,51
//    1+2+6+10+18  1,3,9,19,37  1,5,15,29,55
//    1+2+6+10+22  1,3,9,19,41  1,5,15,29,63
//    1+2+6+10+24  1,3,9,19,43  1,5,15,29,67
//
//    Some of the data in this function was kindly supplied directly by
//    Alan Genz on 24 April 2011.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Genz, Bradley Keister,
//    Fully symmetric interpolatory rules for multiple integrals
//    over infinite regions with Gaussian weight,
//    Journal of Computational and Applied Mathematics,
//    Volume 71, 1996, pages 299-309
//
//    Florian Heiss, Viktor Winschel,
//    Likelihood approximation by numerical integration on sparse grids,
//    Journal of Econometrics,
//    Volume 144, 2008, pages 62-80.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be 1, 3, 9, 19, 35, 37, 41, or 43.
//
//    Output, double X[N], the abscissas.
//
{
  if ( n == 1 )
  {
    x[ 0] =   0.0000000000000000E+00;
  }
  else if ( n == 3 )
  {
    x[ 0] =  -1.2247448713915889E+00;
    x[ 1] =   0.0000000000000000E+00;
    x[ 2] =   1.2247448713915889E+00;
  }
  else if ( n == 9 )
  {
    x[ 0] =  -2.9592107790638380E+00;
    x[ 1] =  -2.0232301911005157E+00;
    x[ 2] =  -1.2247448713915889E+00;
    x[ 3] =  -5.2403354748695763E-01;
    x[ 4] =   0.0000000000000000E+00;
    x[ 5] =   5.2403354748695763E-01;
    x[ 6] =   1.2247448713915889E+00;
    x[ 7] =   2.0232301911005157E+00;
    x[ 8] =   2.9592107790638380E+00;
  }
  else if ( n == 19 )
  {
    x[ 0] =  -4.4995993983103881E+00;
    x[ 1] =  -3.6677742159463378E+00;
    x[ 2] =  -2.9592107790638380E+00;
    x[ 3] =  -2.2665132620567876E+00;
    x[ 4] =  -2.0232301911005157E+00;
    x[ 5] =  -1.8357079751751868E+00;
    x[ 6] =  -1.2247448713915889E+00;
    x[ 7] =  -8.7004089535290285E-01;
    x[ 8] =  -5.2403354748695763E-01;
    x[ 9] =   0.0000000000000000E+00;
    x[10] =   5.2403354748695763E-01;
    x[11] =   8.7004089535290285E-01;
    x[12] =   1.2247448713915889E+00;
    x[13] =   1.8357079751751868E+00;
    x[14] =   2.0232301911005157E+00;
    x[15] =   2.2665132620567876E+00;
    x[16] =   2.9592107790638380E+00;
    x[17] =   3.6677742159463378E+00;
    x[18] =   4.4995993983103881E+00;
  }
  else if ( n == 35 )
  {
    x[ 0] =  -6.3759392709822356E+00;
    x[ 1] =  -5.6432578578857449E+00;
    x[ 2] =  -5.0360899444730940E+00;
    x[ 3] =  -4.4995993983103881E+00;
    x[ 4] =  -4.0292201405043713E+00;
    x[ 5] =  -3.6677742159463378E+00;
    x[ 6] =  -3.3491639537131945E+00;
    x[ 7] =  -2.9592107790638380E+00;
    x[ 8] =  -2.5705583765842968E+00;
    x[ 9] =  -2.2665132620567876E+00;
    x[10] =  -2.0232301911005157E+00;
    x[11] =  -1.8357079751751868E+00;
    x[12] =  -1.5794121348467671E+00;
    x[13] =  -1.2247448713915889E+00;
    x[14] =  -8.7004089535290285E-01;
    x[15] =  -5.2403354748695763E-01;
    x[16] =  -1.7606414208200893E-01;
    x[17] =   0.0000000000000000E+00;
    x[18] =   1.7606414208200893E-01;
    x[19] =   5.2403354748695763E-01;
    x[20] =   8.7004089535290285E-01;
    x[21] =   1.2247448713915889E+00;
    x[22] =   1.5794121348467671E+00;
    x[23] =   1.8357079751751868E+00;
    x[24] =   2.0232301911005157E+00;
    x[25] =   2.2665132620567876E+00;
    x[26] =   2.5705583765842968E+00;
    x[27] =   2.9592107790638380E+00;
    x[28] =   3.3491639537131945E+00;
    x[29] =   3.6677742159463378E+00;
    x[30] =   4.0292201405043713E+00;
    x[31] =   4.4995993983103881E+00;
    x[32] =   5.0360899444730940E+00;
    x[33] =   5.6432578578857449E+00;
    x[34] =   6.3759392709822356E+00;
  }
  else if ( n == 37 )
  {
    x[ 0] =  -6.853200069757519;
    x[ 1] =  -6.124527854622158;
    x[ 2] =  -5.521865209868350;
    x[ 3] =  -4.986551454150765;
    x[ 4] =  -4.499599398310388;
    x[ 5] =  -4.057956316089741;
    x[ 6] =  -3.667774215946338;
    x[ 7] =  -3.315584617593290;
    x[ 8] =  -2.959210779063838;
    x[ 9] =  -2.597288631188366;
    x[10] =  -2.266513262056788;
    x[11] =  -2.023230191100516;
    x[12] =  -1.835707975175187;
    x[13] =  -1.561553427651873;
    x[14] =  -1.224744871391589;
    x[15] =  -0.870040895352903;
    x[16] =  -0.524033547486958;
    x[17] =  -0.214618180588171;
    x[18] =   0.000000000000000;
    x[19] =   0.214618180588171;
    x[20] =   0.524033547486958;
    x[21] =   0.870040895352903;
    x[22] =   1.224744871391589;
    x[23] =   1.561553427651873;
    x[24] =   1.835707975175187;
    x[25] =   2.023230191100516;
    x[26] =   2.266513262056788;
    x[27] =   2.597288631188366;
    x[28] =   2.959210779063838;
    x[29] =   3.315584617593290;
    x[30] =   3.667774215946338;
    x[31] =   4.057956316089741;
    x[32] =   4.499599398310388;
    x[33] =   4.986551454150765;
    x[34] =   5.521865209868350;
    x[35] =   6.124527854622158;
    x[36] =   6.853200069757519;
  }
  else if ( n == 41 )
  {
    x[ 0] =  -7.251792998192644;
    x[ 1] =  -6.547083258397540;
    x[ 2] =  -5.961461043404500;
    x[ 3] =  -5.437443360177798;
    x[ 4] =  -4.953574342912980;
    x[ 5] =  -4.4995993983103881;
    x[ 6] =  -4.070919267883068;
    x[ 7] =  -3.6677742159463378;
    x[ 8] =  -3.296114596212218;
    x[ 9] =  -2.9592107790638380;
    x[10] =  -2.630415236459871;
    x[11] =  -2.2665132620567876;
    x[12] =  -2.043834754429505;
    x[13] =  -2.0232301911005157;
    x[14] =  -1.8357079751751868;
    x[15] =  -1.585873011819188;
    x[16] =  -1.2247448713915889;
    x[17] =  -0.87004089535290285;
    x[18] =  -0.52403354748695763;
    x[19] =  -0.195324784415805;
    x[20] =   0.0000000000000000;
    x[21] =   0.195324784415805;
    x[22] =   0.52403354748695763;
    x[23] =   0.87004089535290285;
    x[24] =   1.2247448713915889;
    x[25] =   1.585873011819188;
    x[26] =   1.8357079751751868;
    x[27] =   2.0232301911005157;
    x[28] =   2.043834754429505;
    x[29] =   2.2665132620567876;
    x[30] =   2.630415236459871;
    x[31] =   2.9592107790638380;
    x[32] =   3.296114596212218;
    x[33] =   3.6677742159463378;
    x[34] =   4.070919267883068;
    x[35] =   4.4995993983103881;
    x[36] =   4.953574342912980;
    x[37] =   5.437443360177798;
    x[38] =   5.961461043404500;
    x[39] =   6.547083258397540;
    x[40] =   7.251792998192644;
  }
  else if ( n == 43 )
  {
    x[ 0] = -10.167574994881873;
    x[ 1] =  -7.231746029072501;
    x[ 2] =  -6.535398426382995;
    x[ 3] =  -5.954781975039809;
    x[ 4] =  -5.434053000365068;
    x[ 5] =  -4.952329763008589;
    x[ 6] =  -4.4995993983103881;
    x[ 7] =  -4.071335874253583;
    x[ 8] =  -3.6677742159463378;
    x[ 9] =  -3.295265921534226;
    x[10] =  -2.9592107790638380;
    x[11] =  -2.633356763661946;
    x[12] =  -2.2665132620567876;
    x[13] =  -2.089340389294661;
    x[14] =  -2.0232301911005157;
    x[15] =  -1.8357079751751868;
    x[16] =  -1.583643465293944;
    x[17] =  -1.2247448713915889;
    x[18] =  -0.87004089535290285;
    x[19] =  -0.52403354748695763;
    x[20] =  -0.196029453662011;
    x[21] =   0.0000000000000000;
    x[22] =   0.196029453662011;
    x[23] =   0.52403354748695763;
    x[24] =   0.87004089535290285;
    x[25] =   1.2247448713915889;
    x[26] =   1.583643465293944;
    x[27] =   1.8357079751751868;
    x[28] =   2.0232301911005157;
    x[29] =   2.089340389294661;
    x[30] =   2.2665132620567876;
    x[31] =   2.633356763661946;
    x[32] =   2.9592107790638380;
    x[33] =   3.295265921534226;
    x[34] =   3.6677742159463378;
    x[35] =   4.071335874253583;
    x[36] =   4.4995993983103881;
    x[37] =   4.952329763008589;
    x[38] =   5.434053000365068;
    x[39] =   5.954781975039809;
    x[40] =   6.535398426382995;
    x[41] =   7.231746029072501;
    x[42] =  10.167574994881873;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "HERMITE_GENZ_KEISTER_LOOKUP_POINTS - Fatal error!\n";
    std::cerr << "  Illegal input value of N.\n";
    std::cerr << "  N must be 1, 3, 9, 19, 35, 37, 41 or 43.\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

void SandiaRules::hermite_genz_keister_lookup_points_np ( int n, int np, double p[],
  double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_GENZ_KEISTER_LOOKUP_POINTS_NP looks up Genz-Keister Hermite abscissas.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo <= x <= +oo ) f(x) exp ( - x * x ) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    A nested family of rules for the Hermite integration problem
//    was produced by Genz and Keister.  The structure of the nested
//    family was denoted by 1+2+6+10+?, that is, it comprised rules
//    of successive orders O = 1, 3, 9, 19, and a final rule of order
//    35, 37, 41 or 43.
//
//    The precisions of these rules are P = 1, 5, 15, 29, 
//    with the final rule of precision 51, 55, 63 or 67.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Genz, Bradley Keister,
//    Fully symmetric interpolatory rules for multiple integrals
//    over infinite regions with Gaussian weight,
//    Journal of Computational and Applied Mathematics,
//    Volume 71, 1996, pages 299-309
//
//    Florian Heiss, Viktor Winschel,
//    Likelihood approximation by numerical integration on sparse grids,
//    Journal of Econometrics,
//    Volume 144, 2008, pages 62-80.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be 1, 3, 9, 19, 35, 37, 41 or 43.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  hermite_genz_keister_lookup_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_genz_keister_lookup_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_GENZ_KEISTER_LOOKUP_WEIGHTS looks up Genz-Keister Hermite weights.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo <= x <= +oo ) f(x) exp ( - x * x ) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    A nested family of rules for the Hermite integration problem
//    was produced by Genz and Keister.  The structure of the nested
//    family was denoted by 1+2+6+10+?, that is, it comprised rules
//    of successive orders O = 1, 3, 9, 19, and a final rule of order
//    35, 37, 41 or 43.
//
//    The precisions of these rules are P = 1, 5, 15, 29, 
//    with the final rule of precision 51, 55, 63 or 67.
//
//    Three related families begin the same way, but end with a different final
//    rule.  As a convenience, this function includes these final rules as well:
//
//    Designation  Orders       Precisions
//
//    1+2+6+10+16, 1,3,9,19,35  1,5,15,29,51
//    1+2+6+10+18  1,3,9,19,37  1,5,15,29,55
//    1+2+6+10+22  1,3,9,19,41  1,5,15,29,63
//    1+2+6+10+24  1,3,9,19,43  1,5,15,29,67
//
//    Some of the data in this function was kindly supplied directly by
//    Alan Genz on 24 April 2011.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Genz, Bradley Keister,
//    Fully symmetric interpolatory rules for multiple integrals
//    over infinite regions with Gaussian weight,
//    Journal of Computational and Applied Mathematics,
//    Volume 71, 1996, pages 299-309
//
//    Florian Heiss, Viktor Winschel,
//    Likelihood approximation by numerical integration on sparse grids,
//    Journal of Econometrics,
//    Volume 144, 2008, pages 62-80.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be 1, 3, 9, 19, 35, 37, 41, or 43.
//
//    Output, double W[N], the weights.
//
{
  //static double sqrtpi = 1.7724538509055159;

  if ( n == 1 )
  {
    w[ 0] =   1.7724538509055159E+00;
  }
  else if ( n == 3 )
  {
    w[ 0] =   2.9540897515091930E-01;
    w[ 1] =   1.1816359006036772E+00;
    w[ 2] =   2.9540897515091930E-01;
  }
  else if ( n == 9 )
  {
    w[ 0] =   1.6708826306882348E-04;
    w[ 1] =   1.4173117873979098E-02;
    w[ 2] =   1.6811892894767771E-01;
    w[ 3] =   4.7869428549114124E-01;
    w[ 4] =   4.5014700975378197E-01;
    w[ 5] =   4.7869428549114124E-01;
    w[ 6] =   1.6811892894767771E-01;
    w[ 7] =   1.4173117873979098E-02;
    w[ 8] =   1.6708826306882348E-04;
  }
  else if ( n == 19 )
  {
    w[ 0] =   1.5295717705322357E-09;
    w[ 1] =   1.0802767206624762E-06;
    w[ 2] =   1.0656589772852267E-04;
    w[ 3] =   5.1133174390883855E-03;
    w[ 4] =  -1.1232438489069229E-02;
    w[ 5] =   3.2055243099445879E-02;
    w[ 6] =   1.1360729895748269E-01;
    w[ 7] =   1.0838861955003017E-01;
    w[ 8] =   3.6924643368920851E-01;
    w[ 9] =   5.3788160700510168E-01;
    w[10] =   3.6924643368920851E-01;
    w[11] =   1.0838861955003017E-01;
    w[12] =   1.1360729895748269E-01;
    w[13] =   3.2055243099445879E-02;
    w[14] =  -1.1232438489069229E-02;
    w[15] =   5.1133174390883855E-03;
    w[16] =   1.0656589772852267E-04;
    w[17] =   1.0802767206624762E-06;
    w[18] =   1.5295717705322357E-09;
  }
  else if ( n == 35 )
  {
    w[ 0] =   1.8684014894510604E-18;
    w[ 1] =   9.6599466278563243E-15;
    w[ 2] =   5.4896836948499462E-12;
    w[ 3] =   8.1553721816916897E-10;
    w[ 4] =   3.7920222392319532E-08;
    w[ 5] =   4.3737818040926989E-07;
    w[ 6] =   4.8462799737020461E-06;
    w[ 7] =   6.3328620805617891E-05;
    w[ 8] =   4.8785399304443770E-04;
    w[ 9] =   1.4515580425155904E-03;
    w[10] =   4.0967527720344047E-03;
    w[11] =   5.5928828911469180E-03;
    w[12] =   2.7780508908535097E-02;
    w[13] =   8.0245518147390893E-02;
    w[14] =   1.6371221555735804E-01;
    w[15] =   2.6244871488784277E-01;
    w[16] =   3.3988595585585218E-01;
    w[17] =   9.1262675363737921E-04;
    w[18] =   3.3988595585585218E-01;
    w[19] =   2.6244871488784277E-01;
    w[20] =   1.6371221555735804E-01;
    w[21] =   8.0245518147390893E-02;
    w[22] =   2.7780508908535097E-02;
    w[23] =   5.5928828911469180E-03;
    w[24] =   4.0967527720344047E-03;
    w[25] =   1.4515580425155904E-03;
    w[26] =   4.8785399304443770E-04;
    w[27] =   6.3328620805617891E-05;
    w[28] =   4.8462799737020461E-06;
    w[29] =   4.3737818040926989E-07;
    w[30] =   3.7920222392319532E-08;
    w[31] =   8.1553721816916897E-10;
    w[32] =   5.4896836948499462E-12;
    w[33] =   9.6599466278563243E-15;
    w[34] =   1.8684014894510604E-18;
  }
  else if ( n == 37 )
  {
    w[ 0] = 0.337304188079177058E-20;
    w[ 1] = 0.332834739632930463E-16;
    w[ 2] = 0.323016866782871498E-13;
    w[ 3] = 0.809333688669950037E-11;
    w[ 4] = 0.748907559239519284E-09;
    w[ 5] = 0.294146671497083432E-07;
    w[ 6] = 0.524482423744884136E-06;
    w[ 7] = 0.586639457073896277E-05;
    w[ 8] = 0.571885531470621903E-04;
    w[ 9] = 0.41642095727577091E-03;
    w[10] = 0.174733389581099482E-02;
    w[11] = 0.313373786000304381E-02;
    w[12] = 0.768092665770660459E-02;
    w[13] = 0.274962713372148476E-01;
    w[14] = 0.783630990508037449E-01;
    w[15] = 0.16611584261479281E+00;
    w[16] = 0.253636910481387185E+00;
    w[17] = 0.261712932511430884E+00;
    w[18] = 0.171719680968980257E+00;
    w[19] = 0.261712932511430884E+00;
    w[20] = 0.253636910481387185E+00;
    w[21] = 0.16611584261479281E+00;
    w[22] = 0.783630990508037449E-01;
    w[23] = 0.274962713372148476E-01;
    w[24] = 0.768092665770660459E-02;
    w[25] = 0.313373786000304381E-02;
    w[26] = 0.174733389581099482E-02;
    w[27] = 0.41642095727577091E-03;
    w[28] = 0.571885531470621903E-04;
    w[29] = 0.586639457073896277E-05;
    w[30] = 0.524482423744884136E-06;
    w[31] = 0.294146671497083432E-07;
    w[32] = 0.748907559239519284E-09;
    w[33] = 0.809333688669950037E-11;
    w[34] = 0.323016866782871498E-13;
    w[35] = 0.332834739632930463E-16;
    w[36] = 0.337304188079177058E-20;
  }
  else if ( n == 41 )
  {
    w[ 0] =   0.117725656974405367E-22;
    w[ 1] =   0.152506745534300636E-18;
    w[ 2] =   0.202183949965101288E-15;
    w[ 3] =   0.724614869051195508E-13;
    w[ 4] =   0.103121966469463034E-10;
    w[ 5] =   0.710371395169350952E-09;
    w[ 6] =   0.264376044449260516E-07;
    w[ 7] =   0.558982787078644997E-06;
    w[ 8] =   0.675628907134744976E-05;
    w[ 9] =   0.512198007019776873E-04;
    w[10] =   0.335013114947200879E-03;
    w[11] =   0.249379691096933139E-02;
    w[12] = - 0.25616995850607458E-01;
    w[13] =   0.317007878644325588E-01;
    w[14] =   0.125041498584003435E-02;
    w[15] =   0.293244560924894295E-01;
    w[16] =   0.799536390803302298E-01;
    w[17] =   0.164543666806555251E+00;
    w[18] =   0.258718519718241095E+00;
    w[19] =   0.293588795735908566E+00;
    w[20] =   0.997525375254611951E-01;
    w[21] =   0.293588795735908566E+00;
    w[22] =   0.258718519718241095E+00;
    w[23] =   0.164543666806555251E+00;
    w[24] =   0.799536390803302298E-01;
    w[25] =   0.293244560924894295E-01;
    w[26] =   0.125041498584003435E-02;
    w[27] =   0.317007878644325588E-01;
    w[28] = - 0.25616995850607458E-01;
    w[29] =   0.249379691096933139E-02;
    w[30] =   0.335013114947200879E-03;
    w[31] =   0.512198007019776873E-04;
    w[32] =   0.675628907134744976E-05;
    w[33] =   0.558982787078644997E-06;
    w[34] =   0.264376044449260516E-07;
    w[35] =   0.710371395169350952E-09;
    w[36] =   0.103121966469463034E-10;
    w[37] =   0.724614869051195508E-13;
    w[38] =   0.202183949965101288E-15;
    w[39] =   0.152506745534300636E-18;
    w[40] =   0.117725656974405367E-22;
  }
  else if ( n == 43 )
  {
    w[ 0] =   0.968100020641528185E-37;
    w[ 1] =   0.15516931262860431E-22;
    w[ 2] =   0.175937309107750992E-18;
    w[ 3] =   0.217337608710893738E-15;
    w[ 4] =   0.747837010380540069E-13;
    w[ 5] =   0.104028132097205732E-10;
    w[ 6] =   0.70903573389336778E-09;
    w[ 7] =   0.263481722999966618E-07;
    w[ 8] =   0.560127964848432175E-06;
    w[ 9] =   0.680410934802210232E-05;
    w[10] =   0.508343873102544037E-04;
    w[11] =   0.32753080006610181E-03;
    w[12] =   0.267479828788552937E-02;
    w[13] = - 0.687704270963253854E-02;
    w[14] =   0.119383201790913588E-01;
    w[15] =   0.248083722871002796E-02;
    w[16] =   0.29000335749726387E-01;
    w[17] =   0.798689557875757008E-01;
    w[18] =   0.164609842422580606E+00;
    w[19] =   0.258535954731607738E+00;
    w[20] =   0.292243810406117141E+00;
    w[21] =   0.102730713753441829E+00;
    w[22] =   0.292243810406117141E+00;
    w[23] =   0.258535954731607738E+00;
    w[24] =   0.164609842422580606E+00;
    w[25] =   0.798689557875757008E-01;
    w[26] =   0.29000335749726387E-01;
    w[27] =   0.248083722871002796E-02;
    w[28] =   0.119383201790913588E-01;
    w[29] = - 0.687704270963253854E-02;
    w[30] =   0.267479828788552937E-02;
    w[31] =   0.32753080006610181E-03;
    w[32] =   0.508343873102544037E-04;
    w[33] =   0.680410934802210232E-05;
    w[34] =   0.560127964848432175E-06;
    w[35] =   0.263481722999966618E-07;
    w[36] =   0.70903573389336778E-09;
    w[37] =   0.104028132097205732E-10;
    w[38] =   0.747837010380540069E-13;
    w[39] =   0.217337608710893738E-15;
    w[40] =   0.175937309107750992E-18;
    w[41] =   0.15516931262860431E-22;
    w[42] =   0.968100020641528185E-37;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "HERMITE_GENZ_KEISTER_LOOKUP_WEIGHTS - Fatal error!\n";
    std::cerr << "  Illegal input value of N.\n";
    std::cerr << "  N must be 1, 3, 9, 19, 35, 37, 41 or 43.\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

void SandiaRules::hermite_genz_keister_lookup_weights_np ( int n, int np, double p[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_GENZ_KEISTER_LOOKUP_WEIGHTS_NP looks up Genz-Keister Hermite weights.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo <= x <= +oo ) f(x) exp ( - x * x ) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    A nested family of rules for the Hermite integration problem
//    was produced by Genz and Keister.  The structure of the nested
//    family was denoted by 1+2+6+10+?, that is, it comprised rules
//    of successive orders O = 1, 3, 9, 19, and a final rule of order
//    35, 37, 41 or 43.
//
//    The precisions of these rules are P = 1, 5, 15, 29, 
//    with the final rule of precision 51, 55, 63 or 67.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Genz, Bradley Keister,
//    Fully symmetric interpolatory rules for multiple integrals
//    over infinite regions with Gaussian weight,
//    Journal of Computational and Applied Mathematics,
//    Volume 71, 1996, pages 299-309
//
//    Florian Heiss, Viktor Winschel,
//    Likelihood approximation by numerical integration on sparse grids,
//    Journal of Econometrics,
//    Volume 144, 2008, pages 62-80.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be 1, 3, 9, 19, 35, 37, 41 or 43.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  hermite_genz_keister_lookup_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_gk18_lookup_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_GK18_LOOKUP_POINTS: abscissas of a Hermite Genz-Keister 18 rule.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo <= x <= +oo ) f(x) exp ( - x * x ) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    A nested family of rules for the Hermite integration problem
//    was produced by Genz and Keister.  The structure of the nested
//    family was denoted by 1+2+6+10+18, that is, it comprised rules 
//    of successive orders O = 1, 3, 9, 19, and 37.
//
//    The precisions of these rules are P = 1, 5, 15, 29, and 55.
//
//    Some of the data in this function was kindly supplied directly by
//    Alan Genz on 24 April 2011.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Genz, Bradley Keister,
//    Fully symmetric interpolatory rules for multiple integrals
//    over infinite regions with Gaussian weight,
//    Journal of Computational and Applied Mathematics,
//    Volume 71, 1996, pages 299-309
//
//    Florian Heiss, Viktor Winschel,
//    Likelihood approximation by numerical integration on sparse grids,
//    Journal of Econometrics,
//    Volume 144, 2008, pages 62-80.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be 1, 3, 9, 19, or 37.
//
//    Output, double X[N], the abscissas.
//
{
  if ( n == 1 )
  {
    x[ 0] =   0.0000000000000000E+00;
  }
  else if ( n == 3 )
  {
    x[ 0] =  -1.2247448713915889E+00;
    x[ 1] =   0.0000000000000000E+00;
    x[ 2] =   1.2247448713915889E+00;
  }
  else if ( n == 9 )
  {
    x[ 0] =  -2.9592107790638380E+00;
    x[ 1] =  -2.0232301911005157E+00;
    x[ 2] =  -1.2247448713915889E+00;
    x[ 3] =  -5.2403354748695763E-01;
    x[ 4] =   0.0000000000000000E+00;
    x[ 5] =   5.2403354748695763E-01;
    x[ 6] =   1.2247448713915889E+00;
    x[ 7] =   2.0232301911005157E+00;
    x[ 8] =   2.9592107790638380E+00;
  }
  else if ( n == 19 )
  {
    x[ 0] =  -4.4995993983103881E+00;
    x[ 1] =  -3.6677742159463378E+00;
    x[ 2] =  -2.9592107790638380E+00;
    x[ 3] =  -2.2665132620567876E+00;
    x[ 4] =  -2.0232301911005157E+00;
    x[ 5] =  -1.8357079751751868E+00;
    x[ 6] =  -1.2247448713915889E+00;
    x[ 7] =  -8.7004089535290285E-01;
    x[ 8] =  -5.2403354748695763E-01;
    x[ 9] =   0.0000000000000000E+00;
    x[10] =   5.2403354748695763E-01;
    x[11] =   8.7004089535290285E-01;
    x[12] =   1.2247448713915889E+00;
    x[13] =   1.8357079751751868E+00;
    x[14] =   2.0232301911005157E+00;
    x[15] =   2.2665132620567876E+00;
    x[16] =   2.9592107790638380E+00;
    x[17] =   3.6677742159463378E+00;
    x[18] =   4.4995993983103881E+00;
  }
  else if ( n == 35 )
  {
    x[ 0] =  -6.853200069757519;
    x[ 1] =  -6.124527854622158;
    x[ 2] =  -5.521865209868350;
    x[ 3] =  -4.986551454150765;
    x[ 4] =  -4.499599398310388;
    x[ 5] =  -4.057956316089741;
    x[ 6] =  -3.667774215946338;
    x[ 7] =  -3.315584617593290;
    x[ 8] =  -2.959210779063838;
    x[ 9] =  -2.597288631188366;
    x[10] =  -2.266513262056788;
    x[11] =  -2.023230191100516;
    x[12] =  -1.835707975175187;
    x[13] =  -1.561553427651873;
    x[14] =  -1.224744871391589;
    x[15] =  -0.870040895352903;
    x[16] =  -0.524033547486958;
    x[17] =  -0.214618180588171;
    x[18] =   0.000000000000000;
    x[19] =   0.214618180588171;
    x[20] =   0.524033547486958;
    x[21] =   0.870040895352903;
    x[22] =   1.224744871391589;
    x[23] =   1.561553427651873;
    x[24] =   1.835707975175187;
    x[25] =   2.023230191100516;
    x[26] =   2.266513262056788;
    x[27] =   2.597288631188366;
    x[28] =   2.959210779063838;
    x[29] =   3.315584617593290;
    x[30] =   3.667774215946338;
    x[31] =   4.057956316089741;
    x[32] =   4.499599398310388;
    x[33] =   4.986551454150765;
    x[34] =   5.521865209868350;
    x[35] =   6.124527854622158;
    x[36] =   6.853200069757519;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "HERMITE_GK18_LOOKUP_POINTS - Fatal error!\n";
    std::cerr << "  Illegal input value of N.\n";
    std::cerr << "  N must be 1, 3, 9, 19, or 37.\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

void SandiaRules::hermite_gk22_lookup_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_GK22_LOOKUP_POINTS looks up Hermite Genz-Keister 22 points.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo <= x <= +oo ) f(x) exp ( - x * x ) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    A nested family of rules for the Hermite integration problem
//    was produced by Genz and Keister.  The structure of the nested
//    family was denoted by 1+2+6+10+16, that is, it comprised rules 
//    of successive orders O = 1, 3, 9, 19, and 41.
//
//    The precisions of these rules are P = 1, 5, 15, 29, and 63.
//
//    Some of the data in this function was kindly supplied directly by
//    Alan Genz on 24 April 2011.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 April 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Genz, Bradley Keister,
//    Fully symmetric interpolatory rules for multiple integrals
//    over infinite regions with Gaussian weight,
//    Journal of Computational and Applied Mathematics,
//    Volume 71, 1996, pages 299-309
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be 1, 3, 9, 19, or 41.
//
//    Output, double X[N], the abscissas.
//
{
  if ( n == 1 )
  {
    x[ 0] =   0.0000000000000000E+00;
  }
  else if ( n == 3 )
  {
    x[ 0] =  -1.2247448713915889E+00;
    x[ 1] =   0.0000000000000000E+00;
    x[ 2] =   1.2247448713915889E+00;
  }
  else if ( n == 9 )
  {
    x[ 0] =  -2.9592107790638380E+00;
    x[ 1] =  -2.0232301911005157E+00;
    x[ 2] =  -1.2247448713915889E+00;
    x[ 3] =  -5.2403354748695763E-01;
    x[ 4] =   0.0000000000000000E+00;
    x[ 5] =   5.2403354748695763E-01;
    x[ 6] =   1.2247448713915889E+00;
    x[ 7] =   2.0232301911005157E+00;
    x[ 8] =   2.9592107790638380E+00;
  }
  else if ( n == 19 )
  {
    x[ 0] =  -4.4995993983103881E+00;
    x[ 1] =  -3.6677742159463378E+00;
    x[ 2] =  -2.9592107790638380E+00;
    x[ 3] =  -2.2665132620567876E+00;
    x[ 4] =  -2.0232301911005157E+00;
    x[ 5] =  -1.8357079751751868E+00;
    x[ 6] =  -1.2247448713915889E+00;
    x[ 7] =  -8.7004089535290285E-01;
    x[ 8] =  -5.2403354748695763E-01;
    x[ 9] =   0.0000000000000000E+00;
    x[10] =   5.2403354748695763E-01;
    x[11] =   8.7004089535290285E-01;
    x[12] =   1.2247448713915889E+00;
    x[13] =   1.8357079751751868E+00;
    x[14] =   2.0232301911005157E+00;
    x[15] =   2.2665132620567876E+00;
    x[16] =   2.9592107790638380E+00;
    x[17] =   3.6677742159463378E+00;
    x[18] =   4.4995993983103881E+00;
  }
  else if ( n == 41 )
  {
    x[ 0] =  -7.251792998192644;
    x[ 1] =  -6.547083258397540;
    x[ 2] =  -5.961461043404500;
    x[ 3] =  -5.437443360177798;
    x[ 4] =  -4.953574342912980;
    x[ 5] =  -4.4995993983103881;
    x[ 6] =  -4.070919267883068;
    x[ 7] =  -3.6677742159463378;
    x[ 8] =  -3.296114596212218;
    x[ 9] =  -2.9592107790638380;
    x[10] =  -2.630415236459871;
    x[11] =  -2.2665132620567876;
    x[12] =  -2.043834754429505;
    x[13] =  -2.0232301911005157;
    x[14] =  -1.8357079751751868;
    x[15] =  -1.585873011819188;
    x[16] =  -1.2247448713915889;
    x[17] =  -0.87004089535290285;
    x[18] =  -0.52403354748695763;
    x[19] =  -0.195324784415805;
    x[20] =   0.0000000000000000;
    x[21] =   0.195324784415805;
    x[22] =   0.52403354748695763;
    x[23] =   0.87004089535290285;
    x[24] =   1.2247448713915889;
    x[25] =   1.585873011819188;
    x[26] =   1.8357079751751868;
    x[27] =   2.0232301911005157;
    x[28] =   2.043834754429505;
    x[29] =   2.2665132620567876;
    x[30] =   2.630415236459871;
    x[31] =   2.9592107790638380;
    x[32] =   3.296114596212218;
    x[33] =   3.6677742159463378;
    x[34] =   4.070919267883068;
    x[35] =   4.4995993983103881;
    x[36] =   4.953574342912980;
    x[37] =   5.437443360177798;
    x[38] =   5.961461043404500;
    x[39] =   6.547083258397540;
    x[40] =   7.251792998192644;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "HERMITE_GK22_LOOKUP_POINTS - Fatal error!\n";
    std::cerr << "  Illegal input value of N.\n";
    std::cerr << "  N must be 1, 3, 9, 19, or 41.\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

void SandiaRules::hermite_gk24_lookup_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_GK24_LOOKUP_POINTS looks up Hermite Genz-Keister 24 points.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo <= x <= +oo ) f(x) exp ( - x * x ) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) )
//
//    A nested family of rules for the Hermite integration problem
//    was produced by Genz and Keister.  The structure of the nested
//    family was denoted by 1+2+6+10+16, that is, it comprised rules 
//    of successive orders O = 1, 3, 9, 19, and 43.
//
//    The precisions of these rules are P = 1, 5, 15, 29, and 67.
//
//    Some of the data in this function was kindly supplied directly by
//    Alan Genz on 24 April 2011.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 April 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Genz, Bradley Keister,
//    Fully symmetric interpolatory rules for multiple integrals
//    over infinite regions with Gaussian weight,
//    Journal of Computational and Applied Mathematics,
//    Volume 71, 1996, pages 299-309
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be 1, 3, 9 19, or 43.
//
//    Output, double X[N], the abscissas.
//
{
  if ( n == 1 )
  {
    x[ 0] =   0.0000000000000000E+00;
  }
  else if ( n == 3 )
  {
    x[ 0] =  -1.2247448713915889E+00;
    x[ 1] =   0.0000000000000000E+00;
    x[ 2] =   1.2247448713915889E+00;
  }
  else if ( n == 9 )
  {
    x[ 0] =  -2.9592107790638380E+00;
    x[ 1] =  -2.0232301911005157E+00;
    x[ 2] =  -1.2247448713915889E+00;
    x[ 3] =  -5.2403354748695763E-01;
    x[ 4] =   0.0000000000000000E+00;
    x[ 5] =   5.2403354748695763E-01;
    x[ 6] =   1.2247448713915889E+00;
    x[ 7] =   2.0232301911005157E+00;
    x[ 8] =   2.9592107790638380E+00;
  }
  else if ( n == 19 )
  {
    x[ 0] =  -4.4995993983103881E+00;
    x[ 1] =  -3.6677742159463378E+00;
    x[ 2] =  -2.9592107790638380E+00;
    x[ 3] =  -2.2665132620567876E+00;
    x[ 4] =  -2.0232301911005157E+00;
    x[ 5] =  -1.8357079751751868E+00;
    x[ 6] =  -1.2247448713915889E+00;
    x[ 7] =  -8.7004089535290285E-01;
    x[ 8] =  -5.2403354748695763E-01;
    x[ 9] =   0.0000000000000000E+00;
    x[10] =   5.2403354748695763E-01;
    x[11] =   8.7004089535290285E-01;
    x[12] =   1.2247448713915889E+00;
    x[13] =   1.8357079751751868E+00;
    x[14] =   2.0232301911005157E+00;
    x[15] =   2.2665132620567876E+00;
    x[16] =   2.9592107790638380E+00;
    x[17] =   3.6677742159463378E+00;
    x[18] =   4.4995993983103881E+00;
  }
  else if ( n == 43 )
  {
    x[ 0] = -10.167574994881873;
    x[ 1] =  -7.231746029072501;
    x[ 2] =  -6.535398426382995;
    x[ 3] =  -5.954781975039809;
    x[ 4] =  -5.434053000365068;
    x[ 5] =  -4.952329763008589;
    x[ 6] =  -4.4995993983103881;
    x[ 7] =  -4.071335874253583;
    x[ 8] =  -3.6677742159463378;
    x[ 9] =  -3.295265921534226;
    x[10] =  -2.9592107790638380;
    x[11] =  -2.633356763661946;
    x[12] =  -2.2665132620567876;
    x[13] =  -2.089340389294661;
    x[14] =  -2.0232301911005157;
    x[15] =  -1.8357079751751868;
    x[16] =  -1.583643465293944;
    x[17] =  -1.2247448713915889;
    x[18] =  -0.87004089535290285;
    x[19] =  -0.52403354748695763;
    x[20] =  -0.196029453662011;
    x[21] =   0.0000000000000000;
    x[22] =   0.196029453662011;
    x[23] =   0.52403354748695763;
    x[24] =   0.87004089535290285;
    x[25] =   1.2247448713915889;
    x[26] =   1.583643465293944;
    x[27] =   1.8357079751751868;
    x[28] =   2.0232301911005157;
    x[29] =   2.089340389294661;
    x[30] =   2.2665132620567876;
    x[31] =   2.633356763661946;
    x[32] =   2.9592107790638380;
    x[33] =   3.295265921534226;
    x[34] =   3.6677742159463378;
    x[35] =   4.071335874253583;
    x[36] =   4.4995993983103881;
    x[37] =   4.952329763008589;
    x[38] =   5.434053000365068;
    x[39] =   5.954781975039809;
    x[40] =   6.535398426382995;
    x[41] =   7.231746029072501;
    x[42] =  10.167574994881873;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "HERMITE_GK24_LOOKUP_POINTS - Fatal error!\n";
    std::cerr << "  Illegal input value of N.\n";
    std::cerr << "  N must be 1, 3, 9, 19, or 43.\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

double SandiaRules::hermite_integral ( int n )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_INTEGRAL evaluates a monomial Hermite integral.
//
//  Discussion:
//
//    H(n) = Integral ( -oo < x < +oo ) x^n exp(-x^2) dx
//
//    H(n) is 0 for n odd.
//
//    H(n) = (n-1)!! * sqrt(pi) / 2^(n/2) for n even.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the integral.
//    0 <= N.
//
//    Output, double VALUE, the value of the integral.
//
{
  double pi = 3.141592653589793;
  double value;

  if ( n < 0 )
  {
    value = - r8_huge ( );
  }
  else if ( ( n % 2 ) == 1 )
  {
    value = 0.0;
  }
  else
  {
    value = r8_factorial2 ( n - 1 ) * std::sqrt ( pi )
      / std::pow ( 2.0, n / 2 );
  }

  return value;
}
//**************************************************************************80

void SandiaRules::hermite_interpolant ( int n, double x[], double y[], double yp[], 
  double xd[], double yd[], double xdp[], double ydp[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_INTERPOLANT sets up a divided difference table from Hermite data.
//
//  Discussion:
//
//    The polynomial represented by the divided difference table can be
//    evaluated by calling DIF_VALS.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    31 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Carl deBoor,
//    A Practical Guide to Splines,
//    Springer, 2001,
//    ISBN: 0387953663,
//    LC: QA1.A647.v27.
//
//  Parameters:
//
//    Input, int N, of items of data 
//    ( X(I), Y(I), YP(I) ).
//
//    Input, double X[N], the abscissas.
//    These values must be distinct.
//
//    Input, double Y[N], YP[N], the function and derivative values.
//
//    Output, double XD[2*N], YD[2*N], the divided difference table
//    for the interpolant value.
//
//    Output, double XDP[2*N-1], YDP[2*N-1], the divided difference 
//    table for the interpolant derivative.
//
{
  int i;
  int j;
  int nd;
  int ndp;
//
//  Copy the data.
//
  nd = 2 * n;

  for ( i = 0; i < n; i++ )
  {
    xd[0+i*2] = x[i];
    xd[1+i*2] = x[i];   
  }
//
//  Carry out the first step of differencing.
//
  yd[0] = y[0];
  for ( i = 1; i < n; i++ )
  {
    yd[0+2*i] = ( y[i] - y[i-1] ) / ( x[i] - x[i-1] );
  }
  for ( i = 0; i < n; i++ )
  {
    yd[1+2*i] = yp[i];
  }
//
//  Carry out the remaining steps in the usual way.
//
  for ( i = 2; i < nd; i++ )
  {
    for ( j = nd - 1; i <= j; j-- )
    {
      yd[j] = ( yd[j] - yd[j-1] ) / ( xd[j] - xd[j-i] );
    }
  }
//
//  Compute the difference table for the derivative.
//
  dif_deriv ( nd, xd, yd, &ndp, xdp, ydp );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_interpolant_rule ( int n, double a, double b, double x[], 
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_INTERPOLANT_RULE: quadrature rule for a Hermite interpolant.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of abscissas.
//
//    Input, double A, B, the integration limits.
//
//    Input, double X[N], the abscissas.
//
//    Output, double W[2*N], the quadrature 
//    coefficients, given as pairs for function and derivative values 
//    at each abscissa.
//
{
  double a_value;
  double b_value;
  double *c;
  int i;
  //int j;
  int k;
  int nd;
  int ndp;
  double *xd;
  double *xdp;
  double *y;
  double *yd;
  double *ydp;
  double *yp;

  y = new double[n];
  yp = new double[n];

  nd = 2 * n;
  c = new double[nd];
  xd = new double[nd];
  yd = new double[nd];

  ndp = 2 * n - 1;
  xdp = new double[ndp];
  ydp = new double[ndp];

  for ( i = 0; i < n; i++ )
  {
    y[i] = 0.0;
    yp[i] = 0.0;
  }

  k = 0;

  for ( i = 0; i < n; i++ )
  {
    y[i] = 1.0;
    hermite_interpolant ( n, x, y, yp, xd, yd, xdp, ydp );
    dif_to_r8poly ( nd, xd, yd, c );
    a_value = r8poly_ant_val ( n, c, a );
    b_value = r8poly_ant_val ( n, c, b );
    w[k] = b_value - a_value;
    y[i] = 0.0;
    k = k + 1;

    yp[i] = 1.0;
    hermite_interpolant ( n, x, y, yp, xd, yd, xdp, ydp );
    dif_to_r8poly ( nd, xd, yd, c );
    a_value = r8poly_ant_val ( n, c, a );
    b_value = r8poly_ant_val ( n, c, b );
    w[k] = b_value - a_value;
    yp[i] = 0.0;
    k = k + 1;
  }

  delete [] c;
  delete [] xd;
  delete [] xdp;
  delete [] y;
  delete [] yd;
  delete [] ydp;
  delete [] yp;

  return;
}
//**************************************************************************80

void SandiaRules::hermite_interpolant_value ( int nd, double xd[], double yd[], double xdp[], 
  double ydp[], int nv, double xv[], double yv[], double yvp[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_INTERPOLANT_VALUE evaluates the Hermite interpolant polynomial.
//
//  Discussion:
//
//    In fact, this function will evaluate an arbitrary polynomial that is
//    represented by a difference table.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    31 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Carl deBoor,
//    A Practical Guide to Splines,
//    Springer, 2001,
//    ISBN: 0387953663,
//    LC: QA1.A647.v27.
//
//  Parameters:
//
//    Input, int ND, the order of the difference table.
//
//    Input, double XD[ND], YD[ND], the difference table for the
//    interpolant value.
//
//    Input, double XDP[ND-1], YDP[ND-1], the difference table for
//    the interpolant derivative.
//
//    Input, int NV, the number of evaluation points.
//
//    Input, double XV[NV], the evaluation points.
//
//    Output, double YV[NV], YVP[NV], the value of the interpolant and
//    its derivative at the evaluation points.
//
{
  int i;
  int j;
  int ndp;

  ndp = nd - 1;

  for ( j = 0; j < nv; j++ )
  {
    yv[j] = yd[nd-1];
    for ( i = nd - 2; 0 <= i; i-- )
    {
      yv[j] = yd[i] + ( xv[j] - xd[i] ) * yv[j];
    }

    yvp[j] = ydp[ndp-1];
    for ( i = ndp - 2; 0 <= i; i-- )
    {
      yvp[j] = ydp[i] + ( xv[j] - xdp[i] ) * yvp[j];
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::hermite_lookup ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_LOOKUP looks up abscissas and weights for Gauss-Hermite quadrature.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798.
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3,
//    LC: QA47.M315.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 20.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  hermite_lookup_points ( n, x );

  hermite_lookup_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::hermite_lookup_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_LOOKUP_POINTS looks up abscissas for Hermite quadrature.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo < x < +oo ) exp ( - x * x ) * f(x) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) ).
//
//    Mathematica can numerically estimate the abscissas
//    of order N to P digits by the command:
//
//      NSolve [ HermiteH [ n, x ] == 0, x, p ]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798,
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3,
//    LC: QA47.M315.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 20.
//
//    Output, double X[N], the abscissas.
//
{
  if ( n == 1 )
  {
    x[0] = 0.0;
  }
  else if ( n == 2 )
  {
    x[0] = - 0.707106781186547524400844362105E+00;
    x[1] =   0.707106781186547524400844362105E+00;
  }
  else if ( n == 3 )
  {
    x[0] = - 0.122474487139158904909864203735E+01;
    x[1] =   0.0E+00;
    x[2] =   0.122474487139158904909864203735E+01;
  }
  else if ( n == 4 )
  {
    x[0] = - 0.165068012388578455588334111112E+01;
    x[1] = - 0.524647623275290317884060253835E+00;
    x[2] =   0.524647623275290317884060253835E+00;
    x[3] =   0.165068012388578455588334111112E+01;
  }
  else if ( n == 5 )
  {
    x[0] = - 0.202018287045608563292872408814E+01;
    x[1] = - 0.958572464613818507112770593893E+00;
    x[2] =   0.0E+00;
    x[3] =   0.958572464613818507112770593893E+00;
    x[4] =   0.202018287045608563292872408814E+01;
  }
  else if ( n == 6 )
  {
    x[0] = - 0.235060497367449222283392198706E+01;
    x[1] = - 0.133584907401369694971489528297E+01;
    x[2] = - 0.436077411927616508679215948251E+00;
    x[3] =   0.436077411927616508679215948251E+00;
    x[4] =   0.133584907401369694971489528297E+01;
    x[5] =   0.235060497367449222283392198706E+01;
  }
  else if ( n == 7 )
  {
    x[0] = - 0.265196135683523349244708200652E+01;
    x[1] = - 0.167355162876747144503180139830E+01;
    x[2] = - 0.816287882858964663038710959027E+00;
    x[3] =   0.0E+00;
    x[4] =   0.816287882858964663038710959027E+00;
    x[5] =   0.167355162876747144503180139830E+01;
    x[6] =   0.265196135683523349244708200652E+01;
  }
  else if ( n == 8 )
  {
    x[0] = - 0.293063742025724401922350270524E+01;
    x[1] = - 0.198165675669584292585463063977E+01;
    x[2] = - 0.115719371244678019472076577906E+01;
    x[3] = - 0.381186990207322116854718885584E+00;
    x[4] =   0.381186990207322116854718885584E+00;
    x[5] =   0.115719371244678019472076577906E+01;
    x[6] =   0.198165675669584292585463063977E+01;
    x[7] =   0.293063742025724401922350270524E+01;
  }
  else if ( n == 9 )
  {
    x[0] = - 0.319099320178152760723004779538E+01;
    x[1] = - 0.226658058453184311180209693284E+01;
    x[2] = - 0.146855328921666793166701573925E+01;
    x[3] = - 0.723551018752837573322639864579E+00;
    x[4] =   0.0E+00;
    x[5] =   0.723551018752837573322639864579E+00;
    x[6] =   0.146855328921666793166701573925E+01;
    x[7] =   0.226658058453184311180209693284E+01;
    x[8] =   0.319099320178152760723004779538E+01;
  }
  else if ( n == 10 )
  {
    x[0] =  - 0.343615911883773760332672549432E+01;
    x[1] =  - 0.253273167423278979640896079775E+01;
    x[2] =  - 0.175668364929988177345140122011E+01;
    x[3] =  - 0.103661082978951365417749191676E+01;
    x[4] =  - 0.342901327223704608789165025557E+00;
    x[5] =    0.342901327223704608789165025557E+00;
    x[6] =    0.103661082978951365417749191676E+01;
    x[7] =    0.175668364929988177345140122011E+01;
    x[8] =    0.253273167423278979640896079775E+01;
    x[9] =    0.343615911883773760332672549432E+01;
  }
  else if ( n == 11 )
  {
    x[0] =  - 0.366847084655958251845837146485E+01;
    x[1] =  - 0.278329009978165177083671870152E+01;
    x[2] =  - 0.202594801582575533516591283121E+01;
    x[3] =  - 0.132655708449493285594973473558E+01;
    x[4] =  - 0.656809566882099765024611575383E+00;
    x[5] =    0.0E+00;
    x[6] =    0.656809566882099765024611575383E+00;
    x[7] =    0.132655708449493285594973473558E+01;
    x[8] =    0.202594801582575533516591283121E+01;
    x[9] =    0.278329009978165177083671870152E+01;
    x[10] =   0.366847084655958251845837146485E+01;
  }
  else if ( n == 12 )
  {
    x[0] =  - 0.388972489786978191927164274724E+01;
    x[1] =  - 0.302063702512088977171067937518E+01;
    x[2] =  - 0.227950708050105990018772856942E+01;
    x[3] =  - 0.159768263515260479670966277090E+01;
    x[4] =  - 0.947788391240163743704578131060E+00;
    x[5] =  - 0.314240376254359111276611634095E+00;
    x[6] =    0.314240376254359111276611634095E+00;
    x[7] =    0.947788391240163743704578131060E+00;
    x[8] =    0.159768263515260479670966277090E+01;
    x[9] =    0.227950708050105990018772856942E+01;
    x[10] =   0.302063702512088977171067937518E+01;
    x[11] =   0.388972489786978191927164274724E+01;
  }
  else if ( n == 13 )
  {
    x[0] =  - 0.410133759617863964117891508007E+01;
    x[1] =  - 0.324660897837240998812205115236E+01;
    x[2] =  - 0.251973568567823788343040913628E+01;
    x[3] =  - 0.185310765160151214200350644316E+01;
    x[4] =  - 0.122005503659074842622205526637E+01;
    x[5] =  - 0.605763879171060113080537108602E+00;
    x[6] =    0.0E+00;
    x[7] =    0.605763879171060113080537108602E+00;
    x[8] =    0.122005503659074842622205526637E+01;
    x[9] =    0.185310765160151214200350644316E+01;
    x[10] =   0.251973568567823788343040913628E+01;
    x[11] =   0.324660897837240998812205115236E+01;
    x[12] =   0.410133759617863964117891508007E+01;
  }
  else if ( n == 14 )
  {
    x[0] =  - 0.430444857047363181262129810037E+01;
    x[1] =  - 0.346265693360227055020891736115E+01;
    x[2] =  - 0.274847072498540256862499852415E+01;
    x[3] =  - 0.209518325850771681573497272630E+01;
    x[4] =  - 0.147668273114114087058350654421E+01;
    x[5] =  - 0.878713787329399416114679311861E+00;
    x[6] =  - 0.291745510672562078446113075799E+00;
    x[7] =    0.291745510672562078446113075799E+00;
    x[8] =    0.878713787329399416114679311861E+00;
    x[9] =    0.147668273114114087058350654421E+01;
    x[10] =   0.209518325850771681573497272630E+01;
    x[11] =   0.274847072498540256862499852415E+01;
    x[12] =   0.346265693360227055020891736115E+01;
    x[13] =   0.430444857047363181262129810037E+01;
  }
  else if ( n == 15 )
  {
    x[0] =  - 0.449999070730939155366438053053E+01;
    x[1] =  - 0.366995037340445253472922383312E+01;
    x[2] =  - 0.296716692790560324848896036355E+01;
    x[3] =  - 0.232573248617385774545404479449E+01;
    x[4] =  - 0.171999257518648893241583152515E+01;
    x[5] =  - 0.113611558521092066631913490556E+01;
    x[6] =  - 0.565069583255575748526020337198E+00;
    x[7] =    0.0E+00;
    x[8] =    0.565069583255575748526020337198E+00;
    x[9] =    0.113611558521092066631913490556E+01;
    x[10] =   0.171999257518648893241583152515E+01;
    x[11] =   0.232573248617385774545404479449E+01;
    x[12] =   0.296716692790560324848896036355E+01;
    x[13] =   0.366995037340445253472922383312E+01;
    x[14] =   0.449999070730939155366438053053E+01;
  }
  else if ( n == 16 )
  {
    x[0] =  - 0.468873893930581836468849864875E+01;
    x[1] =  - 0.386944790486012269871942409801E+01;
    x[2] =  - 0.317699916197995602681399455926E+01;
    x[3] =  - 0.254620215784748136215932870545E+01;
    x[4] =  - 0.195178799091625397743465541496E+01;
    x[5] =  - 0.138025853919888079637208966969E+01;
    x[6] =  - 0.822951449144655892582454496734E+00;
    x[7] =  - 0.273481046138152452158280401965E+00;
    x[8] =    0.273481046138152452158280401965E+00;
    x[9] =    0.822951449144655892582454496734E+00;
    x[10] =   0.138025853919888079637208966969E+01;
    x[11] =   0.195178799091625397743465541496E+01;
    x[12] =   0.254620215784748136215932870545E+01;
    x[13] =   0.317699916197995602681399455926E+01;
    x[14] =   0.386944790486012269871942409801E+01;
    x[15] =   0.468873893930581836468849864875E+01;
  }
  else if ( n == 17 )
  {
    x[0] =  - 0.487134519367440308834927655662E+01;
    x[1] =  - 0.406194667587547430689245559698E+01;
    x[2] =  - 0.337893209114149408338327069289E+01;
    x[3] =  - 0.275776291570388873092640349574E+01;
    x[4] =  - 0.217350282666662081927537907149E+01;
    x[5] =  - 0.161292431422123133311288254454E+01;
    x[6] =  - 0.106764872574345055363045773799E+01;
    x[7] =  - 0.531633001342654731349086553718E+00;
    x[8] =    0.0E+00;
    x[9] =    0.531633001342654731349086553718E+00;
    x[10] =   0.106764872574345055363045773799E+01;
    x[11] =   0.161292431422123133311288254454E+01;
    x[12] =   0.217350282666662081927537907149E+01;
    x[13] =   0.275776291570388873092640349574E+01;
    x[14] =   0.337893209114149408338327069289E+01;
    x[15] =   0.406194667587547430689245559698E+01;
    x[16] =   0.487134519367440308834927655662E+01;
  }
  else if ( n == 18 )
  {
    x[0] =  - 0.504836400887446676837203757885E+01;
    x[1] =  - 0.424811787356812646302342016090E+01;
    x[2] =  - 0.357376906848626607950067599377E+01;
    x[3] =  - 0.296137750553160684477863254906E+01;
    x[4] =  - 0.238629908916668600026459301424E+01;
    x[5] =  - 0.183553160426162889225383944409E+01;
    x[6] =  - 0.130092085838961736566626555439E+01;
    x[7] =  - 0.776682919267411661316659462284E+00;
    x[8] =  - 0.258267750519096759258116098711E+00;
    x[9] =    0.258267750519096759258116098711E+00;
    x[10] =   0.776682919267411661316659462284E+00;
    x[11] =   0.130092085838961736566626555439E+01;
    x[12] =   0.183553160426162889225383944409E+01;
    x[13] =   0.238629908916668600026459301424E+01;
    x[14] =   0.296137750553160684477863254906E+01;
    x[15] =   0.357376906848626607950067599377E+01;
    x[16] =   0.424811787356812646302342016090E+01;
    x[17] =   0.504836400887446676837203757885E+01;
  }
  else if ( n == 19 )
  {
    x[0] =  - 0.522027169053748216460967142500E+01;
    x[1] =  - 0.442853280660377943723498532226E+01;
    x[2] =  - 0.376218735196402009751489394104E+01;
    x[3] =  - 0.315784881834760228184318034120E+01;
    x[4] =  - 0.259113378979454256492128084112E+01;
    x[5] =  - 0.204923170985061937575050838669E+01;
    x[6] =  - 0.152417061939353303183354859367E+01;
    x[7] =  - 0.101036838713431135136859873726E+01;
    x[8] =  - 0.503520163423888209373811765050E+00;
    x[9] =    0.0E+00;
    x[10] =   0.503520163423888209373811765050E+00;
    x[11] =   0.101036838713431135136859873726E+01;
    x[12] =   0.152417061939353303183354859367E+01;
    x[13] =   0.204923170985061937575050838669E+01;
    x[14] =   0.259113378979454256492128084112E+01;
    x[15] =   0.315784881834760228184318034120E+01;
    x[16] =   0.376218735196402009751489394104E+01;
    x[17] =   0.442853280660377943723498532226E+01;
    x[18] =   0.522027169053748216460967142500E+01;
  }
  else if ( n == 20 )
  {
    x[0] =  - 0.538748089001123286201690041068E+01;
    x[1] =  - 0.460368244955074427307767524898E+01;
    x[2] =  - 0.394476404011562521037562880052E+01;
    x[3] =  - 0.334785456738321632691492452300E+01;
    x[4] =  - 0.278880605842813048052503375640E+01;
    x[5] =  - 0.225497400208927552308233334473E+01;
    x[6] =  - 0.173853771211658620678086566214E+01;
    x[7] =  - 0.123407621539532300788581834696E+01;
    x[8] =  - 0.737473728545394358705605144252E+00;
    x[9] =  - 0.245340708300901249903836530634E+00;
    x[10] =   0.245340708300901249903836530634E+00;
    x[11] =   0.737473728545394358705605144252E+00;
    x[12] =   0.123407621539532300788581834696E+01;
    x[13] =   0.173853771211658620678086566214E+01;
    x[14] =   0.225497400208927552308233334473E+01;
    x[15] =   0.278880605842813048052503375640E+01;
    x[16] =   0.334785456738321632691492452300E+01;
    x[17] =   0.394476404011562521037562880052E+01;
    x[18] =   0.460368244955074427307767524898E+01;
    x[19] =   0.538748089001123286201690041068E+01;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "HERMITE_LOOKUP_POINTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::cerr << "  Legal values are 1 through 20.\n";
    std::exit ( 1 );
  }

  return;
}
//**************************************************************************80

void SandiaRules::hermite_lookup_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_LOOKUP_WEIGHTS looks up weights for Hermite quadrature.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -oo < x < +oo ) exp ( - x * x ) * f(x) dx
//
//    The quadrature rule:
//
//      sum ( 1 <= i <= n ) w(i) * f ( x(i) ).
//
//    Mathematica can numerically estimate the abscissas
//    of order N to P digits by the command:
//
//      NSolve [ HermiteH [ n, x ] == 0, x, p ]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798,
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3,
//    LC: QA47.M315.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 20.
//
//    Output, double W[N], the weights.
//
{
  if ( n == 1 )
  {
    w[0] = 1.77245385090551602729816748334;
  }
  else if ( n == 2 )
  {
    w[0] = 0.886226925452758013649083741671E+00;
    w[1] = 0.886226925452758013649083741671E+00;
  }
  else if ( n == 3 )
  {
    w[0] = 0.295408975150919337883027913890E+00;
    w[1] = 0.118163590060367735153211165556E+01;
    w[2] = 0.295408975150919337883027913890E+00;
  }
  else if ( n == 4 )
  {
    w[0] = 0.813128354472451771430345571899E-01;
    w[1] = 0.804914090005512836506049184481E+00;
    w[2] = 0.804914090005512836506049184481E+00;
    w[3] = 0.813128354472451771430345571899E-01;
  }
  else if ( n == 5 )
  {
    w[0] = 0.199532420590459132077434585942E-01;
    w[1] = 0.393619323152241159828495620852E+00;
    w[2] = 0.945308720482941881225689324449E+00;
    w[3] = 0.393619323152241159828495620852E+00;
    w[4] = 0.199532420590459132077434585942E-01;
  }
  else if ( n == 6 )
  {
    w[0] = 0.453000990550884564085747256463E-02;
    w[1] = 0.157067320322856643916311563508E+00;
    w[2] = 0.724629595224392524091914705598E+00;
    w[3] = 0.724629595224392524091914705598E+00;
    w[4] = 0.157067320322856643916311563508E+00;
    w[5] = 0.453000990550884564085747256463E-02;
  }
  else if ( n == 7 )
  {
    w[0] = 0.971781245099519154149424255939E-03;
    w[1] = 0.545155828191270305921785688417E-01;
    w[2] = 0.425607252610127800520317466666E+00;
    w[3] = 0.810264617556807326764876563813E+00;
    w[4] = 0.425607252610127800520317466666E+00;
    w[5] = 0.545155828191270305921785688417E-01;
    w[6] = 0.971781245099519154149424255939E-03;
  }
  else if ( n == 8 )
  {
    w[0] = 0.199604072211367619206090452544E-03;
    w[1] = 0.170779830074134754562030564364E-01;
    w[2] = 0.207802325814891879543258620286E+00;
    w[3] = 0.661147012558241291030415974496E+00;
    w[4] = 0.661147012558241291030415974496E+00;
    w[5] = 0.207802325814891879543258620286E+00;
    w[6] = 0.170779830074134754562030564364E-01;
    w[7] = 0.199604072211367619206090452544E-03;
  }
  else if ( n == 9 )
  {
    w[0] = 0.396069772632643819045862946425E-04;
    w[1] = 0.494362427553694721722456597763E-02;
    w[2] = 0.884745273943765732879751147476E-01;
    w[3] = 0.432651559002555750199812112956E+00;
    w[4] = 0.720235215606050957124334723389E+00;
    w[5] = 0.432651559002555750199812112956E+00;
    w[6] = 0.884745273943765732879751147476E-01;
    w[7] = 0.494362427553694721722456597763E-02;
    w[8] = 0.396069772632643819045862946425E-04;
  }
  else if ( n == 10 )
  {
    w[0] =  0.764043285523262062915936785960E-05;
    w[1] =  0.134364574678123269220156558585E-02;
    w[2] =  0.338743944554810631361647312776E-01;
    w[3] =  0.240138611082314686416523295006E+00;
    w[4] =  0.610862633735325798783564990433E+00;
    w[5] =  0.610862633735325798783564990433E+00;
    w[6] =  0.240138611082314686416523295006E+00;
    w[7] =  0.338743944554810631361647312776E-01;
    w[8] =  0.134364574678123269220156558585E-02;
    w[9] =  0.764043285523262062915936785960E-05;
  }
  else if ( n == 11 )
  {
    w[0] =  0.143956039371425822033088366032E-05;
    w[1] =  0.346819466323345510643413772940E-03;
    w[2] =  0.119113954449115324503874202916E-01;
    w[3] =  0.117227875167708503381788649308E+00;
    w[4] =  0.429359752356125028446073598601E+00;
    w[5] =  0.654759286914591779203940657627E+00;
    w[6] =  0.429359752356125028446073598601E+00;
    w[7] =  0.117227875167708503381788649308E+00;
    w[8] =  0.119113954449115324503874202916E-01;
    w[9] =  0.346819466323345510643413772940E-03;
    w[10] = 0.143956039371425822033088366032E-05;
  }
  else if ( n == 12 )
  {
    w[0] =  0.265855168435630160602311400877E-06;
    w[1] =  0.857368704358785865456906323153E-04;
    w[2] =  0.390539058462906185999438432620E-02;
    w[3] =  0.516079856158839299918734423606E-01;
    w[4] =  0.260492310264161129233396139765E+00;
    w[5] =  0.570135236262479578347113482275E+00;
    w[6] =  0.570135236262479578347113482275E+00;
    w[7] =  0.260492310264161129233396139765E+00;
    w[8] =  0.516079856158839299918734423606E-01;
    w[9] =  0.390539058462906185999438432620E-02;
    w[10] = 0.857368704358785865456906323153E-04;
    w[11] = 0.265855168435630160602311400877E-06;
  }
  else if ( n == 13 )
  {
    w[0] =  0.482573185007313108834997332342E-07;
    w[1] =  0.204303604027070731248669432937E-04;
    w[2] =  0.120745999271938594730924899224E-02;
    w[3] =  0.208627752961699392166033805050E-01;
    w[4] =  0.140323320687023437762792268873E+00;
    w[5] =  0.421616296898543221746893558568E+00;
    w[6] =  0.604393187921161642342099068579E+00;
    w[7] =  0.421616296898543221746893558568E+00;
    w[8] =  0.140323320687023437762792268873E+00;
    w[9] =  0.208627752961699392166033805050E-01;
    w[10] = 0.120745999271938594730924899224E-02;
    w[11] = 0.204303604027070731248669432937E-04;
    w[12] = 0.482573185007313108834997332342E-07;
  }
  else if ( n == 14 )
  {
    w[0] =  0.862859116812515794532041783429E-08;
    w[1] =  0.471648435501891674887688950105E-05;
    w[2] =  0.355092613551923610483661076691E-03;
    w[3] =  0.785005472645794431048644334608E-02;
    w[4] =  0.685055342234652055387163312367E-01;
    w[5] =  0.273105609064246603352569187026E+00;
    w[6] =  0.536405909712090149794921296776E+00;
    w[7] =  0.536405909712090149794921296776E+00;
    w[8] =  0.273105609064246603352569187026E+00;
    w[9] =  0.685055342234652055387163312367E-01;
    w[10] = 0.785005472645794431048644334608E-02;
    w[11] = 0.355092613551923610483661076691E-03;
    w[12] = 0.471648435501891674887688950105E-05;
    w[13] = 0.862859116812515794532041783429E-08;
  }
  else if ( n == 15 )
  {
    w[0] =  0.152247580425351702016062666965E-08;
    w[1] =  0.105911554771106663577520791055E-05;
    w[2] =  0.100004441232499868127296736177E-03;
    w[3] =  0.277806884291277589607887049229E-02;
    w[4] =  0.307800338725460822286814158758E-01;
    w[5] =  0.158488915795935746883839384960E+00;
    w[6] =  0.412028687498898627025891079568E+00;
    w[7] =  0.564100308726417532852625797340E+00;
    w[8] =  0.412028687498898627025891079568E+00;
    w[9] =  0.158488915795935746883839384960E+00;
    w[10] = 0.307800338725460822286814158758E-01;
    w[11] = 0.277806884291277589607887049229E-02;
    w[12] = 0.100004441232499868127296736177E-03;
    w[13] = 0.105911554771106663577520791055E-05;
    w[14] = 0.152247580425351702016062666965E-08;
  }
  else if ( n == 16 )
  {
    w[0] =  0.265480747401118224470926366050E-09;
    w[1] =  0.232098084486521065338749423185E-06;
    w[2] =  0.271186009253788151201891432244E-04;
    w[3] =  0.932284008624180529914277305537E-03;
    w[4] =  0.128803115355099736834642999312E-01;
    w[5] =  0.838100413989858294154207349001E-01;
    w[6] =  0.280647458528533675369463335380E+00;
    w[7] =  0.507929479016613741913517341791E+00;
    w[8] =  0.507929479016613741913517341791E+00;
    w[9] =  0.280647458528533675369463335380E+00;
    w[10] = 0.838100413989858294154207349001E-01;
    w[11] = 0.128803115355099736834642999312E-01;
    w[12] = 0.932284008624180529914277305537E-03;
    w[13] = 0.271186009253788151201891432244E-04;
    w[14] = 0.232098084486521065338749423185E-06;
    w[15] = 0.265480747401118224470926366050E-09;
  }
  else if ( n == 17 )
  {
    w[0] =  0.458057893079863330580889281222E-10;
    w[1] =  0.497707898163079405227863353715E-07;
    w[2] =  0.711228914002130958353327376218E-05;
    w[3] =  0.298643286697753041151336643059E-03;
    w[4] =  0.506734995762753791170069495879E-02;
    w[5] =  0.409200341495762798094994877854E-01;
    w[6] =  0.172648297670097079217645196219E+00;
    w[7] =  0.401826469470411956577635085257E+00;
    w[8] =  0.530917937624863560331883103379E+00;
    w[9] =  0.401826469470411956577635085257E+00;
    w[10] = 0.172648297670097079217645196219E+00;
    w[11] = 0.409200341495762798094994877854E-01;
    w[12] = 0.506734995762753791170069495879E-02;
    w[13] = 0.298643286697753041151336643059E-03;
    w[14] = 0.711228914002130958353327376218E-05;
    w[15] = 0.497707898163079405227863353715E-07;
    w[16] = 0.458057893079863330580889281222E-10;
  }
  else if ( n == 18 )
  {
    w[0] =  0.782819977211589102925147471012E-11;
    w[1] =  0.104672057957920824443559608435E-07;
    w[2] =  0.181065448109343040959702385911E-05;
    w[3] =  0.918112686792940352914675407371E-04;
    w[4] =  0.188852263026841789438175325426E-02;
    w[5] =  0.186400423875446519219315221973E-01;
    w[6] =  0.973017476413154293308537234155E-01;
    w[7] =  0.284807285669979578595606820713E+00;
    w[8] =  0.483495694725455552876410522141E+00;
    w[9] =  0.483495694725455552876410522141E+00;
    w[10] = 0.284807285669979578595606820713E+00;
    w[11] = 0.973017476413154293308537234155E-01;
    w[12] = 0.186400423875446519219315221973E-01;
    w[13] = 0.188852263026841789438175325426E-02;
    w[14] = 0.918112686792940352914675407371E-04;
    w[15] = 0.181065448109343040959702385911E-05;
    w[16] = 0.104672057957920824443559608435E-07;
    w[17] = 0.782819977211589102925147471012E-11;
  }
  else if ( n == 19 )
  {
    w[0] =  0.132629709449851575185289154385E-11;
    w[1] =  0.216305100986355475019693077221E-08;
    w[2] =  0.448824314722312295179447915594E-06;
    w[3] =  0.272091977631616257711941025214E-04;
    w[4] =  0.670877521407181106194696282100E-03;
    w[5] =  0.798886677772299020922211491861E-02;
    w[6] =  0.508103869090520673569908110358E-01;
    w[7] =  0.183632701306997074156148485766E+00;
    w[8] =  0.391608988613030244504042313621E+00;
    w[9] =  0.502974888276186530840731361096E+00;
    w[10] = 0.391608988613030244504042313621E+00;
    w[11] = 0.183632701306997074156148485766E+00;
    w[12] = 0.508103869090520673569908110358E-01;
    w[13] = 0.798886677772299020922211491861E-02;
    w[14] = 0.670877521407181106194696282100E-03;
    w[15] = 0.272091977631616257711941025214E-04;
    w[16] = 0.448824314722312295179447915594E-06;
    w[17] = 0.216305100986355475019693077221E-08;
    w[18] = 0.132629709449851575185289154385E-11;
  }
  else if ( n == 20 )
  {
    w[0] =  0.222939364553415129252250061603E-12;
    w[1] =  0.439934099227318055362885145547E-09;
    w[2] =  0.108606937076928169399952456345E-06;
    w[3] =  0.780255647853206369414599199965E-05;
    w[4] =  0.228338636016353967257145917963E-03;
    w[5] =  0.324377334223786183218324713235E-02;
    w[6] =  0.248105208874636108821649525589E-01;
    w[7] =  0.109017206020023320013755033535E+00;
    w[8] =  0.286675505362834129719659706228E+00;
    w[9] =  0.462243669600610089650328639861E+00;
    w[10] = 0.462243669600610089650328639861E+00;
    w[11] = 0.286675505362834129719659706228E+00;
    w[12] = 0.109017206020023320013755033535E+00;
    w[13] = 0.248105208874636108821649525589E-01;
    w[14] = 0.324377334223786183218324713235E-02;
    w[15] = 0.228338636016353967257145917963E-03;
    w[16] = 0.780255647853206369414599199965E-05;
    w[17] = 0.108606937076928169399952456345E-06;
    w[18] = 0.439934099227318055362885145547E-09;
    w[19] = 0.222939364553415129252250061603E-12;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "HERMITE_LOOKUP_WEIGHTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::cerr << "  Legal values are 1 through 20.\n";
    std::exit ( 1 );
  }

  return;
}
//**************************************************************************80

void SandiaRules::hermite_ss_compute ( int order, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_SS_COMPUTE computes a Hermite quadrature rule.
//
//  Discussion:
//
//    The abscissas are the zeros of the N-th order Hermite polynomial.
//
//    The integral:
//
//      Integral ( -oo < X < +oo ) exp ( - X * X ) * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double cc;
  double dp2;
  int i;
  double p1;
  double s;
  double temp;
  double x0;

  if ( order < 1 )
  {
    std::cerr << "\n";
    std::cerr << "HERMITE_SS_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of ORDER = " << order << "\n";
    std::exit ( 1 );
  }

  cc = 1.7724538509 * r8_gamma ( ( double ) ( order ) )
    / std::pow ( 2.0, order - 1 );

  s = std::pow ( 2.0 * ( double ) ( order ) + 1.0, 1.0 / 6.0 );

  for ( i = 0; i < ( order + 1 ) / 2; i++ )
  {
    if ( i == 0 )
    {
      x0 = s * s * s - 1.85575 / s;
    }
    else if ( i == 1 )
    {
      x0 = x0 - 1.14 * std::pow ( ( double ) ( order ), 0.426 ) / x0;
    }
    else if ( i == 2 )
    {
      x0 = 1.86 * x0 - 0.86 * x[0];
    }
    else if ( i == 3 )
    {
      x0 = 1.91 * x0 - 0.91 * x[1];
    }
    else
    {
      x0 = 2.0 * x0 - x[i-2];
    }

    hermite_ss_root ( &x0, order, &dp2, &p1 );

    x[i] = x0;
    w[i] = ( cc / dp2 ) / p1;

    x[order-i-1] = -x0;
    w[order-i-1] = w[i];
  }
//
//  Reverse the order of the abscissas.
//
  for ( i = 1; i <= order/2; i++ )
  {
    temp       = x[i-1];
    x[i-1]     = x[order-i];
    x[order-i] = temp;
  }

  if ( ( order % 2 ) == 1 )
  {
    x[(order-1)/2] = 0.0;
  }

  return;
}
//**************************************************************************80

void SandiaRules::hermite_ss_recur ( double *p2, double *dp2, double *p1, double x, int order )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_SS_RECUR finds the value and derivative of a Hermite polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Output, double *P2, the value of H(ORDER)(X).
//
//    Output, double *DP2, the value of H'(ORDER)(X).
//
//    Output, double *P1, the value of H(ORDER-1)(X).
//
//    Input, double X, the point at which polynomials are evaluated.
//
//    Input, int ORDER, the order of the polynomial.
//
{
  int i;
  double dq0;
  double dq1;
  double dq2;
  double q0;
  double q1;
  double q2;

  q1 = 1.0;
  dq1 = 0.0;

  q2 = x;
  dq2 = 1.0;

  for ( i = 2; i <= order; i++ )
  {
    q0 = q1;
    dq0 = dq1;

    q1 = q2;
    dq1 = dq2;

    q2  = x * q1 - 0.5 * ( ( double ) ( i ) - 1.0 ) * q0;
    dq2 = x * dq1 + q1 - 0.5 * ( ( double ) ( i ) - 1.0 ) * dq0;
  }

  *p2 = q2;
  *dp2 = dq2;
  *p1 = q1;

  return;
}
//**************************************************************************80

void SandiaRules::hermite_ss_root ( double *x, int order, double *dp2, double *p1 )

//**************************************************************************80
//
//  Purpose:
//
//    HERMITE_SS_ROOT improves an approximate root of a Hermite polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input/output, double *X, the approximate root, which
//    should be improved on output.
//
//    Input, int ORDER, the order of the Hermite polynomial.
//
//    Output, double *DP2, the value of H'(ORDER)(X).
//
//    Output, double *P1, the value of H(ORDER-1)(X).
//
{
  double d;
  double eps;
  double p2;
  int step;
  int step_max = 10;

  eps = r8_epsilon ( );

  for ( step = 1; step <= step_max; step++ )
  {
    hermite_ss_recur ( &p2, dp2, p1, *x, order );

    d = p2 / ( *dp2 );
    *x = *x - d;

    if ( r8_abs ( d ) <= eps * ( r8_abs ( *x ) + 1.0 ) )
    {
      return;
    }
  }
  return;
}
//**************************************************************************80

int SandiaRules::i4_choose ( int n, int k )

//**************************************************************************80
//
//  Purpose:
//
//    I4_CHOOSE computes the binomial coefficient C(N,K).
//
//  Discussion:
//
//    The value is calculated in such a way as to avoid SandiaRules::overflow and
//    roundoff.  The calculation is done in integer arithmetic.
//
//    The formula used is:
//
//      C(N,K) = N! / ( K! * (N-K)! )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    09 November 2007
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    ML Wolfson, HV Wright,
//    Algorithm 160:
//    Combinatorial of M Things Taken N at a Time,
//    Communications of the ACM,
//    Volume 6, Number 4, April 1963, page 161.
//
//  Parameters:
//
//    Input, int N, K, the values of N and K.
//
//    Output, int I4_CHOOSE, the number of combinations of N
//    things taken K at a time.
//
{
  int i;
  int mn;
  int mx;
  int value;

  mn = i4_min ( k, n - k );

  if ( mn < 0 )
  {
    value = 0;
  }
  else if ( mn == 0 )
  {
    value = 1;
  }
  else
  {
    mx = i4_max ( k, n - k );
    value = mx + 1;

    for ( i = 2; i <= mn; i++ )
    {
      value = ( value * ( mx + i ) ) / i;
    }
  }

  return value;
}
//**************************************************************************80

int SandiaRules::i4_log_2 ( int i )

//**************************************************************************80
//
//  Purpose:
//
//    I4_LOG_2 returns the integer part of the logarithm base 2 of an I4.
//
//  Example:
//
//        I  I4_LOG_10
//    -----  --------
//        0    0
//        1    0
//        2    1
//        3    1
//        4    2
//        5    2
//        7    2
//        8    3
//        9    3
//     1000    9
//     1024   10
//
//  Discussion:
//
//    I4_LOG_2 ( I ) + 1 is the number of binary digits in I.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 January 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the number whose logarithm base 2 is desired.
//
//    Output, int I4_LOG_2, the integer part of the logarithm base 2 of
//    the absolute value of X.
//
{
  int i_abs;
  int two_pow;
  int value;

  if ( i == 0 )
  {
    value = 0;
  }
  else
  {
    value = 0;
    two_pow = 2;

    i_abs = std::abs ( i );

    while ( two_pow <= i_abs )
    {
      value = value + 1;
      two_pow = two_pow * 2;
    }
  }

  return value;
}
//**************************************************************************80

int SandiaRules::i4_max ( int i1, int i2 )

//**************************************************************************80
//
//  Purpose:
//
//    I4_MAX returns the maximum of two I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 October 1998
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I1, I2, are two integers to be compared.
//
//    Output, int I4_MAX, the larger of I1 and I2.
//
{
  int value;

  if ( i2 < i1 )
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}
//**************************************************************************80

int SandiaRules::i4_min ( int i1, int i2 )

//**************************************************************************80
//
//  Purpose:
//
//    I4_MIN returns the minimum of two I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 October 1998
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I1, I2, two integers to be compared.
//
//    Output, int I4_MIN, the smaller of I1 and I2.
//
{
  int value;

  if ( i1 < i2 )
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}
//**************************************************************************80

int SandiaRules::i4_power ( int i, int j )

//**************************************************************************80
//
//  Purpose:
//
//    I4_POWER returns the value of I^J.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, J, the base and the power.  J should be nonnegative.
//
//    Output, int I4_POWER, the value of I^J.
//
{
  int k;
  int value;

  if ( j < 0 )
  {
    if ( i == 1 )
    {
      value = 1;
    }
    else if ( i == 0 )
    {
      std::cerr << "\n";
      std::cerr << "I4_POWER - Fatal error!\n";
      std::cerr << "  I^J requested, with I = 0 and J negative.\n";
      std::exit ( 1 );
    }
    else
    {
      value = 0;
    }
  }
  else if ( j == 0 )
  {
    if ( i == 0 )
    {
      std::cerr << "\n";
      std::cerr << "I4_POWER - Fatal error!\n";
      std::cerr << "  I^J requested, with I = 0 and J = 0.\n";
      std::exit ( 1 );
    }
    else
    {
      value = 1;
    }
  }
  else if ( j == 1 )
  {
    value = i;
  }
  else
  {
    value = 1;
    for ( k = 1; k <= j; k++ )
    {
      value = value * i;
    }
  }
  return value;
}
//**************************************************************************80

void SandiaRules::i4mat_copy ( int m, int n, int a1[], int a2[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4MAT_COPY copies one I4MAT to another.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A1[M*N], the matrix to be copied.
//
//    Output, int A2[M*N], the copy of A1.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }
  return;
}
//**************************************************************************80

int *SandiaRules::i4mat_copy_new ( int m, int n, int a1[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4MAT_COPY_NEW copies an I4MAT to a "new" I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A1[M*N], the matrix to be copied.
//
//    Output, int I4MAT_COPY_NEW[M*N], the copy of A1.
//
{
  int *a2;
  int i;
  int j;

  a2 = new int[m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }
  return a2;
}
//**************************************************************************80

void SandiaRules::i4mat_transpose_print ( int m, int n, int a[], std::string title )

//**************************************************************************80
//
//  Purpose:
//
//    I4MAT_TRANSPOSE_PRINT prints an I4MAT, transposed.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    31 January 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, int A[M*N], the M by N matrix.
//
//    Input, string TITLE, a title.
//
{
  i4mat_transpose_print_some ( m, n, a, 1, 1, m, n, title );

  return;
}
//**************************************************************************80

void SandiaRules::i4mat_transpose_print_some ( int m, int n, int a[], int ilo, int jlo,
  int ihi, int jhi, std::string title )

//**************************************************************************80
//
//  Purpose:
//
//    I4MAT_TRANSPOSE_PRINT_SOME prints some of an I4MAT, transposed.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 June 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of the matrix.
//    M must be positive.
//
//    Input, int N, the number of columns of the matrix.
//    N must be positive.
//
//    Input, int A[M*N], the matrix.
//
//    Input, int ILO, JLO, IHI, JHI, designate the first row and
//    column, and the last row and column to be printed.
//
//    Input, string TITLE, a title.
//
{
# define INCX 10

  int i;
  int i2hi;
  int i2lo;
  int j;
  int j2hi;
  int j2lo;

  std::cout << "\n";
  std::cout << title << "\n";
//
//  Print the columns of the matrix, in strips of INCX.
//
  for ( i2lo = ilo; i2lo <= ihi; i2lo = i2lo + INCX )
  {
    i2hi = i2lo + INCX - 1;
    i2hi = i4_min ( i2hi, m );
    i2hi = i4_min ( i2hi, ihi );

    std::cout << "\n";
//
//  For each row I in the current range...
//
//  Write the header.
//
    std::cout << "  Row: ";
    for ( i = i2lo; i <= i2hi; i++ )
    {
      std::cout << std::setw(6) << i - 1 << "  ";
    }
    std::cout << "\n";
    std::cout << "  Col\n";
    std::cout << "\n";
//
//  Determine the range of the rows in this strip.
//
    j2lo = i4_max ( jlo, 1 );
    j2hi = i4_min ( jhi, n );

    for ( j = j2lo; j <= j2hi; j++ )
    {
//
//  Print out (up to INCX) entries in column J, that lie in the current strip.
//
      std::cout << std::setw(5) << j - 1 << ":";
      for ( i = i2lo; i <= i2hi; i++ )
      {
        std::cout << std::setw(6) << a[i-1+(j-1)*m] << "  ";
      }
      std::cout << "\n";
    }
  }

  return;
# undef INCX
}
//**************************************************************************80

void SandiaRules::i4mat_write ( std::string output_filename, int m, int n, int table[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4MAT_WRITE writes an I4MAT file.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, string OUTPUT_FILENAME, the output filename.
//
//    Input, int M, the spatial dimension.
//
//    Input, int N, the number of points.
//
//    Input, int TABLE[M*N], the table data.
//
{
  int i;
  int j;
  std::ofstream output;
//
//  Open the file.
//
  output.open ( output_filename.c_str ( ) );

  if ( !output )
  {
    std::cerr << "\n";
    std::cerr << "I4MAT_WRITE - Fatal error!\n";
    std::cerr << "  Could not open the output file.\n";
    return;
  }
//
//  Write the data.
//
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      output << std::setw(10) << table[i+j*m] << "  ";
    }
    output << "\n";
  }
//
//  Close the file.
//
  output.close ( );

  return;
}
//**************************************************************************80

int *SandiaRules::i4vec_add_new ( int n, int a[], int b[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_ADD_NEW computes C = A + B for I4VEC's.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries.
//
//    Input, int A[N], the first vector.
//
//    Input, int B[N], the second vector.
//
//    Output, int I4VEC_ADD_NEW[N], the sum of the vectors.
//
{
  int *c;
  int i;

  c = new int[n];

  for ( i = 0; i < n; i++ )
  {
    c[i] = a[i] + b[i];
  }
  return c;
}
//**************************************************************************80

bool i4vec_any_lt ( int n, int a[], int b[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_ANY_LT: ( any ( A < B ) ) for I4VEC's.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries.
//
//    Input, int A[N], the first vector.
//
//    Input, int B[N], the second vector.
//
//    Output, bool I4VEC_ANY_LT is TRUE if any entry
//    of A is less than the corresponding entry of B.
//
{
  int i;
  bool value;

  for ( i = 0; i < n; i++ )
  {
    if ( a[i] < b[i] )
    {
      value = true;
      return value;
    }
  }
  value = false;

  return value;
}
//**************************************************************************80

void SandiaRules::i4vec_copy ( int n, int a1[], int a2[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_COPY copies an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, int A1[N], the vector to be copied.
//
//    Output, int A2[N], the copy of A1.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return;
}
//**************************************************************************80

int *SandiaRules::i4vec_copy_new ( int n, int a1[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_COPY_NEW copies an I4VEC to a "new" I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, int A1[N], the vector to be copied.
//
//    Output, int I4VEC_COPY_NEW[N], the copy of A1.
//
{
  int *a2;
  int i;

  a2 = new int[n];

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return a2;
}
//**************************************************************************80

void SandiaRules::i4vec_min_mv ( int m, int n, int u[], int v[], int w[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_MIN_MV determines U(1:N) /\ V for vectors U and a single vector V.
//
//  Discussion:
//
//    For two vectors U and V, each of length M, we define
//
//      ( U /\ V ) (I) = min ( U(I), V(I) ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    12 January 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the dimension of the vectors.
//
//    Input, int N, the number of vectors in U.
//
//    Input, int U[M*N], N vectors, each of length M.
//
//    Input, int V[M], a vector of length M.
//
//    Output, int W[M*N], the value of U /\ W.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      w[i+j*m] = i4_min ( u[i+j*m], v[i] );
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::i4vec_print ( int n, int a[], std::string title )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_PRINT prints an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 November 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, int A[N], the vector to be printed.
//
//    Input, string TITLE, a title.
//
{
  int i;

  std::cout << "\n";
  std::cout << title << "\n";
  std::cout << "\n";
  for ( i = 0; i < n; i++ )
  {
    std::cout << "  " << std::setw(8) << i
              << ": " << std::setw(8) << a[i]  << "\n";
  }
  return;
}
//**************************************************************************80

int SandiaRules::i4vec_product ( int n, int a[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_PRODUCT multiplies the entries of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of integer values.
//
//  Example:
//
//    Input:
//
//      A = ( 1, 2, 3, 4 )
//
//    Output:
//
//      I4VEC_PRODUCT = 24
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector
//
//    Output, int I4VEC_PRODUCT, the product of the entries of A.
//
{
  int i;
  int product;

  product = 1;
  for ( i = 0; i < n; i++ )
  {
    product = product * a[i];
  }

  return product;
}
//**************************************************************************80

int SandiaRules::i4vec_sum ( int n, int a[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_SUM sums the entries of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      A = ( 1, 2, 3, 4 )
//
//    Output:
//
//      I4VEC_SUM = 10
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector to be summed.
//
//    Output, int I4VEC_SUM, the sum of the entries of A.
//
{
  int i;
  int sum;

  sum = 0;
  for ( i = 0; i < n; i++ )
  {
    sum = sum + a[i];
  }

  return sum;
}
//**************************************************************************80

void SandiaRules::i4vec_zero ( int n, int a[] )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_ZERO zeroes an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Output, int A[N], a vector of zeroes.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i] = 0;
  }
  return;
}
//**************************************************************************80

int *SandiaRules::i4vec_zero_new ( int n )

//**************************************************************************80
//
//  Purpose:
//
//    I4VEC_ZERO_NEW creates and zeroes an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Output, int I4VEC_ZERO_NEW[N], a vector of zeroes.
//
{
  int *a;
  int i;

  a = new int[n];

  for ( i = 0; i < n; i++ )
  {
    a[i] = 0;
  }
  return a;
}
//**************************************************************************80

void SandiaRules::imtqlx ( int n, double d[], double e[], double z[] )

//**************************************************************************80
//
//  Purpose:
//
//    IMTQLX diagonalizes a symmetric tridiagonal matrix.
//
//  Discussion:
//
//    This routine is a slightly modified version of the EISPACK routine to 
//    perform the implicit QL algorithm on a symmetric tridiagonal matrix. 
//
//    The authors thank the authors of EISPACK for permission to use this
//    routine. 
//
//    It has been modified to produce the product Q' * Z, where Z is an input 
//    vector and Q is the orthogonal matrix diagonalizing the input matrix.  
//    The changes consist (essentially) of applying the orthogonal transformations
//    directly to Z as they are generated.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 January 2010
//
//  Author:
//
//    Original FORTRAN77 version by Sylvan Elhay, Jaroslav Kautsky.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Sylvan Elhay, Jaroslav Kautsky,
//    Algorithm 655: IQPACK, FORTRAN Subroutines for the Weights of 
//    Interpolatory Quadrature,
//    ACM Transactions on Mathematical Software,
//    Volume 13, Number 4, December 1987, pages 399-415.
//
//    Roger Martin, James Wilkinson,
//    The Implicit QL Algorithm,
//    Numerische Mathematik,
//    Volume 12, Number 5, December 1968, pages 377-383.
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//
//    Input/output, double D(N), the diagonal entries of the matrix.
//    On output, the information in D has been overwritten.
//
//    Input/output, double E(N), the subdiagonal entries of the 
//    matrix, in entries E(1) through E(N-1).  On output, the information in
//    E has been overwritten.
//
//    Input/output, double Z(N).  On input, a vector.  On output,
//    the value of Q' * Z, where Q is the matrix that diagonalizes the
//    input symmetric tridiagonal matrix.
//
{
  double b;
  double c;
  double f;
  double g;
  int i;
  int ii;
  int itn = 30;
  int j;
  int k;
  int l;
  int m;
  int mml;
  double p;
  double prec;
  double r;
  double s;

  prec = r8_epsilon ( );

  if ( n == 1 )
  {
    return;
  }

  e[n-1] = 0.0;

  for ( l = 1; l <= n; l++ )
  {
    j = 0;
    for ( ; ; )
    {
      for ( m = l; m <= n; m++ )
      {
        if ( m == n )
        {
          break;
        }

        if ( r8_abs ( e[m-1] ) <= 
          prec * ( r8_abs ( d[m-1] ) + r8_abs ( d[m] ) ) )
        {
          break;
        }
      }
      p = d[l-1];
      if ( m == l )
      {
        break;
      }
      if ( itn <= j )
      {
        std::cerr << "\n";
        std::cerr << "IMTQLX - Fatal error!\n";
        std::cerr << "  Iteration limit exceeded\n";
        std::exit ( 1 );
      }
      j = j + 1;
      g = ( d[l] - p ) / ( 2.0 * e[l-1] );
      r = std::sqrt ( g * g + 1.0 );
      g = d[m-1] - p + e[l-1] / ( g + r8_abs ( r ) * r8_sign ( g ) );
      s = 1.0;
      c = 1.0;
      p = 0.0;
      mml = m - l;

      for ( ii = 1; ii <= mml; ii++ )
      {
        i = m - ii;
        f = s * e[i-1];
        b = c * e[i-1];

        if ( r8_abs ( g ) <= r8_abs ( f ) )
        {
          c = g / f;
          r = std::sqrt ( c * c + 1.0 );
          e[i] = f * r;
          s = 1.0 / r;
          c = c * s;
        }
        else
        {
          s = f / g;
          r = std::sqrt ( s * s + 1.0 );
          e[i] = g * r;
          c = 1.0 / r;
          s = s * c;
        }
        g = d[i] - p;
        r = ( d[i-1] - g ) * s + 2.0 * c * b;
        p = s * r;
        d[i] = g + p;
        g = c * r - b;
        f = z[i];
        z[i] = s * z[i-1] + c * f;
        z[i-1] = c * z[i-1] - s * f;
      }
      d[l-1] = d[l-1] - p;
      e[l-1] = g;
      e[m-1] = 0.0;
    }
  }
//
//  Sorting.
//
  for ( ii = 2; ii <= m; ii++ )
  {
    i = ii - 1;
    k = i;
    p = d[i-1];

    for ( j = ii; j <= n; j++ )
    {
      if ( d[j-1] < p )
      {
         k = j;
         p = d[j-1];
      }
    }

    if ( k != i )
    {
      d[k-1] = d[i-1];
      d[i-1] = p;
      p = z[i-1];
      z[i-1] = z[k-1];
      z[k-1] = p;
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::jacobi_compute ( int n, double alpha, double beta, double x[], 
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_COMPUTE: Elhay-Kautsky method for Gauss-Jacobi quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) (1-X)**ALPHA * (1+X)**BETA * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) WEIGHT(I) * F ( XTAB(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Sylvan Elhay, Jaroslav Kautsky.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Sylvan Elhay, Jaroslav Kautsky,
//    Algorithm 655: IQPACK, FORTRAN Subroutines for the Weights of
//    Interpolatory Quadrature,
//    ACM Transactions on Mathematical Software,
//    Volume 13, Number 4, December 1987, pages 399-415.
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, double ALPHA, BETA, the exponents of (1-X) and
//    (1+X) in the quadrature rule.  For simple Gauss-Legendre quadrature,
//    set ALPHA = BETA = 0.0.  -1.0 < ALPHA and -1.0 < BETA are required.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double abi;
  double *bj;
  int i;
  double i_r8;
  double zemu;
//
//  Define the zero-th moment.
//
  zemu = std::pow ( 2.0, alpha + beta + 1.0 )
    * r8_gamma ( alpha + 1.0 ) 
    * r8_gamma ( beta + 1.0 ) 
    / r8_gamma ( 2.0 + alpha + beta );
//
//  Define the Jacobi matrix.
//
  bj = new double[n];

  x[0] = ( beta - alpha ) / ( 2.0 + alpha + beta );

  bj[0] = 4.0 * ( 1.0 + alpha ) * ( 1.0 + beta ) 
    / ( ( 3.0 + alpha + beta ) 
      * ( 2.0 + alpha + beta ) * ( 2.0 + alpha + beta ) );

  for ( i = 1; i < n; i++ )
  {
    i_r8 = ( double ) ( i + 1 );
    abi = 2.0 * i_r8 + alpha + beta;
    x[i] = ( beta + alpha ) * ( beta - alpha ) / ( ( abi - 2.0 ) * abi );
    bj[i] = 4.0 * i_r8 * ( i_r8 + alpha ) * ( i_r8 + beta ) 
      * ( i_r8 + alpha + beta ) 
      / ( ( abi - 1.0 ) * ( abi + 1.0 ) * abi * abi );
  }

  for ( i = 0; i < n; i++ )
  {
    bj[i] = std::sqrt ( bj[i] );
  }

  w[0] = std::sqrt ( zemu );

  for ( i = 1; i < n; i++ )
  {
    w[i] = 0.0;
  }
//
//  Diagonalize the Jacobi matrix.
//
  imtqlx ( n, x, bj, w );

  for ( i = 0; i < n; i++ )
  {
    w[i] = w[i] * w[i];
  }

  delete [] bj;

  return;
}
//**************************************************************************80

void SandiaRules::jacobi_compute_np ( int order, int np, double p[], double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_COMPUTE_NP computes a Jacobi quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) (1-X)^ALPHA * (1+X)^BETA * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//    Thanks to Xu Xiang of Fudan University for pointing out that
//    an earlier implementation of this routine was incorrect!
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameter values.
//    P[0] = ALPHA, the exponent of (1-X)
//    P[1] = BETA,  the exponent of (1+X).
//    -1.0 < ALPHA and -1.0 < BETA are required.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha;
  double beta;

  alpha = p[0];
  beta = p[1];

  jacobi_compute ( order, alpha, beta, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::jacobi_compute_points ( int order, double alpha, double beta,
  double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_COMPUTE_POINTS computes Jacobi quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, double ALPHA, BETA, the exponents of the (1-X) and (1+X) factors.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double *w;

  w = new double[order];

  jacobi_compute ( order, alpha, beta, x, w );

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::jacobi_compute_points_np ( int order, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_COMPUTE_POINTS_NP computes Jacobi quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameter values.
//    P[0] = ALPHA, the exponent of (1-X)
//    P[1] = BETA,  the exponent of (1+X).
//    -1.0 < ALPHA and -1.0 < BETA are required.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double alpha;
  double beta;

  alpha = p[0];
  beta = p[1];

  jacobi_compute_points ( order, alpha, beta, x );

  return;
}
//**************************************************************************80

void SandiaRules::jacobi_compute_weights ( int order, double alpha, double beta,
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_COMPUTE_WEIGHTS computes Jacobi quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, double ALPHA, BETA, the exponents of the (1-X) and (1+X) factors.
//
//    Output, double W[ORDER], the weights.
//
{
  double *x;

  x = new double[order];

  jacobi_compute ( order, alpha, beta, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::jacobi_compute_weights_np ( int order, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_COMPUTE_WEIGHTS_NP computes Jacobi quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameter values.
//    P[0] = ALPHA, the exponent of (1-X)
//    P[1] = BETA,  the exponent of (1+X).
//    -1.0 < ALPHA and -1.0 < BETA are required.
//
//    Output, double W[ORDER], the weights.
//
{
  double alpha;
  double beta;

  alpha = p[0];
  beta = p[1];

  jacobi_compute_weights ( order, alpha, beta, w );

  return;
}
//**************************************************************************80

double SandiaRules::jacobi_integral ( int expon, double alpha, double beta )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_INTEGRAL integrates a monomial with Jacobi weight.
//
//  Discussion:
//
//    VALUE = Integral ( -1 <= X <= +1 ) x^EXPON (1-x)^ALPHA (1+x)^BETA dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 September 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int EXPON, the exponent.
//
//    Input, double ALPHA, the exponent of (1-X) in the weight factor.
//
//    Input, double BETA, the exponent of (1+X) in the weight factor.
//
//    Output, double JACOBI_INTEGRAL, the value of the integral.
//
{
  double arg1;
  double arg2;
  double arg3;
  double arg4;
  double c;
  double s;
  double value;
  double value1;
  double value2;

  c = ( double ) ( expon );

  if ( ( expon % 2 ) == 0 )
  {
    s = +1.0;
  }
  else
  {
    s = -1.0;
  }

  arg1 = - alpha;
  arg2 =   1.0 + c;
  arg3 =   2.0 + beta + c;
  arg4 = - 1.0;

  value1 = r8_hyper_2f1 ( arg1, arg2, arg3, arg4 );

  arg1 = - beta;
  arg2 =   1.0 + c;
  arg3 =   2.0 + alpha + c;
  arg4 = - 1.0;

  value2 = r8_hyper_2f1 ( arg1, arg2, arg3, arg4 );

  value = r8_gamma ( 1.0 + c ) * (
      s * r8_gamma ( 1.0 + beta  ) * value1
    / r8_gamma ( 2.0 + beta  + c )
    +     r8_gamma ( 1.0 + alpha ) * value2
    / r8_gamma ( 2.0 + alpha + c ) );

  return value;
}
//**************************************************************************80

void SandiaRules::jacobi_ss_compute ( int order, double alpha, double beta, double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_SS_COMPUTE computes a Jacobi quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) (1-X)^ALPHA * (1+X)^BETA * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//    Thanks to Xu Xiang of Fudan University for pointing out that
//    an earlier implementation of this routine was incorrect!
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, double ALPHA, BETA, the exponents of (1-X) and
//    (1+X) in the quadrature rule.  For simple Legendre quadrature,
//    set ALPHA = BETA = 0.0.  -1.0 < ALPHA and -1.0 < BETA are required.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double an;
  double *b;
  double bn;
  double *c;
  double cc;
  double delta;
  double dp2;
  int i;
  double p1;
  double prod;
  double r1;
  double r2;
  double r3;
  double temp;
  double x0;

  if ( order < 1 )
  {
    std::cerr << "\n";
    std::cerr << "JACOBI_SS_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of ORDER = " << order << "\n";
    std::exit ( 1 );
  }

  b = new double[order];
  c = new double[order];
//
//  Check ALPHA and BETA.
//
  if ( alpha <= -1.0 )
  {
    std::cerr << "\n";
    std::cerr << "JACOBI_SS_COMPUTE - Fatal error!\n";
    std::cerr << "  -1.0 < ALPHA is required.\n";
    std::exit ( 1 );
  }

  if ( beta <= -1.0 )
  {
    std::cerr << "\n";
    std::cerr << "JACOBI_SS_COMPUTE - Fatal error!\n";
    std::cerr << "  -1.0 < BETA is required.\n";
    std::exit ( 1 );
  }
//
//  Set the recursion coefficients.
//
  for ( i = 1; i <= order; i++ )
  {
    if ( alpha + beta == 0.0 || beta - alpha == 0.0 )
    {
      b[i-1] = 0.0;
    }
    else
    {
      b[i-1] = ( alpha + beta ) * ( beta - alpha ) /
             ( ( alpha + beta + ( double ) ( 2 * i ) )
             * ( alpha + beta + ( double ) ( 2 * i - 2 ) ) );
    }

    if ( i == 1 )
    {
      c[i-1] = 0.0;
    }
    else
    {
      c[i-1] = 4.0 * ( double ) ( i - 1 )
         * ( alpha + ( double ) ( i - 1 ) )
          * ( beta + ( double ) ( i - 1 ) )
            * ( alpha + beta + ( double ) ( i - 1 ) ) /
            ( ( alpha + beta + ( double ) ( 2 * i - 1 ) )
            * std::pow ( alpha + beta + ( double ) ( 2 * i - 2 ), 2 )
            * ( alpha + beta + ( double ) ( 2 * i - 3 ) ) );
    }
  }

  delta = r8_gamma ( alpha        + 1.0 )
        * r8_gamma (         beta + 1.0 )
        / r8_gamma ( alpha + beta + 2.0 );

  prod = 1.0;
  for ( i = 2; i <= order; i++ )
  {
    prod = prod * c[i-1];
  }
  cc = delta * std::pow ( 2.0, alpha + beta + 1.0 ) * prod;

  for ( i = 1; i <= order; i++ )
  {
    if ( i == 1 )
    {
      an = alpha / ( double ) ( order );
      bn = beta / ( double ) ( order );

      r1 = ( 1.0 + alpha )
        * ( 2.78 / ( 4.0 + ( double ) ( order * order ) )
        + 0.768 * an / ( double ) ( order ) );

      r2 = 1.0 + 1.48 * an + 0.96 * bn
        + 0.452 * an * an + 0.83 * an * bn;

      x0 = ( r2 - r1 ) / r2;
    }
    else if ( i == 2 )
    {
      r1 = ( 4.1 + alpha ) /
        ( ( 1.0 + alpha ) * ( 1.0 + 0.156 * alpha ) );

      r2 = 1.0 + 0.06 * ( ( double ) ( order ) - 8.0 ) *
        ( 1.0 + 0.12 * alpha ) / ( double ) ( order );

      r3 = 1.0 + 0.012 * beta *
        ( 1.0 + 0.25 * r8_abs ( alpha ) ) / ( double ) ( order );

      x0 = x0 - r1 * r2 * r3 * ( 1.0 - x0 );
    }
    else if ( i == 3 )
    {
      r1 = ( 1.67 + 0.28 * alpha ) / ( 1.0 + 0.37 * alpha );

      r2 = 1.0 + 0.22 * ( ( double ) ( order ) - 8.0 )
        / ( double ) ( order );

      r3 = 1.0 + 8.0 * beta /
        ( ( 6.28 + beta ) * ( double ) ( order * order ) );

      x0 = x0 - r1 * r2 * r3 * ( x[0] - x0 );
    }
    else if ( i < order - 1 )
    {
      x0 = 3.0 * x[i-2] - 3.0 * x[i-3] + x[i-4];
    }
    else if ( i == order - 1 )
    {
      r1 = ( 1.0 + 0.235 * beta ) / ( 0.766 + 0.119 * beta );

      r2 = 1.0 / ( 1.0 + 0.639
        * ( ( double ) ( order ) - 4.0 )
        / ( 1.0 + 0.71 * ( ( double ) ( order ) - 4.0 ) ) );

      r3 = 1.0 / ( 1.0 + 20.0 * alpha / ( ( 7.5 + alpha ) *
        ( double ) ( order * order ) ) );

      x0 = x0 + r1 * r2 * r3 * ( x0 - x[i-3] );
    }
    else if ( i == order )
    {
      r1 = ( 1.0 + 0.37 * beta ) / ( 1.67 + 0.28 * beta );

      r2 = 1.0 /
        ( 1.0 + 0.22 * ( ( double ) ( order ) - 8.0 )
        / ( double ) ( order ) );

      r3 = 1.0 / ( 1.0 + 8.0 * alpha /
        ( ( 6.28 + alpha ) * ( double ) ( order * order ) ) );

      x0 = x0 + r1 * r2 * r3 * ( x0 - x[i-3] );
    }

    jacobi_ss_root ( &x0, order, alpha, beta, &dp2, &p1, b, c );

    x[i-1] = x0;
    w[i-1] = cc / ( dp2 * p1 );
  }
//
//  Reverse the order of the values.
//
  for ( i = 1; i <= order/2; i++ )
  {
    temp       = x[i-1];
    x[i-1]     = x[order-i];
    x[order-i] = temp;
  }

  for ( i = 1; i <=order/2; i++ )
  {
    temp       = w[i-1];
    w[i-1]     = w[order-i];
    w[order-i] = temp;
  }

  delete [] b;
  delete [] c;

  return;
}
//**************************************************************************80

void SandiaRules::jacobi_ss_recur ( double *p2, double *dp2, double *p1, double x, int order,
  double alpha, double beta, double b[], double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_SS_RECUR evaluates a Jacobi polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Output, double *P2, the value of J(ORDER)(X).
//
//    Output, double *DP2, the value of J'(ORDER)(X).
//
//    Output, double *P1, the value of J(ORDER-1)(X).
//
//    Input, double X, the point at which polynomials are evaluated.
//
//    Input, int ORDER, the order of the polynomial.
//
//    Input, double ALPHA, BETA, the exponents of (1-X) and
//    (1+X) in the quadrature rule.
//
//    Input, double B[ORDER], C[ORDER], the recursion coefficients.
//
{
  double dp0;
  double dp1;
  int i;
  double p0;

  *p1 = 1.0;
  dp1 = 0.0;

  *p2 = x + ( alpha - beta ) / ( alpha + beta + 2.0 );
  *dp2 = 1.0;

  for ( i = 2; i <= order; i++ )
  {
    p0 = *p1;
    dp0 = dp1;

    *p1 = *p2;
    dp1 = *dp2;

    *p2 = ( x - b[i-1] ) *  ( *p1 ) - c[i-1] * p0;
    *dp2 = ( x - b[i-1] ) * dp1 + ( *p1 ) - c[i-1] * dp0;
  }
  return;
}
//**************************************************************************80

void SandiaRules::jacobi_ss_root ( double *x, int order, double alpha, double beta,
  double *dp2, double *p1, double b[], double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    JACOBI_SS_ROOT improves an approximate root of a Jacobi polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input/output, double *X, the approximate root, which
//    should be improved on output.
//
//    Input, int ORDER, the order of the polynomial.
//
//    Input, double ALPHA, BETA, the exponents of (1-X) and
//    (1+X) in the quadrature rule.
//
//    Output, double *DP2, the value of J'(ORDER)(X).
//
//    Output, double *P1, the value of J(ORDER-1)(X).
//
//    Input, double B[ORDER], C[ORDER], the recursion coefficients.
//
{
  double d;
  double eps;
  double p2;
  int step;
  int step_max = 10;

  eps = r8_epsilon ( );

  for ( step = 1; step <= step_max; step++ )
  {
    jacobi_ss_recur ( &p2, dp2, p1, *x, order, alpha, beta, b, c );

    d = p2 / ( *dp2 );
    *x = *x - d;

    if ( r8_abs ( d ) <= eps * ( r8_abs ( *x ) + 1.0 ) )
    {
      return;
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::laguerre_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_COMPUTE: Laguerre quadrature rule by the Elhay-Kautsky method.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Sylvan Elhay, Jaroslav Kautsky.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Sylvan Elhay, Jaroslav Kautsky,
//    Algorithm 655: IQPACK, FORTRAN Subroutines for the Weights of
//    Interpolatory Quadrature,
//    ACM Transactions on Mathematical Software,
//    Volume 13, Number 4, December 1987, pages 399-415.
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double *bj;
  int i;
  double zemu;
//
//  Define the zero-th moment.
//
  zemu = 1.0;
//
//  Define the Jacobi matrix.
//
  bj = new double[n];

  for ( i = 0; i < n; i++ )
  {
    bj[i] = ( double ) ( i + 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    x[i] = ( double ) ( 2 * i + 1 );
  }

  w[0] = std::sqrt ( zemu );

  for ( i = 1; i < n; i++ )
  {
    w[i] = 0.0;
  }
//
//  Diagonalize the Jacobi matrix.
//
  imtqlx ( n, x, bj, w );

  for ( i = 0; i < n; i++ )
  {
    w[i] = w[i] * w[i];
  }

  delete [] bj;

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_compute_np ( int order, int np, double p[], double x[],
  double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_COMPUTE_NP computes a Laguerre quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( 0 <= X < +oo ) exp ( - X ) * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//    The integral:
//
//      Integral ( A <= X < +oo ) F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * exp ( X(I) ) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  laguerre_compute ( order, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_compute_points ( int order, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_COMPUTE_POINTS computes Laguerre quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Output, double X[ORDER], the abscissas.
//
{
  double *w;

  w = new double[order];

  laguerre_compute ( order, x, w );

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_compute_points_np ( int order, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_COMPUTE_POINTS_NP computes Laguerre quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[ORDER], the abscissas.
//
{
  laguerre_compute_points ( order, x );

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_compute_weights ( int order, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_COMPUTE_WEIGHTS computes Laguerre quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Output, double W[ORDER], the weights.
//
{
  double *x;

  x = new double[order];

  laguerre_compute ( order, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_compute_weights_np ( int order, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_COMPUTE_WEIGHTS_NP computes Laguerre quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int ORDER, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[ORDER], the weights.
//
{
  laguerre_compute_weights ( order, w );

  return;
}
//**************************************************************************80

double SandiaRules::laguerre_integral ( int expon )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_INTEGRAL evaluates a monomial Laguerre integral.
//
//  Discussion:
//
//    The integral:
//
//      integral ( 0 <= x < +oo ) x^n * exp ( -x ) dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int EXPON, the exponent.
//    0 <= EXPON.
//
//    Output, double EXACT, the value of the integral.
//
{
  double exact;

  exact = r8_factorial ( expon );

  return exact;
}
//**************************************************************************80

void SandiaRules::laguerre_lookup ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_LOOKUP looks up abscissas and weights for Laguerre quadrature.
//
//  Discussion:
//
//    The abscissas are the zeroes of the Laguerre polynomial L(N)(X).
//
//    The integral:
//
//      Integral ( 0 <= X < +oo ) exp ( -X ) * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * f ( X(I) )
//
//    The integral:
//
//      Integral ( 0 <= X < +oo ) F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * exp ( X(I) ) * f ( X(I) )
//
//    Mathematica can numerically estimate the abscissas for the
//    n-th order polynomial to p digits of precision by the command:
//
//      NSolve [ LaguerreL[n,x] == 0, x, p ]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798,
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 20.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  laguerre_lookup_points ( n, x );

  laguerre_lookup_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_lookup_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_LOOKUP_POINTS looks up abscissas for Laguerre quadrature.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798,
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 20.
//
//    Output, double X[N], the abscissas.
//
{
  if ( n == 1 )
  {
    x[0] =  1.00000000000000000000000000000E+00;
  }
  else if ( n == 2 )
  {
    x[0] = 0.585786437626904951198311275790E+00;
    x[1] = 3.41421356237309504880168872421E+00;
  }
  else if ( n == 3 )
  {
    x[0] = 0.415774556783479083311533873128E+00;
    x[1] = 2.29428036027904171982205036136E+00;
    x[2] = 6.28994508293747919686641576551E+00;
  }
  else if ( n == 4 )
  {
    x[0] = 0.322547689619392311800361459104E+00;
    x[1] = 1.74576110115834657568681671252E+00;
    x[2] = 4.53662029692112798327928538496E+00;
    x[3] = 9.39507091230113312923353644342E+00;
  }
  else if ( n == 5 )
  {
    x[0] = 0.263560319718140910203061943361E+00;
    x[1] = 1.41340305910651679221840798019E+00;
    x[2] = 3.59642577104072208122318658878E+00;
    x[3] = 7.08581000585883755692212418111E+00;
    x[4] = 12.6408008442757826594332193066E+00;
  }
  else if ( n == 6 )
  {
    x[0] = 0.222846604179260689464354826787E+00;
    x[1] = 1.18893210167262303074315092194E+00;
    x[2] = 2.99273632605931407769132528451E+00;
    x[3] = 5.77514356910451050183983036943E+00;
    x[4] = 9.83746741838258991771554702994E+00;
    x[5] = 15.9828739806017017825457915674E+00;
  }
  else if ( n == 7 )
  {
    x[0] = 0.193043676560362413838247885004E+00;
    x[1] = 1.02666489533919195034519944317E+00;
    x[2] = 2.56787674495074620690778622666E+00;
    x[3] = 4.90035308452648456810171437810E+00;
    x[4] = 8.18215344456286079108182755123E+00;
    x[5] = 12.7341802917978137580126424582E+00;
    x[6] = 19.3957278622625403117125820576E+00;
  }
  else if ( n == 8 )
  {
    x[0] = 0.170279632305100999788861856608E+00;
    x[1] = 0.903701776799379912186020223555E+00;
    x[2] = 2.25108662986613068930711836697E+00;
    x[3] = 4.26670017028765879364942182690E+00;
    x[4] = 7.04590540239346569727932548212E+00;
    x[5] = 10.7585160101809952240599567880E+00;
    x[6] = 15.7406786412780045780287611584E+00;
    x[7] = 22.8631317368892641057005342974E+00;
  }
  else if ( n == 9 )
  {
    x[0] = 0.152322227731808247428107073127E+00;
    x[1] = 0.807220022742255847741419210952E+00;
    x[2] = 2.00513515561934712298303324701E+00;
    x[3] = 3.78347397333123299167540609364E+00;
    x[4] = 6.20495677787661260697353521006E+00;
    x[5] = 9.37298525168757620180971073215E+00;
    x[6] = 13.4662369110920935710978818397E+00;
    x[7] = 18.8335977889916966141498992996E+00;
    x[8] = 26.3740718909273767961410072937E+00;
  }
  else if ( n == 10 )
  {
    x[0] = 0.137793470540492430830772505653E+00;
    x[1] = 0.729454549503170498160373121676E+00;
    x[2] = 1.80834290174031604823292007575E+00;
    x[3] = 3.40143369785489951448253222141E+00;
    x[4] = 5.55249614006380363241755848687E+00;
    x[5] = 8.33015274676449670023876719727E+00;
    x[6] = 11.8437858379000655649185389191E+00;
    x[7] = 16.2792578313781020995326539358E+00;
    x[8] = 21.9965858119807619512770901956E+00;
    x[9] = 29.9206970122738915599087933408E+00;
  }
  else if ( n == 11 )
  {
    x[0] = 0.125796442187967522675794577516E+00;
    x[1] = 0.665418255839227841678127839420E+00;
    x[2] = 1.64715054587216930958700321365E+00;
    x[3] = 3.09113814303525495330195934259E+00;
    x[4] = 5.02928440157983321236999508366E+00;
    x[5] = 7.50988786380661681941099714450E+00;
    x[6] = 10.6059509995469677805559216457E+00;
    x[7] = 14.4316137580641855353200450349E+00;
    x[8] = 19.1788574032146786478174853989E+00;
    x[9] = 25.2177093396775611040909447797E+00;
    x[10] = 33.4971928471755372731917259395E+00;
  }
  else if ( n == 12 )
  {
    x[0] = 0.115722117358020675267196428240E+00;
    x[1] = 0.611757484515130665391630053042E+00;
    x[2] = 1.51261026977641878678173792687E+00;
    x[3] = 2.83375133774350722862747177657E+00;
    x[4] = 4.59922763941834848460572922485E+00;
    x[5] = 6.84452545311517734775433041849E+00;
    x[6] = 9.62131684245686704391238234923E+00;
    x[7] = 13.0060549933063477203460524294E+00;
    x[8] = 17.1168551874622557281840528008E+00;
    x[9] = 22.1510903793970056699218950837E+00;
    x[10] = 28.4879672509840003125686072325E+00;
    x[11] = 37.0991210444669203366389142764E+00;
  }
  else if ( n == 13 )
  {
    x[0] = 0.107142388472252310648493376977E+00;
    x[1] = 0.566131899040401853406036347177E+00;
    x[2] = 1.39856433645101971792750259921E+00;
    x[3] = 2.61659710840641129808364008472E+00;
    x[4] = 4.23884592901703327937303389926E+00;
    x[5] = 6.29225627114007378039376523025E+00;
    x[6] = 8.81500194118697804733348868036E+00;
    x[7] = 11.8614035888112425762212021880E+00;
    x[8] = 15.5107620377037527818478532958E+00;
    x[9] = 19.8846356638802283332036594634E+00;
    x[10] = 25.1852638646777580842970297823E+00;
    x[11] = 31.8003863019472683713663283526E+00;
    x[12] = 40.7230086692655795658979667001E+00;
  }
  else if ( n == 14 )
  {
    x[0] = 0.0997475070325975745736829452514E+00;
    x[1] = 0.526857648851902896404583451502E+00;
    x[2] = 1.30062912125149648170842022116E+00;
    x[3] = 2.43080107873084463616999751038E+00;
    x[4] = 3.93210282229321888213134366778E+00;
    x[5] = 5.82553621830170841933899983898E+00;
    x[6] = 8.14024014156514503005978046052E+00;
    x[7] = 10.9164995073660188408130510904E+00;
    x[8] = 14.2108050111612886831059780825E+00;
    x[9] = 18.1048922202180984125546272083E+00;
    x[10] = 22.7233816282696248232280886985E+00;
    x[11] = 28.2729817232482056954158923218E+00;
    x[12] = 35.1494436605924265828643121364E+00;
    x[13] = 44.3660817111174230416312423666E+00;
  }
  else if ( n == 15 )
  {
    x[0] = 0.0933078120172818047629030383672E+00;
    x[1] = 0.492691740301883908960101791412E+00;
    x[2] = 1.21559541207094946372992716488E+00;
    x[3] = 2.26994952620374320247421741375E+00;
    x[4] = 3.66762272175143727724905959436E+00;
    x[5] = 5.42533662741355316534358132596E+00;
    x[6] = 7.56591622661306786049739555812E+00;
    x[7] = 10.1202285680191127347927394568E+00;
    x[8] = 13.1302824821757235640991204176E+00;
    x[9] = 16.6544077083299578225202408430E+00;
    x[10] = 20.7764788994487667729157175676E+00;
    x[11] = 25.6238942267287801445868285977E+00;
    x[12] = 31.4075191697539385152432196202E+00;
    x[13] = 38.5306833064860094162515167595E+00;
    x[14] = 48.0260855726857943465734308508E+00;
  }
  else if ( n == 16 )
  {
    x[0] = 0.0876494104789278403601980973401E+00;
    x[1] = 0.462696328915080831880838260664E+00;
    x[2] = 1.14105777483122685687794501811E+00;
    x[3] = 2.12928364509838061632615907066E+00;
    x[4] = 3.43708663389320664523510701675E+00;
    x[5] = 5.07801861454976791292305830814E+00;
    x[6] = 7.07033853504823413039598947080E+00;
    x[7] = 9.43831433639193878394724672911E+00;
    x[8] = 12.2142233688661587369391246088E+00;
    x[9] = 15.4415273687816170767647741622E+00;
    x[10] = 19.1801568567531348546631409497E+00;
    x[11] = 23.5159056939919085318231872752E+00;
    x[12] = 28.5787297428821403675206137099E+00;
    x[13] = 34.5833987022866258145276871778E+00;
    x[14] = 41.9404526476883326354722330252E+00;
    x[15] = 51.7011603395433183643426971197E+00;
  }
  else if ( n == 17 )
  {
    x[0] = 0.0826382147089476690543986151980E+00;
    x[1] = 0.436150323558710436375959029847E+00;
    x[2] = 1.07517657751142857732980316755E+00;
    x[3] = 2.00519353164923224070293371933E+00;
    x[4] = 3.23425612404744376157380120696E+00;
    x[5] = 4.77351351370019726480932076262E+00;
    x[6] = 6.63782920536495266541643929703E+00;
    x[7] = 8.84668551116980005369470571184E+00;
    x[8] = 11.4255293193733525869726151469E+00;
    x[9] = 14.4078230374813180021982874959E+00;
    x[10] = 17.8382847307011409290658752412E+00;
    x[11] = 21.7782682577222653261749080522E+00;
    x[12] = 26.3153178112487997766149598369E+00;
    x[13] = 31.5817716804567331343908517497E+00;
    x[14] = 37.7960938374771007286092846663E+00;
    x[15] = 45.3757165339889661829258363215E+00;
    x[16] = 55.3897517898396106640900199790E+00;
  }
  else if ( n == 18 )
  {
    x[0] = 0.0781691666697054712986747615334E+00;
    x[1] = 0.412490085259129291039101536536E+00;
    x[2] = 1.01652017962353968919093686187E+00;
    x[3] = 1.89488850996976091426727831954E+00;
    x[4] = 3.05435311320265975115241130719E+00;
    x[5] = 4.50420553888989282633795571455E+00;
    x[6] = 6.25672507394911145274209116326E+00;
    x[7] = 8.32782515660563002170470261564E+00;
    x[8] = 10.7379900477576093352179033397E+00;
    x[9] = 13.5136562075550898190863812108E+00;
    x[10] = 16.6893062819301059378183984163E+00;
    x[11] = 20.3107676262677428561313764553E+00;
    x[12] = 24.4406813592837027656442257980E+00;
    x[13] = 29.1682086625796161312980677805E+00;
    x[14] = 34.6279270656601721454012429438E+00;
    x[15] = 41.0418167728087581392948614284E+00;
    x[16] = 48.8339227160865227486586093290E+00;
    x[17] = 59.0905464359012507037157810181E+00;
  }
  else if ( n == 19 )
  {
    x[0] = 0.0741587837572050877131369916024E+00;
    x[1] = 0.391268613319994607337648350299E+00;
    x[2] = 0.963957343997958058624878377130E+00;
    x[3] = 1.79617558206832812557725825252E+00;
    x[4] = 2.89365138187378399116494713237E+00;
    x[5] = 4.26421553962776647436040018167E+00;
    x[6] = 5.91814156164404855815360191408E+00;
    x[7] = 7.86861891533473373105668358176E+00;
    x[8] = 10.1324237168152659251627415800E+00;
    x[9] = 12.7308814638423980045092979656E+00;
    x[10] = 15.6912783398358885454136069861E+00;
    x[11] = 19.0489932098235501532136429732E+00;
    x[12] = 22.8508497608294829323930586693E+00;
    x[13] = 27.1606693274114488789963947149E+00;
    x[14] = 32.0691222518622423224362865906E+00;
    x[15] = 37.7129058012196494770647508283E+00;
    x[16] = 44.3173627958314961196067736013E+00;
    x[17] = 52.3129024574043831658644222420E+00;
    x[18] = 62.8024231535003758413504690673E+00;
  }
  else if ( n == 20 )
  {
    x[0] = 0.0705398896919887533666890045842E+00;
    x[1] = 0.372126818001611443794241388761E+00;
    x[2] = 0.916582102483273564667716277074E+00;
    x[3] = 1.70730653102834388068768966741E+00;
    x[4] = 2.74919925530943212964503046049E+00;
    x[5] = 4.04892531385088692237495336913E+00;
    x[6] = 5.61517497086161651410453988565E+00;
    x[7] = 7.45901745367106330976886021837E+00;
    x[8] = 9.59439286958109677247367273428E+00;
    x[9] = 12.0388025469643163096234092989E+00;
    x[10] = 14.8142934426307399785126797100E+00;
    x[11] = 17.9488955205193760173657909926E+00;
    x[12] = 21.4787882402850109757351703696E+00;
    x[13] = 25.4517027931869055035186774846E+00;
    x[14] = 29.9325546317006120067136561352E+00;
    x[15] = 35.0134342404790000062849359067E+00;
    x[16] = 40.8330570567285710620295677078E+00;
    x[17] = 47.6199940473465021399416271529E+00;
    x[18] = 55.8107957500638988907507734445E+00;
    x[19] = 66.5244165256157538186403187915E+00;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LAGUERRE_LOOKUP_POINTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::cerr << "  Legal values are 1 through 20.\n";
    std::exit ( 1 );
  }

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_lookup_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_LOOKUP_WEIGHTS looks up weights for Laguerre quadrature.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798,
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 20.
//
//    Output, double W[N], the weights.
//
{
  if ( n == 1 )
  {
    w[0] =  1.00000000000000000000000000000E+00;
  }
  else if ( n == 2 )
  {
    w[0] = 0.85355339059327376220042218105E+00;
    w[1] = 0.146446609406726237799577818948E+00;
  }
  else if ( n == 3 )
  {
    w[0] = 0.71109300992917301544959019114E+00;
    w[1] = 0.27851773356924084880144488846E+00;
    w[2] = 0.010389256501586135748964920401E+00;
  }
  else if ( n == 4 )
  {
    w[0] = 0.60315410434163360163596602382E+00;
    w[1] = 0.35741869243779968664149201746E+00;
    w[2] = 0.03888790851500538427243816816E+00;
    w[3] = 0.0005392947055613274501037905676E+00;
  }
  else if ( n == 5 )
  {
    w[0] = 0.52175561058280865247586092879E+00;
    w[1] = 0.3986668110831759274541333481E+00;
    w[2] = 0.0759424496817075953876533114E+00;
    w[3] = 0.00361175867992204845446126257E+00;
    w[4] = 0.00002336997238577622789114908455E+00;
  }
  else if ( n == 6 )
  {
    w[0] = 0.45896467394996359356828487771E+00;
    w[1] = 0.4170008307721209941133775662E+00;
    w[2] = 0.1133733820740449757387061851E+00;
    w[3] = 0.01039919745314907489891330285E+00;
    w[4] = 0.000261017202814932059479242860E+00;
    w[5] = 8.98547906429621238825292053E-07;
  }
  else if ( n == 7 )
  {
    w[0] = 0.40931895170127390213043288002E+00;
    w[1] = 0.4218312778617197799292810054E+00;
    w[2] = 0.1471263486575052783953741846E+00;
    w[3] = 0.0206335144687169398657056150E+00;
    w[4] = 0.00107401014328074552213195963E+00;
    w[5] = 0.0000158654643485642012687326223E+00;
    w[6] = 3.17031547899558056227132215E-08;
  }
  else if ( n == 8 )
  {
    w[0] = 0.36918858934163752992058283938E+00;
    w[1] = 0.4187867808143429560769785813E+00;
    w[2] = 0.175794986637171805699659867E+00;
    w[3] = 0.033343492261215651522132535E+00;
    w[4] = 0.0027945362352256725249389241E+00;
    w[5] = 0.00009076508773358213104238501E+00;
    w[6] = 8.4857467162725315448680183E-07;
    w[7] = 1.04800117487151038161508854E-09;
  }
  else if ( n == 9 )
  {
    w[0] = 0.336126421797962519673467717606E+00;
    w[1] = 0.411213980423984387309146942793E+00;
    w[2] = 0.199287525370885580860575607212E+00;
    w[3] = 0.0474605627656515992621163600479E+00;
    w[4] = 0.00559962661079458317700419900556E+00;
    w[5] = 0.000305249767093210566305412824291E+00;
    w[6] = 6.59212302607535239225572284875E-06;
    w[7] = 4.1107693303495484429024104033E-08;
    w[8] = 3.29087403035070757646681380323E-11;
  }
  else if ( n == 10 )
  {
    w[0] = 0.30844111576502014154747083468E+00;
    w[1] = 0.4011199291552735515157803099E+00;
    w[2] = 0.218068287611809421588648523E+00;
    w[3] = 0.062087456098677747392902129E+00;
    w[4] = 0.009501516975181100553839072E+00;
    w[5] = 0.0007530083885875387754559644E+00;
    w[6] = 0.00002825923349599565567422564E+00;
    w[7] = 4.249313984962686372586577E-07;
    w[8] = 1.839564823979630780921535E-09;
    w[9] = 9.911827219609008558377547E-13;
  }
  else if ( n == 11 )
  {
    w[0] = 0.28493321289420060505605102472E+00;
    w[1] = 0.3897208895278493779375535080E+00;
    w[2] = 0.232781831848991333940223796E+00;
    w[3] = 0.076564453546196686400854179E+00;
    w[4] = 0.014393282767350695091863919E+00;
    w[5] = 0.001518880846484873069847776E+00;
    w[6] = 0.0000851312243547192259720424E+00;
    w[7] = 2.29240387957450407857683E-06;
    w[8] = 2.48635370276779587373391E-08;
    w[9] = 7.71262693369132047028153E-11;
    w[10] = 2.883775868323623861597778E-14;
  }
  else if ( n == 12 )
  {
    w[0] = 0.26473137105544319034973889206E+00;
    w[1] = 0.3777592758731379820244905567E+00;
    w[2] = 0.244082011319877564254870818E+00;
    w[3] = 0.09044922221168093072750549E+00;
    w[4] = 0.02010238115463409652266129E+00;
    w[5] = 0.002663973541865315881054158E+00;
    w[6] = 0.000203231592662999392121433E+00;
    w[7] = 8.3650558568197987453363E-06;
    w[8] = 1.66849387654091026116990E-07;
    w[9] = 1.34239103051500414552392E-09;
    w[10] = 3.06160163503502078142408E-12;
    w[11] = 8.148077467426241682473119E-16;
  }
  else if ( n == 13 )
  {
    w[0] = 0.24718870842996262134624918596E+00;
    w[1] = 0.3656888229005219453067175309E+00;
    w[2] = 0.252562420057658502356824289E+00;
    w[3] = 0.10347075802418370511421863E+00;
    w[4] = 0.02643275441556161577815877E+00;
    w[5] = 0.00422039604025475276555209E+00;
    w[6] = 0.000411881770472734774892473E+00;
    w[7] = 0.0000235154739815532386882897E+00;
    w[8] = 7.3173116202490991040105E-07;
    w[9] = 1.10884162570398067979151E-08;
    w[10] = 6.7708266922058988406462E-11;
    w[11] = 1.15997995990507606094507E-13;
    w[12] = 2.245093203892758415991872E-17;
  }
  else if ( n == 14 )
  {
    w[0] = 0.23181557714486497784077486110E+00;
    w[1] = 0.3537846915975431518023313013E+00;
    w[2] = 0.258734610245428085987320561E+00;
    w[3] = 0.11548289355692321008730499E+00;
    w[4] = 0.03319209215933736003874996E+00;
    w[5] = 0.00619286943700661021678786E+00;
    w[6] = 0.00073989037786738594242589E+00;
    w[7] = 0.000054907194668416983785733E+00;
    w[8] = 2.4095857640853774967578E-06;
    w[9] = 5.801543981676495180886E-08;
    w[10] = 6.819314692484974119616E-10;
    w[11] = 3.2212077518948479398089E-12;
    w[12] = 4.2213524405165873515980E-15;
    w[13] = 6.05237502228918880839871E-19;
  }
  else if ( n == 15 )
  {
    w[0] = 0.21823488594008688985641323645E+00;
    w[1] = 0.3422101779228833296389489568E+00;
    w[2] = 0.263027577941680097414812275E+00;
    w[3] = 0.12642581810593053584303055E+00;
    w[4] = 0.04020686492100091484158548E+00;
    w[5] = 0.00856387780361183836391576E+00;
    w[6] = 0.00121243614721425207621921E+00;
    w[7] = 0.00011167439234425194199258E+00;
    w[8] = 6.459926762022900924653E-06;
    w[9] = 2.226316907096272630332E-07;
    w[10] = 4.227430384979365007351E-09;
    w[11] = 3.921897267041089290385E-11;
    w[12] = 1.4565152640731264063327E-13;
    w[13] = 1.4830270511133013354616E-16;
    w[14] = 1.60059490621113323104998E-20;
  }
  else if ( n == 16 )
  {
    w[0] = 0.20615171495780099433427363674E+00;
    w[1] = 0.3310578549508841659929830987E+00;
    w[2] = 0.265795777644214152599502021E+00;
    w[3] = 0.13629693429637753997554751E+00;
    w[4] = 0.0473289286941252189780623E+00;
    w[5] = 0.0112999000803394532312490E+00;
    w[6] = 0.0018490709435263108642918E+00;
    w[7] = 0.00020427191530827846012602E+00;
    w[8] = 0.00001484458687398129877135E+00;
    w[9] = 6.828319330871199564396E-07;
    w[10] = 1.881024841079673213882E-08;
    w[11] = 2.862350242973881619631E-10;
    w[12] = 2.127079033224102967390E-12;
    w[13] = 6.297967002517867787174E-15;
    w[14] = 5.050473700035512820402E-18;
    w[15] = 4.1614623703728551904265E-22;
  }
  else if ( n == 17 )
  {
    w[0] = 0.19533220525177083214592729770E+00;
    w[1] = 0.3203753572745402813366256320E+00;
    w[2] = 0.267329726357171097238809604E+00;
    w[3] = 0.14512985435875862540742645E+00;
    w[4] = 0.0544369432453384577793806E+00;
    w[5] = 0.0143572977660618672917767E+00;
    w[6] = 0.0026628247355727725684324E+00;
    w[7] = 0.0003436797271562999206118E+00;
    w[8] = 0.00003027551783782870109437E+00;
    w[9] = 1.768515053231676895381E-06;
    w[10] = 6.57627288681043332199E-08;
    w[11] = 1.469730932159546790344E-09;
    w[12] = 1.81691036255544979555E-11;
    w[13] = 1.095401388928687402976E-13;
    w[14] = 2.617373882223370421551E-16;
    w[15] = 1.6729356931461546908502E-19;
    w[16] = 1.06562631627404278815253E-23;
  }
  else if ( n == 18 )
  {
    w[0] = 0.18558860314691880562333775228E+00;
    w[1] = 0.3101817663702252936495975957E+00;
    w[2] = 0.267866567148536354820854395E+00;
    w[3] = 0.15297974746807490655384308E+00;
    w[4] = 0.0614349178609616527076780E+00;
    w[5] = 0.0176872130807729312772600E+00;
    w[6] = 0.0036601797677599177980266E+00;
    w[7] = 0.0005406227870077353231284E+00;
    w[8] = 0.0000561696505121423113818E+00;
    w[9] = 4.01530788370115755859E-06;
    w[10] = 1.91466985667567497969E-07;
    w[11] = 5.8360952686315941292E-09;
    w[12] = 1.07171126695539012773E-10;
    w[13] = 1.08909871388883385562E-12;
    w[14] = 5.38666474837830887608E-15;
    w[15] = 1.049865978035703408779E-17;
    w[16] = 5.405398451631053643566E-21;
    w[17] = 2.6916532692010286270838E-25;
  }
  else if ( n == 19 )
  {
    w[0] = 0.17676847491591250225103547981E+00;
    w[1] = 0.3004781436072543794821568077E+00;
    w[2] = 0.267599547038175030772695441E+00;
    w[3] = 0.15991337213558021678551215E+00;
    w[4] = 0.0682493799761491134552355E+00;
    w[5] = 0.0212393076065443249244062E+00;
    w[6] = 0.0048416273511483959672501E+00;
    w[7] = 0.0008049127473813667665946E+00;
    w[8] = 0.0000965247209315350170843E+00;
    w[9] = 8.20730525805103054409E-06;
    w[10] = 4.8305667247307725394E-07;
    w[11] = 1.90499136112328569994E-08;
    w[12] = 4.8166846309280615577E-10;
    w[13] = 7.3482588395511443768E-12;
    w[14] = 6.2022753875726163989E-14;
    w[15] = 2.54143084301542272372E-16;
    w[16] = 4.07886129682571235007E-19;
    w[17] = 1.707750187593837061004E-22;
    w[18] = 6.715064649908189959990E-27;
  }
  else if ( n == 20 )
  {
    w[0] = 0.168746801851113862149223899689E+00;
    w[1] = 0.291254362006068281716795323812E+00;
    w[2] = 0.266686102867001288549520868998E+00;
    w[3] = 0.166002453269506840031469127816E+00;
    w[4] = 0.0748260646687923705400624639615E+00;
    w[5] = 0.0249644173092832210728227383234E+00;
    w[6] = 0.00620255084457223684744754785395E+00;
    w[7] = 0.00114496238647690824203955356969E+00;
    w[8] = 0.000155741773027811974779809513214E+00;
    w[9] = 0.0000154014408652249156893806714048E+00;
    w[10] = 1.08648636651798235147970004439E-06;
    w[11] = 5.33012090955671475092780244305E-08;
    w[12] = 1.7579811790505820035778763784E-09;
    w[13] = 3.72550240251232087262924585338E-11;
    w[14] = 4.76752925157819052449488071613E-13;
    w[15] = 3.37284424336243841236506064991E-15;
    w[16] = 1.15501433950039883096396247181E-17;
    w[17] = 1.53952214058234355346383319667E-20;
    w[18] = 5.28644272556915782880273587683E-24;
    w[19] = 1.65645661249902329590781908529E-28;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LAGUERRE_LOOKUP_WEIGHTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::cerr << "  Legal values are 1 through 20.\n";
    std::exit ( 1 );
  }

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_ss_compute ( int order, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_SS_COMPUTE computes a Laguerre quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( 0 <= X < +oo ) exp ( - X ) * F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * F ( X(I) )
//
//    The integral:
//
//        Integral ( A <= X < +oo ) F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= ORDER ) W(I) * exp ( X(I) ) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    1 <= ORDER.
//
//    Output, double X[ORDER], the abscissas.
//
//    Output, double W[ORDER], the weights.
//
{
  double *b;
  double *c;
  double cc;
  double dp2;
  int i;
  int j;
  double p1;
  double prod;
  double r1;
  double x0;

  if ( order < 1 )
  {
    std::cerr << "\n";
    std::cerr << "LAGUERRE_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of ORDER = " << order << "\n";
    std::exit ( 1 );
  }

  b = new double[order];
  c = new double[order];
//
//  Set the recursion coefficients.
//
  for ( i = 0; i < order; i++ )
  {
    b[i] = ( double ) ( 2 * i + 1 );
  }

  for ( i = 0; i < order; i++ )
  {
    c[i] = ( double ) ( i * i );
  }
  prod = 1.0;
  for ( i = 1; i < order; i++ )
  {
    prod = prod * c[i];
  }
  cc = prod;

  for ( i = 0; i < order; i++ )
  {
//
//  Compute an estimate for the root.
//
    if ( i == 0 )
    {
      x0 =  3.0 / ( 1.0 + 2.4 * ( double ) ( order ) );
    }
    else if ( i == 1 )
    {
      x0 = x0 + 15.0 / ( 1.0 + 2.5 * ( double ) ( order ) );
    }
    else
    {
      r1 = ( 1.0 + 2.55 * ( double ) ( i - 1 ) )
        / ( 1.9 * ( double ) ( i - 1 ) );

      x0 = x0 + r1 * ( x0 - x[i-2] );
    }
//
//  Use iteration to find the root.
//
    laguerre_ss_root ( &x0, order, &dp2, &p1, b, c );
//
//  Set the abscissa and weight.
//
    x[i] = x0;
//
//  Because of the huge values involved, this calculation breaks down
//  for ORDER = 127.
//
//  It was originally w[i] = ( cc / dp2 ) / p1, which breaks down sooner.
//
    w[i] = ( 1.0 / dp2 );
    for ( j = 2; j <= order; j++ )
    {
      w[i] = w[i] * ( double ) ( j - 1 );
    }
    w[i] = w[i] / p1;
    for ( j = 2; j <= order; j++ )
    {
      w[i] = w[i] * ( double ) ( j - 1 );
    }

//  w[i] = ( cc / dp2 ) / p1;
  }

  delete [] b;
  delete [] c;

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_ss_recur ( double *p2, double *dp2, double *p1, double x,
  int order, double b[], double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_SS_RECUR evaluates a Laguerre polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Output, double *P2, the value of L(ORDER)(X).
//
//    Output, double *DP2, the value of L'(ORDER)(X).
//
//    Output, double *P1, the value of L(ORDER-1)(X).
//
//    Input, double X, the point at which polynomials are evaluated.
//
//    Input, int ORDER, the order of the polynomial.
//
//    Input, double B[ORDER], C[ORDER], the recursion coefficients.
//
{
  double dp0;
  double dp1;
  int i;
  double p0;

  *p1 = 1.0;
  dp1 = 0.0;

  *p2 = x - 1.0;
  *dp2 = 1.0;

  for ( i = 1; i < order; i++ )
  {
    p0 = *p1;
    dp0 = dp1;

    *p1 = *p2;
    dp1 = *dp2;

    *p2  = ( x - b[i] ) * ( *p1 ) - c[i] * p0;
    *dp2 = ( x - b[i] ) * dp1 + ( *p1 ) - c[i] * dp0;
  }

  return;
}
//**************************************************************************80

void SandiaRules::laguerre_ss_root ( double *x, int order, double *dp2, double *p1,
  double b[], double c[] )

//**************************************************************************80
//
//  Purpose:
//
//    LAGUERRE_SS_ROOT improves a root of a Laguerre polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Arthur Stroud, Don Secrest.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input/output, double *X, the approximate root, which
//    should be improved on output.
//
//    Input, int ORDER, the order of the polynomial.
//
//    Output, double *DP2, the value of L'(ORDER)(X).
//
//    Output, double *P1, the value of L(ORDER-1)(X).
//
//    Input, double B[ORDER], C[ORDER], the recursion coefficients.
//
{
  double d;
  double eps;
  double p2;
  int step;
  int step_max = 10;

  eps = r8_epsilon ( );

  for ( step = 1; step <= step_max; step++ )
  {
    laguerre_ss_recur ( &p2, dp2, p1, *x, order, b, c );

    d = p2 / ( *dp2 );
    *x = *x - d;

    if ( r8_abs ( d ) <= eps * ( r8_abs ( *x ) + 1.0 ) )
    {
      break;
    }
  }

  return;
}
//**************************************************************************80

void SandiaRules::legendre_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_COMPUTE: Legendre quadrature rule by the Elhay-Kautsky method.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 April 2011
//
//  Author:
//
//    Original FORTRAN77 version by Sylvan Elhay, Jaroslav Kautsky.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Sylvan Elhay, Jaroslav Kautsky,
//    Algorithm 655: IQPACK, FORTRAN Subroutines for the Weights of
//    Interpolatory Quadrature,
//    ACM Transactions on Mathematical Software,
//    Volume 13, Number 4, December 1987, pages 399-415.
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double *bj;
  int i;
  double zemu;
//
//  Define the zero-th moment.
//
  zemu = 2.0;
//
//  Define the Jacobi matrix.
//
  bj = new double[n];

  for ( i = 0; i < n; i++ )
  {
    bj[i] = ( double ) ( ( i + 1 ) * ( i + 1 ) ) 
          / ( double ) ( 4 * ( i + 1 ) * ( i + 1 ) - 1 );
    bj[i] = std::sqrt ( bj[i] );
  }

  for ( i = 0; i < n; i++ )
  {
    x[i] = 0.0;
  }

  w[0] = std::sqrt ( zemu );

  for ( i = 1; i < n; i++ )
  {
    w[i] = 0.0;
  }
//
//  Diagonalize the Jacobi matrix.
//
  imtqlx ( n, x, bj, w );

  for ( i = 0; i < n; i++ )
  {
    w[i] = w[i] * w[i];
  }

  delete [] bj;

  return;
}
//**************************************************************************80

void SandiaRules::legendre_compute_np ( int n, int np, double p[], double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_COMPUTE_NP computes a Legendre quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    Original FORTRAN77 version by Philip Davis, Philip Rabinowitz.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  legendre_compute ( n, x, w );

  return;
}
//**************************************************************************80

void SandiaRules::legendre_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_COMPUTE_POINTS computes Legendre quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
{
  double *w;

  w= new double[n];

  legendre_compute ( n, x, w );

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::legendre_compute_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_COMPUTE_POINTS_NP computes Legendre quadrature points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  legendre_compute_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::legendre_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_COMPUTE_WEIGHTS computes Legendre quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double W[N], the weights.
//
{
  double *x;

  x = new double[n];

  legendre_compute ( n, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::legendre_compute_weights_np ( int n, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_COMPUTE_WEIGHTS_NP computes Legendre quadrature weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  legendre_compute_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::legendre_dr_compute ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_DR_COMPUTE computes a Legendre quadrature rule.
//
//  Discussion:
//
//    The integral:
//
//      Integral ( -1 <= X <= 1 ) F(X) dX
//
//    The quadrature rule:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 June 2009
//
//  Author:
//
//    Original FORTRAN77 version by Philip Davis, Philip Rabinowitz.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int N, the order.
//    1 <= N.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double d1;
  double d2pn;
  double d3pn;
  double d4pn;
  double dp;
  double dpn;
  double e1;
  double fx;
  double h;
  int i;
  int iback;
  int k;
  int m;
  int mp1mi;
  int ncopy;
  int nmove;
  double p;
  double pi = 3.141592653589793;
  double pk;
  double pkm1;
  double pkp1;
  double t;
  double u;
  double v;
  double x0;
  double xtemp;

  if ( n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "LEGENDRE_DR_COMPUTE - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::exit ( 1 );
  }

  e1 = ( double ) ( n * ( n + 1 ) );

  m = ( n + 1 ) / 2;

  for ( i = 1; i <= m; i++ )
  {
    mp1mi = m + 1 - i;

    t = ( double ) ( 4 * i - 1 ) * pi / ( double ) ( 4 * n + 2 );

    x0 =  std::cos ( t ) * ( 1.0 - ( 1.0 - 1.0 / ( double ) ( n ) )
      / ( double ) ( 8 * n * n ) );

    pkm1 = 1.0;
    pk = x0;

    for ( k = 2; k <= n; k++ )
    {
      pkp1 = 2.0 * x0 * pk - pkm1 - ( x0 * pk - pkm1 ) / ( double ) ( k );
      pkm1 = pk;
      pk = pkp1;
    }

    d1 = ( double ) ( n ) * ( pkm1 - x0 * pk );

    dpn = d1 / ( 1.0 - x0 * x0 );

    d2pn = ( 2.0 * x0 * dpn - e1 * pk ) / ( 1.0 - x0 * x0 );

    d3pn = ( 4.0 * x0 * d2pn + ( 2.0 - e1 ) * dpn ) / ( 1.0 - x0 * x0 );

    d4pn = ( 6.0 * x0 * d3pn + ( 6.0 - e1 ) * d2pn ) / ( 1.0 - x0 * x0 );

    u = pk / dpn;
    v = d2pn / dpn;
//
//  Initial approximation H:
//
    h = -u * ( 1.0 + 0.5 * u * ( v + u * ( v * v - d3pn / ( 3.0 * dpn ) ) ) );
//
//  Refine H using one step of Newton's method:
//
    p = pk + h * ( dpn + 0.5 * h * ( d2pn + h / 3.0
      * ( d3pn + 0.25 * h * d4pn ) ) );

    dp = dpn + h * ( d2pn + 0.5 * h * ( d3pn + h * d4pn / 3.0 ) );

    h = h - p / dp;

    xtemp = x0 + h;

    x[mp1mi-1] = xtemp;

    fx = d1 - h * e1 * ( pk + 0.5 * h * ( dpn + h / 3.0
      * ( d2pn + 0.25 * h * ( d3pn + 0.2 * h * d4pn ) ) ) );

    w[mp1mi-1] = 2.0 * ( 1.0 - xtemp * xtemp ) / ( fx * fx );
  }

  if ( ( n % 2 ) == 1 )
  {
    x[0] = 0.0;
  }
//
//  Shift the data up.
//
  nmove = ( n + 1 ) / 2;
  ncopy = n - nmove;

  for ( i = 1; i <= nmove; i++ )
  {
    iback = n + 1 - i;
    x[iback-1] = x[iback-ncopy-1];
    w[iback-1] = w[iback-ncopy-1];
  }
//
//  Reflect values for the negative abscissas.
//
  for ( i = 1; i <= n - nmove; i++ )
  {
    x[i-1] = - x[n-i];
    w[i-1] = w[n-i];
  }

  return;
}
//**************************************************************************80

double SandiaRules::legendre_integral ( int expon )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_INTEGRAL evaluates a monomial Legendre integral.
//
//  Discussion:
//
//    The integral:
//
//      integral ( -1 <= x <= +1 ) x^n dx
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int EXPON, the exponent.
//
//    Output, double LEGENDRE_INTEGRAL, the value of the exact integral.
//
{
  double exact;
//
//  Get the exact value of the integral.
//
  if ( ( expon % 2 ) == 0 )
  {
    exact = 2.0 / ( double ) ( expon + 1 );
  }
  else
  {
    exact = 0.0;
  }

  return exact;
}
//**************************************************************************80

void SandiaRules::legendre_lookup ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_LOOKUP looks up abscissas and weights for Gauss-Legendre quadrature.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798.
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3,
//    LC: QA47.M315.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 33.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the abscissas.
//
{
  legendre_lookup_points ( n, x );

  legendre_lookup_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::legendre_lookup_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_LOOKUP_POINTS looks up abscissas for Gauss-Legendre quadrature.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798.
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3,
//    LC: QA47.M315.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 33.
//
//    Output, double X[N], the abscissas.
//
{
  if ( n == 1 )
  {
    x[0] = 0.000000000000000000000000000000;
  }
  else if ( n == 2 )
  {
    x[0] = -0.577350269189625764509148780502;
    x[1] = 0.577350269189625764509148780502;
  }
  else if ( n == 3 )
  {
    x[0] = -0.774596669241483377035853079956;
    x[1] = 0.000000000000000000000000000000;
    x[2] = 0.774596669241483377035853079956;
  }
  else if ( n == 4 )
  {
    x[0] = -0.861136311594052575223946488893;
    x[1] = -0.339981043584856264802665759103;
    x[2] = 0.339981043584856264802665759103;
    x[3] = 0.861136311594052575223946488893;
  }
  else if ( n == 5 )
  {
    x[0] = -0.906179845938663992797626878299;
    x[1] = -0.538469310105683091036314420700;
    x[2] = 0.000000000000000000000000000000;
    x[3] = 0.538469310105683091036314420700;
    x[4] = 0.906179845938663992797626878299;
  }
  else if ( n == 6 )
  {
    x[0] = -0.932469514203152027812301554494;
    x[1] = -0.661209386466264513661399595020;
    x[2] = -0.238619186083196908630501721681;
    x[3] = 0.238619186083196908630501721681;
    x[4] = 0.661209386466264513661399595020;
    x[5] = 0.932469514203152027812301554494;
  }
  else if ( n == 7 )
  {
    x[0] = -0.949107912342758524526189684048;
    x[1] = -0.741531185599394439863864773281;
    x[2] = -0.405845151377397166906606412077;
    x[3] = 0.000000000000000000000000000000;
    x[4] = 0.405845151377397166906606412077;
    x[5] = 0.741531185599394439863864773281;
    x[6] = 0.949107912342758524526189684048;
  }
  else if ( n == 8 )
  {
    x[0] = -0.960289856497536231683560868569;
    x[1] = -0.796666477413626739591553936476;
    x[2] = -0.525532409916328985817739049189;
    x[3] = -0.183434642495649804939476142360;
    x[4] = 0.183434642495649804939476142360;
    x[5] = 0.525532409916328985817739049189;
    x[6] = 0.796666477413626739591553936476;
    x[7] = 0.960289856497536231683560868569;
  }
  else if ( n == 9 )
  {
    x[0] = -0.968160239507626089835576203;
    x[1] = -0.836031107326635794299429788;
    x[2] = -0.613371432700590397308702039;
    x[3] = -0.324253423403808929038538015;
    x[4] = 0.000000000000000000000000000;
    x[5] = 0.324253423403808929038538015;
    x[6] = 0.613371432700590397308702039;
    x[7] = 0.836031107326635794299429788;
    x[8] = 0.968160239507626089835576203;
  }
  else if ( n == 10 )
  {
    x[0] = -0.973906528517171720077964012;
    x[1] = -0.865063366688984510732096688;
    x[2] = -0.679409568299024406234327365;
    x[3] = -0.433395394129247190799265943;
    x[4] = -0.148874338981631210884826001;
    x[5] = 0.148874338981631210884826001;
    x[6] = 0.433395394129247190799265943;
    x[7] = 0.679409568299024406234327365;
    x[8] = 0.865063366688984510732096688;
    x[9] = 0.973906528517171720077964012;
  }
  else if ( n == 11 )
  {
    x[0] = -0.978228658146056992803938001;
    x[1] = -0.887062599768095299075157769;
    x[2] = -0.730152005574049324093416252;
    x[3] = -0.519096129206811815925725669;
    x[4] = -0.269543155952344972331531985;
    x[5] = 0.000000000000000000000000000;
    x[6] = 0.269543155952344972331531985;
    x[7] = 0.519096129206811815925725669;
    x[8] = 0.730152005574049324093416252;
    x[9] = 0.887062599768095299075157769;
    x[10] = 0.978228658146056992803938001;
  }
  else if ( n == 12 )
  {
    x[0] = -0.981560634246719250690549090;
    x[1] = -0.904117256370474856678465866;
    x[2] = -0.769902674194304687036893833;
    x[3] = -0.587317954286617447296702419;
    x[4] = -0.367831498998180193752691537;
    x[5] = -0.125233408511468915472441369;
    x[6] = 0.125233408511468915472441369;
    x[7] = 0.367831498998180193752691537;
    x[8] = 0.587317954286617447296702419;
    x[9] = 0.769902674194304687036893833;
    x[10] = 0.904117256370474856678465866;
    x[11] = 0.981560634246719250690549090;
  }
  else if ( n == 13 )
  {
    x[0] = -0.984183054718588149472829449;
    x[1] = -0.917598399222977965206547837;
    x[2] = -0.801578090733309912794206490;
    x[3] = -0.642349339440340220643984607;
    x[4] = -0.448492751036446852877912852;
    x[5] = -0.230458315955134794065528121;
    x[6] = 0.000000000000000000000000000;
    x[7] = 0.230458315955134794065528121;
    x[8] = 0.448492751036446852877912852;
    x[9] = 0.642349339440340220643984607;
    x[10] = 0.80157809073330991279420649;
    x[11] = 0.91759839922297796520654784;
    x[12] = 0.98418305471858814947282945;
  }
  else if ( n == 14 )
  {
    x[0] = -0.986283808696812338841597267;
    x[1] = -0.928434883663573517336391139;
    x[2] = -0.827201315069764993189794743;
    x[3] = -0.687292904811685470148019803;
    x[4] = -0.515248636358154091965290719;
    x[5] = -0.319112368927889760435671824;
    x[6] = -0.108054948707343662066244650;
    x[7] = 0.108054948707343662066244650;
    x[8] = 0.31911236892788976043567182;
    x[9] = 0.51524863635815409196529072;
    x[10] = 0.68729290481168547014801980;
    x[11] = 0.82720131506976499318979474;
    x[12] = 0.92843488366357351733639114;
    x[13] = 0.98628380869681233884159727;
  }
  else if ( n == 15 )
  {
    x[0] = -0.987992518020485428489565719;
    x[1] = -0.937273392400705904307758948;
    x[2] = -0.848206583410427216200648321;
    x[3] = -0.724417731360170047416186055;
    x[4] = -0.570972172608538847537226737;
    x[5] = -0.394151347077563369897207371;
    x[6] = -0.201194093997434522300628303;
    x[7] = 0.00000000000000000000000000;
    x[8] = 0.20119409399743452230062830;
    x[9] = 0.39415134707756336989720737;
    x[10] = 0.57097217260853884753722674;
    x[11] = 0.72441773136017004741618605;
    x[12] = 0.84820658341042721620064832;
    x[13] = 0.93727339240070590430775895;
    x[14] = 0.98799251802048542848956572;
  }
  else if ( n == 16 )
  {
    x[0] = -0.989400934991649932596154173;
    x[1] = -0.944575023073232576077988416;
    x[2] = -0.865631202387831743880467898;
    x[3] = -0.755404408355003033895101195;
    x[4] = -0.617876244402643748446671764;
    x[5] = -0.458016777657227386342419443;
    x[6] = -0.281603550779258913230460501;
    x[7] = -0.09501250983763744018531934;
    x[8] = 0.09501250983763744018531934;
    x[9] = 0.28160355077925891323046050;
    x[10] = 0.45801677765722738634241944;
    x[11] = 0.61787624440264374844667176;
    x[12] = 0.75540440835500303389510119;
    x[13] = 0.86563120238783174388046790;
    x[14] = 0.94457502307323257607798842;
    x[15] = 0.98940093499164993259615417;
  }
  else if ( n == 17 )
  {
    x[0] = -0.990575475314417335675434020;
    x[1] = -0.950675521768767761222716958;
    x[2] = -0.880239153726985902122955694;
    x[3] = -0.781514003896801406925230056;
    x[4] = -0.657671159216690765850302217;
    x[5] = -0.512690537086476967886246569;
    x[6] = -0.35123176345387631529718552;
    x[7] = -0.17848418149584785585067749;
    x[8] = 0.00000000000000000000000000;
    x[9] = 0.17848418149584785585067749;
    x[10] = 0.35123176345387631529718552;
    x[11] = 0.51269053708647696788624657;
    x[12] = 0.65767115921669076585030222;
    x[13] = 0.78151400389680140692523006;
    x[14] = 0.88023915372698590212295569;
    x[15] = 0.95067552176876776122271696;
    x[16] = 0.99057547531441733567543402;
  }
  else if ( n == 18 )
  {
    x[0] = -0.991565168420930946730016005;
    x[1] = -0.955823949571397755181195893;
    x[2] = -0.892602466497555739206060591;
    x[3] = -0.803704958972523115682417455;
    x[4] = -0.691687043060353207874891081;
    x[5] = -0.55977083107394753460787155;
    x[6] = -0.41175116146284264603593179;
    x[7] = -0.25188622569150550958897285;
    x[8] = -0.08477501304173530124226185;
    x[9] = 0.08477501304173530124226185;
    x[10] = 0.25188622569150550958897285;
    x[11] = 0.41175116146284264603593179;
    x[12] = 0.55977083107394753460787155;
    x[13] = 0.69168704306035320787489108;
    x[14] = 0.80370495897252311568241746;
    x[15] = 0.89260246649755573920606059;
    x[16] = 0.95582394957139775518119589;
    x[17] = 0.99156516842093094673001600;
  }
  else if ( n == 19 )
  {
    x[0] = -0.992406843843584403189017670;
    x[1] = -0.960208152134830030852778841;
    x[2] = -0.903155903614817901642660929;
    x[3] = -0.822714656537142824978922487;
    x[4] = -0.72096617733522937861709586;
    x[5] = -0.60054530466168102346963816;
    x[6] = -0.46457074137596094571726715;
    x[7] = -0.31656409996362983199011733;
    x[8] = -0.16035864564022537586809612;
    x[9] = 0.00000000000000000000000000;
    x[10] = 0.16035864564022537586809612;
    x[11] = 0.31656409996362983199011733;
    x[12] = 0.46457074137596094571726715;
    x[13] = 0.60054530466168102346963816;
    x[14] = 0.72096617733522937861709586;
    x[15] = 0.82271465653714282497892249;
    x[16] = 0.90315590361481790164266093;
    x[17] = 0.96020815213483003085277884;
    x[18] = 0.99240684384358440318901767;
  }
  else if ( n == 20 )
  {
    x[0] = -0.993128599185094924786122388;
    x[1] = -0.963971927277913791267666131;
    x[2] = -0.912234428251325905867752441;
    x[3] = -0.83911697182221882339452906;
    x[4] = -0.74633190646015079261430507;
    x[5] = -0.63605368072651502545283670;
    x[6] = -0.51086700195082709800436405;
    x[7] = -0.37370608871541956067254818;
    x[8] = -0.22778585114164507808049620;
    x[9] = -0.07652652113349733375464041;
    x[10] = 0.07652652113349733375464041;
    x[11] = 0.22778585114164507808049620;
    x[12] = 0.37370608871541956067254818;
    x[13] = 0.51086700195082709800436405;
    x[14] = 0.63605368072651502545283670;
    x[15] = 0.74633190646015079261430507;
    x[16] = 0.83911697182221882339452906;
    x[17] = 0.91223442825132590586775244;
    x[18] = 0.96397192727791379126766613;
    x[19] = 0.99312859918509492478612239;
  }
  else if ( n == 21 )
  {
    x[ 0] =  -0.99375217062038950026024204;
    x[ 1] =  -0.96722683856630629431662221;
    x[ 2] =  -0.92009933415040082879018713;
    x[ 3] =  -0.85336336458331728364725064;
    x[ 4] =  -0.76843996347567790861587785;
    x[ 5] =  -0.66713880419741231930596667;
    x[ 6] =  -0.55161883588721980705901880;
    x[ 7] =  -0.42434212020743878357366889;
    x[ 8] =  -0.28802131680240109660079252;
    x[9] =  -0.14556185416089509093703098;
    x[10] =   0.00000000000000000000000000;
    x[11] =  +0.14556185416089509093703098;
    x[12] =  +0.28802131680240109660079252;
    x[13] =  +0.42434212020743878357366889;
    x[14] =  +0.55161883588721980705901880;
    x[15] =  +0.66713880419741231930596667;
    x[16] =  +0.76843996347567790861587785;
    x[17] =  +0.85336336458331728364725064;
    x[18] =  +0.92009933415040082879018713;
    x[19] =  +0.96722683856630629431662221;
    x[20] =  +0.99375217062038950026024204;
  }
  else if ( n == 22 )
  {
    x[0] = -0.99429458548239929207303142;
    x[1] = -0.97006049783542872712395099;
    x[2] = -0.92695677218717400052069294;
    x[3] = -0.86581257772030013653642564;
    x[4] = -0.78781680597920816200427796;
    x[5] = -0.69448726318668278005068984;
    x[6] = -0.58764040350691159295887693;
    x[7] = -0.46935583798675702640633071;
    x[8] = -0.34193582089208422515814742;
    x[9] = -0.20786042668822128547884653;
    x[10] = -0.06973927331972222121384180;
    x[11] = 0.06973927331972222121384180;
    x[12] = 0.20786042668822128547884653;
    x[13] = 0.34193582089208422515814742;
    x[14] = 0.46935583798675702640633071;
    x[15] = 0.58764040350691159295887693;
    x[16] = 0.69448726318668278005068984;
    x[17] = 0.78781680597920816200427796;
    x[18] = 0.86581257772030013653642564;
    x[19] = 0.92695677218717400052069294;
    x[20] = 0.97006049783542872712395099;
    x[21] = 0.99429458548239929207303142;
  }
  else if ( n == 23 )
  {
    x[0] = -0.99476933499755212352392572;
    x[1] = -0.97254247121811523195602408;
    x[2] = -0.93297108682601610234919699;
    x[3] = -0.87675235827044166737815689;
    x[4] = -0.80488840161883989215111841;
    x[5] = -0.71866136313195019446162448;
    x[6] = -0.61960987576364615638509731;
    x[7] = -0.50950147784600754968979305;
    x[8] = -0.39030103803029083142148887;
    x[9] = -0.26413568097034493053386954;
    x[10] = -0.13325682429846611093174268;
    x[11] = 0.00000000000000000000000000;
    x[12] = 0.13325682429846611093174268;
    x[13] = 0.26413568097034493053386954;
    x[14] = 0.39030103803029083142148887;
    x[15] = 0.50950147784600754968979305;
    x[16] = 0.61960987576364615638509731;
    x[17] = 0.71866136313195019446162448;
    x[18] = 0.80488840161883989215111841;
    x[19] = 0.87675235827044166737815689;
    x[20] = 0.93297108682601610234919699;
    x[21] = 0.97254247121811523195602408;
    x[22] = 0.99476933499755212352392572;
  }
  else if ( n == 24 )
  {
    x[0] = -0.99518721999702136017999741;
    x[1] = -0.97472855597130949819839199;
    x[2] = -0.93827455200273275852364900;
    x[3] = -0.88641552700440103421315434;
    x[4] = -0.82000198597390292195394987;
    x[5] = -0.74012419157855436424382810;
    x[6] = -0.64809365193697556925249579;
    x[7] = -0.54542147138883953565837562;
    x[8] = -0.43379350762604513848708423;
    x[9] = -0.31504267969616337438679329;
    x[10] = -0.19111886747361630915863982;
    x[11] = -0.06405689286260562608504308;
    x[12] = 0.06405689286260562608504308;
    x[13] = 0.19111886747361630915863982;
    x[14] = 0.31504267969616337438679329;
    x[15] = 0.43379350762604513848708423;
    x[16] = 0.54542147138883953565837562;
    x[17] = 0.64809365193697556925249579;
    x[18] = 0.74012419157855436424382810;
    x[19] = 0.82000198597390292195394987;
    x[20] = 0.88641552700440103421315434;
    x[21] = 0.93827455200273275852364900;
    x[22] = 0.97472855597130949819839199;
    x[23] = 0.99518721999702136017999741;
  }
  else if ( n == 25 )
  {
    x[0] = -0.99555696979049809790878495;
    x[1] = -0.97666392145951751149831539;
    x[2] = -0.94297457122897433941401117;
    x[3] = -0.89499199787827536885104201;
    x[4] = -0.83344262876083400142102111;
    x[5] = -0.75925926303735763057728287;
    x[6] = -0.67356636847346836448512063;
    x[7] = -0.57766293024122296772368984;
    x[8] = -0.47300273144571496052218212;
    x[9] = -0.36117230580938783773582173;
    x[10] = -0.24386688372098843204519036;
    x[11] = -0.12286469261071039638735982;
    x[12] = 0.00000000000000000000000000;
    x[13] = 0.12286469261071039638735982;
    x[14] = 0.24386688372098843204519036;
    x[15] = 0.36117230580938783773582173;
    x[16] = 0.47300273144571496052218212;
    x[17] = 0.57766293024122296772368984;
    x[18] = 0.67356636847346836448512063;
    x[19] = 0.75925926303735763057728287;
    x[20] = 0.83344262876083400142102111;
    x[21] = 0.89499199787827536885104201;
    x[22] = 0.94297457122897433941401117;
    x[23] = 0.97666392145951751149831539;
    x[24] = 0.99555696979049809790878495;
  }
  else if ( n == 26 )
  {
    x[0] = -0.99588570114561692900321696;
    x[1] = -0.97838544595647099110058035;
    x[2] = -0.94715906666171425013591528;
    x[3] = -0.90263786198430707421766560;
    x[4] = -0.84544594278849801879750706;
    x[5] = -0.77638594882067885619296725;
    x[6] = -0.69642726041995726486381391;
    x[7] = -0.60669229301761806323197875;
    x[8] = -0.50844071482450571769570306;
    x[9] = -0.40305175512348630648107738;
    x[10] = -0.29200483948595689514283538;
    x[11] = -0.17685882035689018396905775;
    x[12] = -0.05923009342931320709371858;
    x[13] = 0.05923009342931320709371858;
    x[14] = 0.17685882035689018396905775;
    x[15] = 0.29200483948595689514283538;
    x[16] = 0.40305175512348630648107738;
    x[17] = 0.50844071482450571769570306;
    x[18] = 0.60669229301761806323197875;
    x[19] = 0.69642726041995726486381391;
    x[20] = 0.77638594882067885619296725;
    x[21] = 0.84544594278849801879750706;
    x[22] = 0.90263786198430707421766560;
    x[23] = 0.94715906666171425013591528;
    x[24] = 0.97838544595647099110058035;
    x[25] = 0.99588570114561692900321696;
  }
  else if ( n == 27 )
  {
    x[0] = -0.99617926288898856693888721;
    x[1] = -0.97992347596150122285587336;
    x[2] = -0.95090055781470500685190803;
    x[3] = -0.90948232067749110430064502;
    x[4] = -0.85620790801829449030273722;
    x[5] = -0.79177163907050822714439734;
    x[6] = -0.71701347373942369929481621;
    x[7] = -0.63290797194649514092773464;
    x[8] = -0.54055156457945689490030094;
    x[9] = -0.44114825175002688058597416;
    x[10] = -0.33599390363850889973031903;
    x[11] = -0.22645936543953685885723911;
    x[12] = -0.11397258560952996693289498;
    x[13] = 0.00000000000000000000000000;
    x[14] = 0.11397258560952996693289498;
    x[15] = 0.22645936543953685885723911;
    x[16] = 0.33599390363850889973031903;
    x[17] = 0.44114825175002688058597416;
    x[18] = 0.54055156457945689490030094;
    x[19] = 0.63290797194649514092773464;
    x[20] = 0.71701347373942369929481621;
    x[21] = 0.79177163907050822714439734;
    x[22] = 0.85620790801829449030273722;
    x[23] = 0.90948232067749110430064502;
    x[24] = 0.95090055781470500685190803;
    x[25] = 0.97992347596150122285587336;
    x[26] = 0.99617926288898856693888721;
  }
  else if ( n == 28 )
  {
    x[0] = -0.99644249757395444995043639;
    x[1] = -0.98130316537087275369455995;
    x[2] = -0.95425928062893819725410184;
    x[3] = -0.91563302639213207386968942;
    x[4] = -0.86589252257439504894225457;
    x[5] = -0.80564137091717917144788596;
    x[6] = -0.73561087801363177202814451;
    x[7] = -0.65665109403886496121989818;
    x[8] = -0.56972047181140171930800328;
    x[9] = -0.47587422495511826103441185;
    x[10] = -0.37625151608907871022135721;
    x[11] = -0.27206162763517807767682636;
    x[12] = -0.16456928213338077128147178;
    x[13] = -0.05507928988403427042651653;
    x[14] = 0.05507928988403427042651653;
    x[15] = 0.16456928213338077128147178;
    x[16] = 0.27206162763517807767682636;
    x[17] = 0.37625151608907871022135721;
    x[18] = 0.47587422495511826103441185;
    x[19] = 0.56972047181140171930800328;
    x[20] = 0.65665109403886496121989818;
    x[21] = 0.73561087801363177202814451;
    x[22] = 0.80564137091717917144788596;
    x[23] = 0.86589252257439504894225457;
    x[24] = 0.91563302639213207386968942;
    x[25] = 0.95425928062893819725410184;
    x[26] = 0.98130316537087275369455995;
    x[27] = 0.99644249757395444995043639;
  }
  else if ( n == 29 )
  {
    x[0] = -0.99667944226059658616319153;
    x[1] = -0.98254550526141317487092602;
    x[2] = -0.95728559577808772579820804;
    x[3] = -0.92118023295305878509375344;
    x[4] = -0.87463780492010279041779342;
    x[5] = -0.81818548761525244498957221;
    x[6] = -0.75246285173447713391261008;
    x[7] = -0.67821453760268651515618501;
    x[8] = -0.59628179713822782037958621;
    x[9] = -0.50759295512422764210262792;
    x[10] = -0.41315288817400866389070659;
    x[11] = -0.31403163786763993494819592;
    x[12] = -0.21135228616600107450637573;
    x[13] = -0.10627823013267923017098239;
    x[14] = 0.00000000000000000000000000;
    x[15] = 0.10627823013267923017098239;
    x[16] = 0.21135228616600107450637573;
    x[17] = 0.31403163786763993494819592;
    x[18] = 0.41315288817400866389070659;
    x[19] = 0.50759295512422764210262792;
    x[20] = 0.59628179713822782037958621;
    x[21] = 0.67821453760268651515618501;
    x[22] = 0.75246285173447713391261008;
    x[23] = 0.81818548761525244498957221;
    x[24] = 0.87463780492010279041779342;
    x[25] = 0.92118023295305878509375344;
    x[26] = 0.95728559577808772579820804;
    x[27] = 0.98254550526141317487092602;
    x[28] = 0.99667944226059658616319153;
  }
  else if ( n == 30 )
  {
    x[0] = -0.99689348407464954027163005;
    x[1] = -0.98366812327974720997003258;
    x[2] = -0.96002186496830751221687103;
    x[3] = -0.92620004742927432587932428;
    x[4] = -0.88256053579205268154311646;
    x[5] = -0.82956576238276839744289812;
    x[6] = -0.76777743210482619491797734;
    x[7] = -0.69785049479331579693229239;
    x[8] = -0.62052618298924286114047756;
    x[9] = -0.53662414814201989926416979;
    x[10] = -0.44703376953808917678060990;
    x[11] = -0.35270472553087811347103721;
    x[12] = -0.25463692616788984643980513;
    x[13] = -0.15386991360858354696379467;
    x[14] = -0.05147184255531769583302521;
    x[15] = 0.05147184255531769583302521;
    x[16] = 0.15386991360858354696379467;
    x[17] = 0.25463692616788984643980513;
    x[18] = 0.35270472553087811347103721;
    x[19] = 0.44703376953808917678060990;
    x[20] = 0.53662414814201989926416979;
    x[21] = 0.62052618298924286114047756;
    x[22] = 0.69785049479331579693229239;
    x[23] = 0.76777743210482619491797734;
    x[24] = 0.82956576238276839744289812;
    x[25] = 0.88256053579205268154311646;
    x[26] = 0.92620004742927432587932428;
    x[27] = 0.96002186496830751221687103;
    x[28] = 0.98366812327974720997003258;
    x[29] = 0.99689348407464954027163005;
  }
  else if ( n == 31 )
  {
    x[0] = -0.99708748181947707405562655;
    x[1] = -0.98468590966515248400246517;
    x[2] = -0.96250392509294966178905240;
    x[3] = -0.93075699789664816495694576;
    x[4] = -0.88976002994827104337419201;
    x[5] = -0.83992032014626734008690454;
    x[6] = -0.78173314841662494040636002;
    x[7] = -0.71577678458685328390597087;
    x[8] = -0.64270672292426034618441820;
    x[9] = -0.56324916140714926272094492;
    x[10] = -0.47819378204490248044059404;
    x[11] = -0.38838590160823294306135146;
    x[12] = -0.29471806998170161661790390;
    x[13] = -0.19812119933557062877241300;
    x[14] = -0.09955531215234152032517479;
    x[15] = 0.00000000000000000000000000;
    x[16] = 0.09955531215234152032517479;
    x[17] = 0.19812119933557062877241300;
    x[18] = 0.29471806998170161661790390;
    x[19] = 0.38838590160823294306135146;
    x[20] = 0.47819378204490248044059404;
    x[21] = 0.56324916140714926272094492;
    x[22] = 0.64270672292426034618441820;
    x[23] = 0.71577678458685328390597087;
    x[24] = 0.78173314841662494040636002;
    x[25] = 0.83992032014626734008690454;
    x[26] = 0.88976002994827104337419201;
    x[27] = 0.93075699789664816495694576;
    x[28] = 0.96250392509294966178905240;
    x[29] = 0.98468590966515248400246517;
    x[30] = 0.99708748181947707405562655;
  }
  else if ( n == 32 )
  {
    x[0] = -0.99726386184948156354498113;
    x[1] = -0.98561151154526833540017504;
    x[2] = -0.96476225558750643077381193;
    x[3] = -0.93490607593773968917091913;
    x[4] = -0.89632115576605212396530724;
    x[5] = -0.84936761373256997013369300;
    x[6] = -0.79448379596794240696309730;
    x[7] = -0.73218211874028968038742667;
    x[8] = -0.66304426693021520097511517;
    x[9] = -0.58771575724076232904074548;
    x[10] = -0.50689990893222939002374747;
    x[11] = -0.42135127613063534536411944;
    x[12] = -0.33186860228212764977991681;
    x[13] = -0.23928736225213707454460321;
    x[14] = -0.14447196158279649348518637;
    x[15] = -0.04830766568773831623481257;
    x[16] = 0.04830766568773831623481257;
    x[17] = 0.14447196158279649348518637;
    x[18] = 0.23928736225213707454460321;
    x[19] = 0.33186860228212764977991681;
    x[20] = 0.42135127613063534536411944;
    x[21] = 0.50689990893222939002374747;
    x[22] = 0.58771575724076232904074548;
    x[23] = 0.66304426693021520097511517;
    x[24] = 0.73218211874028968038742667;
    x[25] = 0.79448379596794240696309730;
    x[26] = 0.84936761373256997013369300;
    x[27] = 0.89632115576605212396530724;
    x[28] = 0.93490607593773968917091913;
    x[29] = 0.96476225558750643077381193;
    x[30] = 0.98561151154526833540017504;
    x[31] = 0.99726386184948156354498113;
  }
  else if ( n == 33 )
  {
    x[0] = -0.99742469424645521726616802;
    x[1] = -0.98645572623064248811037570;
    x[2] = -0.96682290968999276892837771;
    x[3] = -0.93869437261116835035583512;
    x[4] = -0.90231676774343358304053133;
    x[5] = -0.85800965267650406464306148;
    x[6] = -0.80616235627416658979620087;
    x[7] = -0.74723049644956215785905512;
    x[8] = -0.68173195996974278626821595;
    x[9] = -0.61024234583637902730728751;
    x[10] = -0.53338990478634764354889426;
    x[11] = -0.45185001727245069572599328;
    x[12] = -0.36633925774807334107022062;
    x[13] = -0.27760909715249702940324807;
    x[14] = -0.18643929882799157233579876;
    x[15] = -0.09363106585473338567074292;
    x[16] = 0.00000000000000000000000000;
    x[17] = 0.09363106585473338567074292;
    x[18] = 0.18643929882799157233579876;
    x[19] = 0.27760909715249702940324807;
    x[20] = 0.36633925774807334107022062;
    x[21] = 0.45185001727245069572599328;
    x[22] = 0.53338990478634764354889426;
    x[23] = 0.61024234583637902730728751;
    x[24] = 0.68173195996974278626821595;
    x[25] = 0.74723049644956215785905512;
    x[26] = 0.80616235627416658979620087;
    x[27] = 0.85800965267650406464306148;
    x[28] = 0.90231676774343358304053133;
    x[29] = 0.93869437261116835035583512;
    x[30] = 0.96682290968999276892837771;
    x[31] = 0.98645572623064248811037570;
    x[32] = 0.99742469424645521726616802;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEGENDRE_LOOKUP_POINTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::cerr << "  Legal values are 1 through 33.\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

void SandiaRules::legendre_lookup_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_LOOKUP_WEIGHTS looks up weights for Gauss-Legendre quadrature.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Vladimir Krylov,
//    Approximate Calculation of Integrals,
//    Dover, 2006,
//    ISBN: 0486445798.
//    LC: QA311.K713.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996,
//    ISBN: 0-8493-2479-3,
//    LC: QA47.M315.
//
//  Parameters:
//
//    Input, int N, the order.
//    N must be between 1 and 33.
//
//    Output, double W[N], the weights.
//
{
  if ( n == 1 )
  {
    w[0] = 2.000000000000000000000000000000;
  }
  else if ( n == 2 )
  {
    w[0] = 1.000000000000000000000000000000;
    w[1] = 1.000000000000000000000000000000;
  }
  else if ( n == 3 )
  {
    w[0] = 0.555555555555555555555555555556;
    w[1] = 0.888888888888888888888888888889;
    w[2] = 0.555555555555555555555555555556;
  }
  else if ( n == 4 )
  {
    w[0] = 0.347854845137453857373063949222;
    w[1] = 0.652145154862546142626936050778;
    w[2] = 0.652145154862546142626936050778;
    w[3] = 0.347854845137453857373063949222;
  }
  else if ( n == 5 )
  {
    w[0] = 0.236926885056189087514264040720;
    w[1] = 0.478628670499366468041291514836;
    w[2] = 0.568888888888888888888888888889;
    w[3] = 0.478628670499366468041291514836;
    w[4] = 0.236926885056189087514264040720;
  }
  else if ( n == 6 )
  {
    w[0] = 0.171324492379170345040296142173;
    w[1] = 0.360761573048138607569833513838;
    w[2] = 0.467913934572691047389870343990;
    w[3] = 0.467913934572691047389870343990;
    w[4] = 0.360761573048138607569833513838;
    w[5] = 0.171324492379170345040296142173;
  }
  else if ( n == 7 )
  {
    w[0] = 0.129484966168869693270611432679;
    w[1] = 0.279705391489276667901467771424;
    w[2] = 0.381830050505118944950369775489;
    w[3] = 0.417959183673469387755102040816;
    w[4] = 0.381830050505118944950369775489;
    w[5] = 0.279705391489276667901467771424;
    w[6] = 0.129484966168869693270611432679;
  }
  else if ( n == 8 )
  {
    w[0] = 0.101228536290376259152531354310;
    w[1] = 0.222381034453374470544355994426;
    w[2] = 0.313706645877887287337962201987;
    w[3] = 0.362683783378361982965150449277;
    w[4] = 0.362683783378361982965150449277;
    w[5] = 0.313706645877887287337962201987;
    w[6] = 0.222381034453374470544355994426;
    w[7] = 0.101228536290376259152531354310;
  }
  else if ( n == 9 )
  {
    w[0] = 0.081274388361574411971892158111;
    w[1] = 0.18064816069485740405847203124;
    w[2] = 0.26061069640293546231874286942;
    w[3] = 0.31234707704000284006863040658;
    w[4] = 0.33023935500125976316452506929;
    w[5] = 0.31234707704000284006863040658;
    w[6] = 0.26061069640293546231874286942;
    w[7] = 0.18064816069485740405847203124;
    w[8] = 0.081274388361574411971892158111;
  }
  else if ( n == 10 )
  {
    w[0] = 0.066671344308688137593568809893;
    w[1] = 0.14945134915058059314577633966;
    w[2] = 0.21908636251598204399553493423;
    w[3] = 0.26926671930999635509122692157;
    w[4] = 0.29552422471475287017389299465;
    w[5] = 0.29552422471475287017389299465;
    w[6] = 0.26926671930999635509122692157;
    w[7] = 0.21908636251598204399553493423;
    w[8] = 0.14945134915058059314577633966;
    w[9] = 0.066671344308688137593568809893;
  }
  else if ( n == 11 )
  {
    w[0] = 0.055668567116173666482753720443;
    w[1] = 0.12558036946490462463469429922;
    w[2] = 0.18629021092773425142609764143;
    w[3] = 0.23319376459199047991852370484;
    w[4] = 0.26280454451024666218068886989;
    w[5] = 0.27292508677790063071448352834;
    w[6] = 0.26280454451024666218068886989;
    w[7] = 0.23319376459199047991852370484;
    w[8] = 0.18629021092773425142609764143;
    w[9] = 0.12558036946490462463469429922;
    w[10] = 0.055668567116173666482753720443;
  }
  else if ( n == 12 )
  {
    w[0] = 0.047175336386511827194615961485;
    w[1] = 0.10693932599531843096025471819;
    w[2] = 0.16007832854334622633465252954;
    w[3] = 0.20316742672306592174906445581;
    w[4] = 0.23349253653835480876084989892;
    w[5] = 0.24914704581340278500056243604;
    w[6] = 0.24914704581340278500056243604;
    w[7] = 0.23349253653835480876084989892;
    w[8] = 0.20316742672306592174906445581;
    w[9] = 0.16007832854334622633465252954;
    w[10] = 0.10693932599531843096025471819;
    w[11] = 0.047175336386511827194615961485;
  }
  else if ( n == 13 )
  {
    w[0] = 0.040484004765315879520021592201;
    w[1] = 0.092121499837728447914421775954;
    w[2] = 0.13887351021978723846360177687;
    w[3] = 0.17814598076194573828004669200;
    w[4] = 0.20781604753688850231252321931;
    w[5] = 0.22628318026289723841209018604;
    w[6] = 0.23255155323087391019458951527;
    w[7] = 0.22628318026289723841209018604;
    w[8] = 0.20781604753688850231252321931;
    w[9] = 0.17814598076194573828004669200;
    w[10] = 0.13887351021978723846360177687;
    w[11] = 0.092121499837728447914421775954;
    w[12] = 0.040484004765315879520021592201;
  }
  else if ( n == 14 )
  {
    w[0] = 0.035119460331751863031832876138;
    w[1] = 0.08015808715976020980563327706;
    w[2] = 0.12151857068790318468941480907;
    w[3] = 0.15720316715819353456960193862;
    w[4] = 0.18553839747793781374171659013;
    w[5] = 0.20519846372129560396592406566;
    w[6] = 0.21526385346315779019587644332;
    w[7] = 0.21526385346315779019587644332;
    w[8] = 0.20519846372129560396592406566;
    w[9] = 0.18553839747793781374171659013;
    w[10] = 0.15720316715819353456960193862;
    w[11] = 0.12151857068790318468941480907;
    w[12] = 0.08015808715976020980563327706;
    w[13] = 0.035119460331751863031832876138;
  }
  else if ( n == 15 )
  {
    w[0] = 0.030753241996117268354628393577;
    w[1] = 0.070366047488108124709267416451;
    w[2] = 0.107159220467171935011869546686;
    w[3] = 0.13957067792615431444780479451;
    w[4] = 0.16626920581699393355320086048;
    w[5] = 0.18616100001556221102680056187;
    w[6] = 0.19843148532711157645611832644;
    w[7] = 0.20257824192556127288062019997;
    w[8] = 0.19843148532711157645611832644;
    w[9] = 0.18616100001556221102680056187;
    w[10] = 0.16626920581699393355320086048;
    w[11] = 0.13957067792615431444780479451;
    w[12] = 0.107159220467171935011869546686;
    w[13] = 0.070366047488108124709267416451;
    w[14] = 0.030753241996117268354628393577;
  }
  else if ( n == 16 )
  {
    w[0] = 0.027152459411754094851780572456;
    w[1] = 0.062253523938647892862843836994;
    w[2] = 0.09515851168249278480992510760;
    w[3] = 0.12462897125553387205247628219;
    w[4] = 0.14959598881657673208150173055;
    w[5] = 0.16915651939500253818931207903;
    w[6] = 0.18260341504492358886676366797;
    w[7] = 0.18945061045506849628539672321;
    w[8] = 0.18945061045506849628539672321;
    w[9] = 0.18260341504492358886676366797;
    w[10] = 0.16915651939500253818931207903;
    w[11] = 0.14959598881657673208150173055;
    w[12] = 0.12462897125553387205247628219;
    w[13] = 0.09515851168249278480992510760;
    w[14] = 0.062253523938647892862843836994;
    w[15] = 0.027152459411754094851780572456;
  }
  else if ( n == 17 )
  {
    w[0] = 0.024148302868547931960110026288;
    w[1] = 0.055459529373987201129440165359;
    w[2] = 0.085036148317179180883535370191;
    w[3] = 0.111883847193403971094788385626;
    w[4] = 0.13513636846852547328631998170;
    w[5] = 0.15404576107681028808143159480;
    w[6] = 0.16800410215645004450997066379;
    w[7] = 0.17656270536699264632527099011;
    w[8] = 0.17944647035620652545826564426;
    w[9] = 0.17656270536699264632527099011;
    w[10] = 0.16800410215645004450997066379;
    w[11] = 0.15404576107681028808143159480;
    w[12] = 0.13513636846852547328631998170;
    w[13] = 0.111883847193403971094788385626;
    w[14] = 0.085036148317179180883535370191;
    w[15] = 0.055459529373987201129440165359;
    w[16] = 0.024148302868547931960110026288;
  }
  else if ( n == 18 )
  {
    w[0] = 0.021616013526483310313342710266;
    w[1] = 0.049714548894969796453334946203;
    w[2] = 0.07642573025488905652912967762;
    w[3] = 0.10094204410628716556281398492;
    w[4] = 0.12255520671147846018451912680;
    w[5] = 0.14064291467065065120473130375;
    w[6] = 0.15468467512626524492541800384;
    w[7] = 0.16427648374583272298605377647;
    w[8] = 0.16914238296314359184065647013;
    w[9] = 0.16914238296314359184065647013;
    w[10] = 0.16427648374583272298605377647;
    w[11] = 0.15468467512626524492541800384;
    w[12] = 0.14064291467065065120473130375;
    w[13] = 0.12255520671147846018451912680;
    w[14] = 0.10094204410628716556281398492;
    w[15] = 0.07642573025488905652912967762;
    w[16] = 0.049714548894969796453334946203;
    w[17] = 0.021616013526483310313342710266;
  }
  else if ( n == 19 )
  {
    w[0] = 0.019461788229726477036312041464;
    w[1] = 0.044814226765699600332838157402;
    w[2] = 0.069044542737641226580708258006;
    w[3] = 0.091490021622449999464462094124;
    w[4] = 0.111566645547333994716023901682;
    w[5] = 0.12875396253933622767551578486;
    w[6] = 0.14260670217360661177574610944;
    w[7] = 0.15276604206585966677885540090;
    w[8] = 0.15896884339395434764995643946;
    w[9] = 0.16105444984878369597916362532;
    w[10] = 0.15896884339395434764995643946;
    w[11] = 0.15276604206585966677885540090;
    w[12] = 0.14260670217360661177574610944;
    w[13] = 0.12875396253933622767551578486;
    w[14] = 0.111566645547333994716023901682;
    w[15] = 0.091490021622449999464462094124;
    w[16] = 0.069044542737641226580708258006;
    w[17] = 0.044814226765699600332838157402;
    w[18] = 0.019461788229726477036312041464;
  }
  else if ( n == 20 )
  {
    w[0] = 0.017614007139152118311861962352;
    w[1] = 0.040601429800386941331039952275;
    w[2] = 0.062672048334109063569506535187;
    w[3] = 0.08327674157670474872475814322;
    w[4] = 0.10193011981724043503675013548;
    w[5] = 0.11819453196151841731237737771;
    w[6] = 0.13168863844917662689849449975;
    w[7] = 0.14209610931838205132929832507;
    w[8] = 0.14917298647260374678782873700;
    w[9] = 0.15275338713072585069808433195;
    w[10] = 0.15275338713072585069808433195;
    w[11] = 0.14917298647260374678782873700;
    w[12] = 0.14209610931838205132929832507;
    w[13] = 0.13168863844917662689849449975;
    w[14] = 0.11819453196151841731237737771;
    w[15] = 0.10193011981724043503675013548;
    w[16] = 0.08327674157670474872475814322;
    w[17] = 0.062672048334109063569506535187;
    w[18] = 0.040601429800386941331039952275;
    w[19] = 0.017614007139152118311861962352;
  }
  else if ( n == 21 )
  {
    w[ 0] =   0.016017228257774333324224616858;
    w[ 1] =   0.036953789770852493799950668299;
    w[ 2] =   0.057134425426857208283635826472;
    w[ 3] =   0.076100113628379302017051653300;
    w[ 4] =   0.093444423456033861553289741114;
    w[ 5] =   0.108797299167148377663474578070;
    w[ 6] =   0.12183141605372853419536717713;
    w[ 7] =   0.13226893863333746178105257450;
    w[ 8] =   0.13988739479107315472213342387;
    w[9] =   0.14452440398997005906382716655;
    w[10] =   0.14608113364969042719198514768;
    w[11] =   0.14452440398997005906382716655;
    w[12] =   0.13988739479107315472213342387;
    w[13] =   0.13226893863333746178105257450;
    w[14] =   0.12183141605372853419536717713;
    w[15] =   0.108797299167148377663474578070;
    w[16] =   0.093444423456033861553289741114;
    w[17] =   0.076100113628379302017051653300;
    w[18] =   0.057134425426857208283635826472;
    w[19] =   0.036953789770852493799950668299;
    w[20] =   0.016017228257774333324224616858;
  }
  else if ( n == 22 )
  {
    w[0] = 0.014627995298272200684991098047;
    w[1] = 0.033774901584814154793302246866;
    w[2] = 0.052293335152683285940312051273;
    w[3] = 0.06979646842452048809496141893;
    w[4] = 0.08594160621706772741444368137;
    w[5] = 0.10041414444288096493207883783;
    w[6] = 0.11293229608053921839340060742;
    w[7] = 0.12325237681051242428556098615;
    w[8] = 0.13117350478706237073296499253;
    w[9] = 0.13654149834601517135257383123;
    w[10] = 0.13925187285563199337541024834;
    w[11] = 0.13925187285563199337541024834;
    w[12] = 0.13654149834601517135257383123;
    w[13] = 0.13117350478706237073296499253;
    w[14] = 0.12325237681051242428556098615;
    w[15] = 0.11293229608053921839340060742;
    w[16] = 0.10041414444288096493207883783;
    w[17] = 0.08594160621706772741444368137;
    w[18] = 0.06979646842452048809496141893;
    w[19] = 0.052293335152683285940312051273;
    w[20] = 0.033774901584814154793302246866;
    w[21] = 0.014627995298272200684991098047;
  }
  else if ( n == 23 )
  {
    w[0] = 0.013411859487141772081309493459;
    w[1] = 0.030988005856979444310694219642;
    w[2] = 0.048037671731084668571641071632;
    w[3] = 0.064232421408525852127169615159;
    w[4] = 0.079281411776718954922892524742;
    w[5] = 0.092915766060035147477018617370;
    w[6] = 0.104892091464541410074086185015;
    w[7] = 0.11499664022241136494164351293;
    w[8] = 0.12304908430672953046757840067;
    w[9] = 0.12890572218808214997859533940;
    w[10] = 0.13246203940469661737164246470;
    w[11] = 0.13365457218610617535145711055;
    w[12] = 0.13246203940469661737164246470;
    w[13] = 0.12890572218808214997859533940;
    w[14] = 0.12304908430672953046757840067;
    w[15] = 0.11499664022241136494164351293;
    w[16] = 0.104892091464541410074086185015;
    w[17] = 0.092915766060035147477018617370;
    w[18] = 0.079281411776718954922892524742;
    w[19] = 0.064232421408525852127169615159;
    w[20] = 0.048037671731084668571641071632;
    w[21] = 0.030988005856979444310694219642;
    w[22] = 0.013411859487141772081309493459;
  }
  else if ( n == 24 )
  {
    w[0] = 0.012341229799987199546805667070;
    w[1] = 0.028531388628933663181307815952;
    w[2] = 0.044277438817419806168602748211;
    w[3] = 0.059298584915436780746367758500;
    w[4] = 0.07334648141108030573403361525;
    w[5] = 0.08619016153195327591718520298;
    w[6] = 0.09761865210411388826988066446;
    w[7] = 0.10744427011596563478257734245;
    w[8] = 0.11550566805372560135334448391;
    w[9] = 0.12167047292780339120446315348;
    w[10] = 0.12583745634682829612137538251;
    w[11] = 0.12793819534675215697405616522;
    w[12] = 0.12793819534675215697405616522;
    w[13] = 0.12583745634682829612137538251;
    w[14] = 0.12167047292780339120446315348;
    w[15] = 0.11550566805372560135334448391;
    w[16] = 0.10744427011596563478257734245;
    w[17] = 0.09761865210411388826988066446;
    w[18] = 0.08619016153195327591718520298;
    w[19] = 0.07334648141108030573403361525;
    w[20] = 0.059298584915436780746367758500;
    w[21] = 0.044277438817419806168602748211;
    w[22] = 0.028531388628933663181307815952;
    w[23] = 0.012341229799987199546805667070;
  }
  else if ( n == 25 )
  {
    w[0] = 0.0113937985010262879479029641132;
    w[1] = 0.026354986615032137261901815295;
    w[2] = 0.040939156701306312655623487712;
    w[3] = 0.054904695975835191925936891541;
    w[4] = 0.068038333812356917207187185657;
    w[5] = 0.080140700335001018013234959669;
    w[6] = 0.091028261982963649811497220703;
    w[7] = 0.100535949067050644202206890393;
    w[8] = 0.108519624474263653116093957050;
    w[9] = 0.11485825914571164833932554587;
    w[10] = 0.11945576353578477222817812651;
    w[11] = 0.12224244299031004168895951895;
    w[12] = 0.12317605372671545120390287308;
    w[13] = 0.12224244299031004168895951895;
    w[14] = 0.11945576353578477222817812651;
    w[15] = 0.11485825914571164833932554587;
    w[16] = 0.108519624474263653116093957050;
    w[17] = 0.100535949067050644202206890393;
    w[18] = 0.091028261982963649811497220703;
    w[19] = 0.080140700335001018013234959669;
    w[20] = 0.068038333812356917207187185657;
    w[21] = 0.054904695975835191925936891541;
    w[22] = 0.040939156701306312655623487712;
    w[23] = 0.026354986615032137261901815295;
    w[24] = 0.0113937985010262879479029641132;
  }
  else if ( n == 26 )
  {
    w[0] = 0.010551372617343007155651187685;
    w[1] = 0.024417851092631908789615827520;
    w[2] = 0.037962383294362763950303141249;
    w[3] = 0.050975825297147811998319900724;
    w[4] = 0.063274046329574835539453689907;
    w[5] = 0.07468414976565974588707579610;
    w[6] = 0.08504589431348523921044776508;
    w[7] = 0.09421380035591414846366488307;
    w[8] = 0.10205916109442542323841407025;
    w[9] = 0.10847184052857659065657942673;
    w[10] = 0.11336181654631966654944071844;
    w[11] = 0.11666044348529658204466250754;
    w[12] = 0.11832141527926227651637108570;
    w[13] = 0.11832141527926227651637108570;
    w[14] = 0.11666044348529658204466250754;
    w[15] = 0.11336181654631966654944071844;
    w[16] = 0.10847184052857659065657942673;
    w[17] = 0.10205916109442542323841407025;
    w[18] = 0.09421380035591414846366488307;
    w[19] = 0.08504589431348523921044776508;
    w[20] = 0.07468414976565974588707579610;
    w[21] = 0.063274046329574835539453689907;
    w[22] = 0.050975825297147811998319900724;
    w[23] = 0.037962383294362763950303141249;
    w[24] = 0.024417851092631908789615827520;
    w[25] = 0.010551372617343007155651187685;
  }
  else if ( n == 27 )
  {
    w[0] = 0.0097989960512943602611500550912;
    w[1] = 0.022686231596180623196034206447;
    w[2] = 0.035297053757419711022578289305;
    w[3] = 0.047449412520615062704096710114;
    w[4] = 0.058983536859833599110300833720;
    w[5] = 0.069748823766245592984322888357;
    w[6] = 0.079604867773057771263074959010;
    w[7] = 0.088423158543756950194322802854;
    w[8] = 0.096088727370028507565652646558;
    w[9] = 0.102501637817745798671247711533;
    w[10] = 0.107578285788533187212162984427;
    w[11] = 0.111252488356845192672163096043;
    w[12] = 0.113476346108965148620369948092;
    w[13] = 0.11422086737895698904504573690;
    w[14] = 0.113476346108965148620369948092;
    w[15] = 0.111252488356845192672163096043;
    w[16] = 0.107578285788533187212162984427;
    w[17] = 0.102501637817745798671247711533;
    w[18] = 0.096088727370028507565652646558;
    w[19] = 0.088423158543756950194322802854;
    w[20] = 0.079604867773057771263074959010;
    w[21] = 0.069748823766245592984322888357;
    w[22] = 0.058983536859833599110300833720;
    w[23] = 0.047449412520615062704096710114;
    w[24] = 0.035297053757419711022578289305;
    w[25] = 0.022686231596180623196034206447;
    w[26] = 0.0097989960512943602611500550912;
  }
  else if ( n == 28 )
  {
    w[0] = 0.009124282593094517738816153923;
    w[1] = 0.021132112592771259751500380993;
    w[2] = 0.032901427782304379977630819171;
    w[3] = 0.044272934759004227839587877653;
    w[4] = 0.055107345675716745431482918227;
    w[5] = 0.06527292396699959579339756678;
    w[6] = 0.07464621423456877902393188717;
    w[7] = 0.08311341722890121839039649824;
    w[8] = 0.09057174439303284094218603134;
    w[9] = 0.09693065799792991585048900610;
    w[10] = 0.10211296757806076981421663851;
    w[11] = 0.10605576592284641791041643700;
    w[12] = 0.10871119225829413525357151930;
    w[13] = 0.11004701301647519628237626560;
    w[14] = 0.11004701301647519628237626560;
    w[15] = 0.10871119225829413525357151930;
    w[16] = 0.10605576592284641791041643700;
    w[17] = 0.10211296757806076981421663851;
    w[18] = 0.09693065799792991585048900610;
    w[19] = 0.09057174439303284094218603134;
    w[20] = 0.08311341722890121839039649824;
    w[21] = 0.07464621423456877902393188717;
    w[22] = 0.06527292396699959579339756678;
    w[23] = 0.055107345675716745431482918227;
    w[24] = 0.044272934759004227839587877653;
    w[25] = 0.032901427782304379977630819171;
    w[26] = 0.021132112592771259751500380993;
    w[27] = 0.009124282593094517738816153923;
  }
  else if ( n == 29 )
  {
    w[0] = 0.0085169038787464096542638133022;
    w[1] = 0.019732085056122705983859801640;
    w[2] = 0.030740492202093622644408525375;
    w[3] = 0.041402062518682836104830010114;
    w[4] = 0.051594826902497923912594381180;
    w[5] = 0.061203090657079138542109848024;
    w[6] = 0.070117933255051278569581486949;
    w[7] = 0.078238327135763783828144888660;
    w[8] = 0.085472257366172527545344849297;
    w[9] = 0.091737757139258763347966411077;
    w[10] = 0.096963834094408606301900074883;
    w[11] = 0.101091273759914966121820546907;
    w[12] = 0.104073310077729373913328471285;
    w[13] = 0.105876155097320941406591327852;
    w[14] = 0.10647938171831424424651112691;
    w[15] = 0.105876155097320941406591327852;
    w[16] = 0.104073310077729373913328471285;
    w[17] = 0.101091273759914966121820546907;
    w[18] = 0.096963834094408606301900074883;
    w[19] = 0.091737757139258763347966411077;
    w[20] = 0.085472257366172527545344849297;
    w[21] = 0.078238327135763783828144888660;
    w[22] = 0.070117933255051278569581486949;
    w[23] = 0.061203090657079138542109848024;
    w[24] = 0.051594826902497923912594381180;
    w[25] = 0.041402062518682836104830010114;
    w[26] = 0.030740492202093622644408525375;
    w[27] = 0.019732085056122705983859801640;
    w[28] = 0.0085169038787464096542638133022;
  }
  else if ( n == 30 )
  {
    w[0] = 0.007968192496166605615465883475;
    w[1] = 0.018466468311090959142302131912;
    w[2] = 0.028784707883323369349719179611;
    w[3] = 0.038799192569627049596801936446;
    w[4] = 0.048402672830594052902938140423;
    w[5] = 0.057493156217619066481721689402;
    w[6] = 0.06597422988218049512812851512;
    w[7] = 0.07375597473770520626824385002;
    w[8] = 0.08075589522942021535469493846;
    w[9] = 0.08689978720108297980238753072;
    w[10] = 0.09212252223778612871763270709;
    w[11] = 0.09636873717464425963946862635;
    w[12] = 0.09959342058679526706278028210;
    w[13] = 0.10176238974840550459642895217;
    w[14] = 0.10285265289355884034128563671;
    w[15] = 0.10285265289355884034128563671;
    w[16] = 0.10176238974840550459642895217;
    w[17] = 0.09959342058679526706278028210;
    w[18] = 0.09636873717464425963946862635;
    w[19] = 0.09212252223778612871763270709;
    w[20] = 0.08689978720108297980238753072;
    w[21] = 0.08075589522942021535469493846;
    w[22] = 0.07375597473770520626824385002;
    w[23] = 0.06597422988218049512812851512;
    w[24] = 0.057493156217619066481721689402;
    w[25] = 0.048402672830594052902938140423;
    w[26] = 0.038799192569627049596801936446;
    w[27] = 0.028784707883323369349719179611;
    w[28] = 0.018466468311090959142302131912;
    w[29] = 0.007968192496166605615465883475;
  }
  else if ( n == 31 )
  {
    w[0] = 0.0074708315792487758586968750322;
    w[1] = 0.017318620790310582463157996087;
    w[2] = 0.027009019184979421800608708092;
    w[3] = 0.036432273912385464024392010468;
    w[4] = 0.045493707527201102902315857895;
    w[5] = 0.054103082424916853711666259087;
    w[6] = 0.062174786561028426910343543687;
    w[7] = 0.069628583235410366167756126255;
    w[8] = 0.076390386598776616426357674901;
    w[9] = 0.082392991761589263903823367432;
    w[10] = 0.087576740608477876126198069695;
    w[11] = 0.091890113893641478215362871607;
    w[12] = 0.095290242912319512807204197488;
    w[13] = 0.097743335386328725093474010979;
    w[14] = 0.099225011226672307874875514429;
    w[15] = 0.09972054479342645142753383373;
    w[16] = 0.099225011226672307874875514429;
    w[17] = 0.097743335386328725093474010979;
    w[18] = 0.095290242912319512807204197488;
    w[19] = 0.091890113893641478215362871607;
    w[20] = 0.087576740608477876126198069695;
    w[21] = 0.082392991761589263903823367432;
    w[22] = 0.076390386598776616426357674901;
    w[23] = 0.069628583235410366167756126255;
    w[24] = 0.062174786561028426910343543687;
    w[25] = 0.054103082424916853711666259087;
    w[26] = 0.045493707527201102902315857895;
    w[27] = 0.036432273912385464024392010468;
    w[28] = 0.027009019184979421800608708092;
    w[29] = 0.017318620790310582463157996087;
    w[30] = 0.0074708315792487758586968750322;
  }
  else if ( n == 32 )
  {
    w[0] = 0.007018610009470096600407063739;
    w[1] = 0.016274394730905670605170562206;
    w[2] = 0.025392065309262059455752589789;
    w[3] = 0.034273862913021433102687732252;
    w[4] = 0.042835898022226680656878646606;
    w[5] = 0.050998059262376176196163244690;
    w[6] = 0.058684093478535547145283637300;
    w[7] = 0.06582222277636184683765006371;
    w[8] = 0.07234579410884850622539935648;
    w[9] = 0.07819389578707030647174091883;
    w[10] = 0.08331192422694675522219907460;
    w[11] = 0.08765209300440381114277146275;
    w[12] = 0.09117387869576388471286857711;
    w[13] = 0.09384439908080456563918023767;
    w[14] = 0.09563872007927485941908200220;
    w[15] = 0.09654008851472780056676483006;
    w[16] = 0.09654008851472780056676483006;
    w[17] = 0.09563872007927485941908200220;
    w[18] = 0.09384439908080456563918023767;
    w[19] = 0.09117387869576388471286857711;
    w[20] = 0.08765209300440381114277146275;
    w[21] = 0.08331192422694675522219907460;
    w[22] = 0.07819389578707030647174091883;
    w[23] = 0.07234579410884850622539935648;
    w[24] = 0.06582222277636184683765006371;
    w[25] = 0.058684093478535547145283637300;
    w[26] = 0.050998059262376176196163244690;
    w[27] = 0.042835898022226680656878646606;
    w[28] = 0.034273862913021433102687732252;
    w[29] = 0.025392065309262059455752589789;
    w[30] = 0.016274394730905670605170562206;
    w[31] = 0.007018610009470096600407063739;
  }
  else if ( n == 33 )
  {
    w[0] = 0.0066062278475873780586492352085;
    w[1] = 0.015321701512934676127945768534;
    w[2] = 0.023915548101749480350533257529;
    w[3] = 0.032300358632328953281561447250;
    w[4] = 0.040401541331669591563409790527;
    w[5] = 0.048147742818711695670146880138;
    w[6] = 0.055470846631663561284944495439;
    w[7] = 0.062306482530317480031627725771;
    w[8] = 0.068594572818656712805955073015;
    w[9] = 0.074279854843954149342472175919;
    w[10] = 0.079312364794886738363908384942;
    w[11] = 0.083647876067038707613928014518;
    w[12] = 0.087248287618844337607281670945;
    w[13] = 0.090081958660638577239743705500;
    w[14] = 0.092123986643316846213240977717;
    w[15] = 0.093356426065596116160999126274;
    w[16] = 0.09376844616020999656730454155;
    w[17] = 0.093356426065596116160999126274;
    w[18] = 0.092123986643316846213240977717;
    w[19] = 0.090081958660638577239743705500;
    w[20] = 0.087248287618844337607281670945;
    w[21] = 0.083647876067038707613928014518;
    w[22] = 0.079312364794886738363908384942;
    w[23] = 0.074279854843954149342472175919;
    w[24] = 0.068594572818656712805955073015;
    w[25] = 0.062306482530317480031627725771;
    w[26] = 0.055470846631663561284944495439;
    w[27] = 0.048147742818711695670146880138;
    w[28] = 0.040401541331669591563409790527;
    w[29] = 0.032300358632328953281561447250;
    w[30] = 0.023915548101749480350533257529;
    w[31] = 0.015321701512934676127945768534;
    w[32] = 0.0066062278475873780586492352085;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEGENDRE_LOOKUP_WEIGHTS - Fatal error!\n";
    std::cerr << "  Illegal value of N = " << n << "\n";
    std::cerr << "  Legal values are 1 through 33.\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

double *SandiaRules::legendre_zeros ( int order )

//**************************************************************************80
//
//  Purpose:
//
//    LEGENDRE_ZEROS returns the zeros of the Legendre polynomial of degree N.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 June 2011
//
//  Author:
//
//    Original FORTRAN77 version by Philip Davis, Philip Rabinowitz.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Philip Davis, Philip Rabinowitz,
//    Methods of Numerical Integration,
//    Second Edition,
//    Dover, 2007,
//    ISBN: 0486453391,
//    LC: QA299.3.D28.
//
//  Parameters:
//
//    Input, int ORDER, the order.
//    ORDER must be greater than 0.
//
//    Output, double LEGENDRE_ZEROS[ORDER], the zeros.
//
{
  double d1;
  double d2pn;
  double d3pn;
  double d4pn;
  double dp;
  double dpn;
  double e1;
  double fx;
  double h;
  int i;
  int iback;
  int k;
  int m;
  int mp1mi;
  int ncopy;
  int nmove;
  double p;
  double pi = 3.141592653589793;
  double pk;
  double pkm1;
  double pkp1;
  double t;
  double u;
  double v;
  double x0;
  double *xtab;
  double xtemp;

  xtab = new double[order];

  e1 = ( double ) ( order * ( order + 1 ) );

  m = ( order + 1 ) / 2;

  for ( i = 1; i <= m; i++ )
  {
    mp1mi = m + 1 - i;

    t = ( double ) ( 4 * i - 1 ) * pi / ( double ) ( 4 * order + 2 );

    x0 = std::cos ( t ) * ( 1.0 - ( 1.0 - 1.0 / ( double ) ( order ) ) 
      / ( double ) ( 8 * order * order ) );

    pkm1 = 1.0;
    pk = x0;

    for ( k = 2; k <= order; k++ )
    {
      pkp1 = 2.0 * x0 * pk - pkm1 - ( x0 * pk - pkm1 ) / ( double ) ( k );
      pkm1 = pk;
      pk = pkp1;
    }

    d1 = ( double ) ( order ) * ( pkm1 - x0 * pk );

    dpn = d1 / ( 1.0 - x0 * x0 );

    d2pn = ( 2.0 * x0 * dpn - e1 * pk ) / ( 1.0 - x0 * x0 );

    d3pn = ( 4.0 * x0 * d2pn + ( 2.0 - e1 ) * dpn ) / ( 1.0 - x0 * x0 );

    d4pn = ( 6.0 * x0 * d3pn + ( 6.0 - e1 ) * d2pn ) / ( 1.0 - x0 * x0 );

    u = pk / dpn;
    v = d2pn / dpn;
//
//  Initial approximation H:
//
    h = - u * ( 1.0 + 0.5 * u * ( v + u * ( v * v - d3pn / ( 3.0 * dpn ) ) ) );
//
//  Refine H using one step of Newton's method:
//
    p = pk + h * ( dpn + 0.5 * h * ( d2pn + h / 3.0 
      * ( d3pn + 0.25 * h * d4pn ) ) );

    dp = dpn + h * ( d2pn + 0.5 * h * ( d3pn + h * d4pn / 3.0 ) );

    h = h - p / dp;

    xtemp = x0 + h;

    xtab[mp1mi-1] = xtemp;

    fx = d1 - h * e1 * ( pk + 0.5 * h * ( dpn + h / 3.0 
      * ( d2pn + 0.25 * h * ( d3pn + 0.2 * h * d4pn ) ) ) );
  }

  if ( ( order % 2 ) == 1 )
  {
    xtab[0] = 0.0;
  }
//
//  Shift the data up.
//
  nmove = ( order + 1 ) / 2;
  ncopy = order - nmove;

  for ( i = 1; i <= nmove; i++ )
  {
    iback = order + 1 - i;
    xtab[iback-1] = xtab[iback-ncopy-1];
  }
//
//  Reflect values for the negative abscissas.
//
  for ( i = 1; i <= order - nmove; i++ )
  {
    xtab[i-1] = - xtab[order-i];
  }

  return xtab;
}
//**************************************************************************80

void SandiaRules::level_growth_to_order ( int dim_num, int level[], int rule[], 
  int growth[], int order[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEVEL_GROWTH_TO_ORDER: convert Level and Growth to Order.
//
//  Discussion:
//
//    This function is given level, rule, and growth information
//    for each dimension of a quadrature rule, and determines the
//    corresponding order of the rule in each dimension.
//
//    This is a revised version of LEVEL_GROWTH_TO_ORDER.
//
//    In particular, it revises the interpretation of the RULE vector as 
//    far as the values 10, 11, and 12 are concerned.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    16 October 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, int LEVEL[DIM_NUM], the 1D levels.
//
//    Input, int RULE[DIM_NUM], the rule in each dimension.
//     1, "CC",  Clenshaw Curtis, Closed Fully Nested.
//     2, "F2",  Fejer Type 2, Open Fully Nested.
//     3, "GP",  Gauss Patterson, Open Fully Nested.
//     4, "GL",  Gauss Legendre, Open Weakly Nested.
//     5, "GH",  Gauss Hermite, Open Weakly Nested.
//     6, "GGH", Generalized Gauss Hermite, Open Weakly Nested.
//     7, "LG",  Gauss Laguerre, Open Non Nested.
//     8, "GLG", Generalized Gauss Laguerre, Open Non Nested.
//     9, "GJ",  Gauss Jacobi, Open Non Nested.
//    10, "HGK", Hermite Genz-Keister, Open Fully Nested.
//    11, "UO",  User supplied Open, presumably Non Nested.
//    12, "UC",  User supplied Closed, presumably Non Nested.
//
//    Input, int GROWTH[DIM_NUM], the desired growth in each dimension.
//    0, "DF", default growth associated with this quadrature rule;
//    1, "SL", slow linear, L+1;
//    2  "SO", slow linear odd, O=1+2((L+1)/2)
//    3, "ML", moderate linear, 2L+1;
//    4, "SE", slow exponential;
//    5, "ME", moderate exponential;
//    6, "FE", full exponential.
//
//    Output, int ORDER[DIM_NUM], the 1D orders (number of points).
//
{
  int dim;
  int l;
  int o;
  static int o_hgk[6] = { 1, 3, 9, 19, 35, 43 };
  int p;
  static int p_hgk[6] = { 1, 5, 15, 29, 51, 67 };
//
//  Check the input.
//
  for ( dim = 0; dim < dim_num; dim++ )
  {
    if ( level[dim] < 0 )
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
      std::cerr << "  Negative value of LEVEL[DIM]!\n";
      std::cerr << "  LEVEL[" << dim << "] = " << level[dim] << "\n";
      std::exit ( 1 );
    }

    if ( rule[dim] < 1 || 12 < rule[dim] )
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
      std::cerr << "  Illegal value of RULE[DIM]!\n";
      std::cerr << "  RULE[" << dim << "] = " << rule[dim] << "\n";
      std::exit ( 1 );
    }

    if ( growth[dim] < 0 || 6 < growth[dim] )
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
      std::cerr << "  Illegal value of GROWTH[DIM]!\n";
      std::cerr << "  GROWTH[" << dim << "] = " << growth[dim] << "\n";
      std::exit ( 1 );
    }
  }
//
//  Compute the order vector.
//
  for ( dim = 0; dim < dim_num; dim++ )
  {
//
//  CC
//  Default is Moderate Exponential Growth.
//
    if ( rule[dim] == 1 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        if ( level[dim] == 0 )
        {
          o = 1;
        }
        else
        {
          o = 2;
          while ( o < 2 * level[dim] + 1 )
          {
            o = 2 * ( o - 1 ) + 1;
          }
        }
      }
      else if ( growth[dim] == 5 || growth[dim] == 0 )
      {
        if ( level[dim] == 0 )
        {
          o = 1;
        }
        else
        {
          o = 2;
          while ( o < 4 * level[dim] + 1 )
          {
            o = 2 * ( o - 1 ) + 1;
          }
        }
      }
      else if ( growth[dim] == 6 )
      {
        if ( level[dim] == 0 )
        {
          o = 1;
        }
        else
        {
          o = i4_power ( 2, level[dim] ) + 1;
        }
      }
    }
//
//  F2
//  Default is Moderate Exponential Growth.
//
    else if ( rule[dim] == 2 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        o = 1;
        while ( o < 2 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 5 || growth[dim] == 0 )
      {
        o = 1;
        while ( o < 4 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  GP
//  Default is Moderate Exponential Growth.
//
    else if ( rule[dim] == 3 )
    {
      if ( growth[dim] == 1 )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
        std::cerr << "  Growth rate 1 for rule 3 not available!\n";
        std::exit ( 1 );
      }
      else if ( growth[dim] == 2 )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
        std::cerr << "  Growth rate 2 for rule 3 not available!\n";
        std::exit ( 1 );
      }
      else if ( growth[dim] == 3 )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
        std::cerr << "  Growth rate 3 for rule 3 not available!\n";
        std::exit ( 1 );
      }
      else if ( growth[dim] == 4 )
      {
        if ( level[dim] == 0 )
        {
          o = 1;
        }
        else
        {
          p = 5;
          o = 3;
          while ( p < 2 * level[dim] + 1 )
          {
            p = 2 * p + 1;
            o = 2 * o + 1;
          }
        }
      }
      else if ( growth[dim] == 5 || growth[dim] == 0 )
      {
        if ( level[dim] == 0 )
        {
          o = 1;
        }
        else
        {
          p = 5;
          o = 3;
          while ( p < 4 * level[dim] + 1 )
          {
            p = 2 * p + 1;
            o = 2 * o + 1;
          }
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  GL
//  Default is Moderate Linear Growth.
//
    else if ( rule[dim] == 4 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 || growth[dim] == 0 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        o = 1;
        while ( 2 * o - 1 < 2 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 5 )
      {
        o = 1;
        while ( 2 * o - 1 < 4 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  GH
//  Default is Moderate Linear Growth.
//
    else if ( rule[dim] == 5 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 || growth[dim] == 0 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        o = 1;
        while ( 2 * o - 1 < 2 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 5 )
      {
        o = 1;
        while ( 2 * o - 1 < 4 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  GGH
//  Default is Moderate Linear Growth.
//
    else if ( rule[dim] == 6 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 || growth[dim] == 0 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        o = 1;
        while ( 2 * o - 1 < 2 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 5 )
      {
        o = 1;
        while ( 2 * o - 1 < 4 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  LG
//  Default is Moderate Linear Growth.
//
    else if ( rule[dim] == 7 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 || growth[dim] == 0 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        o = 1;
        while ( 2 * o - 1 < 2 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 5 )
      {
        o = 1;
        while ( 2 * o - 1 < 4 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  GLG
//  Default is Moderate Linear Growth.
//
    else if ( rule[dim] == 8 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 || growth[dim] == 0 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        o = 1;
        while ( 2 * o - 1 < 2 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 5 )
      {
        o = 1;
        while ( 2 * o - 1 < 4 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  GJ
//  Default is Moderate Linear Growth.
//
    else if ( rule[dim] == 9 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 || growth[dim] == 0 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        o = 1;
        while ( 2 * o - 1 < 2 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 5 )
      {
        o = 1;
        while ( 2 * o - 1 < 4 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  HGK
//  Default is Moderate Exponential Growth.
//  Exponential growth is interpreted to mean simply take successive rules.
//
    else if ( rule[dim] == 10 )
    {
      if ( growth[dim] == 1 )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
        std::cerr << "  Growth rate 1 for rule 10 not available!\n";
        std::exit ( 1 );
      }
      else if ( growth[dim] == 2 )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
        std::cerr << "  Growth rate 2 for rule 10 not available!\n";
        std::exit ( 1 );
      }
      else if ( growth[dim] == 3 )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
        std::cerr << "  Growth rate 3 for rule 10 not available!\n";
        std::exit ( 1 );
      }
      else if ( growth[dim] == 4 )
      {
        l = 0;
        p = p_hgk[l];
        o = o_hgk[l];
        while ( p < 2 * level[dim] + 1 )
        {
          l = l + 1;
          if ( 5 < l )
          {
            std::cerr << "\n";
            std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
            std::cerr << "  Hermite Genz-Keister maximum level exceeded.\n";
            std::exit ( 1 );
          }
          p = p_hgk[l];
          o = o_hgk[l];
        }
      }
      else if ( growth[dim] == 5 || growth[dim] == 0 )
      {
        l = 0;
        p = p_hgk[l];
        o = o_hgk[l];
        while ( p < 4 * level[dim] + 1 )
        {
          l = l + 1;
          if ( 5 < l )
          {
            std::cerr << "\n";
            std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
            std::cerr << "  Hermite Genz-Keister maximum level exceeded.\n";
            std::exit ( 1 );
          }
          p = p_hgk[l];
          o = o_hgk[l];
        }
      }
      else if ( growth[dim] == 6 )
      {
        l = level[dim];
        l = i4_max ( l, 0 );
        if ( 5 < l )
        {
          std::cerr << "\n";
          std::cerr << "LEVEL_GROWTH_TO_ORDER - Fatal error!\n";
          std::cerr << "  Hermite Genz-Keister maximum level exceeded.\n";
          std::exit ( 1 );
        }
        o = o_hgk[l];
      }
    }
//
//  UO
//  Default is Moderate Linear Growth.
//  We assume the rule is of OPEN type and that it
//  has a precision typical of Gauss rules.
//
    else if ( rule[dim] == 11 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 || growth[dim] == 0 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        o = 1;
        while ( 2 * o - 1 < 2 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 5 )
      {
        o = 1;
        while ( 2 * o - 1 < 4 * level[dim] + 1 )
        {
          o = 2 * o + 1;
        }
      }
      else if ( growth[dim] == 6 )
      {
        o = i4_power ( 2, level[dim] + 1 ) - 1;
      }
    }
//
//  UC
//  Default is Moderate Linear Growth.
//  We assume the rule is of CLOSED type and that it
//  has a precision typical of Clenshaw-Curtis rules.
//
    else if ( rule[dim] == 12 )
    {
      if ( growth[dim] == 1 )
      {
        o = level[dim] + 1;
      }
      else if ( growth[dim] == 2 )
      {
        o = 2 * ( ( level[dim] + 1 ) / 2 ) + 1;
      }
      else if ( growth[dim] == 3 || growth[dim] == 0 )
      {
        o = 2 * level[dim] + 1;
      }
      else if ( growth[dim] == 4 )
      {
        if ( level[dim] == 0 )
        {
          o = 1;
        }
        else
        {
          o = 2;
          while ( o < 2 * level[dim] + 1 )
          {
            o = 2 * ( o - 1 ) + 1;
          }
        }
      }
      else if ( growth[dim] == 5 )
      {
        if ( level[dim] == 0 )
        {
          o = 1;
        }
        else
        {
          o = 2;
          while ( o < 4 * level[dim] + 1 )
          {
            o = 2 * ( o - 1 ) + 1;
          }
        }
      }
      else if ( growth[dim] == 6 )
      {
        if ( level[dim] == 0 )
        {
          o = 1;
        }
        else
        {
          o = i4_power ( 2, level[dim] ) + 1;
        }
      }
    }
    order[dim] = o;
  }
  return;
}
//**************************************************************************80

void SandiaRules::level_to_order_default ( int dim_num, int level[], int rule[],
  int order[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_DEFAULT: default growth.
//
//  Discussion:
//
//    This function uses:
//
//    * exponential growth rates for fully nested quadrature rules,
//      ( "CC", "F2", "GP");
//
//    * linear growth rates for other rules.
//      ( "GL", "GH", "GGH", "LG", "GLG", "GJ", "GW" ).
//
//    * slow exponential growth alternative for fully nested rules:
//      ("CC_SE", "F2_SE", "GP_SE").
//
//    * moderate exponential growth alternative for fully nested rules:
//      ("CC_ME", "F2_ME", "GP_ME").
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, int LEVEL[DIM_NUM], the 1D levels.
//
//    Input, int RULE[DIM_NUM], the rule in each dimension.
//     1, "CC",  Clenshaw Curtis, Closed Fully Nested rule.
//     2, "F2",  Fejer Type 2, Open Fully Nested rule.
//     3, "GP",  Gauss Patterson, Open Fully Nested rule.
//     4, "GL",  Gauss Legendre, Open Weakly Nested rule.
//     5, "GH",  Gauss Hermite, Open Weakly Nested rule.
//     6, "GGH", Generalized Gauss Hermite, Open Weakly Nested rule.
//     7, "LG",  Gauss Laguerre, Open Non Nested rule.
//     8, "GLG", Generalized Gauss Laguerre, Open Non Nested rule.
//     9, "GJ",  Gauss Jacobi, Open Non Nested rule.
//    10, "GW",  Golub Welsch, (presumed) Open Non Nested rule.
//    11, "CC_SE", Clenshaw Curtis Slow Exponential, Closed Fully Nested rule.
//    12, "F2_SE", Fejer Type 2 Slow Exponential, Open Fully Nested rule.
//    13, "GP_SE", Gauss Patterson Slow Exponential, Open Fully Nested rule.
//    14, "CC_ME", Clenshaw Curtis Moderate Exponential, Closed Fully Nested rule.
//    15, "F2_ME", Fejer Type 2 Moderate Exponential, Open Fully Nested rule.
//    16, "GP_ME", Gauss Patterson Moderate Exponential, Open Fully Nested rule.
//    17, "CCN", Clenshaw Curtis Nested, Linear, Closed Fully Nested rule.
//
//    Output, int ORDER[DIM_NUM], the 1D orders (number of points).
//
{
  int dim;
  int o;
  int p;

  for ( dim = 0; dim < dim_num; dim++ )
  {
    if ( level[dim] < 0 )
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_TO_ORDER_DEFAULT - Fatal error!\n";
      std::cerr << "  Negative value of LEVEL[DIM]!\n";
      std::cerr << "  LEVEL[" << dim << "] = " << level[dim] << "\n";
      std::exit ( 1 );
    }
    else if ( rule[dim] == 1 )
    {
      if ( level[dim] == 0 )
      {
        order[dim] = 1;
      }
      else
      {
        order[dim] = i4_power ( 2, level[dim] ) + 1;
      }
    }
    else if ( rule[dim] == 2 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 3 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 4 )
    {
      order[dim] = 2 * level[dim] + 1;
    }
    else if ( rule[dim] == 5 )
    {
      order[dim] = 2 * level[dim] + 1;
    }
    else if ( rule[dim] == 6 )
    {
      order[dim] = 2 * level[dim] + 1;
    }
    else if ( rule[dim] == 7 )
    {
      order[dim] = 2 * level[dim] + 1;
    }
    else if ( rule[dim] == 8 )
    {
      order[dim] = 2 * level[dim] + 1;
    }
    else if ( rule[dim] == 9 )
    {
      order[dim] = 2 * level[dim] + 1;
    }
    else if ( rule[dim] == 10 )
    {
      order[dim] = 2 * level[dim] + 1;
    }
    else if ( rule[dim] == 11 )
    {
      if ( level[dim] == 0 )
      {
        o = 1;
      }
      else
      {
        o = 2;
        while ( o < 2 * level[dim] + 1 )
        {
          o = 2 * ( o - 1 ) + 1;
        }
      }
      order[dim] = o;
    }
    else if ( rule[dim] == 12 )
    {
      o = 1;
      while ( o < 2 * level[dim] + 1 )
      {
        o = 2 * o + 1;
      }
      order[dim] = o;
    }
    else if ( rule[dim] == 13 )
    {
      if ( level[dim] == 0 )
      {
        order[dim] = 1;
      }
      else
      {
        p = 5;
        o = 3;
        while ( p < 2 * level[dim] + 1 )
        {
          p = 2 * p + 1;
          o = 2 * o + 1;
        }
        order[dim] = o;
      }
    }
    else if ( rule[dim] == 14 )
    {
      if ( level[dim] == 0 )
      {
        o = 1;
      }
      else
      {
        o = 2;
        while ( o < 4 * level[dim] + 1 )
        {
          o = 2 * ( o - 1 ) + 1;
        }
      }
      order[dim] = o;
    }
    else if ( rule[dim] == 15 )
    {
      o = 1;
      while ( o < 4 * level[dim] + 1 )
      {
        o = 2 * o + 1;
      }
      order[dim] = o;
    }
    else if ( rule[dim] == 16 )
    {
      if ( level[dim] == 0 )
      {
        order[dim] = 1;
      }
      else
      {
        p = 5;
        o = 3;
        while ( p < 4 * level[dim] + 1 )
        {
          p = 2 * p + 1;
          o = 2 * o + 1;
        }
        order[dim] = o;
      }
    }
    else if ( rule[dim] == 17 )
    {
      order[dim] = 2 * level[dim] + 1;
    }
    else
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_TO_ORDER_DEFAULT - Fatal error!\n";
      std::cerr << "  Unexpected value of RULE["
           << dim << "] = " << rule[dim] << ".\n";
      std::exit ( 1 );
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::level_to_order_exponential ( int dim_num, int level[], int rule[],
  int order[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_EXPONENTIAL: exponential growth.
//
//  Discussion:
//
//    The user must preallocate space for the output array ORDER.
//
//    Closed rules:
//
//      O(0) = 1
//      O(L) = 2^L + 1;
//
//      O = 1, 3, 5, 9, 17, 33, ...
//
//    Open rules:
//
//      O(L) = 2^(L+1) - 1;
//
//      O = 1, 3, 7, 15, 31, 63, ...
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, int LEVEL[DIM_NUM], the 1D levels.
//
//    Input, int RULE[DIM_NUM], the rule in each dimension.
//     1, "CC",  Clenshaw Curtis, Closed Fully Nested rule.
//     2, "F2",  Fejer Type 2, Open Fully Nested rule.
//     3, "GP",  Gauss Patterson, Open Fully Nested rule.
//     4, "GL",  Gauss Legendre, Open Weakly Nested rule.
//     5, "GH",  Gauss Hermite, Open Weakly Nested rule.
//     6, "GGH", Generalized Gauss Hermite, Open Weakly Nested rule.
//     7, "LG",  Gauss Laguerre, Open Non Nested rule.
//     8, "GLG", Generalized Gauss Laguerre, Open Non Nested rule.
//     9, "GJ",  Gauss Jacobi, Open Non Nested rule.
//    10, "GW",  Golub Welsch, (presumed) Open Non Nested rule.
//    11, "CC_SE", Clenshaw Curtis Slow Exponential, Closed Fully Nested rule.
//    12, "F2_SE", Fejer Type 2 Slow Exponential, Open Fully Nested rule.
//    13, "GP_SE", Gauss Patterson Slow Exponential, Open Fully Nested rule.
//    14, "CC_ME", Clenshaw Curtis Moderate Exponential, Closed Fully Nested rule.
//    15, "F2_ME", Fejer Type 2 Moderate Exponential, Open Fully Nested rule.
//    16, "GP_ME", Gauss Patterson Moderate Exponential, Open Fully Nested rule.
//    17, "CCN", Clenshaw Curtis Nested, Linear, Closed Fully Nested rule.
//
//    Output, int ORDER[DIM_NUM], the 1D orders (number of points).
//
{
  int dim;

  for ( dim = 0; dim < dim_num; dim++ )
  {
    if ( level[dim] < 0 )
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_TO_ORDER_EXPONENTIAL - Fatal error!\n";
      std::cerr << "  Negative value of LEVEL[DIM]!\n";
      std::cerr << "  LEVEL[" << dim << "] = " << level[dim] << "\n";
      std::exit ( 1 );
    }
    else if ( rule[dim] == 1 )
    {
      if ( level[dim] == 0 )
      {
        order[dim] = 1;
      }
      else
      {
        order[dim] = i4_power ( 2, level[dim] ) + 1;
      }
    }
    else if ( rule[dim] == 2 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 3 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 4 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 5 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 6 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 7 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 8 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 9 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 10 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 11 )
    {
      if ( level[dim] == 0 )
      {
        order[dim] = 1;
      }
      else
      {
        order[dim] = i4_power ( 2, level[dim] ) + 1;
      }
    }
    else if ( rule[dim] == 12 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 13 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 14 )
    {
      if ( level[dim] == 0 )
      {
        order[dim] = 1;
      }
      else
      {
        order[dim] = i4_power ( 2, level[dim] ) + 1;
      }
    }
    else if ( rule[dim] == 15 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 16 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 ) - 1;
    }
    else if ( rule[dim] == 17 )
    {
      order[dim] = i4_power ( 2, level[dim] + 1 );
    }
    else
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_TO_ORDER_EXPONENTIAL - Fatal error!\n";
      std::cerr << "  Unexpected value of RULE["
           << dim << "] = " << rule[dim] << ".\n";
      std::exit ( 1 );
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::level_to_order_exponential_slow ( int dim_num, int level[], int rule[],
  int order[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_EXPONENTIAL_SLOW: slow exponential growth;
//
//  Discussion:
//
//    We seek a sequence of quadrature rules with two opposing constraints:
//    * a measured rise in polynomial precision with increasing level;
//    * a control on the increase in (new) points per level;
//
//    Essentially, we are trying to keep some of the advantages of nesting,
//    while moderating the cost of the explosive growth in order that occurs
//    due to the repeated order doubling of nesting.
//
//    We wish the number of points at a given level L to be "about" 2 * L + 1,
//    but we also wish the rules to be completely nested.
//
//    One way to do this is to start with a nested family of rules, whose
//    order will tend to grow exponentially (doubling from one to the next),
//    but simply to REPEAT each rule as many times as possible.  We move to
//    the next rule only when the desired precision 2 * L + 1 exceeds the
//    precision of the current rule.
//
//    For both the Clenshaw Curtis and Fejer Type 2 rules, the order and
//    precision are the same if the order is odd.   That is, an 11 point rule
//    will integrate exactly all polynomials up to and including degree 11.
//
//    For Gauss Patterson rules, the relationship between order and precision
//    is somewhat more complicated.  For that rule, we take the philosophy
//    that at each level L, we wish to choose the rule of smallest order
//    so that the precision of 2 * L + 1 is guaranteed.
//
//     L    2*L+1  CC Order    F2 Order    GP Order/Precision
//
//     0        1         1           1        1/1
//     1        3         3           3        3/5
//     2        5         5           7        3/5
//     3        7         9           7        7/11
//     4        9         9          15        7/11
//     5       11        17          15        7/11
//     6       13        17          15       15/23
//     7       15        17          15       15/23
//     8       17        17          31       15/23
//     9       19        33          31       15/23
//    10       21        33          31       15/23
//    11       23        33          31       15/23
//    12       25        33          31       31/47
//    13       27        33          31       31/47
//    14       29        33          31       31/47
//    15       31        33          31       31/47
//    16       33        33          63       31/47
//    17       35        65          63       31/47
//    18       37        65          63       31/47
//    19       39        65          63       31/47
//    20       41        65          63       31/47
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Knut Petras,
//    Smolyak Cubature of Given Polynomial Degree with Few Nodes
//    for Increasing Dimension,
//    Numerische Mathematik,
//    Volume 93, Number 4, February 2003, pages 729-753.
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, int LEVEL[DIM_NUM], the 1D levels.
//
//    Input, int RULE[DIM_NUM], the rule in each dimension.
//     1, "CC",  Clenshaw Curtis, Closed Fully Nested rule.
//     2, "F2",  Fejer Type 2, Open Fully Nested rule.
//     3, "GP",  Gauss Patterson, Open Fully Nested rule.
//     4, "GL",  Gauss Legendre, Open Weakly Nested rule.
//     5, "GH",  Gauss Hermite, Open Weakly Nested rule.
//     6, "GGH", Generalized Gauss Hermite, Open Weakly Nested rule.
//     7, "LG",  Gauss Laguerre, Open Non Nested rule.
//     8, "GLG", Generalized Gauss Laguerre, Open Non Nested rule.
//     9, "GJ",  Gauss Jacobi, Open Non Nested rule.
//    10, "GW",  Golub Welsch, (presumed) Open Non Nested rule.
//    11, "CC_SE", Clenshaw Curtis Slow Exponential, Closed Fully Nested rule.
//    12, "F2_SE", Fejer Type 2 Slow Exponential, Open Fully Nested rule.
//    13, "GP_SE", Gauss Patterson Slow Exponential, Open Fully Nested rule.
//    14, "CC_ME", Clenshaw Curtis Moderate Exponential, Closed Fully Nested rule.
//    15, "F2_ME", Fejer Type 2 Moderate Exponential, Open Fully Nested rule.
//    16, "GP_ME", Gauss Patterson Moderate Exponential, Open Fully Nested rule.
//    17, "CCN", Clenshaw Curtis Nested, Linear, Closed Fully Nested rule.
//
//    Output, int ORDER[DIM_NUM], the 1D orders (number of points).
//
{
  int dim;
  int o;
  int p;

  for ( dim = 0; dim < dim_num; dim++ )
  {
    if ( level[dim] < 0 )
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_TO_ORDER_EXPONENTIAL_SLOW - Fatal error!\n";
      std::cerr << "  Negative value of LEVEL[DIM]!\n";
      std::cerr << "  LEVEL[" << dim << "] = " << level[dim] << "\n";
      std::exit ( 1 );
    }
  }

  for ( dim = 0; dim < dim_num; dim++ )
  {
    if ( rule[dim] == 1 || rule[dim] == 11 || rule[dim] == 14 || rule[dim] == 17 )
    {
      if ( level[dim] == 0 )
      {
        o = 1;
      }
      else
      {
        o = 2;
        while ( o < 2 * level[dim] + 1 )
        {
          o = 2 * ( o - 1 ) + 1;
        }
      }
    }
    else if ( rule[dim] == 3 || rule[dim] == 13 || rule[dim] == 16 )
    {
      if ( level[dim] == 0 )
      {
        o = 1;
      }
      else
      {
        p = 5;
        o = 3;
        while ( p < 2 * level[dim] + 1 )
        {
          p = 2 * p + 1;
          o = 2 * o + 1;
        }
      }
    }
    else
    {
      o = 1;
      while ( o < 2 * level[dim] + 1 )
      {
        o = 2 * o + 1;
      }
    }
    order[dim] = o;
  }

  return;
}
//**************************************************************************80

void SandiaRules::level_to_order_linear ( int dim_num, int level[], int rule[],
  int order[] )

//**************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_LINEAR: linear growth.
//
//  Discussion:
//
//    The user must preallocate space for the output array ORDER.
//
//      O(L) = 2 * L + 1;
//
//      O = 1, 3, 5, 7, 9, ...
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    07 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, int LEVEL[DIM_NUM], the 1D levels.
//
//    Input, int RULE[DIM_NUM], the rule in each dimension.
//     1, "CC",  Clenshaw Curtis, Closed Fully Nested rule.
//     2, "F2",  Fejer Type 2, Open Fully Nested rule.
//     3, "GP",  Gauss Patterson, Open Fully Nested rule.
//     4, "GL",  Gauss Legendre, Open Weakly Nested rule.
//     5, "GH",  Gauss Hermite, Open Weakly Nested rule.
//     6, "GGH", Generalized Gauss Hermite, Open Weakly Nested rule.
//     7, "LG",  Gauss Laguerre, Open Non Nested rule.
//     8, "GLG", Generalized Gauss Laguerre, Open Non Nested rule.
//     9, "GJ",  Gauss Jacobi, Open Non Nested rule.
//    10, "GW",  Golub Welsch, (presumed) Open Non Nested rule.
//    11, "CC_SE", Clenshaw Curtis Slow Exponential, Closed Fully Nested rule.
//    12, "F2_SE", Fejer Type 2 Slow Exponential, Open Fully Nested rule.
//    13, "GP_SE", Gauss Patterson Slow Exponential, Open Fully Nested rule.
//    14, "CC_ME", Clenshaw Curtis Moderate Exponential, Closed Fully Nested rule.
//    15, "F2_ME", Fejer Type 2 Moderate Exponential, Open Fully Nested rule.
//    16, "GP_ME", Gauss Patterson Moderate Exponential, Open Fully Nested rule.
//    17, "CCN", Clenshaw Curtis Nested, Linear, Closed Fully Nested rule.
//
//    Output, int ORDER[DIM_NUM], the 1D orders (number of points).
//
{
  int dim;

  for ( dim = 0; dim < dim_num; dim++ )
  {
    if ( level[dim] < 0 )
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_TO_ORDER_LINEAR - Fatal error!\n";
      std::cerr << "  Negative value of LEVEL[DIM]!\n";
      std::cerr << "  LEVEL[" << dim << "] = " << level[dim] << "\n";
      std::exit ( 1 );
    }
  }

  for ( dim = 0; dim < dim_num; dim++ )
  {
    order[dim] = 2 * level[dim] + 1;
  }

  return;
}

//****************************************************************************80

int SandiaRules::level_to_order_exp_cc ( int level, int growth )

//****************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_EXP_CC is used for Clenshaw-Curtis type rules.
//
//  Discussion:
//
//    Rules of this type are assumed to be closed (including both endpoints
//    except for the level 0 rule) and having a precision
//    behavior typical of Clenshaw Curtis rules, namely, the ORDER-point
//    rule is exact for polynomials of degree less than ORDER, and if
//    ORDER is odd, then the exactness includes polynomials of degree ORDER
//    as well.
//
//    LEVEL  ORDER  ORDER  ORDER
//           G = 0  G = 1  G = 2
//    -----  -----  -----  -----
//        0      1      1      1
//        1      3      5      3
//        2      5      9      5
//        3      9     17      9
//        4      9     17     17
//        5     17     33     33
//        6     17     33     65
//        7     17     33    129
//        8     17     33    257       
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 December 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int LEVEL, the level of the rule.
//
//    Input, int GROWTH, the growth policy:
//    0, slow growth;
//    1, moderate growth;
//    2, full growth.
//
//    Output, int LEVEL_TO_ORDER_EXP_CC, the order of the rule.
//
{
  int o;
//
//  Slow exponential growth.
//
  if ( growth == 0 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = 2;
      while ( o < 2 * level + 1 )
      {
        o = 2 * ( o - 1 ) + 1;
      }
    }
  }
//
//  Moderate Exponential Growth.
//
  else if ( growth == 1 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = 2;
      while ( o < 4 * level + 1 )
      {
        o = 2 * ( o - 1 ) + 1;
      }
    }
  }
//
//  Full Exponential Growth.
//
  else if ( growth == 2 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = i4_power ( 2, level ) + 1;
    }
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEVEL_TO_ORDER_EXP_CC - Fatal error!\n";
    std::cerr << "  Illegal value of GROWTH = " << growth << "\n";
    std::exit ( 1 );
  }
  return o;
}
//****************************************************************************80

int SandiaRules::level_to_order_exp_f2 ( int level, int growth )

//****************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_EXP_F2 is used for Fejer 2 type rules.
//
//  Discussion:
//
//    Rules of this type are assumed to be open (not including either endpoint)
//    and having a precision behavior typical of Fejer Type 2
//    rules, namely, the ORDER-point rule is exact for polynomials of degree 
//    less than ORDER, and if ORDER is odd, then the exactness includes 
//    polynomials of degree ORDER as well.
// 
//    LEVEL  ORDER  ORDER  ORDER
//           G = 0  G = 1  G = 2
//
//        0      1      1      1
//        1      3      7      3
//        2      7     15      7
//        3      7     15     15
//        4     15     31     31
//        5     15     31     63
//        6     15     31    127
//        7     15     31    255
//        8     31     63    511  
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 December 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int LEVEL, the level of the rule.
//
//    Input, int GROWTH, the growth policy:
//    0, slow growth;
//    1, moderate growth;
//    2, full growth.
//
//    Output, int LEVEL_TO_ORDER_EXP_F2, the order of the rule.
//
{
  int o;
//
//  Slow exponential growth.
//
  if ( growth == 0 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = 1;
      while ( o < 2 * level + 1 )
      {
        o = 2 * o + 1;
      }
    }
  }
//
//  Moderate Exponential Growth.
//
  else if ( growth == 1 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = 1;
      while ( o < 4 * level + 1 )
      {
        o = 2 * o + 1;
      }
    }
  }
//
//  Full Exponential Growth.
//
  else if ( growth == 2 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = i4_power ( 2, level + 1 ) - 1;
    }
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEVEL_TO_ORDER_EXP_F2 - Fatal error!\n";
    std::cerr << "  Illegal value of GROWTH = " << growth << "\n";
    std::exit ( 1 );
  }
  return o;
}
//****************************************************************************80

int SandiaRules::level_to_order_exp_gauss ( int level, int growth )

//****************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_EXP_GAUSS is used for Gauss type rules.
//
//  Discussion:
//
//    Rules of this type are assumed to be open (not including either endpoint),
//    and having a precision behavior typical of Gauss rules, namely, the 
//    ORDER-point rule is exact for polynomials of degree less than 2 * ORDER.
//
//    LEVEL  ORDER  ORDER  ORDER
//           G = 0  G = 1  G = 2
//
//        0      1      1      1
//        1      3      3      3
//        2      3      7      7
//        3      7      7     15
//        4      7     15     31
//        5      7     15     63
//        6      7     15    127
//        7     15     15    255
//        8     15     31    511
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 December 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int LEVEL, the level of the rule.
//
//    Input, int GROWTH, the growth policy:
//    0, slow growth;
//    1, moderate growth;
//    2, full growth.
//
//    Output, int LEVEL_TO_ORDER_EXP_GAUSS, the order of the rule.
//
{
  int o;
//
//  Slow exponential growth.
//
  if ( growth == 0 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = 1;
      while ( 2 * o - 1 < 2 * level + 1 )
      {
        o = 2 * o + 1;
      }
    }
  }
//
//  Moderate Exponential Growth.
//
  else if ( growth == 1 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = 1;
      while ( 2 * o - 1 < 4 * level + 1 )
      {
        o = 2 * o + 1;
      }
    }
  }
//
//  Full Exponential Growth.
//
  else if ( growth == 2 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = i4_power ( 2, level + 1 ) - 1;
    }
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEVEL_TO_ORDER_EXP_GAUSS - Fatal error!\n";
    std::cerr << "  Illegal value of GROWTH = " << growth << "\n";
    std::exit ( 1 );
  }

  return o;
}
//****************************************************************************80

int SandiaRules::level_to_order_exp_gp ( int level, int growth )

//****************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_EXP_GP is used for Gauss-Patterson type rules.
//
//  Discussion:
//
//    Rules of this type are assumed to be open (not including either endpoint)
//    and having a precision behavior typical of Gauss Patterson rules.
//
//    Note that there are onlly 9 rules in the family, and so it is possible to
//    specify input for which the function will fail.
//
//    LEVEL  ORDER  ORDER  ORDER
//           G = 0  G = 1  G = 2
//
//        0      1      1      1
//        1      3      3      3
//        2      3      7      7
//        3      7     15     15
//        4      7     15     31
//        5      7     15     63
//        6     15     31    127
//        7     15     31    255
//        8     15     31    511
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 December 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int LEVEL, the level of the rule.
//
//    Input, int GROWTH, the growth policy:
//    0, slow growth;
//    1, moderate growth;
//    2, full growth.
//
//    Output, int LEVEL_TO_ORDER_EXP_GP, the order of the rule.
//
{
  int o;
  int p;
//
//  Slow exponential growth.
//
  if ( growth == 0 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      p = 5;
      o = 3;
      while ( p < 2 * level + 1 )
      {
        p = 2 * p + 1;
        o = 2 * o + 1;
        if ( 511 < o )
        {
          std::cerr << "\n";
          std::cerr << "LEVEL_TO_ORDER_EXP_GP - Fatal error!\n";
          std::cerr << "  Request for unavailable Patterson rule.\n";
          std::exit ( 1 );
        }
      }
    }
  }
//
//  Moderate Exponential Growth.
//
  else if ( growth == 1 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      p = 5;
      o = 3;
      while ( p < 4 * level + 1 )
      {
        p = 2 * p + 1;
        o = 2 * o + 1;
        if ( 511 < o )
        {
          std::cerr << "\n";
          std::cerr << "LEVEL_TO_ORDER_EXP_GP - Fatal error!\n";
          std::cerr << "  Request for unavailable Patterson rule.\n";
          std::exit ( 1 );
        }
      }
    }
  }
//
//  Full Exponential Growth.
//
  else if ( growth == 2 )
  {
    if ( level == 0 )
    {
      o = 1;
    }
    else
    {
      o = i4_power ( 2, level + 1 ) - 1;
      if ( 511 < o )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_TO_ORDER_EXP_GP - Fatal error!\n";
        std::cerr << "  Request for unavailable Patterson rule.\n";
        std::exit ( 1 );
      }
    }
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEVEL_TO_ORDER_EXP_GP - Fatal error!\n";
    std::cerr << "  Illegal value of GROWTH = " << growth << "\n";
    std::exit ( 1 );
  }

  return o;
}
//****************************************************************************80

int SandiaRules::level_to_order_exp_hgk ( int level, int growth )

//****************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_EXP_HGK is used for Hermite Genz-Keister type rules.
//
//  Discussion:
//
//    Rules of this type are assumed to be open (not including either endpoint)
//    and having a precision behavior typical of Hermite Genz-Keister rules.
//
//    Note that there are only 6 rules in the family, and so it is possible to
//    specify input for which the function will fail.
//
//    LEVEL  ORDER  ORDER  ORDER
//           G = 0  G = 1  G = 2
//
//        0      1      1      1
//        1      3      3      3
//        2      3      9      9
//        3      9      9     19
//        4      9     19     35
//        5      9     19     43
//        6      9     19     --
//        7      9     19     --
//        8     19     35     --
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 December 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int LEVEL, the level of the rule.
//
//    Input, int GROWTH, the growth policy:
//    0, slow growth;
//    1, moderate growth;
//    2, full growth.
//
//    Output, int LEVEL_TO_ORDER_EXP_HGK, the order of the rule.
//
{
  int l;
  int o;
  static int o_hgk[6] = { 1, 3, 9, 19, 35, 43 };
  int p;
  static int p_hgk[6] = { 1, 5, 15, 29, 51, 67 };
//
//  Slow exponential growth.
//
  if ( growth == 0 )
  {
    l = 0;
    p = p_hgk[l];
    o = o_hgk[l];
    while ( p < 2 * level + 1 )
    {
      l = l + 1;
      if ( 5 < l )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_TO_ORDER_EXP_HGK - Fatal error!\n";
        std::cerr << "  Hermite Genz-Keister maximum level exceeded.\n";
        std::exit ( 1 );
      }
      p = p_hgk[l];
      o = o_hgk[l];
    }
  }
  else if ( growth == 1 )
  {
    l = 0;
    p = p_hgk[l];
    o = o_hgk[l];
    while ( p < 4 * level + 1 )
    {
      l = l + 1;
      if ( 5 < l )
      {
        std::cerr << "\n";
        std::cerr << "LEVEL_TO_ORDER_EXP_HGK - Fatal error!\n";
        std::cerr << "  Hermite Genz-Keister maximum level exceeded.\n";
        std::exit ( 1 );
      }
      p = p_hgk[l];
      o = o_hgk[l];
    }
  }
  else if ( growth == 2 )
  {
    l = level;
    l = i4_max ( l, 0 );
    if ( 5 < l )
    {
      std::cerr << "\n";
      std::cerr << "LEVEL_TO_ORDER_EXP_HGK - Fatal error!\n";
      std::cerr << "  Hermite Genz-Keister maximum level exceeded.\n";
      std::exit ( 1 );
    }
    o = o_hgk[l];
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEVEL_TO_ORDER_EXP_HGK - Fatal error!\n";
    std::cerr << "  Illegal value of GROWTH = " << growth << "\n";
    std::exit ( 1 );
  }

  return o;
}
//****************************************************************************80

int SandiaRules::level_to_order_linear_nn ( int level, int growth )

//****************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_LINEAR_NN is used for non-nested Gauss type rules.
//
//  Discussion:
//
//    Rules of this type are assumed to be open (not including either endpoint),
//    non-nested, and having a precision behavior typical of Gauss rules.
//
//    LEVEL  ORDER  ORDER
//           G = 0  G = 1
//
//        0      1      1
//        1      2      3
//        2      3      5
//        3      4      7
//        4      5      9
//        5      6     11
//        6      7     13
//        7      8     15
//        8      9     17
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 December 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int LEVEL, the level of the rule.
//
//    Input, int GROWTH, the growth policy:
//    0, slow growth;
//    1, moderate growth;
//
//    Output, int LEVEL_TO_ORDER_LINEAR_NN, the order of the rule.
//
{
  int o;
//
//  Slow linear growth.
//
  if ( growth == 0 )
  {
    o = level + 1;
  }
//
//  Moderate linear growth.
//
  else if ( growth == 1 )
  {
    o = 2 * level + 1;
  }
  else if ( growth == 2 )
  {
    o = 2 * level + 1;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEVEL_TO_ORDER_LINEAR_NN - Fatal error!\n";
    std::cerr << "  Illegal value of GROWTH = " << growth << "\n";
    std::exit ( 1 );
  }
  return o;
}
//****************************************************************************80

int SandiaRules::level_to_order_linear_wn ( int level, int growth )

//****************************************************************************80
//
//  Purpose:
//
//    LEVEL_TO_ORDER_LINEAR_WN is used for weakly-nested Gauss type rules.
//
//  Discussion:
//
//    Rules of this type are assumed to be open (not including either endpoint),
//    nested, and having a precision behavior typical of Gauss rules.
//
//    We assume the rules are to be generated with an odd number of points,
//    and that all the rules will share a single point, namely 0.
//
//    Note that the "moderate growth" option for this function results in the
//    same values as the moderate growth option for LEVEL_TO_ORDER_LINEAR_NN.
//
//    LEVEL  ORDER  ORDER
//           G = 0  G = 1
//
//        0      1      1
//        1      3      3
//        2      3      5
//        3      5      7
//        4      5      9
//        5      7     11
//        6      7     13
//        7      9     15
//        8      9     17
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 January 2012
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int LEVEL, the level of the rule.
//
//    Input, int GROWTH, the growth policy:
//    0, slow growth;
//    1, moderate growth;
//
//    Output, int LEVEL_TO_ORDER_LINEAR_WN, the order of the rule.
//
{
  int o;
//
//  Slow growth.
//
  if ( growth == 0 )
  {
    o = 2 * ( ( level + 1 ) / 2 ) + 1;
  }
  else if ( growth == 1 )
  {
    o = 2 * level + 1;
  }
  else if ( growth == 2 )
  {
    o = 2 * level + 1;
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "LEVEL_TO_ORDER_LINEAR_WN - Fatal error!\n";
    std::cerr << "  Illegal value of GROWTH = " << growth << "\n";
    std::exit ( 1 );
  }
  return o;
}

//**************************************************************************80

void SandiaRules::nc_compute ( int n, double x_min, double x_max, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    NC_COMPUTE computes a Newton-Cotes quadrature rule.
//
//  Discussion:
//
//    For the interval [X_MIN,X_MAX], the Newton-Cotes quadrature rule
//    estimates
//
//      Integral ( X_MIN <= X <= X_MAX ) F(X) dX
//
//    using N abscissas X and weights W:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) ).
//
//    For the CLOSED rule, the abscissas include the end points.
//    For the OPEN rule, the abscissas do not include the end points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 November 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, double X_MIN, X_MAX, the endpoints of the interval.
//
//    Input, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  double *d;
  int i;
  int j;
  int k;
  double yvala;
  double yvalb;

  d = new double[n];

  for ( i = 0; i < n; i++ )
  {
//
//  Compute the Lagrange basis polynomial which is 1 at XTAB(I),
//  and zero at the other nodes.
//
    for ( j = 0; j < n; j++ )
    {
      d[j] = 0.0;
    }
    d[i] = 1.0;

    for ( j = 2; j <= n; j++ )
    {
      for ( k = j; k <= n; k++ )
      {
        d[n+j-k-1] = ( d[n+j-k-1-1] - d[n+j-k-1] ) / ( x[n+1-k-1] - x[n+j-k-1] );
      }
    }

    for ( j = 1; j <= n - 1; j++ )
    {
      for ( k = 1; k <= n - j; k++ )
      {
        d[n-k-1] = d[n-k-1] - x[n-k-j] * d[n-k];
      }
    }
//
//  Evaluate the antiderivative of the polynomial at the left and
//  right endpoints.
//
    yvala = d[n-1] / ( double ) ( n );
    for ( j = n - 2; 0 <= j; j-- )
    {
      yvala = yvala * x_min + d[j] / ( double ) ( j + 1 );
    }
    yvala = yvala * x_min;

    yvalb = d[n-1] / ( double ) ( n );
    for ( j = n - 2; 0 <= j; j-- )
    {
      yvalb = yvalb * x_max + d[j] / ( double ) ( j + 1 );
    }
    yvalb = yvalb * x_max;

    w[i] = yvalb - yvala;
  }

  delete [] d;

  return;
}
//**************************************************************************80

double *SandiaRules::nc_compute_new ( int n, double x_min, double x_max, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    NC_COMPUTE_NEW computes a Newton-Cotes quadrature rule.
//
//  Discussion:
//
//    For the interval [X_MIN,X_MAX], the Newton-Cotes quadrature rule
//    estimates
//
//      Integral ( X_MIN <= X <= X_MAX ) F(X) dX
//
//    using N abscissas X and weights W:
//
//      Sum ( 1 <= I <= N ) W(I) * F ( X(I) ).
//
//    For the CLOSED rule, the abscissas include the end points.
//    For the OPEN rule, the abscissas do not include the end points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 November 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Input, double X_MIN, X_MAX, the endpoints of the interval.
//
//    Input, double X[N], the abscissas.
//
//    Output, double NC_COMPUTE_NEW[N], the weights.
//
{
  double *d;
  int i;
  int j;
  int k;
  double *w;
  double yvala;
  double yvalb;

  d = new double[n];
  w = new double[n];

  for ( i = 0; i < n; i++ )
  {
//
//  Compute the Lagrange basis polynomial which is 1 at XTAB(I),
//  and zero at the other nodes.
//
    for ( j = 0; j < n; j++ )
    {
      d[j] = 0.0;
    }
    d[i] = 1.0;

    for ( j = 2; j <= n; j++ )
    {
      for ( k = j; k <= n; k++ )
      {
        d[n+j-k-1] = ( d[n+j-k-1-1] - d[n+j-k-1] ) / ( x[n+1-k-1] - x[n+j-k-1] );
      }
    }

    for ( j = 1; j <= n - 1; j++ )
    {
      for ( k = 1; k <= n - j; k++ )
      {
        d[n-k-1] = d[n-k-1] - x[n-k-j] * d[n-k];
      }
    }
//
//  Evaluate the antiderivative of the polynomial at the left and
//  right endpoints.
//
    yvala = d[n-1] / ( double ) ( n );
    for ( j = n - 2; 0 <= j; j-- )
    {
      yvala = yvala * x_min + d[j] / ( double ) ( j + 1 );
    }
    yvala = yvala * x_min;

    yvalb = d[n-1] / ( double ) ( n );
    for ( j = n - 2; 0 <= j; j-- )
    {
      yvalb = yvalb * x_max + d[j] / ( double ) ( j + 1 );
    }
    yvalb = yvalb * x_max;

    w[i] = yvalb - yvala;
  }

  delete [] d;

  return w;
}
//**************************************************************************80

void SandiaRules::ncc_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    NCC_COMPUTE_POINTS: points of a Newton-Cotes Closed quadrature rule.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    16 November 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
{
  int i;
  double x_max = 1.0;
  double x_min = -1.0;

  if ( n == 1 )
  {
    x[0] = ( x_max + x_min ) / 2.0;
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      x[i] = ( ( double ) ( n - i - 1 ) * x_min
             + ( double ) (     i     ) * x_max )
             / ( double ) ( n     - 1 );
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::ncc_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    NCC_COMPUTE_WEIGHTS: weights of a Newton-Cotes Closed quadrature rule.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    16 November 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double W[N], the weights.
//
{
  int i;
  double *x;
  double x_max = 1.0;
  double x_min = -1.0;

  if ( n == 1 )
  {
    w[0] = x_max - x_min;
  }
  else
  {
    x = new double[n];

    for ( i = 0; i < n; i++ )
    {
      x[i] = ( ( double ) ( n - i - 1 ) * x_min
             + ( double ) (     i     ) * x_max )
             / ( double ) ( n     - 1 );
    }
    nc_compute ( n, x_min, x_max, x, w );

    delete [] x;
  }
  return;
}
//**************************************************************************80

void SandiaRules::nco_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    NCO_COMPUTE_POINTS: points for a Newton-Cotes Open quadrature rule.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 November 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
{
  int i;
  double x_max = 1.0;
  double x_min = -1.0;

  for ( i = 0; i < n; i++ )
  {
    x[i] = ( ( double ) ( n - i     ) * x_min
           + ( double ) (   + i + 1 ) * x_max )
           / ( double ) ( n     + 1 );
  }

  return;
}
//**************************************************************************80

void SandiaRules::nco_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    NCO_COMPUTE_WEIGHTS: weights for a Newton-Cotes Open quadrature rule.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 November 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double W[N], the weights.
//
{
  //int i;
  double *x;
  double x_max = 1.0;
  double x_min = -1.0;

  x = new double[n];

  nco_compute_points ( n, x );

  nc_compute ( n, x_min, x_max, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::ncoh_compute_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    NCOH_COMPUTE_POINTS computes points for a Newton-Cotes "open half" quadrature rule.
//
//  Discussion:
//
//    The input value N is used to define N equal subintervals of [-1,+1].
//    The I-th abscissa is the center of the I-th subinterval.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double X[N], the abscissas.
//
{
  int i;
  const double x_max = 1.0;
  const double x_min = -1.0;

  for ( i = 0; i < n; i++ )
  {
    x[i] = ( ( double ) ( 2 * n - 2 * i - 1 ) * x_min   
           + ( double ) (         2 * i + 1 ) * x_max ) 
           / ( double ) ( 2 * n             );
  }

  return;
}
//**************************************************************************80

void SandiaRules::ncoh_compute_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    NCOH_COMPUTE_WEIGHTS computes weights for a Newton-Cotes "open half" quadrature rule.
//
//  Discussion:
//
//    The input value N is used to define N equal subintervals of [-1,+1].
//    The I-th abscissa is the center of the I-th subinterval.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order.
//
//    Output, double W[N], the weights.
//
{
  //int i;
  double *x;
  const double x_max = 1.0;
  const double x_min = -1.0;

  x = new double[n];

  ncoh_compute_points ( n, x );

  nc_compute ( n, x_min, x_max, x, w );

  delete [] x;

  return;
}
//**************************************************************************80

void SandiaRules::patterson_lookup ( int n, double x[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    PATTERSON_LOOKUP looks up Patterson quadrature points and weights.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    The rule is defined on [-1,1],    
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 February 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Prem Kythe, Michael Schaeferkotter,
//    Handbook of Computational Methods for Integration,
//    Chapman and Hall, 2004,
//    ISBN: 1-58488-428-2,
//    LC: QA299.3.K98.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    Legal values are 1, 3, 7, 15, 31, 63, 127, 255 and 511.
//
//    Output, double X[N], the abscissas.
//
//    Output, double W[N], the weights.
//
{
  patterson_lookup_points ( n, x );
  patterson_lookup_weights ( n, w );

  return;
}
//**************************************************************************80

void SandiaRules::patterson_lookup_points ( int n, double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    PATTERSON_LOOKUP_POINTS looks up Patterson quadrature points.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    The rule is defined on [-1,1],
//
//    These rules constitute a nested family.  The rules can integrate exactly
//    any polynomial of degree 1, 5, 11, 23, 47, 95, 191, 383 or 767, 
//    respectively.
//
//    The data for N = 511 was supplied by Dirk Laurie, and is derived
//    from a NAG Library function d01arf.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 September 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Prem Kythe, Michael Schaeferkotter,
//    Handbook of Computational Methods for Integration,
//    Chapman and Hall, 2004,
//    ISBN: 1-58488-428-2,
//    LC: QA299.3.K98.
//
//    NAG Library Documentation,
//    D01ARF,
//    The Numerical Algorithms Group.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    Legal values are 1, 3, 7, 15, 31, 63, 127, 255 and 511.
//
//    Output, double X[N], the abscissas.
//
{
  static double x_001[1] =
  {
     0.0
  };
  static double x_003[3] =
  {
    -0.77459666924148337704,
     0.0,
     0.77459666924148337704
  };
  static double x_007[7] =
  {
    -0.96049126870802028342,
    -0.77459666924148337704,
    -0.43424374934680255800,
     0.0,
     0.43424374934680255800,
     0.77459666924148337704,
     0.96049126870802028342
  };
  static double x_015[15] =
  {
    -0.99383196321275502221,
    -0.96049126870802028342,
    -0.88845923287225699889,
    -0.77459666924148337704,
    -0.62110294673722640294,
    -0.43424374934680255800,
    -0.22338668642896688163,
     0.0,
     0.22338668642896688163,
     0.43424374934680255800,
     0.62110294673722640294,
     0.77459666924148337704,
     0.88845923287225699889,
     0.96049126870802028342,
     0.99383196321275502221
  };
  static double x_031[31] =
  {
    -0.99909812496766759766,
    -0.99383196321275502221,
    -0.98153114955374010687,
    -0.96049126870802028342,
    -0.92965485742974005667,
    -0.88845923287225699889,
    -0.83672593816886873550,
    -0.77459666924148337704,
    -0.70249620649152707861,
    -0.62110294673722640294,
    -0.53131974364437562397,
    -0.43424374934680255800,
    -0.33113539325797683309,
    -0.22338668642896688163,
    -0.11248894313318662575,
     0.0,
     0.11248894313318662575,
     0.22338668642896688163,
     0.33113539325797683309,
     0.43424374934680255800,
     0.53131974364437562397,
     0.62110294673722640294,
     0.70249620649152707861,
     0.77459666924148337704,
     0.83672593816886873550,
     0.88845923287225699889,
     0.92965485742974005667,
     0.96049126870802028342,
     0.98153114955374010687,
     0.99383196321275502221,
     0.99909812496766759766
  };
  static double x_063[63] =
  {
    -0.99987288812035761194,
    -0.99909812496766759766,
    -0.99720625937222195908,
    -0.99383196321275502221,
    -0.98868475754742947994,
    -0.98153114955374010687,
    -0.97218287474858179658,
    -0.96049126870802028342,
    -0.94634285837340290515,
    -0.92965485742974005667,
    -0.91037115695700429250,
    -0.88845923287225699889,
    -0.86390793819369047715,
    -0.83672593816886873550,
    -0.80694053195021761186,
    -0.77459666924148337704,
    -0.73975604435269475868,
    -0.70249620649152707861,
    -0.66290966002478059546,
    -0.62110294673722640294,
    -0.57719571005204581484,
    -0.53131974364437562397,
    -0.48361802694584102756,
    -0.43424374934680255800,
    -0.38335932419873034692,
    -0.33113539325797683309,
    -0.27774982202182431507,
    -0.22338668642896688163,
    -0.16823525155220746498,
    -0.11248894313318662575,
    -0.056344313046592789972,
     0.0,
     0.056344313046592789972,
     0.11248894313318662575,
     0.16823525155220746498,
     0.22338668642896688163,
     0.27774982202182431507,
     0.33113539325797683309,
     0.38335932419873034692,
     0.43424374934680255800,
     0.48361802694584102756,
     0.53131974364437562397,
     0.57719571005204581484,
     0.62110294673722640294,
     0.66290966002478059546,
     0.70249620649152707861,
     0.73975604435269475868,
     0.77459666924148337704,
     0.80694053195021761186,
     0.83672593816886873550,
     0.86390793819369047715,
     0.88845923287225699889,
     0.91037115695700429250,
     0.92965485742974005667,
     0.94634285837340290515,
     0.96049126870802028342,
     0.97218287474858179658,
     0.98153114955374010687,
     0.98868475754742947994,
     0.99383196321275502221,
     0.99720625937222195908,
     0.99909812496766759766,
     0.99987288812035761194
  };
  static double x_127[127] =
  {
    -0.99998243035489159858,
    -0.99987288812035761194,
    -0.99959879967191068325,
    -0.99909812496766759766,
    -0.99831663531840739253,
    -0.99720625937222195908,
    -0.99572410469840718851,
    -0.99383196321275502221,
    -0.99149572117810613240,
    -0.98868475754742947994,
    -0.98537149959852037111,
    -0.98153114955374010687,
    -0.97714151463970571416,
    -0.97218287474858179658,
    -0.96663785155841656709,
    -0.96049126870802028342,
    -0.95373000642576113641,
    -0.94634285837340290515,
    -0.93832039777959288365,
    -0.92965485742974005667,
    -0.92034002547001242073,
    -0.91037115695700429250,
    -0.89974489977694003664,
    -0.88845923287225699889,
    -0.87651341448470526974,
    -0.86390793819369047715,
    -0.85064449476835027976,
    -0.83672593816886873550,
    -0.82215625436498040737,
    -0.80694053195021761186,
    -0.79108493379984836143,
    -0.77459666924148337704,
    -0.75748396638051363793,
    -0.73975604435269475868,
    -0.72142308537009891548,
    -0.70249620649152707861,
    -0.68298743109107922809,
    -0.66290966002478059546,
    -0.64227664250975951377,
    -0.62110294673722640294,
    -0.59940393024224289297,
    -0.57719571005204581484,
    -0.55449513263193254887,
    -0.53131974364437562397,
    -0.50768775753371660215,
    -0.48361802694584102756,
    -0.45913001198983233287,
    -0.43424374934680255800,
    -0.40897982122988867241,
    -0.38335932419873034692,
    -0.35740383783153215238,
    -0.33113539325797683309,
    -0.30457644155671404334,
    -0.27774982202182431507,
    -0.25067873030348317661,
    -0.22338668642896688163,
    -0.19589750271110015392,
    -0.16823525155220746498,
    -0.14042423315256017459,
    -0.11248894313318662575,
    -0.084454040083710883710,
    -0.056344313046592789972,
    -0.028184648949745694339,
     0.0,
     0.028184648949745694339,
     0.056344313046592789972,
     0.084454040083710883710,
     0.11248894313318662575,
     0.14042423315256017459,
     0.16823525155220746498,
     0.19589750271110015392,
     0.22338668642896688163,
     0.25067873030348317661,
     0.27774982202182431507,
     0.30457644155671404334,
     0.33113539325797683309,
     0.35740383783153215238,
     0.38335932419873034692,
     0.40897982122988867241,
     0.43424374934680255800,
     0.45913001198983233287,
     0.48361802694584102756,
     0.50768775753371660215,
     0.53131974364437562397,
     0.55449513263193254887,
     0.57719571005204581484,
     0.59940393024224289297,
     0.62110294673722640294,
     0.64227664250975951377,
     0.66290966002478059546,
     0.68298743109107922809,
     0.70249620649152707861,
     0.72142308537009891548,
     0.73975604435269475868,
     0.75748396638051363793,
     0.77459666924148337704,
     0.79108493379984836143,
     0.80694053195021761186,
     0.82215625436498040737,
     0.83672593816886873550,
     0.85064449476835027976,
     0.86390793819369047715,
     0.87651341448470526974,
     0.88845923287225699889,
     0.89974489977694003664,
     0.91037115695700429250,
     0.92034002547001242073,
     0.92965485742974005667,
     0.93832039777959288365,
     0.94634285837340290515,
     0.95373000642576113641,
     0.96049126870802028342,
     0.96663785155841656709,
     0.97218287474858179658,
     0.97714151463970571416,
     0.98153114955374010687,
     0.98537149959852037111,
     0.98868475754742947994,
     0.99149572117810613240,
     0.99383196321275502221,
     0.99572410469840718851,
     0.99720625937222195908,
     0.99831663531840739253,
     0.99909812496766759766,
     0.99959879967191068325,
     0.99987288812035761194,
     0.99998243035489159858
  };
  static double x_255[255] =
  {
    -0.99999759637974846462,
    -0.99998243035489159858,
    -0.99994399620705437576,
    -0.99987288812035761194,
    -0.99976049092443204733,
    -0.99959879967191068325,
    -0.99938033802502358193,
    -0.99909812496766759766,
    -0.99874561446809511470,
    -0.99831663531840739253,
    -0.99780535449595727456,
    -0.99720625937222195908,
    -0.99651414591489027385,
    -0.99572410469840718851,
    -0.99483150280062100052,
    -0.99383196321275502221,
    -0.99272134428278861533,
    -0.99149572117810613240,
    -0.99015137040077015918,
    -0.98868475754742947994,
    -0.98709252795403406719,
    -0.98537149959852037111,
    -0.98351865757863272876,
    -0.98153114955374010687,
    -0.97940628167086268381,
    -0.97714151463970571416,
    -0.97473445975240266776,
    -0.97218287474858179658,
    -0.96948465950245923177,
    -0.96663785155841656709,
    -0.96364062156981213252,
    -0.96049126870802028342,
    -0.95718821610986096274,
    -0.95373000642576113641,
    -0.95011529752129487656,
    -0.94634285837340290515,
    -0.94241156519108305981,
    -0.93832039777959288365,
    -0.93406843615772578800,
    -0.92965485742974005667,
    -0.92507893290707565236,
    -0.92034002547001242073,
    -0.91543758715576504064,
    -0.91037115695700429250,
    -0.90514035881326159519,
    -0.89974489977694003664,
    -0.89418456833555902286,
    -0.88845923287225699889,
    -0.88256884024734190684,
    -0.87651341448470526974,
    -0.87029305554811390585,
    -0.86390793819369047715,
    -0.85735831088623215653,
    -0.85064449476835027976,
    -0.84376688267270860104,
    -0.83672593816886873550,
    -0.82952219463740140018,
    -0.82215625436498040737,
    -0.81462878765513741344,
    -0.80694053195021761186,
    -0.79909229096084140180,
    -0.79108493379984836143,
    -0.78291939411828301639,
    -0.77459666924148337704,
    -0.76611781930376009072,
    -0.75748396638051363793,
    -0.74869629361693660282,
    -0.73975604435269475868,
    -0.73066452124218126133,
    -0.72142308537009891548,
    -0.71203315536225203459,
    -0.70249620649152707861,
    -0.69281376977911470289,
    -0.68298743109107922809,
    -0.67301883023041847920,
    -0.66290966002478059546,
    -0.65266166541001749610,
    -0.64227664250975951377,
    -0.63175643771119423041,
    -0.62110294673722640294,
    -0.61031811371518640016,
    -0.59940393024224289297,
    -0.58836243444766254143,
    -0.57719571005204581484,
    -0.56590588542365442262,
    -0.55449513263193254887,
    -0.54296566649831149049,
    -0.53131974364437562397,
    -0.51955966153745702199,
    -0.50768775753371660215,
    -0.49570640791876146017,
    -0.48361802694584102756,
    -0.47142506587165887693,
    -0.45913001198983233287,
    -0.44673538766202847374,
    -0.43424374934680255800,
    -0.42165768662616330006,
    -0.40897982122988867241,
    -0.39621280605761593918,
    -0.38335932419873034692,
    -0.37042208795007823014,
    -0.35740383783153215238,
    -0.34430734159943802278,
    -0.33113539325797683309,
    -0.31789081206847668318,
    -0.30457644155671404334,
    -0.29119514851824668196,
    -0.27774982202182431507,
    -0.26424337241092676194,
    -0.25067873030348317661,
    -0.23705884558982972721,
    -0.22338668642896688163,
    -0.20966523824318119477,
    -0.19589750271110015392,
    -0.18208649675925219825,
    -0.16823525155220746498,
    -0.15434681148137810869,
    -0.14042423315256017459,
    -0.12647058437230196685,
    -0.11248894313318662575,
    -0.098482396598119202090,
    -0.084454040083710883710,
    -0.070406976042855179063,
    -0.056344313046592789972,
    -0.042269164765363603212,
    -0.028184648949745694339,
    -0.014093886410782462614,
    0.0,
    0.014093886410782462614,
    0.028184648949745694339,
    0.042269164765363603212,
    0.056344313046592789972,
    0.070406976042855179063,
    0.084454040083710883710,
    0.098482396598119202090,
    0.11248894313318662575,
    0.12647058437230196685,
    0.14042423315256017459,
    0.15434681148137810869,
    0.16823525155220746498,
    0.18208649675925219825,
    0.19589750271110015392,
    0.20966523824318119477,
    0.22338668642896688163,
    0.23705884558982972721,
    0.25067873030348317661,
    0.26424337241092676194,
    0.27774982202182431507,
    0.29119514851824668196,
    0.30457644155671404334,
    0.31789081206847668318,
    0.33113539325797683309,
    0.34430734159943802278,
    0.35740383783153215238,
    0.37042208795007823014,
    0.38335932419873034692,
    0.39621280605761593918,
    0.40897982122988867241,
    0.42165768662616330006,
    0.43424374934680255800,
    0.44673538766202847374,
    0.45913001198983233287,
    0.47142506587165887693,
    0.48361802694584102756,
    0.49570640791876146017,
    0.50768775753371660215,
    0.51955966153745702199,
    0.53131974364437562397,
    0.54296566649831149049,
    0.55449513263193254887,
    0.56590588542365442262,
    0.57719571005204581484,
    0.58836243444766254143,
    0.59940393024224289297,
    0.61031811371518640016,
    0.62110294673722640294,
    0.63175643771119423041,
    0.64227664250975951377,
    0.65266166541001749610,
    0.66290966002478059546,
    0.67301883023041847920,
    0.68298743109107922809,
    0.69281376977911470289,
    0.70249620649152707861,
    0.71203315536225203459,
    0.72142308537009891548,
    0.73066452124218126133,
    0.73975604435269475868,
    0.74869629361693660282,
    0.75748396638051363793,
    0.76611781930376009072,
    0.77459666924148337704,
    0.78291939411828301639,
    0.79108493379984836143,
    0.79909229096084140180,
    0.80694053195021761186,
    0.81462878765513741344,
    0.82215625436498040737,
    0.82952219463740140018,
    0.83672593816886873550,
    0.84376688267270860104,
    0.85064449476835027976,
    0.85735831088623215653,
    0.86390793819369047715,
    0.87029305554811390585,
    0.87651341448470526974,
    0.88256884024734190684,
    0.88845923287225699889,
    0.89418456833555902286,
    0.89974489977694003664,
    0.90514035881326159519,
    0.91037115695700429250,
    0.91543758715576504064,
    0.92034002547001242073,
    0.92507893290707565236,
    0.92965485742974005667,
    0.93406843615772578800,
    0.93832039777959288365,
    0.94241156519108305981,
    0.94634285837340290515,
    0.95011529752129487656,
    0.95373000642576113641,
    0.95718821610986096274,
    0.96049126870802028342,
    0.96364062156981213252,
    0.96663785155841656709,
    0.96948465950245923177,
    0.97218287474858179658,
    0.97473445975240266776,
    0.97714151463970571416,
    0.97940628167086268381,
    0.98153114955374010687,
    0.98351865757863272876,
    0.98537149959852037111,
    0.98709252795403406719,
    0.98868475754742947994,
    0.99015137040077015918,
    0.99149572117810613240,
    0.99272134428278861533,
    0.99383196321275502221,
    0.99483150280062100052,
    0.99572410469840718851,
    0.99651414591489027385,
    0.99720625937222195908,
    0.99780535449595727456,
    0.99831663531840739253,
    0.99874561446809511470,
    0.99909812496766759766,
    0.99938033802502358193,
    0.99959879967191068325,
    0.99976049092443204733,
    0.99987288812035761194,
    0.99994399620705437576,
    0.99998243035489159858,
    0.99999759637974846462
  };
  static double x_511[511] =
  {
    -0.999999672956734384381,
    -0.999997596379748464620,
    -0.999992298136257588028,
    -0.999982430354891598580,
    -0.999966730098486276883,
    -0.999943996207054375764,
    -0.999913081144678282800,
    -0.999872888120357611938,
    -0.999822363679787739196,
    -0.999760490924432047330,
    -0.999686286448317731776,
    -0.999598799671910683252,
    -0.999497112467187190535,
    -0.999380338025023581928,
    -0.999247618943342473599,
    -0.999098124967667597662,
    -0.998931050830810562236,
    -0.998745614468095114704,
    -0.998541055697167906027,
    -0.998316635318407392531,
    -0.998071634524930323302,
    -0.997805354495957274562,
    -0.997517116063472399965,
    -0.997206259372221959076,
    -0.996872143485260161299,
    -0.996514145914890273849,
    -0.996131662079315037786,
    -0.995724104698407188509,
    -0.995290903148810302261,
    -0.994831502800621000519,
    -0.994345364356723405931,
    -0.993831963212755022209,
    -0.993290788851684966211,
    -0.992721344282788615328,
    -0.992123145530863117683,
    -0.991495721178106132399,
    -0.990838611958294243677,
    -0.990151370400770159181,
    -0.989433560520240838716,
    -0.988684757547429479939,
    -0.987904547695124280467,
    -0.987092527954034067190,
    -0.986248305913007552681,
    -0.985371499598520371114,
    -0.984461737328814534596,
    -0.983518657578632728762,
    -0.982541908851080604251,
    -0.981531149553740106867,
    -0.980486047876721339416,
    -0.979406281670862683806,
    -0.978291538324758539526,
    -0.977141514639705714156,
    -0.975955916702011753129,
    -0.974734459752402667761,
    -0.973476868052506926773,
    -0.972182874748581796578,
    -0.970852221732792443256,
    -0.969484659502459231771,
    -0.968079947017759947964,
    -0.966637851558416567092,
    -0.965158148579915665979,
    -0.963640621569812132521,
    -0.962085061904651475741,
    -0.960491268708020283423,
    -0.958859048710200221356,
    -0.957188216109860962736,
    -0.955478592438183697574,
    -0.953730006425761136415,
    -0.951942293872573589498,
    -0.950115297521294876558,
    -0.948248866934137357063,
    -0.946342858373402905148,
    -0.944397134685866648591,
    -0.942411565191083059813,
    -0.940386025573669721370,
    -0.938320397779592883655,
    -0.936214569916450806625,
    -0.934068436157725787999,
    -0.931881896650953639345,
    -0.929654857429740056670,
    -0.927387230329536696843,
    -0.925078932907075652364,
    -0.922729888363349241523,
    -0.920340025470012420730,
    -0.917909278499077501636,
    -0.915437587155765040644,
    -0.912924896514370590080,
    -0.910371156957004292498,
    -0.907776324115058903624,
    -0.905140358813261595189,
    -0.902463227016165675048,
    -0.899744899776940036639,
    -0.896985353188316590376,
    -0.894184568335559022859,
    -0.891342531251319871666,
    -0.888459232872256998890,
    -0.885534668997285008926,
    -0.882568840247341906842,
    -0.879561752026556262568,
    -0.876513414484705269742,
    -0.873423842480859310192,
    -0.870293055548113905851,
    -0.867121077859315215614,
    -0.863907938193690477146,
    -0.860653669904299969802,
    -0.857358310886232156525,
    -0.854021903545468625813,
    -0.850644494768350279758,
    -0.847226135891580884381,
    -0.843766882672708601038,
    -0.840266795261030442350,
    -0.836725938168868735503,
    -0.833144380243172624728,
    -0.829522194637401400178,
    -0.825859458783650001088,
    -0.822156254364980407373,
    -0.818412667287925807395,
    -0.814628787655137413436,
    -0.810804709738146594361,
    -0.806940531950217611856,
    -0.803036356819268687782,
    -0.799092290960841401800,
    -0.795108445051100526780,
    -0.791084933799848361435,
    -0.787021875923539422170,
    -0.782919394118283016385,
    -0.778777615032822744702,
    -0.774596669241483377036,
    -0.770376691217076824278,
    -0.766117819303760090717,
    -0.761820195689839149173,
    -0.757483966380513637926,
    -0.753109281170558142523,
    -0.748696293616936602823,
    -0.744245161011347082309,
    -0.739756044352694758677,
    -0.735229108319491547663,
    -0.730664521242181261329,
    -0.726062455075389632685,
    -0.721423085370098915485,
    -0.716746591245747095767,
    -0.712033155362252034587,
    -0.707282963891961103412,
    -0.702496206491527078610,
    -0.697673076273711232906,
    -0.692813769779114702895,
    -0.687918486947839325756,
    -0.682987431091079228087,
    -0.678020808862644517838,
    -0.673018830230418479199,
    -0.667981708447749702165,
    -0.662909660024780595461,
    -0.657802904699713735422,
    -0.652661665410017496101,
    -0.647486168263572388782,
    -0.642276642509759513774,
    -0.637033320510492495071,
    -0.631756437711194230414,
    -0.626446232611719746542,
    -0.621102946737226402941,
    -0.615726824608992638014,
    -0.610318113715186400156,
    -0.604877064481584353319,
    -0.599403930242242892974,
    -0.593898967210121954393,
    -0.588362434447662541434,
    -0.582794593837318850840,
    -0.577195710052045814844,
    -0.571566050525742833992,
    -0.565905885423654422623,
    -0.560215487612728441818,
    -0.554495132631932548866,
    -0.548745098662529448608,
    -0.542965666498311490492,
    -0.537157119515795115982,
    -0.531319743644375623972,
    -0.525453827336442687395,
    -0.519559661537457021993,
    -0.513637539655988578507,
    -0.507687757533716602155,
    -0.501710613415391878251,
    -0.495706407918761460170,
    -0.489675444004456155436,
    -0.483618026945841027562,
    -0.477534464298829155284,
    -0.471425065871658876934,
    -0.465290143694634735858,
    -0.459130011989832332874,
    -0.452944987140767283784,
    -0.446735387662028473742,
    -0.440501534168875795783,
    -0.434243749346802558002,
    -0.427962357921062742583,
    -0.421657686626163300056,
    -0.415330064175321663764,
    -0.408979821229888672409,
    -0.402607290368737092671,
    -0.396212806057615939183,
    -0.389796704618470795479,
    -0.383359324198730346916,
    -0.376901004740559344802,
    -0.370422087950078230138,
    -0.363922917266549655269,
    -0.357403837831532152376,
    -0.350865196458001209011,
    -0.344307341599438022777,
    -0.337730623318886219621,
    -0.331135393257976833093,
    -0.324522004605921855207,
    -0.317890812068476683182,
    -0.311242171836871800300,
    -0.304576441556714043335,
    -0.297893980296857823437,
    -0.291195148518246681964,
    -0.284480308042725577496,
    -0.277749822021824315065,
    -0.271004054905512543536,
    -0.264243372410926761945,
    -0.257468141491069790481,
    -0.250678730303483176613,
    -0.243875508178893021593,
    -0.237058845589829727213,
    -0.230229114119222177156,
    -0.223386686428966881628,
    -0.216531936228472628081,
    -0.209665238243181194766,
    -0.202786968183064697557,
    -0.195897502711100153915,
    -0.188997219411721861059,
    -0.182086496759252198246,
    -0.175165714086311475707,
    -0.168235251552207464982,
    -0.161295490111305257361,
    -0.154346811481378108692,
    -0.147389598111939940054,
    -0.140424233152560174594,
    -0.133451100421161601344,
    -0.126470584372301966851,
    -0.119483070065440005133,
    -0.112488943133186625746,
    -0.105488589749541988533,
    -0.984823965981192020903E-01,
    -0.914707508403553909095E-01,
    -0.844540400837108837102E-01,
    -0.774326523498572825675E-01,
    -0.704069760428551790633E-01,
    -0.633773999173222898797E-01,
    -0.563443130465927899720E-01,
    -0.493081047908686267156E-01,
    -0.422691647653636032124E-01,
    -0.352278828084410232603E-01,
    -0.281846489497456943394E-01,
    -0.211398533783310883350E-01,
    -0.140938864107824626142E-01,
    -0.704713845933674648514E-02,
    +0.000000000000000000000,
    +0.704713845933674648514E-02,
    +0.140938864107824626142E-01,
    +0.211398533783310883350E-01,
    +0.281846489497456943394E-01,
    +0.352278828084410232603E-01,
    +0.422691647653636032124E-01,
    +0.493081047908686267156E-01,
    +0.563443130465927899720E-01,
    +0.633773999173222898797E-01,
    +0.704069760428551790633E-01,
    +0.774326523498572825675E-01,
    +0.844540400837108837102E-01,
    +0.914707508403553909095E-01,
    +0.984823965981192020903E-01,
    +0.105488589749541988533,
    +0.112488943133186625746,
    +0.119483070065440005133,
    +0.126470584372301966851,
    +0.133451100421161601344,
    +0.140424233152560174594,
    +0.147389598111939940054,
    +0.154346811481378108692,
    +0.161295490111305257361,
    +0.168235251552207464982,
    +0.175165714086311475707,
    +0.182086496759252198246,
    +0.188997219411721861059,
    +0.195897502711100153915,
    +0.202786968183064697557,
    +0.209665238243181194766,
    +0.216531936228472628081,
    +0.223386686428966881628,
    +0.230229114119222177156,
    +0.237058845589829727213,
    +0.243875508178893021593,
    +0.250678730303483176613,
    +0.257468141491069790481,
    +0.264243372410926761945,
    +0.271004054905512543536,
    +0.277749822021824315065,
    +0.284480308042725577496,
    +0.291195148518246681964,
    +0.297893980296857823437,
    +0.304576441556714043335,
    +0.311242171836871800300,
    +0.317890812068476683182,
    +0.324522004605921855207,
    +0.331135393257976833093,
    +0.337730623318886219621,
    +0.344307341599438022777,
    +0.350865196458001209011,
    +0.357403837831532152376,
    +0.363922917266549655269,
    +0.370422087950078230138,
    +0.376901004740559344802,
    +0.383359324198730346916,
    +0.389796704618470795479,
    +0.396212806057615939183,
    +0.402607290368737092671,
    +0.408979821229888672409,
    +0.415330064175321663764,
    +0.421657686626163300056,
    +0.427962357921062742583,
    +0.434243749346802558002,
    +0.440501534168875795783,
    +0.446735387662028473742,
    +0.452944987140767283784,
    +0.459130011989832332874,
    +0.465290143694634735858,
    +0.471425065871658876934,
    +0.477534464298829155284,
    +0.483618026945841027562,
    +0.489675444004456155436,
    +0.495706407918761460170,
    +0.501710613415391878251,
    +0.507687757533716602155,
    +0.513637539655988578507,
    +0.519559661537457021993,
    +0.525453827336442687395,
    +0.531319743644375623972,
    +0.537157119515795115982,
    +0.542965666498311490492,
    +0.548745098662529448608,
    +0.554495132631932548866,
    +0.560215487612728441818,
    +0.565905885423654422623,
    +0.571566050525742833992,
    +0.577195710052045814844,
    +0.582794593837318850840,
    +0.588362434447662541434,
    +0.593898967210121954393,
    +0.599403930242242892974,
    +0.604877064481584353319,
    +0.610318113715186400156,
    +0.615726824608992638014,
    +0.621102946737226402941,
    +0.626446232611719746542,
    +0.631756437711194230414,
    +0.637033320510492495071,
    +0.642276642509759513774,
    +0.647486168263572388782,
    +0.652661665410017496101,
    +0.657802904699713735422,
    +0.662909660024780595461,
    +0.667981708447749702165,
    +0.673018830230418479199,
    +0.678020808862644517838,
    +0.682987431091079228087,
    +0.687918486947839325756,
    +0.692813769779114702895,
    +0.697673076273711232906,
    +0.702496206491527078610,
    +0.707282963891961103412,
    +0.712033155362252034587,
    +0.716746591245747095767,
    +0.721423085370098915485,
    +0.726062455075389632685,
    +0.730664521242181261329,
    +0.735229108319491547663,
    +0.739756044352694758677,
    +0.744245161011347082309,
    +0.748696293616936602823,
    +0.753109281170558142523,
    +0.757483966380513637926,
    +0.761820195689839149173,
    +0.766117819303760090717,
    +0.770376691217076824278,
    +0.774596669241483377036,
    +0.778777615032822744702,
    +0.782919394118283016385,
    +0.787021875923539422170,
    +0.791084933799848361435,
    +0.795108445051100526780,
    +0.799092290960841401800,
    +0.803036356819268687782,
    +0.806940531950217611856,
    +0.810804709738146594361,
    +0.814628787655137413436,
    +0.818412667287925807395,
    +0.822156254364980407373,
    +0.825859458783650001088,
    +0.829522194637401400178,
    +0.833144380243172624728,
    +0.836725938168868735503,
    +0.840266795261030442350,
    +0.843766882672708601038,
    +0.847226135891580884381,
    +0.850644494768350279758,
    +0.854021903545468625813,
    +0.857358310886232156525,
    +0.860653669904299969802,
    +0.863907938193690477146,
    +0.867121077859315215614,
    +0.870293055548113905851,
    +0.873423842480859310192,
    +0.876513414484705269742,
    +0.879561752026556262568,
    +0.882568840247341906842,
    +0.885534668997285008926,
    +0.888459232872256998890,
    +0.891342531251319871666,
    +0.894184568335559022859,
    +0.896985353188316590376,
    +0.899744899776940036639,
    +0.902463227016165675048,
    +0.905140358813261595189,
    +0.907776324115058903624,
    +0.910371156957004292498,
    +0.912924896514370590080,
    +0.915437587155765040644,
    +0.917909278499077501636,
    +0.920340025470012420730,
    +0.922729888363349241523,
    +0.925078932907075652364,
    +0.927387230329536696843,
    +0.929654857429740056670,
    +0.931881896650953639345,
    +0.934068436157725787999,
    +0.936214569916450806625,
    +0.938320397779592883655,
    +0.940386025573669721370,
    +0.942411565191083059813,
    +0.944397134685866648591,
    +0.946342858373402905148,
    +0.948248866934137357063,
    +0.950115297521294876558,
    +0.951942293872573589498,
    +0.953730006425761136415,
    +0.955478592438183697574,
    +0.957188216109860962736,
    +0.958859048710200221356,
    +0.960491268708020283423,
    +0.962085061904651475741,
    +0.963640621569812132521,
    +0.965158148579915665979,
    +0.966637851558416567092,
    +0.968079947017759947964,
    +0.969484659502459231771,
    +0.970852221732792443256,
    +0.972182874748581796578,
    +0.973476868052506926773,
    +0.974734459752402667761,
    +0.975955916702011753129,
    +0.977141514639705714156,
    +0.978291538324758539526,
    +0.979406281670862683806,
    +0.980486047876721339416,
    +0.981531149553740106867,
    +0.982541908851080604251,
    +0.983518657578632728762,
    +0.984461737328814534596,
    +0.985371499598520371114,
    +0.986248305913007552681,
    +0.987092527954034067190,
    +0.987904547695124280467,
    +0.988684757547429479939,
    +0.989433560520240838716,
    +0.990151370400770159181,
    +0.990838611958294243677,
    +0.991495721178106132399,
    +0.992123145530863117683,
    +0.992721344282788615328,
    +0.993290788851684966211,
    +0.993831963212755022209,
    +0.994345364356723405931,
    +0.994831502800621000519,
    +0.995290903148810302261,
    +0.995724104698407188509,
    +0.996131662079315037786,
    +0.996514145914890273849,
    +0.996872143485260161299,
    +0.997206259372221959076,
    +0.997517116063472399965,
    +0.997805354495957274562,
    +0.998071634524930323302,
    +0.998316635318407392531,
    +0.998541055697167906027,
    +0.998745614468095114704,
    +0.998931050830810562236,
    +0.999098124967667597662,
    +0.999247618943342473599,
    +0.999380338025023581928,
    +0.999497112467187190535,
    +0.999598799671910683252,
    +0.999686286448317731776,
    +0.999760490924432047330,
    +0.999822363679787739196,
    +0.999872888120357611938,
    +0.999913081144678282800,
    +0.999943996207054375764,
    +0.999966730098486276883,
    +0.999982430354891598580,
    +0.999992298136257588028,
    +0.999997596379748464620,
    +0.999999672956734384381
  };

  if ( n == 1 )
  {
    r8vec_copy ( n, x_001, x );
  }
  else if ( n == 3 )
  {
    r8vec_copy ( n, x_003, x );
  }
  else if ( n == 7 )
  {
    r8vec_copy ( n, x_007, x );
  }
  else if ( n == 15 )
  {
    r8vec_copy ( n, x_015, x );
  }
  else if ( n == 31 )
  {
    r8vec_copy ( n, x_031, x );
  }
  else if ( n == 63 )
  {
    r8vec_copy ( n, x_063, x );
  }
  else if ( n == 127 )
  {
    r8vec_copy ( n, x_127, x );
  }
  else if ( n == 255 )
  {
    r8vec_copy ( n, x_255, x );
  }
  else if ( n == 511 )
  {
    r8vec_copy ( n, x_511, x );
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "PATTERSON_LOOKUP_POINTS - Fatal error!\n";
    std::cerr << "  Unexpected value of N = " << n << "\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

void SandiaRules::patterson_lookup_points_np ( int n, int np, double p[], double x[] )

//**************************************************************************80
//
//  Purpose:
//
//    PATTERSON_LOOKUP_POINTS_NP looks up Patterson quadrature points.
//
//  Discussion:
//
//    Our convention is that the abscissas are numbered from left to right.
//
//    The rule is defined on [-1,1],
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 December 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Prem Kythe, Michael Schaeferkotter,
//    Handbook of Computational Methods for Integration,
//    Chapman and Hall, 2004,
//    ISBN: 1-58488-428-2,
//    LC: QA299.3.K98.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    Legal values are 1, 3, 7, 15, 31, 63, 127, 255 and 511.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double X[N], the abscissas.
//
{
  patterson_lookup_points ( n, x );

  return;
}
//**************************************************************************80

void SandiaRules::patterson_lookup_weights ( int n, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    PATTERSON_LOOKUP_WEIGHTS looks up Patterson quadrature weights.
//
//  Discussion:
//
//    The allowed orders are 1, 3, 7, 15, 31, 63, 127, 255 and 511.
//
//    The weights are positive, symmetric and should sum to 2.
//
//    The user must preallocate space for the output array W.
//
//    These rules constitute a nested family.  The rules can integrate exactly
//    any polynomial of degree 1, 5, 11, 23, 47, 95, 191, 383 or 767, 
//    respectively.
//
//    The data for N = 511 was supplied by Dirk Laurie, and is derived
//    from a NAG Library function d01arf.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 September 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Prem Kythe, Michael Schaeferkotter,
//    Handbook of Computational Methods for Integration,
//    Chapman and Hall, 2004,
//    ISBN: 1-58488-428-2,
//    LC: QA299.3.K98.
//
//    NAG Library Documentation,
//    D01ARF,
//    The Numerical Algorithms Group.
//
//    Thomas Patterson,
//    The Optimal Addition of Points to Quadrature Formulae,
//    Mathematics of Computation,
//    Volume 22, Number 104, October 1968, pages 847-856.
//
//  Parameters:
//
//    Input, int N, the order.
//    Legal values are 1, 3, 7, 15, 31, 63, 127, 255 or 511.
//
//    Output, double W[N], the weights.
//
{
  static double w_001[1] =
  {
    2.0
  };
  static double w_003[3] =
  {
    0.555555555555555555556,
    0.888888888888888888889,
    0.555555555555555555556
  };
  static double w_007[7] =
  {
    0.104656226026467265194,
    0.268488089868333440729,
    0.401397414775962222905,
    0.450916538658474142345,
    0.401397414775962222905,
    0.268488089868333440729,
    0.104656226026467265194
  };
  static double w_015[15] =
  {
    0.0170017196299402603390,
    0.0516032829970797396969,
    0.0929271953151245376859,
    0.134415255243784220360,
    0.171511909136391380787,
    0.200628529376989021034,
    0.219156858401587496404,
    0.225510499798206687386,
    0.219156858401587496404,
    0.200628529376989021034,
    0.171511909136391380787,
    0.134415255243784220360,
    0.0929271953151245376859,
    0.0516032829970797396969,
    0.0170017196299402603390
  };
  static double w_031[31] =
  {
    0.00254478079156187441540,
    0.00843456573932110624631,
    0.0164460498543878109338,
    0.0258075980961766535646,
    0.0359571033071293220968,
    0.0464628932617579865414,
    0.0569795094941233574122,
    0.0672077542959907035404,
    0.0768796204990035310427,
    0.0857559200499903511542,
    0.0936271099812644736167,
    0.100314278611795578771,
    0.105669893580234809744,
    0.109578421055924638237,
    0.111956873020953456880,
    0.112755256720768691607,
    0.111956873020953456880,
    0.109578421055924638237,
    0.105669893580234809744,
    0.100314278611795578771,
    0.0936271099812644736167,
    0.0857559200499903511542,
    0.0768796204990035310427,
    0.0672077542959907035404,
    0.0569795094941233574122,
    0.0464628932617579865414,
    0.0359571033071293220968,
    0.0258075980961766535646,
    0.0164460498543878109338,
    0.00843456573932110624631,
    0.00254478079156187441540
  };
  static double w_063[63] =
  {
    0.000363221481845530659694,
    0.00126515655623006801137,
    0.00257904979468568827243,
    0.00421763044155885483908,
    0.00611550682211724633968,
    0.00822300795723592966926,
    0.0104982469096213218983,
    0.0129038001003512656260,
    0.0154067504665594978021,
    0.0179785515681282703329,
    0.0205942339159127111492,
    0.0232314466399102694433,
    0.0258696793272147469108,
    0.0284897547458335486125,
    0.0310735511116879648799,
    0.0336038771482077305417,
    0.0360644327807825726401,
    0.0384398102494555320386,
    0.0407155101169443189339,
    0.0428779600250077344929,
    0.0449145316536321974143,
    0.0468135549906280124026,
    0.0485643304066731987159,
    0.0501571393058995374137,
    0.0515832539520484587768,
    0.0528349467901165198621,
    0.0539054993352660639269,
    0.0547892105279628650322,
    0.0554814043565593639878,
    0.0559784365104763194076,
    0.0562776998312543012726,
    0.0563776283603847173877,
    0.0562776998312543012726,
    0.0559784365104763194076,
    0.0554814043565593639878,
    0.0547892105279628650322,
    0.0539054993352660639269,
    0.0528349467901165198621,
    0.0515832539520484587768,
    0.0501571393058995374137,
    0.0485643304066731987159,
    0.0468135549906280124026,
    0.0449145316536321974143,
    0.0428779600250077344929,
    0.0407155101169443189339,
    0.0384398102494555320386,
    0.0360644327807825726401,
    0.0336038771482077305417,
    0.0310735511116879648799,
    0.0284897547458335486125,
    0.0258696793272147469108,
    0.0232314466399102694433,
    0.0205942339159127111492,
    0.0179785515681282703329,
    0.0154067504665594978021,
    0.0129038001003512656260,
    0.0104982469096213218983,
    0.00822300795723592966926,
    0.00611550682211724633968,
    0.00421763044155885483908,
    0.00257904979468568827243,
    0.00126515655623006801137,
    0.000363221481845530659694
  };
  static double w_127[127] =
  {
    0.0000505360952078625176247,
    0.000180739564445388357820,
    0.000377746646326984660274,
    0.000632607319362633544219,
    0.000938369848542381500794,
    0.00128952408261041739210,
    0.00168114286542146990631,
    0.00210881524572663287933,
    0.00256876494379402037313,
    0.00305775341017553113613,
    0.00357289278351729964938,
    0.00411150397865469304717,
    0.00467105037211432174741,
    0.00524912345480885912513,
    0.00584344987583563950756,
    0.00645190005017573692280,
    0.00707248999543355546805,
    0.00770337523327974184817,
    0.00834283875396815770558,
    0.00898927578406413572328,
    0.00964117772970253669530,
    0.0102971169579563555237,
    0.0109557333878379016480,
    0.0116157233199551347270,
    0.0122758305600827700870,
    0.0129348396636073734547,
    0.0135915710097655467896,
    0.0142448773729167743063,
    0.0148936416648151820348,
    0.0155367755558439824399,
    0.0161732187295777199419,
    0.0168019385741038652709,
    0.0174219301594641737472,
    0.0180322163903912863201,
    0.0186318482561387901863,
    0.0192199051247277660193,
    0.0197954950480974994880,
    0.0203577550584721594669,
    0.0209058514458120238522,
    0.0214389800125038672465,
    0.0219563663053178249393,
    0.0224572658268160987071,
    0.0229409642293877487608,
    0.0234067774953140062013,
    0.0238540521060385400804,
    0.0242821652033365993580,
    0.0246905247444876769091,
    0.0250785696529497687068,
    0.0254457699654647658126,
    0.0257916269760242293884,
    0.0261156733767060976805,
    0.0264174733950582599310,
    0.0266966229274503599062,
    0.0269527496676330319634,
    0.0271855132296247918192,
    0.0273946052639814325161,
    0.0275797495664818730349,
    0.0277407021782796819939,
    0.0278772514766137016085,
    0.0279892182552381597038,
    0.0280764557938172466068,
    0.0281388499156271506363,
    0.0281763190330166021307,
    0.0281888141801923586938,
    0.0281763190330166021307,
    0.0281388499156271506363,
    0.0280764557938172466068,
    0.0279892182552381597038,
    0.0278772514766137016085,
    0.0277407021782796819939,
    0.0275797495664818730349,
    0.0273946052639814325161,
    0.0271855132296247918192,
    0.0269527496676330319634,
    0.0266966229274503599062,
    0.0264174733950582599310,
    0.0261156733767060976805,
    0.0257916269760242293884,
    0.0254457699654647658126,
    0.0250785696529497687068,
    0.0246905247444876769091,
    0.0242821652033365993580,
    0.0238540521060385400804,
    0.0234067774953140062013,
    0.0229409642293877487608,
    0.0224572658268160987071,
    0.0219563663053178249393,
    0.0214389800125038672465,
    0.0209058514458120238522,
    0.0203577550584721594669,
    0.0197954950480974994880,
    0.0192199051247277660193,
    0.0186318482561387901863,
    0.0180322163903912863201,
    0.0174219301594641737472,
    0.0168019385741038652709,
    0.0161732187295777199419,
    0.0155367755558439824399,
    0.0148936416648151820348,
    0.0142448773729167743063,
    0.0135915710097655467896,
    0.0129348396636073734547,
    0.0122758305600827700870,
    0.0116157233199551347270,
    0.0109557333878379016480,
    0.0102971169579563555237,
    0.00964117772970253669530,
    0.00898927578406413572328,
    0.00834283875396815770558,
    0.00770337523327974184817,
    0.00707248999543355546805,
    0.00645190005017573692280,
    0.00584344987583563950756,
    0.00524912345480885912513,
    0.00467105037211432174741,
    0.00411150397865469304717,
    0.00357289278351729964938,
    0.00305775341017553113613,
    0.00256876494379402037313,
    0.00210881524572663287933,
    0.00168114286542146990631,
    0.00128952408261041739210,
    0.000938369848542381500794,
    0.000632607319362633544219,
    0.000377746646326984660274,
    0.000180739564445388357820,
    0.0000505360952078625176247
  };
  static double w_255[255] =
  {
    0.69379364324108267170E-05,
    0.25157870384280661489E-04,
    0.53275293669780613125E-04,
    0.90372734658751149261E-04,
    0.13575491094922871973E-03,
    0.18887326450650491366E-03,
    0.24921240048299729402E-03,
    0.31630366082226447689E-03,
    0.38974528447328229322E-03,
    0.46918492424785040975E-03,
    0.55429531493037471492E-03,
    0.64476204130572477933E-03,
    0.74028280424450333046E-03,
    0.84057143271072246365E-03,
    0.94536151685852538246E-03,
    0.10544076228633167722E-02,
    0.11674841174299594077E-02,
    0.12843824718970101768E-02,
    0.14049079956551446427E-02,
    0.15288767050877655684E-02,
    0.16561127281544526052E-02,
    0.17864463917586498247E-02,
    0.19197129710138724125E-02,
    0.20557519893273465236E-02,
    0.21944069253638388388E-02,
    0.23355251860571608737E-02,
    0.24789582266575679307E-02,
    0.26245617274044295626E-02,
    0.27721957645934509940E-02,
    0.29217249379178197538E-02,
    0.30730184347025783234E-02,
    0.32259500250878684614E-02,
    0.33803979910869203823E-02,
    0.35362449977167777340E-02,
    0.36933779170256508183E-02,
    0.38516876166398709241E-02,
    0.40110687240750233989E-02,
    0.41714193769840788528E-02,
    0.43326409680929828545E-02,
    0.44946378920320678616E-02,
    0.46573172997568547773E-02,
    0.48205888648512683476E-02,
    0.49843645647655386012E-02,
    0.51485584789781777618E-02,
    0.53130866051870565663E-02,
    0.54778666939189508240E-02,
    0.56428181013844441585E-02,
    0.58078616599775673635E-02,
    0.59729195655081658049E-02,
    0.61379152800413850435E-02,
    0.63027734490857587172E-02,
    0.64674198318036867274E-02,
    0.66317812429018878941E-02,
    0.67957855048827733948E-02,
    0.69593614093904229394E-02,
    0.71224386864583871532E-02,
    0.72849479805538070639E-02,
    0.74468208324075910174E-02,
    0.76079896657190565832E-02,
    0.77683877779219912200E-02,
    0.79279493342948491103E-02,
    0.80866093647888599710E-02,
    0.82443037630328680306E-02,
    0.84009692870519326354E-02,
    0.85565435613076896192E-02,
    0.87109650797320868736E-02,
    0.88641732094824942641E-02,
    0.90161081951956431600E-02,
    0.91667111635607884067E-02,
    0.93159241280693950932E-02,
    0.94636899938300652943E-02,
    0.96099525623638830097E-02,
    0.97546565363174114611E-02,
    0.98977475240487497440E-02,
    0.10039172044056840798E-01,
    0.10178877529236079733E-01,
    0.10316812330947621682E-01,
    0.10452925722906011926E-01,
    0.10587167904885197931E-01,
    0.10719490006251933623E-01,
    0.10849844089337314099E-01,
    0.10978183152658912470E-01,
    0.11104461134006926537E-01,
    0.11228632913408049354E-01,
    0.11350654315980596602E-01,
    0.11470482114693874380E-01,
    0.11588074033043952568E-01,
    0.11703388747657003101E-01,
    0.11816385890830235763E-01,
    0.11927026053019270040E-01,
    0.12035270785279562630E-01,
    0.12141082601668299679E-01,
    0.12244424981611985899E-01,
    0.12345262372243838455E-01,
    0.12443560190714035263E-01,
    0.12539284826474884353E-01,
    0.12632403643542078765E-01,
    0.12722884982732382906E-01,
    0.12810698163877361967E-01,
    0.12895813488012114694E-01,
    0.12978202239537399286E-01,
    0.13057836688353048840E-01,
    0.13134690091960152836E-01,
    0.13208736697529129966E-01,
    0.13279951743930530650E-01,
    0.13348311463725179953E-01,
    0.13413793085110098513E-01,
    0.13476374833816515982E-01,
    0.13536035934956213614E-01,
    0.13592756614812395910E-01,
    0.13646518102571291428E-01,
    0.13697302631990716258E-01,
    0.13745093443001896632E-01,
    0.13789874783240936517E-01,
    0.13831631909506428676E-01,
    0.13870351089139840997E-01,
    0.13906019601325461264E-01,
    0.13938625738306850804E-01,
    0.13968158806516938516E-01,
    0.13994609127619079852E-01,
    0.14017968039456608810E-01,
    0.14038227896908623303E-01,
    0.14055382072649964277E-01,
    0.14069424957813575318E-01,
    0.14080351962553661325E-01,
    0.14088159516508301065E-01,
    0.14092845069160408355E-01,
    0.14094407090096179347E-01,
    0.14092845069160408355E-01,
    0.14088159516508301065E-01,
    0.14080351962553661325E-01,
    0.14069424957813575318E-01,
    0.14055382072649964277E-01,
    0.14038227896908623303E-01,
    0.14017968039456608810E-01,
    0.13994609127619079852E-01,
    0.13968158806516938516E-01,
    0.13938625738306850804E-01,
    0.13906019601325461264E-01,
    0.13870351089139840997E-01,
    0.13831631909506428676E-01,
    0.13789874783240936517E-01,
    0.13745093443001896632E-01,
    0.13697302631990716258E-01,
    0.13646518102571291428E-01,
    0.13592756614812395910E-01,
    0.13536035934956213614E-01,
    0.13476374833816515982E-01,
    0.13413793085110098513E-01,
    0.13348311463725179953E-01,
    0.13279951743930530650E-01,
    0.13208736697529129966E-01,
    0.13134690091960152836E-01,
    0.13057836688353048840E-01,
    0.12978202239537399286E-01,
    0.12895813488012114694E-01,
    0.12810698163877361967E-01,
    0.12722884982732382906E-01,
    0.12632403643542078765E-01,
    0.12539284826474884353E-01,
    0.12443560190714035263E-01,
    0.12345262372243838455E-01,
    0.12244424981611985899E-01,
    0.12141082601668299679E-01,
    0.12035270785279562630E-01,
    0.11927026053019270040E-01,
    0.11816385890830235763E-01,
    0.11703388747657003101E-01,
    0.11588074033043952568E-01,
    0.11470482114693874380E-01,
    0.11350654315980596602E-01,
    0.11228632913408049354E-01,
    0.11104461134006926537E-01,
    0.10978183152658912470E-01,
    0.10849844089337314099E-01,
    0.10719490006251933623E-01,
    0.10587167904885197931E-01,
    0.10452925722906011926E-01,
    0.10316812330947621682E-01,
    0.10178877529236079733E-01,
    0.10039172044056840798E-01,
    0.98977475240487497440E-02,
    0.97546565363174114611E-02,
    0.96099525623638830097E-02,
    0.94636899938300652943E-02,
    0.93159241280693950932E-02,
    0.91667111635607884067E-02,
    0.90161081951956431600E-02,
    0.88641732094824942641E-02,
    0.87109650797320868736E-02,
    0.85565435613076896192E-02,
    0.84009692870519326354E-02,
    0.82443037630328680306E-02,
    0.80866093647888599710E-02,
    0.79279493342948491103E-02,
    0.77683877779219912200E-02,
    0.76079896657190565832E-02,
    0.74468208324075910174E-02,
    0.72849479805538070639E-02,
    0.71224386864583871532E-02,
    0.69593614093904229394E-02,
    0.67957855048827733948E-02,
    0.66317812429018878941E-02,
    0.64674198318036867274E-02,
    0.63027734490857587172E-02,
    0.61379152800413850435E-02,
    0.59729195655081658049E-02,
    0.58078616599775673635E-02,
    0.56428181013844441585E-02,
    0.54778666939189508240E-02,
    0.53130866051870565663E-02,
    0.51485584789781777618E-02,
    0.49843645647655386012E-02,
    0.48205888648512683476E-02,
    0.46573172997568547773E-02,
    0.44946378920320678616E-02,
    0.43326409680929828545E-02,
    0.41714193769840788528E-02,
    0.40110687240750233989E-02,
    0.38516876166398709241E-02,
    0.36933779170256508183E-02,
    0.35362449977167777340E-02,
    0.33803979910869203823E-02,
    0.32259500250878684614E-02,
    0.30730184347025783234E-02,
    0.29217249379178197538E-02,
    0.27721957645934509940E-02,
    0.26245617274044295626E-02,
    0.24789582266575679307E-02,
    0.23355251860571608737E-02,
    0.21944069253638388388E-02,
    0.20557519893273465236E-02,
    0.19197129710138724125E-02,
    0.17864463917586498247E-02,
    0.16561127281544526052E-02,
    0.15288767050877655684E-02,
    0.14049079956551446427E-02,
    0.12843824718970101768E-02,
    0.11674841174299594077E-02,
    0.10544076228633167722E-02,
    0.94536151685852538246E-03,
    0.84057143271072246365E-03,
    0.74028280424450333046E-03,
    0.64476204130572477933E-03,
    0.55429531493037471492E-03,
    0.46918492424785040975E-03,
    0.38974528447328229322E-03,
    0.31630366082226447689E-03,
    0.24921240048299729402E-03,
    0.18887326450650491366E-03,
    0.13575491094922871973E-03,
    0.90372734658751149261E-04,
    0.53275293669780613125E-04,
    0.25157870384280661489E-04,
    0.69379364324108267170E-05
  };
  static double w_511[511] =
  {
    0.945715933950007048827E-06,
    0.345456507169149134898E-05,
    0.736624069102321668857E-05,
    0.125792781889592743525E-04,
    0.190213681905875816679E-04,
    0.266376412339000901358E-04,
    0.353751372055189588628E-04,
    0.451863674126296143105E-04,
    0.560319507856164252140E-04,
    0.678774554733972416227E-04,
    0.806899228014035293851E-04,
    0.944366322532705527066E-04,
    0.109085545645741522051E-03,
    0.124606200241498368482E-03,
    0.140970302204104791413E-03,
    0.158151830411132242924E-03,
    0.176126765545083195474E-03,
    0.194872642236641146532E-03,
    0.214368090034216937149E-03,
    0.234592462123925204879E-03,
    0.255525589595236862014E-03,
    0.277147657465187357459E-03,
    0.299439176850911730874E-03,
    0.322381020652862389664E-03,
    0.345954492129903871350E-03,
    0.370141402122251665232E-03,
    0.394924138246873704434E-03,
    0.420285716355361231823E-03,
    0.446209810101403247488E-03,
    0.472680758429262691232E-03,
    0.499683553312800484519E-03,
    0.527203811431658386125E-03,
    0.555227733977307579715E-03,
    0.583742058714979703847E-03,
    0.612734008012225209294E-03,
    0.642191235948505088403E-03,
    0.672101776960108194646E-03,
    0.702453997827572321358E-03,
    0.733236554224767912055E-03,
    0.764438352543882784191E-03,
    0.796048517297550871506E-03,
    0.828056364077226302608E-03,
    0.860451377808527848128E-03,
    0.893223195879324912340E-03,
    0.926361595613111283368E-03,
    0.959856485506936206261E-03,
    0.993697899638760857945E-03,
    0.102787599466367326179E-02,
    0.106238104885340071375E-02,
    0.109720346268191941940E-02,
    0.113233376051597664917E-02,
    0.116776259302858043685E-02,
    0.120348074001265964881E-02,
    0.123947911332878396534E-02,
    0.127574875977346947345E-02,
    0.131228086370221478128E-02,
    0.134906674928353113127E-02,
    0.138609788229672549700E-02,
    0.142336587141720519900E-02,
    0.146086246895890987689E-02,
    0.149857957106456636214E-02,
    0.153650921735128916170E-02,
    0.157464359003212166189E-02,
    0.161297501254393423070E-02,
    0.165149594771914570655E-02,
    0.169019899554346019117E-02,
    0.172907689054461607168E-02,
    0.176812249885838886701E-02,
    0.180732881501808930079E-02,
    0.184668895851282540913E-02,
    0.188619617015808475394E-02,
    0.192584380831993546204E-02,
    0.196562534503150547732E-02,
    0.200553436203751169944E-02,
    0.204556454679958293446E-02,
    0.208570968849203942640E-02,
    0.212596367401472533045E-02,
    0.216632048404649142727E-02,
    0.220677418916003329194E-02,
    0.224731894601603393082E-02,
    0.228794899365195972378E-02,
    0.232865864987842738864E-02,
    0.236944230779380495146E-02,
    0.241029443242563417382E-02,
    0.245120955750556483923E-02,
    0.249218228238276930060E-02,
    0.253320726907925325750E-02,
    0.257427923948908888092E-02,
    0.261539297272236109225E-02,
    0.265654330259352828314E-02,
    0.269772511525294586667E-02,
    0.273893334695947541201E-02,
    0.278016298199139435045E-02,
    0.282140905069222207923E-02,
    0.286266662764757868253E-02,
    0.290393082998878368175E-02,
    0.294519681581857582284E-02,
    0.298645978275408290247E-02,
    0.302771496658198544480E-02,
    0.306895764002069252174E-02,
    0.311018311158427546158E-02,
    0.315138672454287935858E-02,
    0.319256385597434736790E-02,
    0.323370991590184336368E-02,
    0.327482034651233969564E-02,
    0.331589062145094394706E-02,
    0.335691624518616761342E-02,
    0.339789275244138669739E-02,
    0.343881570768790591876E-02,
    0.347968070469521146972E-02,
    0.352048336613417922682E-02,
    0.356121934322919357659E-02,
    0.360188431545532431869E-02,
    0.364247399027690353194E-02,
    0.368298410292403911967E-02,
    0.372341041620379550870E-02,
    0.376374872034296338241E-02,
    0.380399483285952829161E-02,
    0.384414459846013158917E-02,
    0.388419388896099560998E-02,
    0.392413860322995774660E-02,
    0.396397466714742455513E-02,
    0.400369803358421688562E-02,
    0.404330468239442998549E-02,
    0.408279062042157838350E-02,
    0.412215188151643401528E-02,
    0.416138452656509745764E-02,
    0.420048464352596631772E-02,
    0.423944834747438184434E-02,
    0.427827178065384480959E-02,
    0.431695111253279479928E-02,
    0.435548253986604343679E-02,
    0.439386228676004195260E-02,
    0.443208660474124713206E-02,
    0.447015177282692726900E-02,
    0.450805409759782158001E-02,
    0.454578991327213285488E-02,
    0.458335558178039420335E-02,
    0.462074749284080687482E-02,
    0.465796206403469754658E-02,
    0.469499574088179046532E-02,
    0.473184499691503264714E-02,
    0.476850633375474925263E-02,
    0.480497628118194150483E-02,
    0.484125139721057135214E-02,
    0.487732826815870573054E-02,
    0.491320350871841897367E-02,
    0.494887376202437487201E-02,
    0.498433569972103029914E-02,
    0.501958602202842039909E-02,
    0.505462145780650125058E-02,
    0.508943876461803986674E-02,
    0.512403472879005351831E-02,
    0.515840616547381084096E-02,
    0.519254991870341614863E-02,
    0.522646286145300596306E-02,
    0.526014189569259311205E-02,
    0.529358395244259896547E-02,
    0.532678599182711857974E-02,
    0.535974500312596681161E-02,
    0.539245800482555593606E-02,
    0.542492204466865704951E-02,
    0.545713419970309863995E-02,
    0.548909157632945623482E-02,
    0.552079131034778706457E-02,
    0.555223056700346326850E-02,
    0.558340654103215637610E-02,
    0.561431645670402467678E-02,
    0.564495756786715368885E-02,
    0.567532715799029830087E-02,
    0.570542254020497332312E-02,
    0.573524105734693719020E-02,
    0.576478008199711142954E-02,
    0.579403701652197628421E-02,
    0.582300929311348057702E-02,
    0.585169437382850155033E-02,
    0.588008975062788803205E-02,
    0.590819294541511788161E-02,
    0.593600151007459827614E-02,
    0.596351302650963502011E-02,
    0.599072510668009471472E-02,
    0.601763539263978131522E-02,
    0.604424155657354634589E-02,
    0.607054130083414983949E-02,
    0.609653235797888692923E-02,
    0.612221249080599294931E-02,
    0.614757949239083790214E-02,
    0.617263118612191922727E-02,
    0.619736542573665996342E-02,
    0.622178009535701763157E-02,
    0.624587310952490748541E-02,
    0.626964241323744217671E-02,
    0.629308598198198836688E-02,
    0.631620182177103938227E-02,
    0.633898796917690165912E-02,
    0.636144249136619145314E-02,
    0.638356348613413709795E-02,
    0.640534908193868098342E-02,
    0.642679743793437438922E-02,
    0.644790674400605734710E-02,
    0.646867522080231481688E-02,
    0.648910111976869964292E-02,
    0.650918272318071200827E-02,
    0.652891834417652442012E-02,
    0.654830632678944064054E-02,
    0.656734504598007641819E-02,
    0.658603290766824937794E-02,
    0.660436834876456498276E-02,
    0.662234983720168509457E-02,
    0.663997587196526532519E-02,
    0.665724498312454708217E-02,
    0.667415573186258997654E-02,
    0.669070671050613006584E-02,
    0.670689654255504925648E-02,
    0.672272388271144108036E-02,
    0.673818741690825799086E-02,
    0.675328586233752529078E-02,
    0.676801796747810680683E-02,
    0.678238251212300746082E-02,
    0.679637830740619795480E-02,
    0.681000419582894688374E-02,
    0.682325905128564571420E-02,
    0.683614177908911221841E-02,
    0.684865131599535812903E-02,
    0.686078663022780697951E-02,
    0.687254672150094831613E-02,
    0.688393062104341470995E-02,
    0.689493739162046825872E-02,
    0.690556612755588354803E-02,
    0.691581595475321433825E-02,
    0.692568603071643155621E-02,
    0.693517554456992049848E-02,
    0.694428371707782549438E-02,
    0.695300980066273063177E-02,
    0.696135307942366551493E-02,
    0.696931286915342540213E-02,
    0.697688851735519545845E-02,
    0.698407940325846925786E-02,
    0.699088493783425207545E-02,
    0.699730456380953992594E-02,
    0.700333775568106572820E-02,
    0.700898401972830440494E-02,
    0.701424289402572916425E-02,
    0.701911394845431165171E-02,
    0.702359678471225911031E-02,
    0.702769103632498213858E-02,
    0.703139636865428709508E-02,
    0.703471247890678765907E-02,
    0.703763909614153052319E-02,
    0.704017598127683066242E-02,
    0.704232292709631209597E-02,
    0.704407975825415053266E-02,
    0.704544633127951476780E-02,
    0.704642253458020417748E-02,
    0.704700828844548013730E-02,
    0.704720354504808967346E-02,
    0.704700828844548013730E-02,
    0.704642253458020417748E-02,
    0.704544633127951476780E-02,
    0.704407975825415053266E-02,
    0.704232292709631209597E-02,
    0.704017598127683066242E-02,
    0.703763909614153052319E-02,
    0.703471247890678765907E-02,
    0.703139636865428709508E-02,
    0.702769103632498213858E-02,
    0.702359678471225911031E-02,
    0.701911394845431165171E-02,
    0.701424289402572916425E-02,
    0.700898401972830440494E-02,
    0.700333775568106572820E-02,
    0.699730456380953992594E-02,
    0.699088493783425207545E-02,
    0.698407940325846925786E-02,
    0.697688851735519545845E-02,
    0.696931286915342540213E-02,
    0.696135307942366551493E-02,
    0.695300980066273063177E-02,
    0.694428371707782549438E-02,
    0.693517554456992049848E-02,
    0.692568603071643155621E-02,
    0.691581595475321433825E-02,
    0.690556612755588354803E-02,
    0.689493739162046825872E-02,
    0.688393062104341470995E-02,
    0.687254672150094831613E-02,
    0.686078663022780697951E-02,
    0.684865131599535812903E-02,
    0.683614177908911221841E-02,
    0.682325905128564571420E-02,
    0.681000419582894688374E-02,
    0.679637830740619795480E-02,
    0.678238251212300746082E-02,
    0.676801796747810680683E-02,
    0.675328586233752529078E-02,
    0.673818741690825799086E-02,
    0.672272388271144108036E-02,
    0.670689654255504925648E-02,
    0.669070671050613006584E-02,
    0.667415573186258997654E-02,
    0.665724498312454708217E-02,
    0.663997587196526532519E-02,
    0.662234983720168509457E-02,
    0.660436834876456498276E-02,
    0.658603290766824937794E-02,
    0.656734504598007641819E-02,
    0.654830632678944064054E-02,
    0.652891834417652442012E-02,
    0.650918272318071200827E-02,
    0.648910111976869964292E-02,
    0.646867522080231481688E-02,
    0.644790674400605734710E-02,
    0.642679743793437438922E-02,
    0.640534908193868098342E-02,
    0.638356348613413709795E-02,
    0.636144249136619145314E-02,
    0.633898796917690165912E-02,
    0.631620182177103938227E-02,
    0.629308598198198836688E-02,
    0.626964241323744217671E-02,
    0.624587310952490748541E-02,
    0.622178009535701763157E-02,
    0.619736542573665996342E-02,
    0.617263118612191922727E-02,
    0.614757949239083790214E-02,
    0.612221249080599294931E-02,
    0.609653235797888692923E-02,
    0.607054130083414983949E-02,
    0.604424155657354634589E-02,
    0.601763539263978131522E-02,
    0.599072510668009471472E-02,
    0.596351302650963502011E-02,
    0.593600151007459827614E-02,
    0.590819294541511788161E-02,
    0.588008975062788803205E-02,
    0.585169437382850155033E-02,
    0.582300929311348057702E-02,
    0.579403701652197628421E-02,
    0.576478008199711142954E-02,
    0.573524105734693719020E-02,
    0.570542254020497332312E-02,
    0.567532715799029830087E-02,
    0.564495756786715368885E-02,
    0.561431645670402467678E-02,
    0.558340654103215637610E-02,
    0.555223056700346326850E-02,
    0.552079131034778706457E-02,
    0.548909157632945623482E-02,
    0.545713419970309863995E-02,
    0.542492204466865704951E-02,
    0.539245800482555593606E-02,
    0.535974500312596681161E-02,
    0.532678599182711857974E-02,
    0.529358395244259896547E-02,
    0.526014189569259311205E-02,
    0.522646286145300596306E-02,
    0.519254991870341614863E-02,
    0.515840616547381084096E-02,
    0.512403472879005351831E-02,
    0.508943876461803986674E-02,
    0.505462145780650125058E-02,
    0.501958602202842039909E-02,
    0.498433569972103029914E-02,
    0.494887376202437487201E-02,
    0.491320350871841897367E-02,
    0.487732826815870573054E-02,
    0.484125139721057135214E-02,
    0.480497628118194150483E-02,
    0.476850633375474925263E-02,
    0.473184499691503264714E-02,
    0.469499574088179046532E-02,
    0.465796206403469754658E-02,
    0.462074749284080687482E-02,
    0.458335558178039420335E-02,
    0.454578991327213285488E-02,
    0.450805409759782158001E-02,
    0.447015177282692726900E-02,
    0.443208660474124713206E-02,
    0.439386228676004195260E-02,
    0.435548253986604343679E-02,
    0.431695111253279479928E-02,
    0.427827178065384480959E-02,
    0.423944834747438184434E-02,
    0.420048464352596631772E-02,
    0.416138452656509745764E-02,
    0.412215188151643401528E-02,
    0.408279062042157838350E-02,
    0.404330468239442998549E-02,
    0.400369803358421688562E-02,
    0.396397466714742455513E-02,
    0.392413860322995774660E-02,
    0.388419388896099560998E-02,
    0.384414459846013158917E-02,
    0.380399483285952829161E-02,
    0.376374872034296338241E-02,
    0.372341041620379550870E-02,
    0.368298410292403911967E-02,
    0.364247399027690353194E-02,
    0.360188431545532431869E-02,
    0.356121934322919357659E-02,
    0.352048336613417922682E-02,
    0.347968070469521146972E-02,
    0.343881570768790591876E-02,
    0.339789275244138669739E-02,
    0.335691624518616761342E-02,
    0.331589062145094394706E-02,
    0.327482034651233969564E-02,
    0.323370991590184336368E-02,
    0.319256385597434736790E-02,
    0.315138672454287935858E-02,
    0.311018311158427546158E-02,
    0.306895764002069252174E-02,
    0.302771496658198544480E-02,
    0.298645978275408290247E-02,
    0.294519681581857582284E-02,
    0.290393082998878368175E-02,
    0.286266662764757868253E-02,
    0.282140905069222207923E-02,
    0.278016298199139435045E-02,
    0.273893334695947541201E-02,
    0.269772511525294586667E-02,
    0.265654330259352828314E-02,
    0.261539297272236109225E-02,
    0.257427923948908888092E-02,
    0.253320726907925325750E-02,
    0.249218228238276930060E-02,
    0.245120955750556483923E-02,
    0.241029443242563417382E-02,
    0.236944230779380495146E-02,
    0.232865864987842738864E-02,
    0.228794899365195972378E-02,
    0.224731894601603393082E-02,
    0.220677418916003329194E-02,
    0.216632048404649142727E-02,
    0.212596367401472533045E-02,
    0.208570968849203942640E-02,
    0.204556454679958293446E-02,
    0.200553436203751169944E-02,
    0.196562534503150547732E-02,
    0.192584380831993546204E-02,
    0.188619617015808475394E-02,
    0.184668895851282540913E-02,
    0.180732881501808930079E-02,
    0.176812249885838886701E-02,
    0.172907689054461607168E-02,
    0.169019899554346019117E-02,
    0.165149594771914570655E-02,
    0.161297501254393423070E-02,
    0.157464359003212166189E-02,
    0.153650921735128916170E-02,
    0.149857957106456636214E-02,
    0.146086246895890987689E-02,
    0.142336587141720519900E-02,
    0.138609788229672549700E-02,
    0.134906674928353113127E-02,
    0.131228086370221478128E-02,
    0.127574875977346947345E-02,
    0.123947911332878396534E-02,
    0.120348074001265964881E-02,
    0.116776259302858043685E-02,
    0.113233376051597664917E-02,
    0.109720346268191941940E-02,
    0.106238104885340071375E-02,
    0.102787599466367326179E-02,
    0.993697899638760857945E-03,
    0.959856485506936206261E-03,
    0.926361595613111283368E-03,
    0.893223195879324912340E-03,
    0.860451377808527848128E-03,
    0.828056364077226302608E-03,
    0.796048517297550871506E-03,
    0.764438352543882784191E-03,
    0.733236554224767912055E-03,
    0.702453997827572321358E-03,
    0.672101776960108194646E-03,
    0.642191235948505088403E-03,
    0.612734008012225209294E-03,
    0.583742058714979703847E-03,
    0.555227733977307579715E-03,
    0.527203811431658386125E-03,
    0.499683553312800484519E-03,
    0.472680758429262691232E-03,
    0.446209810101403247488E-03,
    0.420285716355361231823E-03,
    0.394924138246873704434E-03,
    0.370141402122251665232E-03,
    0.345954492129903871350E-03,
    0.322381020652862389664E-03,
    0.299439176850911730874E-03,
    0.277147657465187357459E-03,
    0.255525589595236862014E-03,
    0.234592462123925204879E-03,
    0.214368090034216937149E-03,
    0.194872642236641146532E-03,
    0.176126765545083195474E-03,
    0.158151830411132242924E-03,
    0.140970302204104791413E-03,
    0.124606200241498368482E-03,
    0.109085545645741522051E-03,
    0.944366322532705527066E-04,
    0.806899228014035293851E-04,
    0.678774554733972416227E-04,
    0.560319507856164252140E-04,
    0.451863674126296143105E-04,
    0.353751372055189588628E-04,
    0.266376412339000901358E-04,
    0.190213681905875816679E-04,
    0.125792781889592743525E-04,
    0.736624069102321668857E-05,
    0.345456507169149134898E-05,
    0.945715933950007048827E-06,
  };

  if ( n == 1 )
  {
    r8vec_copy ( n, w_001, w );
  }
  else if ( n == 3 )
  {
    r8vec_copy ( n, w_003, w );
  }
  else if ( n == 7 )
  {
    r8vec_copy ( n, w_007, w );
  }
  else if ( n == 15 )
  {
    r8vec_copy ( n, w_015, w );
  }
  else if ( n == 31 )
  {
    r8vec_copy ( n, w_031, w );
  }
  else if ( n == 63 )
  {
    r8vec_copy ( n, w_063, w );
  }
  else if ( n == 127 )
  {
    r8vec_copy ( n, w_127, w );
  }
  else if ( n == 255 )
  {
    r8vec_copy ( n, w_255, w );
  }
  else if ( n == 511 )
  {
    r8vec_copy ( n, w_511, w );
  }
  else
  {
    std::cerr << "\n";
    std::cerr << "PATTERSON_LOOKUP_WEIGHTS - Fatal error!\n";
    std::cerr << "  Unexpected value of N = " << n << ".\n";
    std::exit ( 1 );
  }
  return;
}
//**************************************************************************80

void SandiaRules::patterson_lookup_weights_np ( int n, int np, double p[], double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    PATTERSON_LOOKUP_WEIGHTS_NP looks up Patterson quadrature weights.
//
//  Discussion:
//
//    The allowed orders are 1, 3, 7, 15, 31, 63, 127, 255 and 511.
//
//    The weights are positive, symmetric and should sum to 2.
//
//    The user must preallocate space for the output array W.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 April 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Arthur Stroud, Don Secrest,
//    Gaussian Quadrature Formulas,
//    Prentice Hall, 1966,
//    LC: QA299.4G3S7.
//
//  Parameters:
//
//    Input, int N, the order.
//    Legal values are 1, 3, 7, 15, 31, 63, 127, 255 or 511.
//
//    Input, int NP, the number of parameters.
//
//    Input, double P[NP], parameters which are not needed by this function.
//
//    Output, double W[N], the weights.
//
{
  patterson_lookup_weights ( n, w );

  return;
}
//**************************************************************************80

int SandiaRules::point_radial_tol_unique_count ( int m, int n, double a[], double tol,
  int *seed )

//**************************************************************************80
//
//  Purpose:
//
//    POINT_RADIAL_TOL_UNIQUE_COUNT counts the tolerably unique points.
//
//  Discussion:
//
//    The input data is an M x N array A, representing the M-dimensional
//    coordinates of N points.
//
//    The output is the number of tolerably unique points in the list.
//
//    This program performs the same task as POINT_TOL_UNIQUE_COUNT.
//    But that program is guaranteed to use N^2 comparisons.
//
//    It is hoped that this function, on the other hand, will tend
//    to use O(N) comparisons after an O(NLog(N)) sort.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 July 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows.
//
//    Input, int N, the number of columns.
//
//    Input, double A[M*N], the array of N columns of data.
//
//    Input, double TOL, a tolerance for equality.
//
//    Input/output, int *SEED, a seed for the random
//    number generator.
//
//    Output, int POINT_RADIAL_TOL_UNIQUE_COUNT, the number of tolerably
//    unique points.
//
{
  double dist;
  int hi;
  int i;
  int *indx;
  int j;
  int k;
  double *r;
  bool *unique;
  int unique_num;
  double *w;
  double w_sum;
  double *z;

  if ( n <= 0 )
  {
    unique_num = 0;
    return unique_num;
  }
//
//  Assign a base point Z randomly in the convex hull.
//
  w = r8vec_uniform_01_new ( n, seed );
  w_sum = r8vec_sum ( n, w );
  for ( j = 0; j < n; j++ )
  {
    w[j] = w[j] / w_sum;
  }

  z = new double[m];
  for ( i = 0; i < m; i++ )
  {
    z[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      z[i] = z[i] + a[i+j*m] * w[j];
    }
  }
//
//  Compute the radial distance R of each point to Z.
//
  r = new double[n];

  for ( j = 0; j < n; j++ )
  {
    r[j] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      r[j] = r[j] + std::pow ( a[i+j*m] - z[i], 2 );
    }
    r[j] = std::sqrt ( r[j] );
  }
//
//  Implicitly sort the R array.
//
  indx = r8vec_sort_heap_index_a_new ( n, r );
//
//  To determine if a point I is tolerably unique, we only have to check
//  whether it is distinct from all points J such that R(I) <= R(J) <= R(J)+TOL.
//
  unique_num = 0;

  unique = new bool[n];
  for ( i = 0; i < n; i++ )
  {
    unique[i] = true;
  }

  for ( i = 0; i < n; i++ )
  {
    if ( unique[indx[i]] )
    {
//
//  Point INDX(I) is unique, in that no earlier point is near it.
//
      unique_num = unique_num + 1;
//
//  Look for later points which are close to point INDX(I)
//  in terms of R.
//
      hi = i;

      while ( hi < n - 1 )
      {
        if ( r[indx[i]] + tol < r[indx[hi+1]] )
        {
          break;
        }
        hi = hi + 1;
      }
//
//  Points INDX(I+1) through INDX(HI) have an R value close to
//  point INDX(I).  Are they truly close to point INDEX(I)?
//
      for ( j = i + 1; j <= hi; j++ )
      {
        if ( unique[indx[j]] )
        {
          dist = 0.0;
          for ( k = 0; k < m; k++ )
          {
            dist = dist + std::pow ( a[k+indx[i]*m] - a[k+indx[j]*m], 2 );
          }
          dist = std::sqrt ( dist );

          if ( dist <= tol )
          {
            unique[indx[j]] = false;
          }
        }
      }
    }
  }

  delete [] indx;
  delete [] r;
  delete [] unique;
  delete [] w;
  delete [] z;

  return unique_num;
}
//**************************************************************************80

void SandiaRules::point_radial_tol_unique_count_inc1 ( int m, int n1, double a1[],
  double tol, int *seed, double z[], double r1[], int indx1[], bool unique1[],
  int *unique_num1 )

//**************************************************************************80
//
//  Purpose:
//
//    POINT_RADIAL_TOL_UNIQUE_COUNT_INC1 counts the tolerably unique points.
//
//  Discussion:
//
//    The input data includes an M x N1 array A1 of a set of N1
//    "permanent" points and N2 "temporary" points.
//
//    This is a two step version of POINT_RADIAL_TOL_UNIQUE_COUNT_INC.
//
//    This means that we want to identify the tolerably unique points
//    among the permanent points before processing the temporary points.
//
//    If many sets of temporary data are considered, this function will
//    do a lot of unnecessary work resorting the permanent data; it would
//    be possible to avoid SandiaRules::repetitions of that work at the expense of saving
//    various work vectors.  This function accepts the overhead of the
//    repeated calculations for the benefit of only having to "remember"
//    the number of unique points discovered.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 October 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows.
//
//    Input, int N1, the number of permanent points.
//
//    Input, double A1[M*N1], the permanent points.
//
//    Input, double TOL, a tolerance for equality.
//
//    Input/output, int *SEED, a seed for the random
//    number generator.
//
//    Output, double Z[M], a random base vector used to
//    linearly sort the data.
//
//    Output, double R1[N1], the scalar values assigned to
//    the data for sorting.
//
//    Output, int INDX1[N1], the ascending sort index
//    for A1.
//
//    Output, bool UNIQUE1[N1], is TRUE for each unique permanent point.
//
//    Output, int *UNIQUE_NUM1, the number of tolerably
//    unique permanent points.
//
{
  double dist;
  int hi;
  int i;
  //int j;
  int j1;
  int k1;
  double *w;
  double w_sum;
//
//  Assign a base point Z randomly in the convex hull of the permanent points.
//
  w = r8vec_uniform_01_new ( n1, seed );
  w_sum = r8vec_sum ( n1, w );
  for ( j1 = 0; j1 < n1; j1++ )
  {
    w[j1] = w[j1] / w_sum;
  }
  for ( i = 0; i < m; i++ )
  {
    z[i] = 0.0;
    for ( j1 = 0; j1 < n1; j1++ )
    {
      z[i] = z[i] + a1[i+j1*m] * w[j1];
    }
  }
//
//  Initialize the permanent point data.
//
  for ( j1 = 0; j1 < n1; j1++ )
  {
    r1[j1] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      r1[j1] = r1[j1] + std::pow ( a1[i+j1*m] - z[i], 2 );
    }
    r1[j1] = std::sqrt ( r1[j1] );
  }
  r8vec_sort_heap_index_a ( n1, r1, indx1 );

  *unique_num1 = 0;
  for ( j1 = 0; j1 < n1; j1++ )
  {
    unique1[j1] = true;
  }
//
//  STEP 1:
//  Compare PERMANENT POINTS to PERMANENT POINTS.
//
  for ( j1 = 0; j1 < n1; j1++ )
  {
    if ( unique1[indx1[j1]] )
    {
      *unique_num1 = *unique_num1 + 1;

      hi = j1;

      while ( hi < n1 - 1 )
      {
        if ( r1[indx1[j1]] + tol < r1[indx1[hi+1]] )
        {
          break;
        }
        hi = hi + 1;
      }

      for ( k1 = j1 + 1; k1 <= hi; k1++ )
      {
        if ( unique1[indx1[k1]] )
        {
          dist = 0.0;
          for ( i = 0; i < m; i++ )
          {
            dist = dist + std::pow ( a1[i+indx1[j1]*m] - a1[i+indx1[k1]*m], 2 );
          }
          dist = std::sqrt ( dist );

          if ( dist <= tol )
          {
            unique1[indx1[k1]] = false;
          }
        }
      }
    }
  }

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::point_radial_tol_unique_count_inc2 ( int m, int n1, double a1[], int n2,
  double a2[], double tol, double z[], double r1[], int indx1[], bool unique1[],
  int *unique_num2 )

//**************************************************************************80
//
//  Purpose:
//
//    POINT_RADIAL_TOL_UNIQUE_COUNT_INC2 counts the tolerably unique points.
//
//  Discussion:
//
//    The input data includes an M x N1 array A1 and an M x N2 array A2,
//    representing the M-dimensional coordinates of a set of N1
//    "permanent" points and N2 "temporary" points.
//
//    This is an "incremental" version of POINT_RADIAL_TOL_UNIQUE_COUNT.
//
//    This means that we want to identify the tolerably unique points
//    among the permanent points before processing the temporary points.
//
//    If many sets of temporary data are considered, this function will
//    do a lot of unnecessary work resorting the permanent data; it would
//    be possible to avoid SandiaRules::repetitions of that work at the expense of saving
//    various work vectors.  This function accepts the overhead of the
//    repeated calculations for the benefit of only having to "remember"
//    the number of unique points discovered.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 October 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows.
//
//    Input, int N1, the number of permanent points.
//
//    Input, double A1[M*N1], the permanent points.
//
//    Input, int N2, the number of temporary points.
//
//    Input, double A2[M*N2], the temporary points.
//
//    Input, double TOL, a tolerance for equality.
//
//    Input, double Z[M], a random base vector used to
//    linearly sort the data.
//
//    Input, double R1[N1], the scalar values assigned to
//    the data for sorting.
//
//    Input, int INDX1[N1], the ascending sort index
//    for A1.
//
//    Input, bool UNIQUE1[N1], is TRUE for each unique permanent point.
//
//    Output, int *UNIQUE_NUM2, the number of additional
//    tolerably unique points if the temporary points are included.
//
{
  double dist;
  int hi;
  int i;
  int *indx2;
  //int j;
  int j1;
  int j2;
  int j2_hi;
  int j2_lo;
  //int k1;
  int k2;
  double r_hi;
  double r_lo;
  double *r2;
  bool *unique2;
//
//  Initialize the temporary point data.
//
  r2 = new double[n2];
  for ( j2 = 0; j2 < n2; j2++ )
  {
    r2[j2] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      r2[j2] = r2[j2] + std::pow ( a2[i+j2*m] - z[i], 2 );
    }
    r2[j2] = std::sqrt ( r2[j2] );
  }

  indx2 = new int[n2];
  r8vec_sort_heap_index_a ( n2, r2, indx2 );

  unique2 = new bool[n2];
  for ( j2 = 0; j2 < n2; j2++ )
  {
    unique2[j2] = true;
  }

  *unique_num2 = 0;
//
//  STEP 2:
//  Use PERMANENT points to eliminate TEMPORARY points.
//
  for ( j1 = 0; j1 < n1; j1++ )
  {
    if ( unique1[indx1[j1]] )
    {
      r_lo = r1[indx1[j1]] - tol;
      r_hi = r1[indx1[j1]] + tol;

      r8vec_index_sorted_range ( n2, r2, indx2, r_lo, r_hi,
        &j2_lo, &j2_hi );

      for ( j2 = j2_lo; j2 <= j2_hi; j2++ )
      {
        if ( unique2[indx2[j2]] )
        {
          dist = 0.0;
          for ( i = 0; i < m; i++ )
          {
            dist = dist + std::pow ( a1[i+indx1[j1]*m]
                                   - a2[i+indx2[j2]*m], 2 );
          }
          dist = std::sqrt ( dist );
          if ( dist <= tol )
          {
            unique2[indx2[j2]] = false;
          }
        }
      }
    }
  }
//
//  STEP 3:
//  Use TEMPORARY points to eliminate TEMPORARY points.
//
  for ( j2 = 0; j2 < n2; j2++ )
  {
    if ( unique2[indx2[j2]] )
    {
      *unique_num2 = *unique_num2 + 1;

      hi = j2;

      while ( hi < n2 - 1 )
      {
        if ( r2[indx2[j2]] + tol < r2[indx2[hi+1]] )
        {
          break;
        }
        hi = hi + 1;
      }

      for ( k2 = j2 + 1; k2 <= hi; k2++ )
      {
        if ( unique2[indx2[k2]] )
        {
          dist = 0.0;
          for ( i = 0; i < m; i++ )
          {
            dist = dist + std::pow ( a2[i+indx2[j2]*m] - a2[i+indx2[k2]*m], 2 );
          }
          dist = std::sqrt ( dist );

          if ( dist <= tol )
          {
            unique2[indx2[k2]] = false;
          }
        }
      }
    }
  }
  delete [] indx2;
  delete [] r2;
  delete [] unique2;

  return;
}
//**************************************************************************80

int SandiaRules::point_radial_tol_unique_index ( int m, int n, double a[], double tol,
  int *seed, int undx[], int xdnu[] )

//**************************************************************************80
//
//  Purpose:
//
//    POINT_RADIAL_TOL_UNIQUE_INDEX indexes the tolerably unique points.
//
//  Discussion:
//
//    The input data is an M x N array A, representing the M-dimensional
//    coordinates of N points.
//
//    The output is:
//    * the number of tolerably unique points in the list;
//    * the index, in the list of unique items, of the representatives
//      of each point;
//    * the index, in A, of the tolerably unique representatives.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 July 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows.
//
//    Input, int N, the number of columns.
//
//    Input, double A[M*N], the array of N columns of data.
//
//    Input, double TOL, a tolerance for equality.
//
//    Input/output, int SEED, a seed for the random
//    number generator.
//
//    Output, int UNDX[UNIQUE_NUM], the index, in A, of the
//    tolerably unique points.
//
//    Output, int XDNU[N], the index, in UNDX, of the
//    tolerably unique point that "represents" this point.
//
//    Output, int POINT_RADIAL_TOL_UNIQUE_INDEX, the number of tolerably
//    unique points.
//
{
  double dist;
  int hi;
  int i;
  int *indx;
  int j;
  int k;
  double *r;
  bool *unique;
  int unique_num;
  double *w;
  double w_sum;
  double *z;

  if ( n <= 0 )
  {
    unique_num = 0;
    return unique_num;
  }
//
//  Assign a base point Z randomly in the convex hull.
//
  w = r8vec_uniform_01_new ( n, seed );
  w_sum = r8vec_sum ( n, w );
  for ( j = 0; j < n; j++ )
  {
    w[j] = w[j] / w_sum;
  }

  z = new double[m];
  for ( i = 0; i < m; i++ )
  {
    z[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      z[i] = z[i] + a[i+j*m] * w[j];
    }
  }
//
//  Compute the radial distance R of each point to Z.
//
  r = new double[n];

  for ( j = 0; j < n; j++ )
  {
    r[j] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      r[j] = r[j] + std::pow ( a[i+j*m] - z[i], 2 );
    }
    r[j] = std::sqrt ( r[j] );
  }
//
//  Implicitly sort the R array.
//
  indx = r8vec_sort_heap_index_a_new ( n, r );
//
//  To determine if a point I is tolerably unique, we only have to check
//  whether it is distinct from all points J such that R(I) <= R(J) <= R(J)+TOL.
//
  unique_num = 0;

  unique = new bool[n];
  for ( i = 0; i < n; i++ )
  {
    unique[i] = true;
  }

  for ( i = 0; i < n; i++ )
  {
    if ( unique[indx[i]] )
    {
//
//  Point INDX(I) is unique, in that no earlier point is near it.
//
      xdnu[indx[i]] = unique_num;
      undx[unique_num] = indx[i];
      unique_num = unique_num + 1;
//
//  Look for later points which are close to point INDX(I)
//  in terms of R.
//
      hi = i;

      while ( hi < n - 1 )
      {
        if ( r[indx[i]] + tol < r[indx[hi+1]] )
        {
          break;
        }
        hi = hi + 1;
      }
//
//  Points INDX(I+1) through INDX(HI) have an R value close to
//  point INDX(I).  Are they truly close to point INDEX(I)?
//
      for ( j = i + 1; j <= hi; j++ )
      {
        if ( unique[indx[j]] )
        {
          dist = 0.0;
          for ( k = 0; k < m; k++ )
          {
            dist = dist + std::pow ( a[k+indx[i]*m] - a[k+indx[j]*m], 2 );
          }
          dist = std::sqrt ( dist );

          if ( dist <= tol )
          {
            unique[indx[j]] = false;
            xdnu[indx[j]] = xdnu[indx[i]];
          }
        }
      }
    }
  }

  delete [] indx;
  delete [] r;
  delete [] unique;
  delete [] w;
  delete [] z;

  return unique_num;
}
//**************************************************************************80

void SandiaRules::point_radial_tol_unique_index_inc1 ( int m, int n1, double a1[],
  double tol, int *seed, double z[], double r1[], int indx1[], bool unique1[],
  int *unique_num1, int undx1[], int xdnu1[] )

//**************************************************************************80
//
//  Purpose:
//
//    POINT_RADIAL_TOL_UNIQUE_INDEX_INC1 indexes the tolerably unique points.
//
//  Discussion:
//
//    The input data includes an M x N1 array A1 of
//    "permanent" points.
//
//    This is a two step version of POINT_RADIAL_TOL_UNIQUE_INDEX_INC.
//
//    The output is:
//    * the number of tolerably unique points in the list;
//    * the index, in the list of unique items, of the representatives
//      of each point;
//    * the index, in A1, of the tolerably unique representatives.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 October 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows.
//
//    Input, int N1, the number of permanent points.
//
//    Input, double A1[M*N1], the permanent points.
//
//    Input, double TOL, a tolerance for equality.
//
//    Input/output, int *SEED, a seed for the random
//    number generator.
//
//    Output, double Z[M], a random base vector used to
//    linearly sort the data.
//
//    Output, double R1[N1], the scalar values assigned to
//    the data for sorting.
//
//    Output, int INDX1[N1], the ascending sort index for A1.
//
//    Output, bool UNIQUE1[N1], is TRUE for unique permanent points.
//
//    Output, int *UNIQUE_NUM1, the number of tolerably unique points
//    with just the permanent points.
//
//    Output, int UNDX1[UNIQUE_NUM1], the index, in A1, of the tolerably
//    unique points.
//
//    Output, int XDNU1[N1], the index, in UNDX1, of the tolerably unique
//    point that "represents" this point.
//
{
  double dist;
  int hi;
  int i;
  //int j;
  int j1;
  int k1;
  double *w;
  double w_sum;
//
//  Assign a base point Z randomly in the convex hull of the permanent points.
//
  w = r8vec_uniform_01_new ( n1, seed );
  w_sum = r8vec_sum ( n1, w );
  for ( j1 = 0; j1 < n1; j1++ )
  {
    w[j1] = w[j1] / w_sum;
  }

  for ( i = 0; i < m; i++ )
  {
    z[i] = 0.0;
    for ( j1 = 0; j1 < n1; j1++ )
    {
      z[i] = z[i] + a1[i+j1*m] * w[j1];
    }
  }
//
//  Initialize the permanent point data.
//
  for ( j1 = 0; j1 < n1; j1++ )
  {
    r1[j1] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      r1[j1] = r1[j1] + std::pow ( a1[i+j1*m] - z[i], 2 );
    }
    r1[j1] = std::sqrt ( r1[j1] );
  }
  r8vec_sort_heap_index_a ( n1, r1, indx1 );

  *unique_num1 = 0;
  for ( j1 = 0; j1 < n1; j1++ )
  {
    unique1[j1] = true;
  }
//
//  STEP 1:
//  Compare PERMANENT POINTS to PERMANENT POINTS.
//
  for ( j1 = 0; j1 < n1; j1++ )
  {
    if ( unique1[indx1[j1]] )
    {
      xdnu1[indx1[j1]] = *unique_num1;
      undx1[*unique_num1] = indx1[j1];
      *unique_num1 = *unique_num1 + 1;

      hi = j1;

      while ( hi < n1 - 1 )
      {
        if ( r1[indx1[j1]] + tol < r1[indx1[hi+1]] )
        {
          break;
        }
        hi = hi + 1;
      }

      for ( k1 = j1 + 1; k1 <= hi; k1++ )
      {
        if ( unique1[indx1[k1]] )
        {
          dist = 0.0;
          for ( i = 0; i < m; i++ )
          {
            dist = dist + std::pow ( a1[i+indx1[j1]*m] - a1[i+indx1[k1]*m], 2 );
          }
          dist = std::sqrt ( dist );

          if ( dist <= tol )
          {
            unique1[indx1[k1]] = false;
            xdnu1[indx1[k1]] = xdnu1[indx1[j1]];
          }
        }
      }
    }
  }

  delete [] w;

  return;
}
//**************************************************************************80

void SandiaRules::point_radial_tol_unique_index_inc2 ( int m, int n1, double a1[], int n2,
  double a2[], double tol, double z[], double r1[], int indx1[], bool unique1[],
  int unique_num1, int undx1[], int xdnu1[], double r2[],
  int indx2[], bool unique2[], int *unique_num2, int undx2[], int xdnu2[] )

//**************************************************************************80
//
//  Purpose:
//
//    POINT_RADIAL_TOL_UNIQUE_INDEX_INC2 indexes unique temporary points.
//
//  Discussion:
//
//    The input data includes an M x N1 array A1 and an M x N2 array A2,
//    representing the M-dimensional coordinates of a set of N1
//    "permanent" points and N2 "temporary" points.
//
//    For notation, we use "A" to describe the M x (N1+N2) array that would be
//    formed by starting with A1 and appending A2.
//
//    The output is:
//    * the number of tolerably unique points in the list;
//    * the index, in the list of unique items, of the representatives
//      of each point;
//    * the index, in A, of the tolerably unique representatives.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 October 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows.
//
//    Input, int N1, the number of permanent points.
//
//    Input, double A1[M*N1], the permanent points.
//
//    Input, int N2, the number of temporary points.
//
//    Input, double A2[M*N2], the temporary points.
//
//    Input, double TOL, a tolerance for equality.
//
//    Input, double Z[M], a random base vector used to
//    linearly sort the data.
//
//    Input, double R1[N1], the scalar values assigned to
//    A1 for sorting.
//
//    Input, int INDX1[N1], the ascending sort index for A1.
//
//    Input, bool UNIQUE1[N1], is TRUE for unique permanent points.
//
//    Input, int UNIQUE_NUM1, the number of tolerably unique permanent points.
//
//    Input, int UNDX1[UNIQUE_NUM1],
//    the index in A1 of the tolerably unique permanent points.
//
//    Input, int XDNU1[N1], the index in UNDX1
//    of the tolerably unique permanent point that "represents" this point.
//
//    Output, double R2[N2], the scalar values assigned to
//    A2 for sorting.
//
//    Output, int INDX2[N2], the ascending sort index for A2.
//
//    Output, bool UNIQUE2[N2], is TRUE for unique temporary points.
//
//    Output, int *UNIQUE_NUM2, the number
//    of tolerably unique temporary points.
//
//    Output, int UNDX2[UNIQUE_NUM2],
//    the index in A2 of the tolerably unique points, incremented by N1.
//
//    Output, int XDNU2[N2], the index, in UNDX1
//    or UNDX2, of the tolerably unique point that "represents" this
//    temporary point.  If the value represents an index in UNDX2, this
//    can be inferred by the fact that its value is greater than or
//    equal to UNIQUE_NUM1.  To reference UNDX2, the value should then be
//    decremented by UNIQUE_NUM1.
//
{
  double dist;
  int hi;
  int i;
  //int j;
  int j1;
  int j2;
  int j2_hi;
  int j2_lo;
  //int k1;
  int k2;
  double r_hi;
  double r_lo;
//
//  Initialize the temporary point data.
//
  for ( j2 = 0; j2 < n2; j2++ )
  {
    r2[j2] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      r2[j2] = r2[j2] + std::pow ( a2[i+j2*m] - z[i], 2 );
    }
    r2[j2] = std::sqrt ( r2[j2] );
  }

  r8vec_sort_heap_index_a ( n2, r2, indx2 );

  for ( j2 = 0; j2 < n2; j2++ )
  {
    unique2[j2] = true;
  }

  *unique_num2 = 0;
//
//  STEP 2:
//  Use PERMANENT points to eliminate TEMPORARY points.
//
  for ( j1 = 0; j1 < n1; j1++ )
  {
    if ( unique1[indx1[j1]] )
    {
      r_lo = r1[indx1[j1]] - tol;
      r_hi = r1[indx1[j1]] + tol;

      r8vec_index_sorted_range ( n2, r2, indx2, r_lo, r_hi,
        &j2_lo, &j2_hi );

      for ( j2 = j2_lo; j2 <= j2_hi; j2++ )
      {
        if ( unique2[indx2[j2]] )
        {
          dist = 0.0;
          for ( i = 0; i < m; i++ )
          {
            dist = dist + std::pow ( a1[i+indx1[j1]*m]
                                   - a2[i+indx2[j2]*m], 2 );
          }
          dist = std::sqrt ( dist );
          if ( dist <= tol )
          {
            unique2[indx2[j2]] = false;
            xdnu2[indx2[j2]] = xdnu1[indx1[j1]];
          }
        }
      }
    }
  }
//
//  STEP 3:
//  Use TEMPORARY points to eliminate TEMPORARY points.
//
  for ( j2 = 0; j2 < n2; j2++ )
  {
    if ( unique2[indx2[j2]] )
    {
      xdnu2[indx2[j2]] = unique_num1 + *unique_num2;
      undx2[*unique_num2] = indx2[j2] + n1;
      *unique_num2 = *unique_num2 + 1;

      hi = j2;

      while ( hi < n2 - 1 )
      {
        if ( r2[indx2[j2]] + tol < r2[indx2[hi+1]] )
        {
          break;
        }
        hi = hi + 1;
      }

      for ( k2 = j2 + 1; k2 <= hi; k2++ )
      {
        if ( unique2[indx2[k2]] )
        {
          dist = 0.0;
          for ( i = 0; i < m; i++ )
          {
            dist = dist + std::pow ( a2[i+indx2[j2]*m] - a2[i+indx2[k2]*m], 2 );
          }
          dist = std::sqrt ( dist );

          if ( dist <= tol )
          {
            unique2[indx2[k2]] = false;
            xdnu2[indx2[k2]] = xdnu2[indx2[j2]];
          }
        }
      }
    }
  }

  return;
}
//**************************************************************************80

void SandiaRules::point_radial_tol_unique_index_inc3 ( int m, int n1, double a1[],
  double r1[], int indx1[], bool unique1[], int unique_num1, int undx1[],
  int xdnu1[], int n2, double a2[], double r2[], int indx2[], bool unique2[],
  int unique_num2, int undx2[], int xdnu2[], int *n3, double a3[], double r3[],
  int indx3[], bool unique3[], int *unique_num3, int undx3[], int xdnu3[] )

//**************************************************************************80
//
//  Purpose:
//
//    POINT_RADIAL_TOL_UNIQUE_INDEX_INC3 merges index data.
//
//  Discussion:
//
//    This function may be called after *INDEX_INC1 has created index
//    information for the permanent data, and *INDEX_INC2 has created
//    augmenting information for a set of temporary data which now is
//    to be merged with the permanent data.
//
//    The function merges the data and index information to create a
//    new "permanent" data set.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 October 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows.
//
//    Input, int N1, the number of permanent points.
//
//    Input, double A1[M*N1], the permanent points.
//
//    Input, double R1[N1], the scalar values assigned to
//    the data for sorting.
//
//    Input, int INDX1[N1], the ascending sort index
//    for A1.
//
//    Input, bool UNIQUE1[N1], is TRUE for each unique permanent point.
//
//    Input, int UNIQUE_NUM1, the number
//    of tolerably unique points with just the permanent points.
//
//    Input, int UNDX1[UNIQUE_NUM1],
//    the index in A1 of the tolerably unique points.
//
//    Input, int XDNU1[N1], the index in UNDX1
//    of the tolerably unique point that "represents" this point.
//
//    Input, int N2, the number of temporary points.
//
//    Input, double A2[M,N2], the temporary points.
//
//    Input, double R2[N2], the scalar values assigned to
//    the data for sorting.
//
//    Input, int INDX2[N2], the ascending sort index
//    for A2.
//
//    Input, bool UNIQUE2[N2], is TRUE for each unique temporary point.
//
//    Input, int UNIQUE_NUM2, the number
//    of tolerably unique temporary points.
//
//    Input, int UNDX2[UNIQUE_NUM2],
//    the index in A2 of the tolerably unique points, incremented by UNIQUE_NUM1.
//
//    Input, int XDNU2[N2], the index in UNDX1 or UNDX2
//    of the tolerably unique point that "represents" this point.
//
//    Output, int *N3, the number of permanent points.
//
//    Output, double A3[M,N3], the permanent points.
//
//    Output, double R3[N3], the scalar values assigned to
//    the data for sorting.
//
//    Output, int INDX3[N3], the ascending sort index
//    for A3.
//
//    Output, bool UNIQUE3[N3], is TRUE for each unique permanent point.
//
//    Output, int *UNIQUE_NUM3, the number
//    of tolerably unique points.
//
//    Output, int UNDX3[UNIQUE_NUM3],
//    the index in A3 of the tolerably unique points.
//
//    Output, int XDNU3[N3], the index in UNDX3
//    of the tolerably unique point that "represents" this point.
//
{
  int i;
  int i1;
  int i2;
  int i3;
  double v1;
  double v2;

  *n3 = n1 + n2;

  for ( i1 = 0; i1 < n1; i1++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a3[i+i1*m] = a1[i+i1*m];
    }
  }
  for ( i2 = 0; i2 < n2; i2++ )
  {
    i3 = n1 + i2;
    for ( i = 0; i < m; i++ )
    {
      a3[i+i3*m] = a2[i+i2*m];
    }
  }
  for ( i1 = 0; i1 < n1; i1++ )
  {
    r3[i1]= r1[i1];
  }
  for ( i2 = 0; i2 < n2; i2++ )
  {
    i3 = n1 + i2;
    r3[i3] = r2[i2];
  }
//
//  Interleave the two INDX arrays so that INDX3 presents the entries
//  of A3 in ascending R3 order.
//
  i1 = 0;
  i2 = 0;

  for ( i3 = 0; i3 < *n3; i3++ )
  {
    if ( i1 < n1 )
    {
      v1 = r1[indx1[i1]];
    }
    else
    {
      v1 = r8_huge ( );
    }

    if ( i2 < n2 )
    {
      v2 = r2[indx2[i2]];
    }
    else
    {
      v2 = r8_huge ( );
    }

    if ( v1 <= v2 )
    {
      indx3[i3] = indx1[i1];
      i1 = i1 + 1;
    }
    else
    {
      indx3[i3] = indx2[i2] + n1;
      i2 = i2 + 1;
    }
  }

  *unique_num3 = unique_num1 + unique_num2;

  for ( i1 = 0; i1 < n1; i1++ )
  {
    unique3[i1] = unique1[i1];
  }
  for ( i2 = 0; i2 < n2; i2++ )
  {
    i3 = n1 + i2;
    unique3[i3] = unique2[i2];
  }
//
//  The entries in UNDX2 were already incremented by N2 if they pointed
//  to an entry of A2, so all entries in UNDX2 correctly index A3.
//
  for ( i1 = 0; i1 < unique_num1; i1++ )
  {
    undx3[i1] = undx1[i1];
  }
  for ( i2 = 0; i2 < unique_num2; i2++ )
  {
    i3 = unique_num1 + i2;
    undx3[i3] = undx2[i2];
  }
//
//  Note that the entries of XDNU2 were already incremented by N2
//  so that they correctly index A3, not A2.
//
  for ( i1 = 0; i1 < n1; i1++ )
  {
    xdnu3[i1] = xdnu1[i1];
  }
  for ( i2 = 0; i2 < n2; i2++ )
  {
    i3 = n1 + i2;
    xdnu3[i3] = xdnu2[i2];
  }

  return;
}
//**************************************************************************80

void SandiaRules::point_unique_index ( int m, int n, double a[], int unique_num, int undx[],
  int xdnu[] )

//**************************************************************************80
//
//  Purpose:
//
//    POINT_UNIQUE_INDEX indexes unique points.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points to the unique elements of A, in sorted order,
//    and a vector XDNU, which identifies, for each entry of A, the index of
//    the unique sorted element of A.
//
//    This is all done with index vectors, so that the elements of
//    A are never moved.
//
//    The first step of the algorithm requires the indexed sorting
//    of A, which creates arrays INDX and XDNI.  (If all the entries
//    of A are unique, then these arrays are the same as UNDX and XDNU.)
//
//    We then use INDX to examine the entries of A in sorted order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector A could be
//    replaced by a compressed vector XU, containing the unique entries
//    of A in sorted order, using the formula
//
//      XU(*) = A(UNDX(*)).
//
//    We could then, if we wished, reconstruct the entire vector A, or
//    any element of it, by index, as follows:
//
//      A(I) = XU(XDNU(I)).
//
//    We could then replace A by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of A, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector A, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I     A  Indx  Xdni       XU  Undx  Xdnu
//    ----+-----+-----+-----+--------+-----+-----+
//      0 | 11.     0     0 |    11.     0     0
//      1 | 22.     2     4 |    22.     1     1
//      2 | 11.     5     1 |    33.     3     0
//      3 | 33.     8     7 |    55.     4     2
//      4 | 55.     1     8 |                  3
//      5 | 11.     6     2 |                  0
//      6 | 22.     7     5 |                  1
//      7 | 22.     3     6 |                  1
//      8 | 11.     4     3 |                  0
//
//    INDX(2) = 3 means that sorted item(2) is A(3).
//    XDNI(2) = 5 means that A(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at A(4).
//    XDNU(8) = 2 means that A(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = A(I).
//    XU(I)        = A(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 July 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the dimension of the data values.
//
//    Input, int N, the number of data values,
//
//    Input, double A[M*N], the data values.
//
//    Input, int UNIQUE_NUM, the number of unique values in A.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Output, int UNDX[UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[N], the XDNU vector.
//
{
  double diff;
  int i;
  int *indx;
  int j;
  int k;
//
//  Implicitly sort the array.
//
  indx = r8col_sort_heap_index_a ( m, n, a );
//
//  Walk through the implicitly sorted array.
//
  i = 0;

  j = 0;
  undx[j] = indx[i];

  xdnu[indx[i]] = j;

  for ( i = 1; i < n; i++ )
  {
    diff = 0.0;
    for ( k = 0; k < m; k++ )
    {
      diff = r8_max ( diff,
        r8_abs ( a[k+indx[i]*m] - a[k+undx[j]*m] ) );
    }
    if ( 0.0 < diff )
    {
      j = j + 1;
      undx[j] = indx[i];
    }
    xdnu[indx[i]] = j;
  }
  delete [] indx;

  return;
}
//**************************************************************************80

void SandiaRules::product_mixed_weight ( int dim_num, int order_1d[], int order_nd,
  int rule[], double alpha[], double beta[], double weight_nd[] )

//**************************************************************************80
//
//  Purpose:
//
//    PRODUCT_MIXED_WEIGHT computes the weights of a mixed product rule.
//
//  Discussion:
//
//    This routine computes the weights for a quadrature rule which is
//    a product of 1D rules of varying order and kind.
//
//    The user must preallocate space for the output array WEIGHT_ND.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 February 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, int ORDER_1D[DIM_NUM], the order of the 1D rules.
//
//    Input, int ORDER_ND, the order of the product rule.
//
//    Input, int RULE[DIM_NUM], the rule in each dimension.
//     1, "CC",  Clenshaw Curtis, Closed Fully Nested rule.
//     2, "F2",  Fejer Type 2, Open Fully Nested rule.
//     3, "GP",  Gauss Patterson, Open Fully Nested rule.
//     4, "GL",  Gauss Legendre, Open Weakly Nested rule.
//     5, "GH",  Gauss Hermite, Open Weakly Nested rule.
//     6, "GGH", Generalized Gauss Hermite, Open Weakly Nested rule.
//     7, "LG",  Gauss Laguerre, Open Non Nested rule.
//     8, "GLG", Generalized Gauss Laguerre, Open Non Nested rule.
//     9, "GJ",  Gauss Jacobi, Open Non Nested rule.
//    10, "GW",  Golub Welsch, (presumed) Open Non Nested rule.
//    11, "CC_SE", Clenshaw Curtis Slow Exponential, Closed Fully Nested rule.
//    12, "F2_SE", Fejer Type 2 Slow Exponential, Open Fully Nested rule.
//    13, "GP_SE", Gauss Patterson Slow Exponential, Open Fully Nested rule.
//    14, "CC_ME", Clenshaw Curtis Moderate Exponential, Closed Fully Nested rule.
//    15, "F2_ME", Fejer Type 2 Moderate Exponential, Open Fully Nested rule.
//    16, "GP_ME", Gauss Patterson Moderate Exponential, Open Fully Nested rule.
//    17, "CCN", Clenshaw Curtis Nested, Linear, Closed Fully Nested rule.
//
//    Input, double ALPHA[DIM_NUM], BETA[DIM_NUM], parameters used for
//    Generalized Gauss Hermite, Generalized Gauss Laguerre,
//    and Gauss Jacobi rules.
//
//    Output, double WEIGHT_ND[ORDER_ND], the product rule weights.
//
{
  int dim;
  int i;
  double *weight_1d;

  for ( i = 0; i < order_nd; i++ )
  {
    weight_nd[i] = 1.0;
  }

  for ( dim = 0; dim < dim_num; dim++ )
  {
    weight_1d = new double[order_1d[dim]];

    if ( rule[dim] == 1 )
    {
      clenshaw_curtis_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 2 )
    {
      fejer2_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 3 )
    {
      patterson_lookup_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 4 )
    {
      legendre_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 5 )
    {
      hermite_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 6 )
    {
      gen_hermite_compute_weights ( order_1d[dim], alpha[dim], weight_1d );
    }
    else if ( rule[dim] == 7 )
    {
      laguerre_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 8 )
    {
      gen_laguerre_compute_weights ( order_1d[dim], alpha[dim], weight_1d );
    }
    else if ( rule[dim] == 9 )
    {
      jacobi_compute_weights ( order_1d[dim], alpha[dim], beta[dim], weight_1d );
    }
    else if ( rule[dim] == 10 )
    {
      std::cerr << "\n";
      std::cerr << "PRODUCT_MIXED_WEIGHT - Fatal error!\n";
      std::cerr << "  Do not know how to set weights for rule 10.\n";
      std::exit ( 1 );
    }
    else if ( rule[dim] == 11 )
    {
      clenshaw_curtis_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 12 )
    {
      fejer2_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 13 )
    {
      patterson_lookup_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 14 )
    {
      clenshaw_curtis_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 15 )
    {
      fejer2_compute_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 16 )
    {
      patterson_lookup_weights ( order_1d[dim], weight_1d );
    }
    else if ( rule[dim] == 17 )
    {
      ccn_compute_weights ( order_1d[dim], weight_1d );
    }
    else
    {
      std::cerr << "\n";
      std::cerr << "PRODUCT_MIXED_WEIGHT - Fatal error!\n";
      std::cerr << "  Unexpected value of RULE[" << dim << "] = "
           << rule[dim] << ".\n";
      std::exit ( 1 );
    }

    r8vec_direct_product2 ( dim, order_1d[dim], weight_1d,
      dim_num, order_nd, weight_nd );

    delete [] weight_1d;
  }
  return;
}
//**************************************************************************80

double SandiaRules::r8_abs ( double x )

//**************************************************************************80
//
//  Purpose:
//
//    R8_ABS returns the absolute value of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the quantity whose absolute value is desired.
//
//    Output, double R8_ABS, the absolute value of X.
//
{
  double value;

  if ( 0.0 <= x )
  {
    value = x;
  }
  else
  {
    value = -x;
  }
  return value;
}
//**************************************************************************80

double SandiaRules::r8_ceiling ( double x )

//**************************************************************************80
//
//  Purpose:
//
//    R8_CEILING rounds an R8 "up" (towards +oo) to the next integer.
//
//  Example:
//
//    X        R8_CEILING(X)
//
//   -1.1      -1.0
//   -1.0      -1.0
//   -0.9       0.0
//   -0.1       0.0
//    0.0       0.0
//    0.1       1.0
//    0.9       1.0
//    1.0       1.0
//    1.1       2.0
//    2.9       3.0
//    3.0       3.0
//    3.14159   4.0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose ceiling is desired.
//
//    Output, double R8_CEILING, the ceiling of X.
//
{
  double value;

  value = ( int ) x;

  if ( value < x )
  {
    value = value + 1.0;
  }

  return value;
}
//**************************************************************************80

double SandiaRules::r8_choose ( int n, int k )

//**************************************************************************80
//
//  Purpose:
//
//    R8_CHOOSE computes the binomial coefficient C(N,K) as an R8.
//
//  Discussion:
//
//    The value is calculated in such a way as to avoid SandiaRules::overflow and
//    roundoff.  The calculation is done in R8 arithmetic.
//
//    The formula used is:
//
//      C(N,K) = N! / ( K! * (N-K)! )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 March 2008
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    ML Wolfson, HV Wright,
//    Algorithm 160:
//    Combinatorial of M Things Taken N at a Time,
//    Communications of the ACM,
//    Volume 6, Number 4, April 1963, page 161.
//
//  Parameters:
//
//    Input, int N, K, the values of N and K.
//
//    Output, double R8_CHOOSE, the number of combinations of N
//    things taken K at a time.
//
{
  int i;
  int mn;
  int mx;
  double value;

  mn = i4_min ( k, n - k );

  if ( mn < 0 )
  {
    value = 0.0;
  }
  else if ( mn == 0 )
  {
    value = 1.0;
  }
  else
  {
    mx = i4_max ( k, n - k );
    value = ( double ) ( mx + 1 );

    for ( i = 2; i <= mn; i++ )
    {
      value = ( value * ( double ) ( mx + i ) ) / ( double ) i;
    }
  }
  return value;
}
//**************************************************************************80

double SandiaRules::r8_epsilon ( )

//**************************************************************************80
//
//  Purpose:
//
//    R8_EPSILON returns the R8 roundoff unit.
//
//  Discussion:
//
//    The roundoff unit is a number R which is a power of 2 with the
//    property that, to the precision of the computer's arithmetic,
//      1 < 1 + R
//    but
//      1 = ( 1 + R / 2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double R8_EPSILON, the R8 round-off unit.
//
{
  double value;

  value = 1.0;

  while ( 1.0 < ( double ) ( 1.0 + value )  )
  {
    value = value / 2.0;
  }

  value = 2.0 * value;

  return value;
}
//**************************************************************************80

double SandiaRules::r8_factorial ( int n )

//**************************************************************************80
//
//  Purpose:
//
//    R8_FACTORIAL computes the factorial of N.
//
//  Discussion:
//
//    factorial ( N ) = product ( 1 <= I <= N ) I
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    16 January 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the argument of the factorial function.
//    If N is less than 1, the function value is returned as 1.
//
//    Output, double R8_FACTORIAL, the factorial function.
//
{
  int i;
  double value;

  value = 1.0;

  for ( i = 1; i <= n; i++ )
  {
    value = value * ( double ) ( i );
  }

  return value;
}
//**************************************************************************80

double SandiaRules::r8_factorial2 ( int n )

//**************************************************************************80
//
//  Purpose:
//
//    R8_FACTORIAL2 computes the double factorial function.
//
//  Discussion:
//
//    FACTORIAL2( N ) = Product ( N * (N-2) * (N-4) * ... * 2 )  (N even)
//                    = Product ( N * (N-2) * (N-4) * ... * 1 )  (N odd)
//
//  Example:
//
//     N    FACTORIAL2(N)
//
//     0     1
//     1     1
//     2     2
//     3     3
//     4     8
//     5    15
//     6    48
//     7   105
//     8   384
//     9   945
//    10  3840
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 January 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the argument of the double factorial
//    function.  If N is less than 1, R8_FACTORIAL2 is returned as 1.0.
//
//    Output, double R8_FACTORIAL2, the double factorial function.
//
{
  int n_copy;
  double value;

  value = 1.0;

  if ( n < 1 )
  {
    return value;
  }

  n_copy = n;

  while ( 1 < n_copy )
  {
    value = value * ( double ) n_copy;
    n_copy = n_copy - 2;
  }

  return value;
}
//**************************************************************************80

double SandiaRules::r8_floor ( double x )

//**************************************************************************80
//
//  Purpose:
//
//    R8_FLOOR rounds an R8 "down" (towards -infinity) to the next integer.
//
//  Example:
//
//    X        R8_FLOOR(X)
//
//   -1.1      -2.0
//   -1.0      -1.0
//   -0.9      -1.0
//   -0.1      -1.0
//    0.0       0.0
//    0.1       0.0
//    0.9       0.0
//    1.0       1.0
//    1.1       1.0
//    2.9       2.0
//    3.0       3.0
//    3.14159   3.0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    15 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose floor is desired.
//
//    Output, double R8_FLOOR, the floor of X.
//
{
  double value;

  value = ( int ) x;

  if ( x < value )
  {
    value = value - 1.0;
  }

  return value;
}
//**************************************************************************80

double SandiaRules::r8_gamma ( double x )

//**************************************************************************80
//
//  Purpose:
//
//    R8_GAMMA evaluates Gamma(X) for a real argument.
//
//  Discussion:
//
//    This routine calculates the gamma function for a real argument X.
//
//    Computation is based on an algorithm outlined in reference 1.
//    The program uses rational functions that approximate the gamma
//    function to at least 20 significant decimal digits.  Coefficients
//    for the approximation over the interval (1,2) are unpublished.
//    Those for the approximation for 12 <= X are from reference 2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 January 2008
//
//  Author:
//
//    Original FORTRAN77 version by William Cody, Laura Stoltz.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    William Cody,
//    An Overview of Software Development for Special Functions,
//    in Numerical Analysis Dundee, 1975,
//    edited by GA Watson,
//    Lecture Notes in Mathematics 506,
//    Springer, 1976.
//
//    John Hart, Ward Cheney, Charles Lawson, Hans Maehly,
//    Charles Mesztenyi, John Rice, Henry Thatcher,
//    Christoph Witzgall,
//    Computer Approximations,
//    Wiley, 1968,
//    LC: QA297.C64.
//
//  Parameters:
//
//    Input, double X, the argument of the function.
//
//    Output, double R8_GAMMA, the value of the function.
//
{
//
//  Coefficients for minimax approximation over (12, INF).
//
  double c[7] = {
   -1.910444077728E-03,
    8.4171387781295E-04,
   -5.952379913043012E-04,
    7.93650793500350248E-04,
   -2.777777777777681622553E-03,
    8.333333333333333331554247E-02,
    5.7083835261E-03 };
  double eps = 2.22E-16;
  double fact;
  int i;
  int n;
  double one = 1.0;
  double p[8] = {
  -1.71618513886549492533811E+00,
   2.47656508055759199108314E+01,
  -3.79804256470945635097577E+02,
   6.29331155312818442661052E+02,
   8.66966202790413211295064E+02,
  -3.14512729688483675254357E+04,
  -3.61444134186911729807069E+04,
   6.64561438202405440627855E+04 };
  bool parity;
  double pi = 3.1415926535897932384626434;
  double q[8] = {
  -3.08402300119738975254353E+01,
   3.15350626979604161529144E+02,
  -1.01515636749021914166146E+03,
  -3.10777167157231109440444E+03,
   2.25381184209801510330112E+04,
   4.75584627752788110767815E+03,
  -1.34659959864969306392456E+05,
  -1.15132259675553483497211E+05 };
  double res;
  double sqrtpi = 0.9189385332046727417803297;
  double sum;
  double twelve = 12.0;
  double two = 2.0;
  double value;
  double xbig = 171.624;
  double xden;
  double xinf = 1.79E+308;
  double xminin = 2.23E-308;
  double xnum;
  double y;
  double y1;
  double ysq;
  double z;

  parity = false;
  fact = one;
  n = 0;
  y = x;
//
//  Argument is negative.
//
  if ( y <= 0.0 )
  {
    y = - x;
    y1 = ( double ) ( int ) ( y );
    res = y - y1;

    if ( res != 0.0 )
    {
      if ( y1 != ( double ) ( int ) ( y1 * 0.5 ) * two )
      {
        parity = true;
      }

      fact = - pi / std::sin ( pi * res );
      y = y + one;
    }
    else
    {
      res = xinf;
      value = res;
      return value;
    }
  }
//
//  Argument is positive.
//
  if ( y < eps )
  {
//
//  Argument < EPS.
//
    if ( xminin <= y )
    {
      res = one / y;
    }
    else
    {
      res = xinf;
      value = res;
      return value;
    }
  }
  else if ( y < twelve )
  {
    y1 = y;
//
//  0.0 < argument < 1.0.
//
    if ( y < one )
    {
      z = y;
      y = y + one;
    }
//
//  1.0 < argument < 12.0.
//  Reduce argument if necessary.
//
    else
    {
      n = ( int ) ( y ) - 1;
      y = y - ( double ) ( n );
      z = y - one;
    }
//
//  Evaluate approximation for 1.0 < argument < 2.0.
//
    xnum = 0.0;
    xden = one;
    for ( i = 0; i < 8; i++ )
    {
      xnum = ( xnum + p[i] ) * z;
      xden = xden * z + q[i];
    }
    res = xnum / xden + one;
//
//  Adjust result for case  0.0 < argument < 1.0.
//
    if ( y1 < y )
    {
      res = res / y1;
    }
//
//  Adjust result for case 2.0 < argument < 12.0.
//
    else if ( y < y1 )
    {
      for ( i = 1; i <= n; i++ )
      {
        res = res * y;
        y = y + one;
      }
    }
  }
  else
  {
//
//  Evaluate for 12.0 <= argument.
//
    if ( y <= xbig )
    {
      ysq = y * y;
      sum = c[6];
      for ( i = 0; i < 6; i++ )
      {
        sum = sum / ysq + c[i];
      }
      sum = sum / y - y + sqrtpi;
      sum = sum + ( y - 0.5 ) * std::log ( y );
      res = std::exp ( sum );
    }
    else
    {
      res = xinf;
      value = res;
      return value;
    }
  }
//
//  Final adjustments and return.
//
  if ( parity )
  {
    res = - res;
  }

  if ( fact != one )
  {
    res = fact / res;
  }

  value = res;

  return value;
}
//**************************************************************************80

double SandiaRules::r8_huge ( )

//**************************************************************************80
//
//  Purpose:
//
//    R8_HUGE returns a "huge" R8.
//
//  Discussion:
//
//    The value returned by this function is NOT required to be the
//    maximum representable R8.  This value varies from machine to machine,
//    from compiler to compiler, and may cause problems when being printed.
//    We simply want a "very large" but non-infinite number.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    06 October 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double R8_HUGE, a "huge" R8 value.
//
{
  double value;

  value = 1.0E+30;

  return value;
}
//**************************************************************************80

double SandiaRules::r8_hyper_2f1 ( double a, double b, double c, double x )

//**************************************************************************80
//
//  Purpose:
//
//    R8_HYPER_2F1 evaluates the hypergeometric function 2F1(A,B,C,X).
//
//  Discussion:
//
//    A bug was corrected.  A line which read
//      c1 = - ( - 1.0, m ) * gc / ( gam * gbm * rm );
//    was corrected to read
//      c1 = - std::pow ( - 1.0, m ) * gc / ( gam * gbm * rm );
//    JVB, 05 July 2009.
//
//    A minor bug was corrected.  The HW variable, used in several places as
//    the "old" value of a quantity being iteratively improved, was not
//    being initialized.  JVB, 11 February 2008.
//
//    The FORTRAN77 original version of this routine is copyrighted by
//    Shanjie Zhang and Jianming Jin.  However, they give permission to
//    incorporate this routine into a user program provided that the copyright
//    is acknowledged.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    05 July 2009
//
//  Author:
//
//    Original FORTRAN77 version by Shanjie Zhang, Jianming Jin.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Shanjie Zhang, Jianming Jin,
//    Computation of Special Functions,
//    Wiley, 1996,
//    ISBN: 0-471-11963-6,
//    LC: QA351.C45
//
//  Parameters:
//
//    Input, double A, B, C, X, the arguments of the function.
//    C must not be equal to a nonpositive integer.
//    X < 1.
//
//    Output, double R8_HYPER_2F1, the value of the function.
//
{
  double a0;
  double aa;
  double bb;
  double c0;
  double c1;
  double el = 0.5772156649015329;
  double eps;
  double f0;
  double f1;
  double g0;
  double g1;
  double g2;
  double g3;
  double ga;
  double gabc;
  double gam;
  double gb;
  double gbm;
  double gc;
  double gca;
  double gcab;
  double gcb;
  double gm;
  double hf;
  double hw;
  int j;
  int k;
  bool l0;
  bool l1;
  bool l2;
  bool l3;
  bool l4;
  bool l5;
  int m;
  int nm;
  double pa;
  double pb;
  double pi = 3.141592653589793;
  double r;
  double r0;
  double r1;
  double rm;
  double rp;
  double sm;
  double sp;
  double sp0;
  double x1;

  l0 = ( c == ( int ) ( c ) ) && ( c < 0.0 );
  l1 = ( 1.0 - x < 1.0E-15 ) && ( c - a - b <= 0.0 );
  l2 = ( a == ( int ) ( a ) ) && ( a < 0.0 );
  l3 = ( b == ( int ) ( b ) ) && ( b < 0.0 );
  l4 = ( c - a == ( int ) ( c - a ) ) && ( c - a <= 0.0 );
  l5 = ( c - b == ( int ) ( c - b ) ) && ( c - b <= 0.0 );

  if ( l0 )
  {
    std::cerr << "\n";
    std::cerr << "R8_HYPER_2F1 - Fatal error!\n";
    std::cerr << "  The hypergeometric series is divergent.\n";
    std::cerr << "  C is integral and negative.\n";
    std::cerr << "  C = " << c << "\n";
    std::exit ( 1 );
  }

  if ( l1 )
  {
    std::cerr << "\n";
    std::cerr << "R8_HYPER_2F1 - Fatal error!\n";
    std::cerr << "  The hypergeometric series is divergent.\n";
    std::cerr << "  1 - X < 0, C - A - B <= 0\n";
    std::cerr << "  A = " << a << "\n";
    std::cerr << "  B = " << b << "\n";
    std::cerr << "  C = " << c << "\n";
    std::cerr << "  X = " << x << "\n";
    std::exit ( 1 );
  }

  if ( 0.95 < x )
  {
    eps = 1.0E-08;
  }
  else
  {
    eps = 1.0E-15;
  }

  if ( x == 0.0 || a == 0.0 || b == 0.0 )
  {
    hf = 1.0;
    return hf;
  }
  else if ( 1.0 - x == eps && 0.0 < c - a - b )
  {
    gc = r8_gamma ( c );
    gcab = r8_gamma ( c - a - b );
    gca = r8_gamma ( c - a );
    gcb = r8_gamma ( c - b );
    hf = gc * gcab / ( gca * gcb );
    return hf;
  }
  else if ( 1.0 + x <= eps && r8_abs ( c - a + b - 1.0 ) <= eps )
  {
    g0 = std::sqrt ( pi ) * std::pow ( 2.0, - a );
    g1 = r8_gamma ( c );
    g2 = r8_gamma ( 1.0 + a / 2.0 - b );
    g3 = r8_gamma ( 0.5 + 0.5 * a );
    hf = g0 * g1 / ( g2 * g3 );
    return hf;
  }
  else if ( l2 || l3 )
  {
    if ( l2 )
    {
      nm = ( int ) ( r8_abs ( a ) );
    }

    if ( l3 )
    {
      nm = ( int ) ( r8_abs ( b ) );
    }

    hf = 1.0;
    r = 1.0;

    for ( k = 1; k <= nm; k++ )
    {
      r = r * ( a + k - 1.0 ) * ( b + k - 1.0 )
        / ( k * ( c + k - 1.0 ) ) * x;
      hf = hf + r;
    }

    return hf;
  }
  else if ( l4 || l5 )
  {
    if ( l4 )
    {
      nm = ( int ) ( r8_abs ( c - a ) );
    }

    if ( l5 )
    {
      nm = ( int ) ( r8_abs ( c - b ) );
    }

    hf = 1.0;
    r  = 1.0;
    for ( k = 1; k <= nm; k++ )
    {
      r = r * ( c - a + k - 1.0 ) * ( c - b + k - 1.0 )
        / ( k * ( c + k - 1.0 ) ) * x;
      hf = hf + r;
    }
    hf = std::pow ( 1.0 - x, c - a - b ) * hf;
    return hf;
  }

  aa = a;
  bb = b;
  x1 = x;

  if ( x < 0.0 )
  {
    x = x / ( x - 1.0 );
    if ( a < c && b < a && 0.0 < b )
    {
      a = bb;
      b = aa;
    }
    b = c - b;
  }

  if ( 0.75 <= x )
  {
    gm = 0.0;

    if ( r8_abs ( c - a - b - ( int ) ( c - a - b ) ) < 1.0E-15 )
    {
      m = ( int ) ( c - a - b );
      ga = r8_gamma ( a );
      gb = r8_gamma ( b );
      gc = r8_gamma ( c );
      gam = r8_gamma ( a + m );
      gbm = r8_gamma ( b + m );

      pa = r8_psi ( a );
      pb = r8_psi ( b );

      if ( m != 0 )
      {
        gm = 1.0;
      }

      for ( j = 1; j <= std::abs ( m ) - 1; j++ )
      {
        gm = gm * j;
      }

      rm = 1.0;
      for ( j = 1; j <= std::abs ( m ); j++ )
      {
        rm = rm * j;
      }

      f0 = 1.0;
      r0 = 1.0;;
      r1 = 1.0;
      sp0 = 0.0;;
      sp = 0.0;

      if ( 0 <= m )
      {
        c0 = gm * gc / ( gam * gbm );
        c1 = - gc * std::pow ( x - 1.0, m ) / ( ga * gb * rm );

        for ( k = 1; k <= m - 1; k++ )
        {
          r0 = r0 * ( a + k - 1.0 ) * ( b + k - 1.0 )
            / ( k * ( k - m ) ) * ( 1.0 - x );
          f0 = f0 + r0;
        }

        for ( k = 1; k <= m; k++ )
        {
          sp0 = sp0 + 1.0 / ( a + k - 1.0 ) + 1.0 / ( b + k - 1.0 )
          - 1.0 / ( double ) ( k );
        }

        f1 = pa + pb + sp0 + 2.0 * el + std::log ( 1.0 - x );
        hw = f1;

        for ( k = 1; k <= 250; k++ )
        {
          sp = sp + ( 1.0 - a ) / ( k * ( a + k - 1.0 ) )
            + ( 1.0 - b ) / ( k * ( b + k - 1.0 ) );

          sm = 0.0;
          for ( j = 1; j <= m; j++ )
          {
            sm = sm + ( 1.0 - a )
              / ( ( j + k ) * ( a + j + k - 1.0 ) )
              + 1.0 / ( b + j + k - 1.0 );
          }

          rp = pa + pb + 2.0 * el + sp + sm + std::log ( 1.0 - x );

          r1 = r1 * ( a + m + k - 1.0 ) * ( b + m + k - 1.0 )
            / ( k * ( m + k ) ) * ( 1.0 - x );

          f1 = f1 + r1 * rp;

          if ( r8_abs ( f1 - hw ) < r8_abs ( f1 ) * eps )
          {
            break;
          }
          hw = f1;
        }
        hf = f0 * c0 + f1 * c1;
      }
      else if ( m < 0 )
      {
        m = - m;
        c0 = gm * gc / ( ga * gb * std::pow ( 1.0 - x, m ) );
        c1 = - std::pow ( - 1.0, m ) * gc / ( gam * gbm * rm );

        for ( k = 1; k <= m - 1; k++ )
        {
          r0 = r0 * ( a - m + k - 1.0 ) * ( b - m + k - 1.0 )
            / ( k * ( k - m ) ) * ( 1.0 - x );
          f0 = f0 + r0;
        }

        for ( k = 1; k <= m; k++ )
        {
          sp0 = sp0 + 1.0 / ( double ) ( k );
        }

        f1 = pa + pb - sp0 + 2.0 * el + std::log ( 1.0 - x );
        hw = f1;

        for ( k = 1; k <= 250; k++ )
        {
          sp = sp + ( 1.0 - a )
            / ( k * ( a + k - 1.0 ) )
            + ( 1.0 - b ) / ( k * ( b + k - 1.0 ) );

          sm = 0.0;
          for ( j = 1; j <= m; j++ )
          {
            sm = sm + 1.0 / ( double ) ( j + k );
          }

          rp = pa + pb + 2.0 * el + sp - sm + std::log ( 1.0 - x );

          r1 = r1 * ( a + k - 1.0 ) * ( b + k - 1.0 )
            / ( k * ( m + k ) ) * ( 1.0 - x );

          f1 = f1 + r1 * rp;

          if ( r8_abs ( f1 - hw ) < r8_abs ( f1 ) * eps )
          {
            break;
          }

          hw = f1;
        }

        hf = f0 * c0 + f1 * c1;
      }
    }
    else
    {
      ga = r8_gamma ( a );
      gb = r8_gamma ( b );
      gc = r8_gamma ( c );
      gca = r8_gamma ( c - a );
      gcb = r8_gamma ( c - b );
      gcab = r8_gamma ( c - a - b );
      gabc = r8_gamma ( a + b - c );
      c0 = gc * gcab / ( gca * gcb );
      c1 = gc * gabc / ( ga * gb ) * std::pow ( 1.0 - x, c - a - b );
      hf = 0.0;
      hw = hf;
      r0 = c0;
      r1 = c1;

      for ( k = 1; k <= 250; k++ )
      {
        r0 = r0 * ( a + k - 1.0 ) * ( b + k - 1.0 )
          / ( k * ( a + b - c + k ) ) * ( 1.0 - x );

        r1 = r1 * ( c - a + k - 1.0 ) * ( c - b + k - 1.0 )
          / ( k * ( c - a - b + k ) ) * ( 1.0 - x );

        hf = hf + r0 + r1;

        if ( r8_abs ( hf - hw ) < r8_abs ( hf ) * eps )
        {
          break;
        }
        hw = hf;
      }
      hf = hf + c0 + c1;
    }
  }
  else
  {
    a0 = 1.0;

    if ( a < c && c < 2.0 * a && b < c && c < 2.0 * b )
    {
      a0 = std::pow ( 1.0 - x, c - a - b );
      a = c - a;
      b = c - b;
    }

    hf = 1.0;
    hw = hf;
    r = 1.0;

    for ( k = 1; k <= 250; k++ )
    {
      r = r * ( a + k - 1.0 ) * ( b + k - 1.0 )
        / ( k * ( c + k - 1.0 ) ) * x;

      hf = hf + r;

      if ( r8_abs ( hf - hw ) <= r8_abs ( hf ) * eps )
      {
        break;
      }

      hw = hf;
    }
    hf = a0 * hf;
  }

  if ( x1 < 0.0 )
  {
    x = x1;
    c0 = 1.0 / std::pow ( 1.0 - x, aa );
    hf = c0 * hf;
  }

  a = aa;
  b = bb;

  if ( 120 < k )
  {
    std::cerr << "\n";
    std::cerr << "R8_HYPER_2F1 - Warning!\n";
    std::cerr << "  A large number of iterations were needed.\n";
    std::cerr << "  The accuracy of the results should be checked.\n";
  }

  return hf;
}
//**************************************************************************80

double SandiaRules::r8_max ( double x, double y )

//**************************************************************************80
//
//  Purpose:
//
//    R8_MAX returns the maximum of two R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, Y, the quantities to compare.
//
//    Output, double R8_MAX, the maximum of X and Y.
//
{
  double value;

  if ( y < x )
  {
    value = x;
  }
  else
  {
    value = y;
  }
  return value;
}
//**************************************************************************80

double SandiaRules::r8_min ( double x, double y )

//**************************************************************************80
//
//  Purpose:
//
//    R8_MIN returns the minimum of two R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    31 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, Y, the quantities to compare.
//
//    Output, double R8_MIN, the minimum of X and Y.
//
{
  double value;

  if ( y < x )
  {
    value = y;
  }
  else
  {
    value = x;
  }
  return value;
}
//**************************************************************************80

double SandiaRules::r8_mop ( int i )

//**************************************************************************80
//
//  Purpose:
//
//    R8_MOP returns the I-th power of -1 as an R8 value.
//
//  Discussion:
//
//    An R8 is an double value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    16 November 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the power of -1.
//
//    Output, double R8_MOP, the I-th power of -1.
//
{
  double value;

  if ( ( i % 2 ) == 0 )
  {
    value = 1.0;
  }
  else
  {
    value = -1.0;
  }

  return value;
}
//**************************************************************************80

double SandiaRules::r8_psi ( double xx )

//**************************************************************************80
//
//  Purpose:
//
//    R8_PSI evaluates the function Psi(X).
//
//  Discussion:
//
//    This routine evaluates the logarithmic derivative of the
//    Gamma function,
//
//      PSI(X) = d/dX ( GAMMA(X) ) / GAMMA(X)
//             = d/dX LN ( GAMMA(X) )
//
//    for real X, where either
//
//      - XMAX1 < X < - XMIN, and X is not a negative integer,
//
//    or
//
//      XMIN < X.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    09 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by William Cody.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    William Cody, Anthony Strecok, Henry Thacher,
//    Chebyshev Approximations for the Psi Function,
//    Mathematics of Computation,
//    Volume 27, Number 121, January 1973, pages 123-127.
//
//  Parameters:
//
//    Input, double XX, the argument of the function.
//
//    Output, double R8_PSI, the value of the function.
//
{
  double aug;
  double den;
  int i;
  int n;
  int nq;
  double one = 1.0;
  double p1[9] = {
   4.5104681245762934160E-03,
   5.4932855833000385356,
   3.7646693175929276856E+02,
   7.9525490849151998065E+03,
   7.1451595818951933210E+04,
   3.0655976301987365674E+05,
   6.3606997788964458797E+05,
   5.8041312783537569993E+05,
   1.6585695029761022321E+05 };
  double p2[7] = {
  -2.7103228277757834192,
  -1.5166271776896121383E+01,
  -1.9784554148719218667E+01,
  -8.8100958828312219821,
  -1.4479614616899842986,
  -7.3689600332394549911E-02,
  -6.5135387732718171306E-21 };
  double piov4 = 0.78539816339744830962;
  double q1[8] = {
   9.6141654774222358525E+01,
   2.6287715790581193330E+03,
   2.9862497022250277920E+04,
   1.6206566091533671639E+05,
   4.3487880712768329037E+05,
   5.4256384537269993733E+05,
   2.4242185002017985252E+05,
   6.4155223783576225996E-08 };
  double q2[6] = {
   4.4992760373789365846E+01,
   2.0240955312679931159E+02,
   2.4736979003315290057E+02,
   1.0742543875702278326E+02,
   1.7463965060678569906E+01,
   8.8427520398873480342E-01 };
  double sgn;
  double three = 3.0;
  double upper;
  double value;
  double w;
  double x;
  double x01 = 187.0;
  double x01d = 128.0;
  double x02 = 6.9464496836234126266E-04;
  double xinf = 1.70E+38;
  double xlarge = 2.04E+15;
  double xmax1 = 3.60E+16;
  double xmin1 = 5.89E-39;
  double xsmall = 2.05E-09;
  double z;

  x = xx;
  w = r8_abs ( x );
  aug = 0.0;
//
//  Check for valid arguments, then branch to appropriate algorithm.
//
  if ( xmax1 <= - x || w < xmin1 )
  {
    if ( 0.0 < x )
    {
      value = - xinf;
    }
    else
    {
      value = xinf;
    }
    return value;
  }

  if ( x < 0.5 )
  {
//
//  X < 0.5, use reflection formula: psi(1-x) = psi(x) + pi * cot(pi*x)
//  Use 1/X for PI*COTAN(PI*X)  when  XMIN1 < |X| <= XSMALL.
//
    if ( w <= xsmall )
    {
      aug = - one / x;
    }
//
//  Argument reduction for cotangent.
//
    else
    {
      if ( x < 0.0 )
      {
        sgn = piov4;
      }
      else
      {
        sgn = - piov4;
      }

      w = w - ( double ) ( ( int ) ( w ) );
      nq = ( int ) ( w * 4.0 );
      w = 4.0 * ( w - ( double ) ( nq ) * 0.25 );
//
//  W is now related to the fractional part of 4.0 * X.
//  Adjust argument to correspond to values in the first
//  quadrant and determine the sign.
//
      n = nq / 2;

      if ( n + n != nq )
      {
        w = one - w;
      }

      z = piov4 * w;

      if ( ( n % 2 ) != 0 )
      {
        sgn = - sgn;
      }
//
//  Determine the final value for  -pi * cotan(pi*x).
//
      n = ( nq + 1 ) / 2;
      if ( ( n % 2 ) == 0 )
      {
//
//  Check for singularity.
//
        if ( z == 0.0 )
        {
          if ( 0.0 < x )
          {
            value = -xinf;
          }
          else
          {
            value = xinf;
          }
          return value;
        }
        aug = sgn * ( 4.0 / std::tan ( z ) );
      }
      else
      {
        aug = sgn * ( 4.0 * std::tan ( z ) );
      }
    }
    x = one - x;
  }
//
//  0.5 <= X <= 3.0.
//
  if ( x <= three )
  {
    den = x;
    upper = p1[0] * x;
    for ( i = 1; i <= 7; i++ )
    {
      den = ( den + q1[i-1] ) * x;
      upper = ( upper + p1[i]) * x;
    }
    den = ( upper + p1[8] ) / ( den + q1[7] );
    x = ( x - x01 / x01d ) - x02;
    value = den * x + aug;
    return value;
  }
//
//  3.0 < X.
//
  if ( x < xlarge )
  {
    w = one / ( x * x );
    den = w;
    upper = p2[0] * w;
    for ( i = 1; i <= 5; i++ )
    {
      den = ( den + q2[i-1] ) * w;
      upper = ( upper + p2[i] ) * w;
    }
    aug = ( upper + p2[6] ) / ( den + q2[5] ) - 0.5 / x + aug;
  }

  value = aug + std::log ( x );

  return value;
}
//**************************************************************************80

double SandiaRules::r8_sign ( double x )

//**************************************************************************80
//
//  Purpose:
//
//    R8_SIGN returns the sign of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose sign is desired.
//
//    Output, double R8_SIGN, the sign of X.
//
{
  double value;

  if ( x < 0.0 )
  {
    value = -1.0;
  }
  else
  {
    value = 1.0;
  }
  return value;
}
//**************************************************************************80

int SandiaRules::r8col_compare ( int m, int n, double a[], int i, int j )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_COMPARE compares two columns in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Example:
//
//    Input:
//
//      M = 3, N = 4, I = 2, J = 4
//
//      A = (
//        1.  2.  3.  4.
//        5.  6.  7.  8.
//        9. 10. 11. 12. )
//
//    Output:
//
//      R8COL_COMPARE = -1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the M by N array.
//
//    Input, int I, J, the columns to be compared.
//    I and J must be between 1 and N.
//
//    Output, int R8COL_COMPARE, the results of the comparison:
//    -1, column I < column J,
//     0, column I = column J,
//    +1, column J < column I.
//
{
  int k;
  int value;
//
//  Check.
//
  if ( i < 1 || n < i )
  {
    std::cerr << "\n";
    std::cerr << "R8COL_COMPARE - Fatal error!\n";
    std::cerr << "  Column index I is out of bounds.\n";
    std::cerr << "  I = " << i << "\n";
    std::exit ( 1 );
  }

  if ( j < 1 || n < j )
  {
    std::cerr << "\n";
    std::cerr << "R8COL_COMPARE - Fatal error!\n";
    std::cerr << "  Column index J is out of bounds.\n";
    std::cerr << "  J = " << j << "\n";
    std::exit ( 1 );
  }

  value = 0;

  if ( i == j )
  {
    return value;
  }

  k = 0;

  while ( k < m )
  {
    if ( a[k+(i-1)*m] < a[k+(j-1)*m] )
    {
      value = -1;
      return value;
    }
    else if ( a[k+(j-1)*m] < a[k+(i-1)*m] )
    {
      value = +1;
      return value;
    }
    k = k + 1;
  }

  return value;
}
//**************************************************************************80

void SandiaRules::r8col_sort_heap_a ( int m, int n, double a[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_SORT_HEAP_A ascending heapsorts an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    In lexicographic order, the statement "X < Y", applied to two real
//    vectors X and Y of length M, means that there is some index I, with
//    1 <= I <= M, with the property that
//
//      X(J) = Y(J) for J < I,
//    and
//      X(I) < Y(I).
//
//    In other words, the first time they differ, X is smaller.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, double A[M*N].
//    On input, the array of N columns of M-vectors.
//    On output, the columns of A have been sorted in lexicographic order.
//
{
  int i;
  int indx;
  int isgn;
  int j;

  if ( m <= 0 )
  {
    return;
  }

  if ( n <= 1 )
  {
    return;
  }
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      r8col_swap ( m, n, a, i, j );
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = r8col_compare ( m, n, a, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }
  }

  return;
}
//**************************************************************************80

int *SandiaRules::r8col_sort_heap_index_a ( int m, int n, double a[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_SORT_HEAP_INDEX_A does an indexed heap ascending sort of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    A(*,J1) < A(*,J2) if the first nonzero entry of A(*,J1)-A(*,J2)
//    is negative.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      A(*,INDX(*)) is sorted,
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in each column of A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the array.
//
//    Output, int SandiaRules::r8col_SORT_HEAP_INDEX_A[N], contains the sort index.  The
//    I-th column of the sorted array is A(*,INDX(I)).
//
{
  double *column;
  int i;
  int *indx;
  int indxt;
  int ir;
  int isgn;
  int j;
  int k;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  column = new double[m];

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      for ( k = 0; k < m; k++ )
      {
        column[k] = a[k+indxt*m];
      }
    }
    else
    {
      indxt = indx[ir-1];
      for ( k = 0; k < m; k++ )
      {
        column[k] = a[k+indxt*m];
      }
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        isgn = r8vec_compare ( m, a+indx[j-1]*m, a+indx[j]*m );

        if ( isgn < 0 )
        {
          j = j + 1;
        }
      }

      isgn = r8vec_compare ( m, column, a+indx[j-1]*m );

      if ( isgn < 0 )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }
  delete [] column;

  return indx;
}
//**************************************************************************80

int SandiaRules::r8col_sorted_unique_count ( int m, int n, double a[], double tol )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_SORTED_UNIQUE_COUNT counts unique elements in a sorted R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The columns of the array may be ascending or descending sorted.
//
//    If the tolerance is large enough, then the concept of uniqueness
//    can become ambiguous.  If we have a tolerance of 1.5, then in the
//    list ( 1, 2, 3, 4, 5, 6, 7, 8, 9 ) is it fair to say we have only
//    one unique entry?  That would be because 1 may be regarded as unique,
//    and then 2 is too close to 1 to be unique, and 3 is too close to 2 to
//    be unique and so on.
//
//    This seems wrongheaded.  So I prefer the idea that an item is not
//    unique under a tolerance only if it is close to something that IS unique.
//    Thus, the unique items are guaranteed to cover the space if we include
//    a disk of radius TOL around each one.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], a sorted array, containing
//    N columns of data.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int R8COL_SORTED_UNIQUE_COUNT, the number of unique columns.
//
{
  double diff;
  int i;
  int j1;
  int j2;
  int unique_num;

  unique_num = 0;

  if ( n <= 0 )
  {
    return unique_num;
  }

  unique_num = 1;
  j1 = 0;

  for ( j2 = 1; j2 < n; j2++ )
  {
    diff = 0.0;
    for ( i = 0; i < m; i++ )
    {
      diff = r8_max ( diff, r8_abs ( a[i+j1*m] - a[i+j2*m] ) );
    }
    if ( tol < diff )
    {
      unique_num = unique_num + 1;
      j1 = j2;
    }
  }

  return unique_num;
}
//**************************************************************************80

void SandiaRules::r8col_swap ( int m, int n, double a[], int j1, int j2 )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_SWAP swaps columns J1 and J2 of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Example:
//
//    Input:
//
//      M = 3, N = 4, J1 = 2, J2 = 4
//
//      A = (
//        1.  2.  3.  4.
//        5.  6.  7.  8.
//        9. 10. 11. 12. )
//
//    Output:
//
//      A = (
//        1.  4.  3.  2.
//        5.  8.  7.  6.
//        9. 12. 11. 10. )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, double A[M*N], the M by N array.
//
//    Input, int J1, J2, the columns to be swapped.
//    These columns are 1-based.
//
{
  int i;
  double temp;

  if ( j1 < 1 || n < j1 || j2 < 1 || n < j2 )
  {
    std::cerr << "\n";
    std::cerr << "R8COL_SWAP - Fatal error!\n";
    std::cerr << "  J1 or J2 is out of bounds.\n";
    std::cerr << "  J1 =   " << j1 << "\n";
    std::cerr << "  J2 =   " << j2 << "\n";
    std::cerr << "  NCOL = " << n << "\n";
    std::exit ( 1 );
  }

  if ( j1 == j2 )
  {
    return;
  }

  for ( i = 0; i < m; i++ )
  {
    temp          = a[i+(j1-1)*m];
    a[i+(j1-1)*m] = a[i+(j2-1)*m];
    a[i+(j2-1)*m] = temp;
  }

  return;
}
//**************************************************************************80

void SandiaRules::r8col_tol_undex ( int m, int n, double a[], int unique_num, double tol,
  int undx[], int xdnu[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_TOL_UNDEX indexes tolerably unique entries of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points to the unique elements of A, in sorted order,
//    and a vector XDNU, which identifies, for each entry of A, the index of
//    the unique sorted element of A.
//
//    This is all done with index vectors, so that the elements of
//    A are never moved.
//
//    The first step of the algorithm requires the indexed sorting
//    of A, which creates arrays INDX and XDNI.  (If all the entries
//    of A are unique, then these arrays are the same as UNDX and XDNU.)
//
//    We then use INDX to examine the entries of A in sorted order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector A could be
//    replaced by a compressed vector XU, containing the unique entries
//    of A in sorted order, using the formula
//
//      XU(*) = A(UNDX(*)).
//
//    We could then, if we wished, reconstruct the entire vector A, or
//    any element of it, by index, as follows:
//
//      A(I) = XU(XDNU(I)).
//
//    We could then replace A by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of A, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector A, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I     A  Indx  Xdni       XU  Undx  Xdnu
//    ----+-----+-----+-----+--------+-----+-----+
//      0 | 11.     0     0 |    11.     0     0
//      1 | 22.     2     4 |    22.     1     1
//      2 | 11.     5     1 |    33.     3     0
//      3 | 33.     8     7 |    55.     4     2
//      4 | 55.     1     8 |                  3
//      5 | 11.     6     2 |                  0
//      6 | 22.     7     5 |                  1
//      7 | 22.     3     6 |                  1
//      8 | 11.     4     3 |                  0
//
//    INDX(2) = 3 means that sorted item(2) is A(3).
//    XDNI(2) = 5 means that A(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at A(4).
//    XDNU(8) = 2 means that A(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = X(I).
//    XU(I)        = X(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 July 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the dimension of the data values.
//
//    Input, int N, the number of data values,
//
//    Input, double A[M*N], the data values.
//
//    Input, int UNIQUE_NUM, the number of unique values in A.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int UNDX[UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[N], the XDNU vector.
//
{
  double diff;
  int i;
  int i2;
  int *indx;
  int j;
  int k;
  bool unique;
//
//  Implicitly sort the array.
//
  indx = r8col_sort_heap_index_a ( m, n, a );
//
//  Consider entry I = 0.
//  It is unique, so set the number of unique items to K.
//  Set the K-th unique item to I.
//  Set the representative of item I to the K-th unique item.
//
  i = 0;
  k = 0;
  undx[k] = indx[i];
  xdnu[indx[i]] = k;
//
//  Consider entry I.
//
//  If it is unique, increase the unique count K, set the
//  K-th unique item to I, and set the representative of I to K.
//
//  If it is not unique, set the representative of item I to a
//  previously determined unique item that is close to it.
//
  for ( i = 1; i < n; i++ )
  {
    unique = true;
    for ( j = 0; j <= k; j++ )
    {
      diff = 0.0;
      for ( i2 = 0; i2 < m; i2++ )
      {
        diff = r8_max ( diff,
          r8_abs ( a[i2+indx[i]*m] - a[i2+undx[j]*m] ) );
      }
      if ( diff <= tol )
      {
        unique = false;
        xdnu[indx[i]] = j;
        break;
      }
    }
    if ( unique )
    {
      k = k + 1;
      undx[k] = indx[i];
      xdnu[indx[i]] = k;
    }
  }
  delete [] indx;

  return;
}
//**************************************************************************80

int SandiaRules::r8col_tol_unique_count ( int m, int n, double a[], double tol )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_TOL_UNIQUE_COUNT counts tolerably unique entries in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The columns of the array may be ascending or descending sorted.
//
//    If the tolerance is large enough, then the concept of uniqueness
//    can become ambiguous.  If we have a tolerance of 1.5, then in the
//    list ( 1, 2, 3, 4, 5, 6, 7, 8, 9 ) is it fair to say we have only
//    one unique entry?  That would be because 1 may be regarded as unique,
//    and then 2 is too close to 1 to be unique, and 3 is too close to 2 to
//    be unique and so on.
//
//    This seems wrongheaded.  So I prefer the idea that an item is not
//    unique under a tolerance only if it is close to something that IS unique.
//    Thus, the unique items are guaranteed to cover the space if we include
//    a disk of radius TOL around each one.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 July 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the array of N columns of data.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int R8COL_TOL_UNIQUE_COUNT, the number of unique columns.
//
{
  double diff;
  int i;
  int i2;
  int *indx;
  int j;
  int k;
  bool unique;
  int *undx;

  undx = new int[n];
//
//  Implicitly sort the array.
//
  indx = r8col_sort_heap_index_a ( m, n, a );
//
//  Consider entry I = 0.
//  It is unique, so set the number of unique items to K.
//  Set the K-th unique item to I.
//  Set the representative of item I to the K-th unique item.
//
  i = 0;
  k = 0;
  undx[k] = indx[i];
//
//  Consider entry I.
//
//  If it is unique, increase the unique count K, set the
//  K-th unique item to I, and set the representative of I to K.
//
//  If it is not unique, set the representative of item I to a
//  previously determined unique item that is close to it.
//
  for ( i = 1; i < n; i++ )
  {
    unique = true;
    for ( j = 0; j <= k; j++ )
    {
      diff = 0.0;
      for ( i2 = 0; i2 < m; i2++ )
      {
        diff = r8_max ( diff,
          r8_abs ( a[i2+indx[i]*m] - a[i2+undx[j]*m] ) );
      }
      if ( diff <= tol )
      {
        unique = false;
        break;
      }
    }
    if ( unique )
    {
      k = k + 1;
      undx[k] = indx[i];
    }
  }
  delete [] indx;
  delete [] undx;

  k = k + 1;

  return k;
}
//**************************************************************************80

void SandiaRules::r8col_undex ( int x_dim, int x_num, double x_val[], int x_unique_num,
  double tol, int undx[], int xdnu[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_UNDEX returns unique sorted indexes for an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points to the unique elements of X, in sorted order,
//    and a vector XDNU, which identifies, for each entry of X, the index of
//    the unique sorted element of X.
//
//    This is all done with index vectors, so that the elements of
//    X are never moved.
//
//    The first step of the algorithm requires the indexed sorting
//    of X, which creates arrays INDX and XDNI.  (If all the entries
//    of X are unique, then these arrays are the same as UNDX and XDNU.)
//
//    We then use INDX to examine the entries of X in sorted order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector X could be
//    replaced by a compressed vector XU, containing the unique entries
//    of X in sorted order, using the formula
//
//      XU(*) = X(UNDX(*)).
//
//    We could then, if we wished, reconstruct the entire vector X, or
//    any element of it, by index, as follows:
//
//      X(I) = XU(XDNU(I)).
//
//    We could then replace X by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of X, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector X, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I     X  Indx  Xdni       XU  Undx  Xdnu
//    ----+-----+-----+-----+--------+-----+-----+
//      0 | 11.     0     0 |    11.     0     0
//      1 | 22.     2     4 |    22.     1     1
//      2 | 11.     5     1 |    33.     3     0
//      3 | 33.     8     7 |    55.     4     2
//      4 | 55.     1     8 |                  3
//      5 | 11.     6     2 |                  0
//      6 | 22.     7     5 |                  1
//      7 | 22.     3     6 |                  1
//      8 | 11.     4     3 |                  0
//
//    INDX(2) = 3 means that sorted item(2) is X(3).
//    XDNI(2) = 5 means that X(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at X(4).
//    XDNU(8) = 2 means that X(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = X(I).
//    XU(I)        = X(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X_DIM, the dimension of the data values.
//    (the number of rows in the R8COL).
//
//    Input, int X_NUM, the number of data values,
//    (the number of columns in the R8COL).
//
//    Input, double X_VAL[X_DIM*X_NUM], the data values.
//
//    Input, int X_UNIQUE_NUM, the number of unique values in X_VAL.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int UNDX[X_UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[X_NUM], the XDNU vector.
//
{
  double diff;
  int i;
  int *indx;
  int j;
  int k;
//
//  Implicitly sort the array.
//
  indx = r8col_sort_heap_index_a ( x_dim, x_num, x_val );
//
//  Walk through the implicitly sorted array X.
//
  i = 0;

  j = 0;
  undx[j] = indx[i];

  xdnu[indx[i]] = j;

  for ( i = 1; i < x_num; i++ )
  {
    diff = 0.0;
    for ( k = 0; k < x_dim; k++ )
    {
      diff = r8_max ( diff,
        r8_abs ( x_val[k+indx[i]*x_dim] - x_val[k+undx[j]*x_dim] ) );
    }
    if ( tol < diff )
    {
      j = j + 1;
      undx[j] = indx[i];
    }
    xdnu[indx[i]] = j;
  }
  delete [] indx;

  return;
}
//**************************************************************************80

void SandiaRules::r8col_unique_index ( int m, int n, double a[], double tol,
  int unique_index[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8COL_UNIQUE_INDEX indexes the first occurrence of values in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8 values.
//    It is regarded as an array of N columns of length M.
//
//    For element A(1:M,J) of the matrix, UNIQUE_INDEX(J) is the uniqueness
//   index of A(1:M,J).  That is, if A_UNIQUE contains the unique elements
//    of A, gathered in order, then
//
//      A_UNIQUE ( 1:M, UNIQUE_INDEX(J) ) = A(1:M,J)
//
//    The user must preallocate space for the output array UNIQUE_INDEX.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of A.
//    The length of an "element" of A, and the number of "elements".
//
//    Input, double A[M*N], the array.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int UNIQUE_INDEX[N], the unique index.
//
{
  double diff;
  int i;
  int j1;
  int j2;
  int unique_num;

  for ( j1 = 0; j1 < n; j1++ )
  {
    unique_index[j1] = -1;
  }
  unique_num = 0;

  for ( j1 = 0; j1 < n; j1++ )
  {
    if ( unique_index[j1] == -1 )
    {
      unique_index[j1] = unique_num;

      for ( j2 = j1 + 1; j2 < n; j2++ )
      {
        diff = 0.0;
        for ( i = 0; i < m; i++ )
        {
          diff = r8_max ( diff,
            r8_abs ( a[i+j1*m] - a[i+j2*m] ) );
        }
        if ( diff <= tol )
        {
          unique_index[j2] = unique_num;
        }
      }
      unique_num = unique_num + 1;
    }
  }
  return;
}
//**************************************************************************80

void SandiaRules::r8mat_transpose_print ( int m, int n, double a[], std::string title )

//**************************************************************************80
//
//  Purpose:
//
//    R8MAT_TRANSPOSE_PRINT prints an R8MAT, transposed.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values, stored as a vector
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    10 September 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], an M by N matrix to be printed.
//
//    Input, string TITLE, a title.
//
{
  r8mat_transpose_print_some ( m, n, a, 1, 1, m, n, title );

  return;
}
//**************************************************************************80

void SandiaRules::r8mat_transpose_print_some ( int m, int n, double a[], int ilo, int jlo,
  int ihi, int jhi, std::string title )

//**************************************************************************80
//
//  Purpose:
//
//    R8MAT_TRANSPOSE_PRINT_SOME prints some of an R8MAT, transposed.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values, stored as a vector
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    10 September 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], an M by N matrix to be printed.
//
//    Input, int ILO, JLO, the first row and column to print.
//
//    Input, int IHI, JHI, the last row and column to print.
//
//    Input, string TITLE, a title.
//
{
# define INCX 5

  int i;
  int i2;
  int i2hi;
  int i2lo;
  int inc;
  int j;
  int j2hi;
  int j2lo;

  std::cout << "\n";
  std::cout << title << "\n";

  for ( i2lo = i4_max ( ilo, 1 ); i2lo <= i4_min ( ihi, m ); i2lo = i2lo + INCX )
  {
    i2hi = i2lo + INCX - 1;
    i2hi = i4_min ( i2hi, m );
    i2hi = i4_min ( i2hi, ihi );

    inc = i2hi + 1 - i2lo;

    std::cout << "\n";
    std::cout << "  Row: ";
    for ( i = i2lo; i <= i2hi; i++ )
    {
      std::cout << std::setw(7) << i - 1 << "       ";
    }
    std::cout << "\n";
    std::cout << "  Col\n";
    std::cout << "\n";

    j2lo = i4_max ( jlo, 1 );
    j2hi = i4_min ( jhi, n );

    for ( j = j2lo; j <= j2hi; j++ )
    {
      std::cout << std::setw(5) << j - 1 << ":";
      for ( i2 = 1; i2 <= inc; i2++ )
      {
        i = i2lo - 1 + i2;
        std::cout << std::setw(14) << a[(i-1)+(j-1)*m];
      }
      std::cout << "\n";
    }
  }

  return;
# undef INCX
}
//**************************************************************************80

void SandiaRules::r8mat_write ( std::string output_filename, int m, int n, double table[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8MAT_WRITE writes an R8MAT file.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 August 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, string OUTPUT_FILENAME, the output filename.
//
//    Input, int M, the spatial dimension.
//
//    Input, int N, the number of points.
//
//    Input, double TABLE[M*N], the table data.
//
{
  int i;
  int j;
  std::ofstream output;
//
//  Open the file.
//
  output.open ( output_filename.c_str ( ) );

  if ( !output )
  {
    std::cerr << "\n";
    std::cerr << "R8MAT_WRITE - Fatal error!\n";
    std::cerr << "  Could not open the output file.\n";
    return;
  }
//
//  Write the data.
//
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      output << "  " << std::setw(24) << std::setprecision(16) << table[i+j*m];
    }
    output << "\n";
  }
//
//  Close the file.
//
  output.close ( );

  return;
}
//**************************************************************************80

double SandiaRules::r8poly_ant_val ( int n, double poly_cof[], double xval )

//**************************************************************************80
//
//  Purpose:
//
//    R8POLY_ANT_VAL evaluates the antiderivative of an R8POLY in standard form.
//
//  Discussion:
//
//    The constant term of the antiderivative is taken to be zero.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 June 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the polynomial.
//
//    Input, double POLY_COF[N], the polynomial coefficients.  POLY_COF[0]
//    is the constant term, and POLY_COF[N-1] is the coefficient of X**(N-1).
//
//    Input, double XVAL, the point where the antiderivative is to be
//    evaluated.
//
//    Output, double R8POLY_ANT_VAL, the value of the antiderivative of the polynomial
//    at XVAL.
//
{
  int i;
  double value;

  value = 0.0;

  for ( i = n - 1; 0 <= i; i-- )
  {
    value = ( value + poly_cof[i] / ( double ) ( i + 1 ) ) * xval;
  }

  return value;
}
//**************************************************************************80

double *SandiaRules::r8vec_chebyshev_new ( int n, double a_first, double a_last )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_CHEBYSHEV_NEW creates a vector of Chebyshev spaced values.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 June 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double A_FIRST, A_LAST, the first and last entries.
//
//    Output, double R8VEC_CHEBYSHEV_NEW[N], a vector of Chebyshev spaced data.
//
{
  double *a;
  double c;
  int i;
  double pi = 3.141592653589793;
  double theta;

  a = new double[n];

  if ( n == 1 )
  {
    a[0] = ( a_first + a_last ) / 2.0;
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      theta = ( double ) ( n - i - 1 ) * pi / ( double ) ( n - 1 );

      c = std::cos ( theta );

      if ( ( n % 2 ) == 1 )
      {
        if ( 2 * i + 1 == n )
        {
          c = 0.0;
        }
      }

      a[i] = ( ( 1.0 - c ) * a_first  
             + ( 1.0 + c ) * a_last ) 
             /   2.0;
    }
  }
  return a;
}
//**************************************************************************80

int SandiaRules::r8vec_compare ( int n, double a[], double b[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_COMPARE compares two R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The lexicographic ordering is used.
//
//  Example:
//
//    Input:
//
//      A1 = ( 2.0, 6.0, 2.0 )
//      A2 = ( 2.0, 8.0, 12.0 )
//
//    Output:
//
//      ISGN = -1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A[N], B[N], the vectors to be compared.
//
//    Output, int R8VEC_COMPARE, the results of the comparison:
//    -1, A is lexicographically less than B,
//     0, A is equal to B,
//    +1, A is lexicographically greater than B.
//
{
  int isgn;
  int k;

  isgn = 0;

  for ( k = 0; k < n; k++ )
  {
    if ( a[k] < b[k] )
    {
      isgn = -1;
      return isgn;
    }
    else if ( b[k] < a[k] )
    {
      isgn = +1;
      return isgn;
    }
  }
  return isgn;
}
//**************************************************************************80

void SandiaRules::r8vec_copy ( int n, double a1[], double a2[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_COPY copies an R8VEC.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A1[N], the vector to be copied.
//
//    Output, double A2[N], the copy of A1.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return;
}
//**************************************************************************80

double *SandiaRules::r8vec_copy_new ( int n, double a1[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_COPY_NEW copies an R8VEC to a "new" R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A1[N], the vector to be copied.
//
//    Output, double R8VEC_COPY_NEW[N], the copy of A1.
//
{
  double *a2;
  int i;

  a2 = new double[n];

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return a2;
}
//**************************************************************************80

double SandiaRules::r8vec_diff_norm_li ( int n, double a[], double b[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_DIFF_NORM_LI returns the L-oo norm of the difference of R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The vector L-oo norm is defined as:
//
//      R8VEC_NORM_LI = max ( 1 <= I <= N ) abs ( A(I) ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 April 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], B[N], the vectors.
//
//    Output, double R8VEC_DIFF_NORM_LI, the L-oo norm of A - B.
//
{
  int i;
  double value;

  value = 0.0;

  for ( i = 0; i < n; i++ )
  {
    value = r8_max ( value, r8_abs ( a[i] - b[i] ) );
  }
  return value;
}
//**************************************************************************80

void SandiaRules::r8vec_direct_product2 ( int factor_index, int factor_order,
  double factor_value[], int factor_num, int point_num, double w[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_DIRECT_PRODUCT2 creates a direct product of R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    To explain what is going on here, suppose we had to construct
//    a multidimensional quadrature rule as the product of K rules
//    for 1D quadrature.
//
//    The product rule will be represented as a list of points and weights.
//
//    The J-th item in the product rule will be associated with
//      item J1 of 1D rule 1,
//      item J2 of 1D rule 2,
//      ...,
//      item JK of 1D rule K.
//
//    In particular,
//      X(J) = ( X(1,J1), X(2,J2), ..., X(K,JK))
//    and
//      W(J) = W(1,J1) * W(2,J2) * ... * W(K,JK)
//
//    So we can construct the quadrature rule if we can properly
//    distribute the information in the 1D quadrature rules.
//
//    This routine carries out that task for the weights W.
//
//    Another way to do this would be to compute, one by one, the
//    set of all possible indices (J1,J2,...,JK), and then index
//    the appropriate information.  An advantage of the method shown
//    here is that you can process the K-th set of information and
//    then discard it.
//
//  Example:
//
//    Rule 1:
//      Order = 4
//      W(1:4) = ( 2, 3, 5, 7 )
//
//    Rule 2:
//      Order = 3
//      W(1:3) = ( 11, 13, 17 )
//
//    Rule 3:
//      Order = 2
//      W(1:2) = ( 19, 23 )
//
//    Product Rule:
//      Order = 24
//      W(1:24) =
//        ( 2 * 11 * 19 )
//        ( 3 * 11 * 19 )
//        ( 4 * 11 * 19 )
//        ( 7 * 11 * 19 )
//        ( 2 * 13 * 19 )
//        ( 3 * 13 * 19 )
//        ( 5 * 13 * 19 )
//        ( 7 * 13 * 19 )
//        ( 2 * 17 * 19 )
//        ( 3 * 17 * 19 )
//        ( 5 * 17 * 19 )
//        ( 7 * 17 * 19 )
//        ( 2 * 11 * 23 )
//        ( 3 * 11 * 23 )
//        ( 5 * 11 * 23 )
//        ( 7 * 11 * 23 )
//        ( 2 * 13 * 23 )
//        ( 3 * 13 * 23 )
//        ( 5 * 13 * 23 )
//        ( 7 * 13 * 23 )
//        ( 2 * 17 * 23 )
//        ( 3 * 17 * 23 )
//        ( 5 * 17 * 23 )
//        ( 7 * 17 * 23 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 April 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int FACTOR_INDEX, the index of the factor being processed.
//    The first factor processed must be factor 0.
//
//    Input, int FACTOR_ORDER, the order of the factor.
//
//    Input, double FACTOR_VALUE[FACTOR_ORDER], the factor values for
//    factor FACTOR_INDEX.
//
//    Input, int FACTOR_NUM, the number of factors.
//
//    Input, int POINT_NUM, the number of elements in the direct product.
//
//    Input/output, double W[POINT_NUM], the elements of the
//    direct product, which are built up gradually.
//
//  Local Parameters:
//
//    Local, integer START, the first location of a block of values to set.
//
//    Local, integer CONTIG, the number of consecutive values to set.
//
//    Local, integer SKIP, the distance from the current value of START
//    to the next location of a block of values to set.
//
//    Local, integer REP, the number of blocks of values to set.
//
{
  static int contig = 0;
  int i;
  int j;
  int k;
  static int rep = 0;
  static int skip = 0;
  int start;

  if ( factor_index == 0 )
  {
    contig = 1;
    skip = 1;
    rep = point_num;
    for ( i = 0; i < point_num; i++ )
    {
      w[i] = 1.0;
    }
  }

  rep = rep / factor_order;
  skip = skip * factor_order;

  for ( j = 0; j < factor_order; j++ )
  {
    start = 0 + j * contig;

    for ( k = 1; k <= rep; k++ )
    {
      for ( i = start; i < start + contig; i++ )
      {
        w[i] = w[i] * factor_value[j];
      }
      start = start + skip;
    }
  }

  contig = contig * factor_order;

  return;
}
//**************************************************************************80

double SandiaRules::r8vec_dot_product ( int n, double a1[], double a2[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_DOT_PRODUCT computes the dot product of a pair of R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A1[N], A2[N], the two vectors to be considered.
//
//    Output, double R8VEC_DOT_PRODUCT, the dot product of the vectors.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < n; i++ )
  {
    value = value + a1[i] * a2[i];
  }
  return value;
}
//**************************************************************************80

double SandiaRules::r8vec_i4vec_dot_product ( int n, double r8vec[], int i4vec[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_I4VEC_DOT_PRODUCT computes the dot product of an R8VEC and an I4VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    30 June 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double R8VEC[N], the first vector.
//
//    Input, int I4VEC[N], the second vector.
//
//    Output, double R8VEC_I4VEC_DOT_PRODUCT, the dot product of the vectors.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < n; i++ )
  {
    value = value + r8vec[i] * ( double ) ( i4vec[i] );
  }
  return value;
}
//**************************************************************************80

void SandiaRules::r8vec_index_sorted_range ( int n, double r[], int indx[], double r_lo,
  double r_hi, int *i_lo, int *i_hi )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_SORTED_RANGE: search index sorted vector for elements in a range.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    27 September 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of items in the vector.
//
//    Input, double R[N], the index sorted vector.
//
//    Input, int INDX[N], the vector used to sort R.
//    The vector R[INDX[*]] is sorted.
//
//    Input, double R_LO, R_HI, the limits of the range.
//
//    Output, int *I_LO, *I_HI, the range of indices
//    so that I_LO <= I <= I_HI => R_LO <= R[INDX[I]] <= R_HI.  If no
//    values in R lie in the range, then I_HI < I_LO will be returned.
//
{
  int i1;
  int i2;
  int j1;
  int j2;
//
//  Cases we can handle immediately.
//
  if ( r[indx[n-1]] < r_lo )
  {
    *i_lo = n;
    *i_hi = n - 1;
    return;
  }

  if ( r_hi < r[indx[0]] )
  {
    *i_lo = 0;
    *i_hi = -1;
    return;
  }
//
//  Are there are least two intervals?
//
  if ( n == 1 )
  {
    if ( r_lo <= r[indx[0]] && r[indx[0]] <= r_hi )
    {
      *i_lo = 0;
      *i_hi = 0;
    }
    else
    {
      *i_lo = -1;
      *i_hi = -2;
    }
    return;
  }
//
//  Bracket R_LO.
//
  if ( r_lo <= r[indx[0]] )
  {
    *i_lo = 0;
  }
  else
  {
//
//  R_LO is in one of the intervals spanned by R(INDX(J1)) to R(INDX(J2)).
//  Examine the intermediate interval [R(INDX(I1)), R(INDX(I1+1))].
//  Does R_LO lie here, or below or above?
//
    j1 = 0;
    j2 = n - 1;
    i1 = ( j1 + j2 - 1 ) / 2;
    i2 = i1 + 1;

    for ( ; ; )
    {
      if ( r_lo < r[indx[i1]] )
      {
        j2 = i1;
        i1 = ( j1 + j2 - 1 ) / 2;
        i2 = i1 + 1;
      }
      else if ( r[indx[i2]] < r_lo )
      {
        j1 = i2;
        i1 = ( j1 + j2 - 1 ) / 2;
        i2 = i1 + 1;
      }
      else
      {
        *i_lo = i1;
        break;
      }
    }
  }
//
//  Bracket R_HI.
//
  if ( r[indx[n-1]] <= r_hi )
  {
    *i_hi = n - 1;
  }
  else
  {
    j1 = *i_lo;
    j2 = n - 1;
    i1 = ( j1 + j2 - 1 ) / 2;
    i2 = i1 + 1;

    for ( ; ; )
    {
      if ( r_hi < r[indx[i1]] )
      {
        j2 = i1;
        i1 = ( j1 + j2 - 1 ) / 2;
        i2 = i1 + 1;
      }
      else if ( r[indx[i2]] < r_hi )
      {
        j1 = i2;
        i1 = ( j1 + j2 - 1 ) / 2;
        i2 = i1 + 1;
      }
      else
      {
        *i_hi = i2;
        break;
      }
    }
  }
//
//  We expect to have computed the largest I_LO and smallest I_HI such that
//    R(INDX(I_LO)) <= R_LO <= R_HI <= R(INDX(I_HI))
//  but what we want is actually
//    R_LO <= R(INDX(I_LO)) <= R(INDX(I_HI)) <= R_HI
//  which we can usually get simply by incrementing I_LO and decrementing I_HI.
//
  if ( r[indx[*i_lo]] < r_lo )
  {
    *i_lo = *i_lo + 1;
    if ( n - 1 < *i_lo )
    {
      *i_hi = *i_lo - 1;
    }
  }

  if ( r_hi < r[indx[*i_hi]] )
  {
    *i_hi = *i_hi - 1;
    if ( i_hi < 0 )
    {
      *i_lo = *i_hi + 1;
    }
  }

  return;
}
//**************************************************************************80

void SandiaRules::r8vec_indexed_heap_d ( int n, double a[], int indx[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEXED_HEAP_D creates a descending heap from an indexed R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    An indexed R8VEC is an R8VEC of data values, and an R8VEC of N indices,
//    each referencing an entry of the data vector.
//
//    The function adjusts the index vector INDX so that, for 1 <= J <= N/2,
//    we have:
//      A[INDX[2*J+1]]   <= A[INDX[J]]
//    and
//      A[INDX[2*J+2]] <= A[INDX[J]]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 August 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms for Computers and Calculators,
//    Academic Press, 1978,
//    ISBN: 0-12-519260-6,
//    LC: QA164.N54.
//
//  Parameters:
//
//    Input, int N, the size of the index array.
//
//    Input, double A[*], the data vector.
//
//    Input/output, int INDX[N], the index array.
//    Each entry of INDX must be a valid index for the array A.
//    On output, the indices have been reordered into a descending heap.
//
{
  int i;
  int ifree;
  int key;
  int m;
//
//  Only nodes N/2 - 1 down to 0 can be "parent" nodes.
//
  for ( i = ( n / 2 ) - 1; 0 <= i; i-- )
  {
//
//  Copy the value out of the parent node.
//  Position IFREE is now "open".
//
    key = indx[i];
    ifree = i;

    for ( ; ; )
    {
//
//  Positions 2*IFREE+1 and 2*IFREE+2 are the descendants of position
//  IFREE.  (One or both may not exist because they exceed N-1.)
//
      m = 2 * ifree + 1;
//
//  Does the first position exist?
//
      if ( n - 1 < m )
      {
        break;
      }
//
//  Does the second position exist?
//
      if ( m + 1 <= n - 1 )
      {
//
//  If both positions exist, take the larger of the two values,
//  and update M if necessary.
//
        if ( a[indx[m]] < a[indx[m+1]] )
        {
          m = m + 1;
        }
      }
//
//  If the large descendant is larger than KEY, move it up,
//  and update IFREE, the location of the free position, and
//  consider the descendants of THIS position.
//
      if ( a[indx[m]] <= a[key] )
      {
        break;
      }

      indx[ifree] = indx[m];
      ifree = m;
    }
//
//  Once there is no more shifting to do, KEY moves into the free spot IFREE.
//
    indx[ifree] = key;
  }

  return;
}
//**************************************************************************80

int SandiaRules::r8vec_indexed_heap_d_extract ( int *n, double a[], int indx[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEXED_HEAP_D_EXTRACT: extract from heap descending indexed R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    An indexed R8VEC is an R8VEC of data values, and an R8VEC of N indices,
//    each referencing an entry of the data vector.
//
//    The routine finds the maximum value in the heap, returns that value to the
//    user, deletes that value from the heap, and restores the heap to its
//    proper form.
//
//    Note that the argument N must be a variable, which will be decremented
//    before return, and that INDX will hold one less value on output than it
//    held on input.
//
//    This is one of three functions needed to model a priority queue.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 August 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Thomas Cormen, Charles Leiserson, Ronald Rivest,
//    Introduction to Algorithms,
//    MIT Press, 2001,
//    ISBN: 0262032937,
//    LC: QA76.C662.
//
//  Parameters:
//
//    Input/output, int *N, the number of items in the index vector.
//
//    Input, double A[*], the data vector.
//
//    Input/output, int INDX[N], the index vector.
//
//    Output, int R8VEC_INDEXED_HEAP_D_EXTRACT, the index in A of the item of
//    maximum value, which has now been removed from the heap.
//
{
  int indx_extract;

  if ( *n < 1 )
  {
    std::cerr << "\n";
    std::cerr << "R8VEC_INDEXED_HEAP_D_EXTRACT - Fatal error!\n";
    std::cerr << "  The heap is empty.\n";
    std::exit ( 1 );
  }
//
//  Get the index of the maximum value.
//
  indx_extract = indx[0];

  if ( *n == 1 )
  {
    *n = 0;
    return indx_extract;
  }
//
//  Shift the last index down.
//
  indx[0] = indx[*n-1];
//
//  Restore the heap structure.
//
  *n = *n - 1;
  r8vec_indexed_heap_d ( *n, a, indx );

  return indx_extract;
}
//**************************************************************************80

void SandiaRules::r8vec_indexed_heap_d_insert ( int *n, double a[], int indx[],
  int indx_insert )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEXED_HEAP_D_INSERT: insert value into heap descending indexed R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    An indexed R8VEC is an R8VEC of data values, and an R8VEC of N indices,
//    each referencing an entry of the data vector.
//
//    Note that the argument N must be a variable, and will be incremented before
//    return, and that INDX must be able to hold one more entry on output than
//    it held on input.
//
//    This is one of three functions needed to model a priority queue.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 August 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Thomas Cormen, Charles Leiserson, Ronald Rivest,
//    Introduction to Algorithms,
//    MIT Press, 2001,
//    ISBN: 0262032937,
//    LC: QA76.C662.
//
//  Parameters:
//
//    Input/output, int *N, the number of items in the index vector.
//
//    Input, double A[*], the data vector.
//
//    Input/output, int INDX[N], the index vector.
//
//    Input, int INDX_INSERT, the index in A of the value
//    to be inserted into the heap.
//
{
  int i;
  int parent;

  *n = *n + 1;
  i = *n - 1;

  while ( 0 < i )
  {
    parent = ( i - 1 ) / 2;

    if ( a[indx_insert] <= a[indx[parent]] )
    {
      break;
    }

    indx[i] = indx[parent];
    i = parent;
  }

  indx[i] = indx_insert;

  return;
}
//**************************************************************************80

int SandiaRules::r8vec_indexed_heap_d_max ( int n, double a[], int indx[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEXED_HEAP_D_MAX: maximum value in heap descending indexed R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    An indexed R8VEC is an R8VEC of data values, and an R8VEC of N indices,
//    each referencing an entry of the data vector.
//
//    This is one of three functions needed to model a priority queue.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 August 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Thomas Cormen, Charles Leiserson, Ronald Rivest,
//    Introduction to Algorithms,
//    MIT Press, 2001,
//    ISBN: 0262032937,
//    LC: QA76.C662.
//
//  Parameters:
//
//    Input, int N, the number of items in the index vector.
//
//    Input, double A[*], the data vector.
//
//    Input, int INDX[N], the index vector.
//
//    Output, int R8VEC_INDEXED_HEAP_D_MAX, the index in A of the maximum value
//    in the heap.
//
{
  int indx_max;

  indx_max = indx[0];

  return indx_max;
}
//**************************************************************************80

double *SandiaRules::r8vec_legendre_new ( int n, double a_first, double a_last )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_LEGENDRE_NEW creates a vector of Chebyshev spaced values.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 June 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double A_FIRST, A_LAST, the first and last entries.
//
//    Output, double R8VEC_LEGENDRE_NEW[N], a vector of Legendre spaced data.
//
{
  double *a;
  int i;

  a = legendre_zeros ( n );

  for ( i = 0; i < n; i++ )
  {
    a[i] = ( ( 1.0 - a[i] ) * a_first  
           + ( 1.0 + a[i] ) * a_last ) 
           /   2.0;
  }
  return a;
}
//**************************************************************************80

double *SandiaRules::r8vec_linspace_new ( int n, double a_first, double a_last )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_LINSPACE_NEW creates a vector of linearly spaced values.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double A_FIRST, A_LAST, the first and last entries.
//
//    Output, double R8VEC_LINSPACE_NEW[N], a vector of linearly spaced data.
//
{
  double *a;
  int i;

  a = new double[n];

  if ( n == 1 )
  {
    a[0] = ( a_first + a_last ) / 2.0;
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      a[i] = ( ( double ) ( n - 1 - i ) * a_first 
             + ( double ) (         i ) * a_last ) 
             / ( double ) ( n - 1     );
    }
  }
  return a;
}
//**************************************************************************80

double SandiaRules::r8vec_min ( int n, double r8vec[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_MIN returns the value of the minimum element in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double R8VEC[N], the array to be checked.
//
//    Output, double R8VEC_MIN, the value of the minimum element.
//
{
  int i;
  double value;

  value = r8vec[0];

  for ( i = 1; i < n; i++ )
  {
    if ( r8vec[i] < value )
    {
      value = r8vec[i];
    }
  }
  return value;
}
//**************************************************************************80

double SandiaRules::r8vec_min_pos ( int n, double a[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_MIN_POS returns the minimum positive value of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 November 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries.
//
//    Input, double A[N], the array.
//
//    Output, double R8VEC_MIN_POS, the smallest positive entry,
//    or R8_HUGE if no entry is positive.
//
{
  int i;
  double r8_huge = 1.0E+30;
  double value;

  value = r8_huge;

  for ( i = 0; i < n; i++ )
  {
    if ( 0.0 < a[i] )
    {
      if ( a[i] < value )
      {
        value = a[i];
      }
    }
  }
  return value;
}
//**************************************************************************80

void SandiaRules::r8vec_print ( int n, double a[], std::string title )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_PRINT prints an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    16 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, double A[N], the vector to be printed.
//
//    Input, string TITLE, a title.
//
{
  int i;

  std::cout << "\n";
  std::cout << title << "\n";
  std::cout << "\n";
  for ( i = 0; i < n; i++ )
  {
    std::cout << "  " << std::setw(8)  << i
              << ": " << std::setw(14) << a[i]  << "\n";
  }

  return;
}
//**************************************************************************80

void SandiaRules::r8vec_scale ( double s, int n, double a[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_SCALE multiples an R8VEC by a scale factor.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    22 September 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double S, the scale factor.
//
//    Input, int N, the number of entries in the vectors.
//
//    Input/output, double A[N], the vector to be scaled.
//    On output, A[] = S * A[].
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i] = s * a[i];
  }
  return;
}
//**************************************************************************80

void SandiaRules::r8vec_sort_heap_index_a ( int n, double a[], int indx[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_HEAP_INDEX_A does an indexed heap ascending sort of an R8VEC
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      a(indx(*))
//
//    or explicitly, by the call
//
//      r8vec_permute ( n, indx, 0, a )
//
//    after which a(*) is sorted.
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 October 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], an array to be index-sorted.
//
//    Output, int INDX[N], contains the sort index.  The
//    I-th element of the sorted array is A(INDX(I)).
//
{
  double aval;
  int i;
  int indxt;
  int ir;
  int j;
  int l;

  if ( n < 1 )
  {
    return;
  }

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return;
  }

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      aval = a[indxt];
    }
    else
    {
      indxt = indx[ir-1];
      aval = a[indxt];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if ( a[indx[j-1]] < a[indx[j]] )
        {
          j = j + 1;
        }
      }

      if ( aval < a[indx[j-1]] )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }

  return;
}
//**************************************************************************80

int *SandiaRules::r8vec_sort_heap_index_a_new ( int n, double a[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_HEAP_INDEX_A_NEW does an indexed heap ascending sort of an R8VEC
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      a(indx(*))
//
//    or explicitly, by the call
//
//      r8vec_permute ( n, indx, 0, a )
//
//    after which a(*) is sorted.
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 October 2010
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], an array to be index-sorted.
//
//    Output, int R8VEC_SORT_HEAP_INDEX_A_NEW[N], contains the sort index.  The
//    I-th element of the sorted array is A(INDX(I)).
//
{
  double aval;
  int i;
  int *indx;
  int indxt;
  int ir;
  int j;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      aval = a[indxt];
    }
    else
    {
      indxt = indx[ir-1];
      aval = a[indxt];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if ( a[indx[j-1]] < a[indx[j]] )
        {
          j = j + 1;
        }
      }

      if ( aval < a[indx[j-1]] )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }

  return indx;
}
//**************************************************************************80

void SandiaRules::r8vec_stutter ( int n, double a[], int m, double am[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_STUTTER makes a "stuttering" copy of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Applying a stuttering factor M of 3, the vector A = ( 1, 5, 8 ) becomes
//    AM = ( 1, 1, 1, 5, 5, 5, 8, 8, 8 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 March 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the input vector.
//
//    Input, double A[N], the vector.
//
//    Input, int M, the "stuttering factor".
//
//    Output, double AM[M*N], the stuttering vector.
//
{
  int i;
  int j;
  int k;

  k = 0;
  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < m; j++ )
    {
      am[k] = a[i];
      k = k + 1;
    }
  }
  return;
}
//**************************************************************************80

double SandiaRules::r8vec_sum ( int n, double a[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_SUM returns the sum of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a double precision vector.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    15 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double A[N], the vector.
//
//    Output, double R8VEC_SUM, the sum of the vector.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < n; i++ )
  {
    value = value + a[i];
  }

  return value;
}
//**************************************************************************80

void SandiaRules::r8vec_uniform_01 ( int n, int *seed, double r[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNIFORM_01 returns a unit pseudorandom R8VEC.
//
//  Discussion:
//
//    This routine implements the recursion
//
//      seed = ( 16807 * seed ) mod ( 2^31 - 1 )
//      u = seed / ( 2^31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Second Edition,
//    Springer, 1987,
//    ISBN: 0387964673,
//    LC: QA76.9.C65.B73.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, December 1986, pages 362-376.
//
//    Pierre L'Ecuyer,
//    Random Number Generation,
//    in Handbook of Simulation,
//    edited by Jerry Banks,
//    Wiley, 1998,
//    ISBN: 0471134031,
//    LC: T57.62.H37.
//
//    Peter Lewis, Allen Goodman, James Miller,
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, Number 2, 1969, pages 136-143.
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R[N], the vector of pseudorandom values.
//
{
  int i;
  int i4_huge = 2147483647;
  int k;

  if ( *seed == 0 )
  {
    std::cerr << "\n";
    std::cerr << "R8VEC_UNIFORM_01 - Fatal error!\n";
    std::cerr << "  Input value of SEED = 0.\n";
    std::exit ( 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    k = *seed / 127773;

    *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

    if ( *seed < 0 )
    {
      *seed = *seed + i4_huge;
    }

    r[i] = ( double ) ( *seed ) * 4.656612875E-10;
  }

  return;
}
//**************************************************************************80

double *SandiaRules::r8vec_uniform_01_new ( int n, int *seed )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNIFORM_01_NEW returns a new unit pseudorandom R8VEC.
//
//  Discussion:
//
//    This routine implements the recursion
//
//      seed = ( 16807 * seed ) mod ( 2^31 - 1 )
//      u = seed / ( 2^31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Second Edition,
//    Springer, 1987,
//    ISBN: 0387964673,
//    LC: QA76.9.C65.B73.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, December 1986, pages 362-376.
//
//    Pierre L'Ecuyer,
//    Random Number Generation,
//    in Handbook of Simulation,
//    edited by Jerry Banks,
//    Wiley, 1998,
//    ISBN: 0471134031,
//    LC: T57.62.H37.
//
//    Peter Lewis, Allen Goodman, James Miller,
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, Number 2, 1969, pages 136-143.
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R8VEC_UNIFORM_01_NEW[N], the vector of pseudorandom values.
//
{
  int i;
  int i4_huge = 2147483647;
  int k;
  double *r;

  if ( *seed == 0 )
  {
    std::cerr << "\n";
    std::cerr << "R8VEC_UNIFORM_01_NEW - Fatal error!\n";
    std::cerr << "  Input value of SEED = 0.\n";
    std::exit ( 1 );
  }

  r = new double[n];

  for ( i = 0; i < n; i++ )
  {
    k = *seed / 127773;

    *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

    if ( *seed < 0 )
    {
      *seed = *seed + i4_huge;
    }

    r[i] = ( double ) ( *seed ) * 4.656612875E-10;
  }

  return r;
}
//**************************************************************************80

void SandiaRules::r8vec_zero ( int n, double a[] )

//**************************************************************************80
//
//  Purpose:
//
//    R8VEC_ZERO zeroes an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Output, double A[N], a vector of zeroes.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i] = 0.0;
  }
  return;
}
//**************************************************************************80

void SandiaRules::sort_heap_external ( int n, int *indx, int *i, int *j, int isgn )

//**************************************************************************80
//
//  Purpose:
//
//    SORT_HEAP_EXTERNAL externally sorts a list of items into ascending order.
//
//  Discussion:
//
//    The actual list is not passed to the routine.  Hence it may
//    consist of integers, reals, numbers, names, etc.  The user,
//    after each return from the routine, will be asked to compare or
//    interchange two items.
//
//    The current version of this code mimics the FORTRAN version,
//    so the values of I and J, in particular, are FORTRAN indices.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    05 February 2004
//
//  Author:
//
//    Original FORTRAN77 version by Albert Nijenhuis, Herbert Wilf.
//    C++ version by John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the length of the input list.
//
//    Input/output, int *INDX.
//    The user must set INDX to 0 before the first call.
//    On return,
//      if INDX is greater than 0, the user must interchange
//      items I and J and recall the routine.
//      If INDX is less than 0, the user is to compare items I
//      and J and return in ISGN a negative value if I is to
//      precede J, and a positive value otherwise.
//      If INDX is 0, the sorting is done.
//
//    Output, int *I, *J.  On return with INDX positive,
//    elements I and J of the user's list should be
//    interchanged.  On return with INDX negative, elements I
//    and J are to be compared by the user.
//
//    Input, int ISGN. On return with INDX negative, the
//    user should compare elements I and J of the list.  If
//    item I is to precede item J, set ISGN negative,
//    otherwise set ISGN positive.
//
{
  static int i_save = 0;
  static int j_save = 0;
  static int k = 0;
  static int k1 = 0;
  static int n1 = 0;
//
//  INDX = 0: This is the first call.
//
  if ( *indx == 0 )
  {

    i_save = 0;
    j_save = 0;
    k = n / 2;
    k1 = k;
    n1 = n;
  }
//
//  INDX < 0: The user is returning the results of a comparison.
//
  else if ( *indx < 0 )
  {
    if ( *indx == -2 )
    {
      if ( isgn < 0 )
      {
        i_save = i_save + 1;
      }
      j_save = k1;
      k1 = i_save;
      *indx = -1;
      *i = i_save;
      *j = j_save;
      return;
    }

    if ( 0 < isgn )
    {
      *indx = 2;
      *i = i_save;
      *j = j_save;
      return;
    }

    if ( k <= 1 )
    {
      if ( n1 == 1 )
      {
        i_save = 0;
        j_save = 0;
        *indx = 0;
      }
      else
      {
        i_save = n1;
        j_save = 1;
        n1 = n1 - 1;
        *indx = 1;
      }
      *i = i_save;
      *j = j_save;
      return;
    }
    k = k - 1;
    k1 = k;
  }
//
//  0 < INDX: the user was asked to make an interchange.
//
  else if ( *indx == 1 )
  {
    k1 = k;
  }

  for ( ; ; )
  {

    i_save = 2 * k1;

    if ( i_save == n1 )
    {
      j_save = k1;
      k1 = i_save;
      *indx = -1;
      *i = i_save;
      *j = j_save;
      return;
    }
    else if ( i_save <= n1 )
    {
      j_save = i_save + 1;
      *indx = -2;
      *i = i_save;
      *j = j_save;
      return;
    }

    if ( k <= 1 )
    {
      break;
    }

    k = k - 1;
    k1 = k;
  }

  if ( n1 == 1 )
  {
    i_save = 0;
    j_save = 0;
    *indx = 0;
    *i = i_save;
    *j = j_save;
  }
  else
  {
    i_save = n1;
    j_save = 1;
    n1 = n1 - 1;
    *indx = 1;
    *i = i_save;
    *j = j_save;
  }

  return;
}
//**************************************************************************80

void SandiaRules::timestamp ( )

//**************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 July 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}


//**************************************************************************80
void SandiaRules::vec_colex_next3 ( int dim_num, int base[], int a[], bool *more )
//**************************************************************************80
//
//  Purpose:
//
//    VEC_COLEX_NEXT3 generates vectors in colex order.
//
//  Discussion:
//
//    The vectors are produced in colexical order, starting with
//
//    (1,        1,        ...,1),
//    (2,        1,        ...,1),
//     ...
//    (BASE(1),  1,        ...,1)
//
//    (1,        2,        ...,1)
//    (2,        2,        ...,1)
//    ...
//    (BASE(1),  2,        ...,1)
//
//    (1,        3,        ...,1)
//    (2,        3,        ...,1)
//    ...
//    (BASE(1),  BASE(2), ...,BASE(DIM_NUM)).
//
//  Example:
//
//    DIM_NUM = 2,
//    BASE = { 3, 3 }
//
//    1   1
//    2   1
//    3   1
//    1   2
//    2   2
//    3   2
//    1   3
//    2   3
//    3   3
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, int BASE[DIM_NUM], the bases to be used in each dimension.
//    In dimension I, entries will range from 1 to BASE[I].
//
//    Output, int A[DIM_NUM], the next vector.
//
//    Input/output, bool *MORE.  Set this variable false before
//    the first call.  On return, MORE is TRUE if another vector has
//    been computed.  If MORE is returned FALSE, ignore the output
//    vector and stop calling the routine.
//
{
  int i;

  if ( !( *more ) )
  {
    for ( i = 0; i < dim_num; i++ )
    {
      a[i] = 1;
    }
    *more = true;
  }
  else
  {
    for ( i = 0; i < dim_num; i++ )
    {
      a[i] = a[i] + 1;

      if ( a[i] <= base[i] )
      {
        return;
      }
      a[i] = 1;
    }
    *more = false;
  }

  return;
}

} // namespace ROL

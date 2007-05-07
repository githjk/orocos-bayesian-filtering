// $Id$
// Copyright (C) 2002 Klaas Gadeyne <first dot last at gmail dot com>
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//  
#include "gaussian.h"

#include "../wrappers/rng/rng.h" // Wrapper around several rng libraries

#include <cmath> // For sqrt and exp
#include <cassert>

namespace BFL
{
  using namespace MatrixWrapper;

  Gaussian::Gaussian (const ColumnVector& m, const SymmetricMatrix& s) 
    : Pdf<ColumnVector> ( m.rows() )
  {
    // check if inputs are consistent
    assert (m.rows() == s.columns());
    _Mu = m;
    _Sigma = s;
    _Sigma_changed = true;
  }

  Gaussian::Gaussian (int dimension)
    : Pdf<ColumnVector>(dimension)
  {
    _Mu.resize(dimension);
    _Sigma.resize(dimension);
    _Sigma_inverse.resize(dimension);
  }

  Gaussian::~Gaussian(){}

  std::ostream& operator<< (std::ostream& os, const Gaussian& g)
  {
    os << "\nMu:\n"    << g.ExpectedValueGet()
       << "\nSigma:\n" << g.CovarianceGet() << endl;
    return os;
  }

  Probability Gaussian::ProbabilityGet(const ColumnVector& input) const
  {
    // only calculate these variables if sigma has changed
    if (_Sigma_changed){
      _Sigma_changed = false;
      _Sigma_inverse = _Sigma.inverse();
      _sqrt_pow = 1 / sqrt(pow(M_PI*2,(double)DimensionGet()) * _Sigma.determinant());
    }

    ColumnVector diff = input - _Mu;
    Probability temp = diff.transpose() * (_Sigma_inverse * diff);
    Probability result = exp(-0.5 * temp) * _sqrt_pow;
    return result;
  }

  // Redefined for optimal performance.  Eg. do Cholesky decomposition
  // only once when drawing multiple samples at once!
  // See method below for more info regarding the algorithms
  bool
  Gaussian::SampleFrom (vector<Sample<ColumnVector> >& list_samples, const int num_samples, int method, void * args) const
  {
    // Perform memory allocation
    list_samples.resize(num_samples);
    vector<Sample<ColumnVector> >::iterator rit = list_samples.begin();
    switch(method)
      {
      case DEFAULT: // Cholesky Sampling
      case CHOLESKY: 
	{
	  Matrix Low_triangle;
	  bool result = _Sigma.cholesky_semidefinite(Low_triangle);
	  ColumnVector samples(DimensionGet()); ColumnVector SampleValue(DimensionGet());
	  while (rit != list_samples.end())
	    {
	      for (unsigned int j=1; j < DimensionGet()+1; j++) samples(j) = rnorm(0,1);
	      SampleValue = (Low_triangle * samples) + this->_Mu;
	      rit->ValueSet(SampleValue);
	      rit++;
	    }
	  return result;
	}
      case BOXMULLER: // Implement box-muller here
	// Only for univariate distributions.
	return false;
      default:
	return false;
      }
  }


  bool
  Gaussian::SampleFrom (Sample<ColumnVector>& one_sample, int method, void * args) const
  {
    /*  Exact i.i.d. samples from a Gaussian can be drawn in several
	ways:
	- if the DimensionGet() = 1 or 2 (and the 2 variables are
	independant), we can use inversion sampling (Box-Muller
	method) 
        - For larger dimensions, we use can use the Cholesky method or
	an approached based on conditional distributions.
	(see ripley87, P.98 (bibtex below)).  The Cholesky method is
	generally preferred and the only one implemented for now.
    */
    switch(method)
      {
      case DEFAULT: // Cholesky Sampling, see eg.
      case CHOLESKY: // Cholesky Sampling, see eg.
	/*
	  @Book{		  ripley87,
	  author	= {Ripley, Brian D.},
	  title		= {Stochastic Simulation},
	  publisher	= {John Wiley and Sons},
	  year		= {1987},
	  annote	= {ISBN 0271-6356, WBIB 1 519.245}
	  }
	  p.98
	*/
	{
	  Matrix Low_triangle;
          bool result = _Sigma.cholesky_semidefinite(Low_triangle);

	  /* For now we keep it simple, and use the scythe library
	     (although wrapped) with the uRNG that it uses itself only */
	  ColumnVector samples(DimensionGet()); ColumnVector SampleValue(DimensionGet());
	  /* Sample Gaussian._dimension samples from univariate
	     gaussian This could be done using several available
	     libraries, combined with different uniform RNG.  Both the
	     library to be used and the uRNG could be implemented as
	     #ifdef conditions, although I'm sure there must exist a
	     cleaner way to implement this!
	  */
	  for (unsigned int j=1; j < DimensionGet()+1; j++) samples(j) = rnorm(0,1);
	  SampleValue = (Low_triangle * samples) + this->_Mu;
	  one_sample.ValueSet(SampleValue);
	  return result;
	}
      case BOXMULLER: // Implement box-muller here
	// Only for univariate distributions.
	return false;
      default:
	return false;
      }
  }


  ColumnVector
  Gaussian::ExpectedValueGet (  ) const 
  { 
    return _Mu;
  }

  SymmetricMatrix
  Gaussian::CovarianceGet () const
  {
    return _Sigma;
  }

  void 
  Gaussian::ExpectedValueSet (const ColumnVector& mu)
  { 
    _Mu = mu;
    assert(this->DimensionGet() == mu.rows());
    if (this->DimensionGet() == 0)
      {
	this->DimensionSet(mu.rows());
      }
  }

  void 
  Gaussian::CovarianceSet (const SymmetricMatrix& cov)
  {
    _Sigma = cov;
    _Sigma_changed = true;
    assert(this->DimensionGet() == cov.rows());

    if (this->DimensionGet() == 0)
      {
	this->DimensionSet(cov.rows());
      }
  }

} // End namespace BFL
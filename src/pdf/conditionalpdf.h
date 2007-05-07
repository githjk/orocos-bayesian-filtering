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
#ifndef __CONDITIONAL_PDF__
#define __CONDITIONAL_PDF__

#include "pdf.h"
#include <vector>
#include <cassert>
namespace BFL
{

  /// Abstract Class representing conditional Pdfs P(x | ...)
  /** This class inherits from Pdf Virtual public because of the multiple
      inheritance that follows 
      Two templates are here to allow a mixture of discrete and continu
      variables in the Pdf!
      @bug All conditional arguments should be of the same type T for now!
      @todo Investigate if we can allow.  It is for sure that we'll need
      another class then the std::list to implement this!
      @see Pdf
  */
  template <typename Var, typename CondArg> class ConditionalPdf : public Pdf<Var>
    {
    public:
      /// Constructor
      /** @param dimension int representing the number of rows of the state
	  vector for the continuous case, or the discrete number of states
	  for the discrete case
	  @param num_conditional_arguments the number of arguments behind
	  the |
      */
      ConditionalPdf(int dimension=0, unsigned int num_conditional_arguments=0);

      // Default copy constructor will do

      /// Destructor
      virtual ~ConditionalPdf();
  
      ///Get the Number of conditional arguments
      /**@return the number of conditional arguments
       */
      unsigned int NumConditionalArgumentsGet() const;

      /// Set the Number of conditional arguments
      /** @param numconditionalarguments the number of
	  conditionalarguments 
	  @bug will probably give rise to memory allocation problems
	  if you herit from this class and do not redefine this method.
      */
      virtual void NumConditionalArgumentsSet(unsigned int numconditionalarguments);

      /// Get the whole list of conditional arguments
      /** @return an STL-vector containing all the current values of the
	  conditional arguments
      */
      const std::vector<CondArg>& ConditionalArgumentsGet() const;

      /// Set the whole list of conditional arguments
      /** @param ConditionalArguments an STL-vector of type <pre>T</pre>
	  containing the condtional arguments
      */
      void ConditionalArgumentsSet(std::vector<CondArg> ConditionalArguments);

      /// Get the n-th argument of the list
      /** @return The current value of the n-th conditional argument
	  (starting from 0!)
      */
      const CondArg& ConditionalArgumentGet(unsigned int n_argument) const;

      /// Set the n-th argument of the list
      /** @param n_argument which one of the conditional arguments

	  @param argument value of the n-th argument
      */     
      void ConditionalArgumentSet(unsigned int n_argument, const CondArg& argument);

    private:
      /// # of conditional arguments (# of args after the | sign)
      unsigned int _NumConditionalArguments;
      /// vector containing the values of the conditional args
      std::vector<CondArg> _ConditionalArguments;
    };


  // constructor
  template<typename Var, typename CondArg> 
    ConditionalPdf<Var,CondArg>::ConditionalPdf(int dim, unsigned int num_args) 
    : Pdf<Var>(dim)
    , _NumConditionalArguments(num_args)
    , _ConditionalArguments(num_args)
    {}

  // destructor
  template<typename Var, typename CondArg> 
    ConditionalPdf<Var,CondArg>::~ConditionalPdf()
    {}


  template<typename Var, typename CondArg> inline unsigned int 
    ConditionalPdf<Var,CondArg>::NumConditionalArgumentsGet() const
    {
      return _NumConditionalArguments;
    }

  template<typename Var, typename CondArg> inline void 
    ConditionalPdf<Var,CondArg>::NumConditionalArgumentsSet(unsigned int numconditionalarguments) 
    {
      if (numconditionalarguments != _NumConditionalArguments)
	{
	  _NumConditionalArguments = numconditionalarguments;
	  this->_ConditionalArguments.resize(_NumConditionalArguments);
	}
    }


  template<typename Var, typename CondArg> const std::vector<CondArg>&
    ConditionalPdf<Var,CondArg>::ConditionalArgumentsGet() const
    {
      return _ConditionalArguments;
    }

  template<typename Var, typename CondArg> void 
    ConditionalPdf<Var,CondArg>::ConditionalArgumentsSet(std::vector<CondArg> condargs)
    {
      assert (condargs.size() == _NumConditionalArguments);
      this->_ConditionalArguments = condargs;
    }
  
  template<typename Var, typename CondArg> const CondArg&
    ConditionalPdf<Var,CondArg>::ConditionalArgumentGet(unsigned int n_argument) const
    {
      assert( n_argument < _NumConditionalArguments );
      // index of conditional arguments of ConditionalPdf out of range
      return _ConditionalArguments[n_argument];
    }
  
  template<typename Var, typename CondArg> void 
    ConditionalPdf<Var,CondArg>::ConditionalArgumentSet(unsigned int n_argument, 
							const CondArg& argument)
    {
      assert ( n_argument < _NumConditionalArguments );
      // index of conditional arguments of ConditionalPdf out of range
      this->_ConditionalArguments[n_argument]= argument;
    }

} // End namespace
#endif // __CONDITIONAL_PDF__
// Copyright (C) 2002-2006 Klaas Gadeyne <first dot last at gmail dot com>
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
#include "rng.h"

#include <../config.h>
#ifdef __RNGWRAPPER_BOOST__  // BOOST RANDOM LIBRARY
// THE BOOST RANDOM NUMBER GENERATION LIBRARY

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_real.hpp>

static boost::mt19937 Boost_Rng; // Source of randomness

static boost::uniform_real<double> Uniform_Distribution; // Uniform distribution
static boost::variate_generator<boost::mt19937&,boost::uniform_real<double> > roll(Boost_Rng,Uniform_Distribution);

double BFL::rnorm(const double& mu,const double& sigma)
{
  boost::normal_distribution<double> TestDist(mu,sigma);
  boost::variate_generator <boost::mt19937 &,boost::normal_distribution<double> > TestGen(Boost_Rng,TestDist);
  return TestGen();
}

double BFL::runif()
{
  return roll();
}

#endif // __RNGWRAPPER_BOOST__

#if 0
#ifdef __RNGWRAPPER_SCYTHE__  // SCYTHE RANDOM LIBRARY

#include <scythe/Scythe_Optimize.h>
#include <scythe/Scythe_Simulate.h>

// Sample from univariate normal distribution with mu and sigma
double BFL::rnorm(const double & mu, const double & sigma)
{
  return (SCYTHE::rnorm(mu,sigma));
}

// Sample from uniform distribution
double BFL::runif()
{
  return (SCYTHE::ranmars());
}
#endif // __RNGWRAPPER_SCYTHE__
#endif // 0
#ifdef __RNGWRAPPER_LTI__  // LTILIB

#include <ltilib/ltiUniformDist.h>
#include <ltilib/ltiGaussDist.h>

// Sample from univariate normal distribution with mu and sigma
double BFL::rnorm(const double & mu, const double & sigma)
{
  lti::gaussianDistribution g(mu,sigma);
  return g.draw();
}

// FIXME: Check quality of RNG!!
// Create uniform distribution between 0 and 1
static lti::uniformDistribution unif;

// Sample from uniform distribution
double BFL::runif()
{
  return unif.draw();
}

#endif // __RNGWRAPPER_LTI__

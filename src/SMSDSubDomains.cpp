/*
 * @ Copyright 2014-2017 CERN and Instituto de Fisica de Cantabria - Universidad de Cantabria. All rigths not expressly granted are reserved [tracs.ssd@cern.ch]
 * This file is part of TRACS.
 *
 * TRACS is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation,
 * either version 3 of the Licence.
 *
 * TRACS is distributed in the hope that it will be useful , but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with TRACS. If not, see <http://www.gnu.org/licenses/>
 */

/************************************SMSSubDomains***********************************
 *
 *
 *
 */
#include <SMSDSubDomains.h>

using namespace dolfin;

/**
 *
 * @param pitch
 * @param width
 * @param nns
 */
CentralStripBoundary::CentralStripBoundary(double pitch, double width, int nns )
{
  _pitch = pitch;
  _width = width;
  _nns = nns;
}

/**
 *
 * @param x
 * @param on_boundary
 * @return
 */
bool CentralStripBoundary::inside(const Array<double>& x, bool on_boundary) const
{
  bool is_inside = false;
  if ((x[1] < DOLFIN_EPS ) && on_boundary) // y = 0 condition
  {
    double x_translated = x[0] - _pitch*_nns; // reference system change
    double l_lim = (_pitch - _width) / 2.0;
    double r_lim = l_lim + _width;
    if ((x_translated > l_lim*(1-DOLFIN_EPS)) && (x_translated < r_lim*(1+DOLFIN_EPS))) // check if strip
    {
      is_inside = true;
    }
  }
  return is_inside;
}
/**
 *
 * @param pitch
 * @param width
 * @param nns
 */
NeighbourStripBoundary::NeighbourStripBoundary(double pitch, double width, int nns )
{
  _pitch = pitch;
  _width = width;
  _nns = nns;
}
/**
 *
 * @param x
 * @param on_boundary
 * @return
 */
bool NeighbourStripBoundary::inside(const Array<double>& x, bool on_boundary) const
{
  bool is_inside = false;
  if ((x[1] < DOLFIN_EPS ) && on_boundary) // y = 0 condition
  {
    int t_nns = 1 + 2*_nns; // total number of strips
    for ( int count = 0; count < t_nns; count++) // for loop for each strip
    {
      if (count != _nns) // not central strip
      {
        double x_translated = x[0] - _pitch*count; // reference system change
        double l_lim = (_pitch - _width) / 2.0;
        double r_lim = l_lim + _width;
        if ((x_translated > l_lim*(1-DOLFIN_EPS)) && (x_translated < r_lim*(1+DOLFIN_EPS))) // check if strip
        {
          is_inside = true;
        }
      }
    }
  }
  return is_inside;
}
/**
 *
 * @param x_min
 * @param x_max
 * @param depth
 */
BackPlaneBoundary::BackPlaneBoundary(double x_min, double x_max, double depth)
{
  _x_min = x_min;
  _x_max = x_max;
  _depth = depth;
}

bool BackPlaneBoundary::inside(const Array<double>& x, bool on_boundary) const
{
  bool is_inside = false;
  if ((x[1] > (_depth - DOLFIN_EPS*_depth) ) && on_boundary) // y = depth condition
  {
    if ((x[0] > _x_min - DOLFIN_EPS) && (x[0] < _x_max + DOLFIN_EPS )) // within boundaries
    {
      is_inside = true;
    }
  }
  return is_inside;
}
/**
 *
 * @param x_min
 * @param x_max
 * @param depth
 */
PeriodicLateralBoundary::PeriodicLateralBoundary(double x_min, double x_max, double depth)
{
  _x_min = x_min;
  _x_max = x_max;
  _depth = depth;
}
/**
 *
 * @param x
 * @param on_boundary
 * @return
 */
// Left boundary is "target domain"
bool PeriodicLateralBoundary::inside(const Array<double>& x, bool on_boundary) const
{
  return (std::abs(x[0]) < DOLFIN_EPS); // left lateral domain
}

// Map right boundary to left boundary
/**
 *
 * @param x
 * @param y
 */
void PeriodicLateralBoundary::map(const Array<double>& x, Array<double>& y) const
{
  y[0] = x[0] - _x_max; //translate x coordinate
  y[1] = x[1];  // leave y equal
}

/*******************************************************/

CentralStripBoundaryWP::CentralStripBoundaryWP(double pitch, double width, int nns, double depletion_width )
{
  _pitch = pitch;
  _width = width;
  _nns = nns;
  _depletion_width = depletion_width;
}

bool CentralStripBoundaryWP::inside(const Array<double>& x, bool on_boundary) const
{
 bool is_inside = false;

 if (x[1] < (_depletion_width - DOLFIN_EPS* _depletion_width) && on_boundary){
	 double x_translated = x[0] - _pitch*_nns; // reference system change
	     double l_lim = (_pitch - _width) / 2.0;
	     double r_lim = l_lim + _width;
	     if ((x_translated > l_lim*(1-DOLFIN_EPS)) && (x_translated < r_lim*(1+DOLFIN_EPS))) // check if strip
	     {
		 	 is_inside = true;
	     }
 }
	 return is_inside;
}

NeighbourStripBoundaryWP::NeighbourStripBoundaryWP(double pitch, double width, int nns, double depletion_width )
{
  _pitch = pitch;
  _width = width;
  _nns = nns;
  _depletion_width = depletion_width;
}

bool NeighbourStripBoundaryWP::inside(const Array<double>& x, bool on_boundary) const
{
  bool is_inside = false;
  if (x[1] < (_depletion_width - DOLFIN_EPS* _depletion_width) && on_boundary) // y = 0 condition
  {
    int t_nns = 1 + 2*_nns; // total number of strips
    for ( int count = 0; count < t_nns; count++) // for loop for each strip
    {
      if (count != _nns) // not central strip
      {
        double x_translated = x[0] - _pitch*count; // reference system change
        double l_lim = (_pitch - _width) / 2.0;
        double r_lim = l_lim + _width;
        if ((x_translated > l_lim*(1-DOLFIN_EPS)) && (x_translated < r_lim*(1+DOLFIN_EPS))) // check if strip
        {
          is_inside = true;
        }
      }
    }
  }
  return is_inside;
}

BackPlaneBoundaryWP::BackPlaneBoundaryWP(double x_min, double x_max, double depletion)
{
  _x_min = x_min;
  _x_max = x_max;
  _depletion_width = depletion;
}


bool BackPlaneBoundaryWP::inside(const Array<double>& x, bool on_boundary) const
{
  bool is_inside = false;
  if (x[1] > ((_depletion_width) - DOLFIN_EPS* (_depletion_width)) )
  {
      is_inside = true;
 }
 return is_inside;
}


/*

bool BackPlaneBoundaryWP::inside(const Array<double>& x, bool on_boundary) const
{
  bool is_inside = false;
  if ((x[1] > (_depletion_width - DOLFIN_EPS*_depletion_width) ) && on_boundary) // y = depth condition
  {
    if ((x[0] > _x_min - DOLFIN_EPS) && (x[0] < _x_max + DOLFIN_EPS )) // within boundaries
    {
      is_inside = true;
    }
  }
  return is_inside;
}
*/










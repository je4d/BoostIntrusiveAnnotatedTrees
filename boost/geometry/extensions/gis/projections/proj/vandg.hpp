#ifndef BOOST_GEOMETRY_PROJECTIONS_VANDG_HPP
#define BOOST_GEOMETRY_PROJECTIONS_VANDG_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace vandg{ 
            static const double TOL = 1.e-10;
            static const double THIRD = .33333333333333333333;
            static const double TWO_THRD = .66666666666666666666;
            static const double C2_27 = .07407407407407407407;
            static const double PI4_3 = 4.18879020478639098458;
            static const double PISQ = 9.86960440108935861869;
            static const double TPISQ = 19.73920880217871723738;
            static const double HPISQ = 4.93480220054467930934;


            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_vandg_spheroid : public base_t_fi<base_vandg_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;


                inline base_vandg_spheroid(const Parameters& par)
                    : base_t_fi<base_vandg_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double  al, al2, g, g2, p2;
                
                    p2 = fabs(lp_lat / HALFPI);
                    if ((p2 - TOL) > 1.) throw proj_exception();;
                    if (p2 > 1.)
                        p2 = 1.;
                    if (fabs(lp_lat) <= TOL) {
                        xy_x = lp_lon;
                        xy_y = 0.;
                    } else if (fabs(lp_lon) <= TOL || fabs(p2 - 1.) < TOL) {
                        xy_x = 0.;
                        xy_y = PI * tan(.5 * asin(p2));
                        if (lp_lat < 0.) xy_y = -xy_y;
                    } else {
                        al = .5 * fabs(PI / lp_lon - lp_lon / PI);
                        al2 = al * al;
                        g = sqrt(1. - p2 * p2);
                        g = g / (p2 + g - 1.);
                        g2 = g * g;
                        p2 = g * (2. / p2 - 1.);
                        p2 = p2 * p2;
                        xy_x = g - p2; g = p2 + al2;
                        xy_x = PI * (al * xy_x + sqrt(al2 * xy_x * xy_x - g * (g2 - p2))) / g;
                        if (lp_lon < 0.) xy_x = -xy_x;
                        xy_y = fabs(xy_x / PI);
                        xy_y = 1. - xy_y * (xy_y + 2. * al);
                        if (xy_y < -TOL) throw proj_exception();;
                        if (xy_y < 0.)    xy_y = 0.;
                        else        xy_y = sqrt(xy_y) * (lp_lat < 0. ? -PI : PI);
                    }
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double t, c0, c1, c2, c3, al, r2, r, m, d, ay, x2, y2;
                
                    x2 = xy_x * xy_x;
                    if ((ay = fabs(xy_y)) < TOL) {
                        lp_lat = 0.;
                        t = x2 * x2 + TPISQ * (x2 + HPISQ);
                        lp_lon = fabs(xy_x) <= TOL ? 0. :
                           .5 * (x2 - PISQ + sqrt(t)) / xy_x;
                            return;
                    }
                    y2 = xy_y * xy_y;
                    r = x2 + y2;    r2 = r * r;
                    c1 = - PI * ay * (r + PISQ);
                    c3 = r2 + TWOPI * (ay * r + PI * (y2 + PI * (ay + HALFPI)));
                    c2 = c1 + PISQ * (r - 3. *  y2);
                    c0 = PI * ay;
                    c2 /= c3;
                    al = c1 / c3 - THIRD * c2 * c2;
                    m = 2. * sqrt(-THIRD * al);
                    d = C2_27 * c2 * c2 * c2 + (c0 * c0 - THIRD * c2 * c1) / c3;
                    if (((t = fabs(d = 3. * d / (al * m))) - TOL) <= 1.) {
                        d = t > 1. ? (d > 0. ? 0. : PI) : acos(d);
                        lp_lat = PI * (m * cos(d * THIRD + PI4_3) - THIRD * c2);
                        if (xy_y < 0.) lp_lat = -lp_lat;
                        t = r2 + TPISQ * (x2 - y2 + HPISQ);
                        lp_lon = fabs(xy_x) <= TOL ? 0. :
                           .5 * (r - PISQ + (t <= 0. ? 0. : sqrt(t))) / xy_x;
                    } else
                        throw proj_exception();;
                }
            };

            // van der Grinten (I)
            template <typename Parameters>
            void setup_vandg(Parameters& par)
            {
                par.es = 0.;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
            }

        }} // namespace detail::vandg
    #endif // doxygen 

    /*!
        \brief van der Grinten (I) projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Miscellaneous
         - Spheroid
        \par Example
        \image html ex_vandg.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct vandg_spheroid : public detail::vandg::base_vandg_spheroid<Geographic, Cartesian, Parameters>
    {
        inline vandg_spheroid(const Parameters& par) : detail::vandg::base_vandg_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::vandg::setup_vandg(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class vandg_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<vandg_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void vandg_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("vandg", new vandg_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_VANDG_HPP


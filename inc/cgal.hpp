#pragma once

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_naive_point_location.h>

typedef CGAL::Cartesian<CGAL::Exact_rational> Double_Kernel;
typedef Double_Kernel::Point_2 Double_Point_2;

typedef CGAL::Cartesian<CGAL::Exact_rational> Trait_Kernel;
typedef CGAL::Arr_circle_segment_traits_2<Trait_Kernel> Traits_2;
typedef Traits_2::Point_2 Trait_Point_2;
typedef Traits_2::CoordNT CoordNT;
typedef Traits_2::X_monotone_curve_2 Segment_2;
typedef Trait_Kernel::Circle_2 Circle_2;
typedef Traits_2::Curve_2 Curve_2;
typedef CGAL::Arrangement_2<Traits_2> Arrangement_2;
typedef CGAL::Arr_naive_point_location<Arrangement_2> Naive_pl;
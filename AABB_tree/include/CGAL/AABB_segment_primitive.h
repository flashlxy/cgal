// Copyright (c) 2009, 2011 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Pierre Alliez, Stephane Tayeb, Sebastien Loriot
//
//******************************************************************************
// File Description :
//
//******************************************************************************

#ifndef CGAL_AABB_SEGMENT_PRIMITIVE_H_
#define CGAL_AABB_SEGMENT_PRIMITIVE_H_

#include <CGAL/internal/AABB_tree/Primitive_caching.h>
#include <CGAL/property_map.h>
#include <CGAL/Default.h>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/if.hpp>

namespace CGAL {

namespace internal{

  //use the property_map to access the point
  template < class GeomTraits, 
             class Iterator,
             class SegmentPropertyMap,
             class PointPropertyMap,
             bool cache_primitive >
  class Segment_point_accessor{
    Segment_point_accessor(PointPropertyMap pmap):m_Point_pmap(pmap){}
    
    typedef typename PointPropertyMap::reference result_type;
    
    template <class PrimitiveCaching>
    result_type get_point(Iterator m_it,const PrimitiveCaching&) const
    {
      return get(m_Point_pmap,m_it);
    }
  private:
    PointPropertyMap m_Point_pmap;
  };
  
  //use Datum to access the point
  template < class GeomTraits, 
             class Iterator,
             class SegmentPropertyMap,
             bool cache_primitive >
  struct Segment_point_accessor<GeomTraits,Iterator,SegmentPropertyMap,::CGAL::Default,cache_primitive>{
    Segment_point_accessor( ::CGAL::Default ){}
    
    typedef Primitive_caching<typename GeomTraits::Segment_3,Iterator,SegmentPropertyMap,cache_primitive> Prim_caching;
    typedef typename boost::mpl::if_<
      boost::is_const<typename Prim_caching::result_type>,
      typename boost::add_const<typename GeomTraits::Point_3>::type,
      typename GeomTraits::Point_3
    >::type const_type;
      
    typedef typename boost::mpl::if_<
      boost::is_reference<typename Prim_caching::result_type>,
      typename boost::add_reference<const_type>::type,
      const_type
    >::type result_type;

    result_type get_point(Iterator m_it,
                          const Prim_caching& pcaching) const
    {
      return pcaching.get_primitive(m_it).source();
    }
  };
}
  
template <class GeomTraits, 
          class Iterator,
          class SegmentPropertyMap=boost::typed_identity_property_map<typename GeomTraits::Segment_3>,
          class PointPropertyMap=Default,
          bool cache_primitive=false>
class AABB_segment_primitive :
  public internal::Primitive_caching<typename GeomTraits::Segment_3,Iterator,SegmentPropertyMap,cache_primitive>,
  public internal::Segment_point_accessor<GeomTraits,Iterator,SegmentPropertyMap,PointPropertyMap,cache_primitive>
{
        // types
        typedef internal::Primitive_caching<typename GeomTraits::Segment_3,Iterator,SegmentPropertyMap,cache_primitive> Primitive_base;
        typedef internal::Segment_point_accessor<GeomTraits,Iterator,SegmentPropertyMap,PointPropertyMap,cache_primitive> Point_accessor_base;
public:
        typedef typename GeomTraits::Point_3 Point; // point type
        typedef typename GeomTraits::Segment_3 Datum; // datum type
        typedef Iterator Id; // Id type

        // member data
private:
        Id m_it;
public:
        // constructors
        AABB_segment_primitive() {}
        AABB_segment_primitive(Id it,SegmentPropertyMap s_pmap=SegmentPropertyMap(),PointPropertyMap p_pmap=PointPropertyMap())
                : Point_accessor_base(p_pmap), m_it(it)
        {
          this->set_primitive(it,s_pmap);
        }
public:
        Id& id() { return m_it; }
        const Id& id() const { return m_it; }
        typename Primitive_base::result_type datum() const {
          return this->get_primitive(m_it);
        }

        /// Returns a point on the primitive
        typename Point_accessor_base::result_type reference_point() const {
          return this->get_point(m_it,*this);
        }
};

}  // end namespace CGAL

#endif // CGAL_AABB_SEGMENT_PRIMITIVE_H_


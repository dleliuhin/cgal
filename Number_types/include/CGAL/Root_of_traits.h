// Copyright (c) 2005,2006  INRIA Sophia-Antipolis (France)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
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
// Author(s)     : Sylvain Pion, Monique Teillaud, Athanasios Kakargias

#ifndef CGAL_ROOT_OF_TRAITS_H
#define CGAL_ROOT_OF_TRAITS_H

#include <CGAL/number_type_basic.h>
#include <CGAL/Root_of_2.h>
#include <CGAL/Quotient.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

template < typename NT, class Algebraic_structure_tag>
struct Root_of_traits_helper{
    typedef Quotient<NT> Root_of_1;
    typedef Root_of_2<NT> Root_of_2;
    struct Make_root_of_2{
        typedef Root_of_2 result_type;
        NT operator()(const NT& a, const NT& b, const NT& c){
            return Root_of_2(a,b,c);
        }
        Root_of_1 operator()(const Root_of_1& a, 
                             const Root_of_1& b, 
                             const Root_of_1& c){
            return Root_of_2(a,b,c);
        }
    };
};

template < typename FT>
struct Root_of_traits_helper < FT, Field_tag >
{
    typedef FT               Root_of_1;
private:
    typedef Fraction_traits<FT> FrT;
    typedef typename FrT::Numerator      RT;
    typedef typename FrT::Decompose Decompose;
public:
    typedef Root_of_2< RT >  Root_of_2;
    
    struct Make_root_of_2{
        typedef FT result_type;
        Root_of_2 
        operator()(const FT& a, const FT& b, const FT& c){
            return Root_of_2(a,b,c);
        }
        Root_of_2 
        operator()(const FT& a, const FT& b, const FT& c, bool smaller){
            Decompose decompose;
            RT a_num,b_num,c_num;
            RT a_den,b_den,c_den; // Denomiantor same as RT
 
            decompose(a,a_num,a_den);
            decompose(b,b_num,b_den);
            decompose(c,c_num,c_den);
            
            RT a_ = a_num * b_den * c_den;
            RT b_ = b_num * a_den * c_den;
            RT c_ = c_num * a_den * b_den;
 
            return make_root_of_2(a_,b_,c_,smaller);
        }
    };
};

template < typename NT >
struct Root_of_traits_helper < NT, Field_with_sqrt_tag >
{
    typedef NT  Root_of_1;
    typedef NT  Root_of_2;
    
    struct Make_root_of_2{
        typedef NT result_type;
        // just a copy, not sure about the semantic of smaller 
        NT operator()(const NT& a, const NT& b, const NT& c, bool smaller){
            // former make_root_of_2_sqrt()
            CGAL_assertion( a != 0 );
            NT discriminant = CGAL_NTS square(b) - a*c*4;
            CGAL_assertion( discriminant >= 0 );
            NT d = CGAL_NTS sqrt(discriminant);
            if ((smaller && a>0) || (!smaller && a<0))
                d = -d;
            return (d-b)/(a*2);
        }
        // it's so easy 
        NT operator()(const NT& a, const NT& b, const NT& c){
            return a + b * CGAL_NTS sqrt(c) ;
        }
    };
};

template < typename NT >
struct Root_of_traits_helper < NT, Field_with_root_of_tag >
    :public Root_of_traits_helper < NT, Field_with_sqrt_tag>{};

} // namespace CGALi


// Default Traits class for NT types
template < typename NT >
struct Root_of_traits
    : public CGALi::Root_of_traits_helper<NT,
      typename Algebraic_structure_traits<NT>::Algebraic_structure_tag> {
    typedef CGALi::Root_of_traits_helper<NT,
      typename Algebraic_structure_traits<NT>::Algebraic_structure_tag> Base;
    typedef typename Base::Root_of_1 RootOf_1;
    typedef typename Base::Root_of_2 RootOf_2;    
};

template < class NT >
inline
typename Root_of_traits< NT >::Root_of_2
make_root_of_2(const NT &a, const NT &b, const NT &c)
{
    typename Root_of_traits<NT>::Make_root_of_2 make_root_of_2;
    return make_root_of_2(a,b,c);
}


template < class NT >
inline
typename Root_of_traits< NT >::Root_of_2
make_root_of_2(const NT &a, int b_, const NT &c)
{
    NT b(b_);
    typename Root_of_traits<NT>::Make_root_of_2 make_root_of_2;
    return make_root_of_2(a,b,c);
}

CGAL_END_NAMESPACE

#endif // CGAL_ROOT_OF_TRAITS_H

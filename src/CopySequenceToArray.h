/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_CopySequenceToArray_h_GUID_69c01a32_d2ff_4bb4_8e83_4dfebe825089
#define INCLUDED_CopySequenceToArray_h_GUID_69c01a32_d2ff_4bb4_8e83_4dfebe825089

// Internal Includes
// - none

// Library/third-party includes
#include <boost/fusion/include/sequence.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/utility/enable_if.hpp>

// Standard includes
// - none

namespace detail {
	/// @brief base case overload
	template<typename Index, typename Sequence, typename Iter>
	inline void copySequenceToArrayImpl(Sequence const&,
	                                    Iter,
	                                    typename boost::disable_if<boost::mpl::less<Index, typename boost::fusion::result_of::size<Sequence>::type> >::type * = NULL) {
	}

	/// @brief recursive implementation of copySequenceToArray
	template<typename Index, typename Sequence, typename Iter>
	inline void copySequenceToArrayImpl(Sequence const& s,
	                                    Iter i,
	                                    typename boost::enable_if<boost::mpl::less<Index, typename boost::fusion::result_of::size<Sequence>::type> >::type * = NULL) {
		*i = boost::fusion::at<Index>(s);
		copySequenceToArrayImpl<typename boost::mpl::plus<Index, boost::mpl::int_<1> >::type>(s, i + 1);
	}


} // end of namespace detail


template<typename Sequence, typename Iter>
inline void copySequenceToArray(Sequence const& s, Iter i) {
	detail::copySequenceToArrayImpl<boost::mpl::int_<0> >(s, i);
}

#endif // INCLUDED_CopySequenceToArray_h_GUID_69c01a32_d2ff_4bb4_8e83_4dfebe825089

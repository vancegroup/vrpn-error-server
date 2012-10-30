/** @file
	@brief Header

	@date 2012

	This header is maintained as a part of 'util-headers' - you can always
	find the latest version online at https://github.com/vancegroup/util-headers

	This GUID can help identify the project: d1dbc94e-e863-49cf-bc08-ab4d9f486613

	This copy of the header is from the revision that Git calls
	4659208a2b3a694a2bf0bac5051cde4c64a4c0ee

	Commit date: "2012-10-30 12:23:38 -0500"

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
#ifndef INCLUDED_TypeId_h_GUID_db8ba085_2a20_480f_bea4_90d9ca6a4a3c
#define INCLUDED_TypeId_h_GUID_db8ba085_2a20_480f_bea4_90d9ca6a4a3c

// Internal Includes
// - none

// Library/third-party includes
#include <boost/mpl/identity.hpp>
#include <boost/operators.hpp>

// Standard includes
#include <typeinfo>

namespace util {
	/// @brief A simple wrapper/handle class for type_info for use in containers, etc.
	class TypeId : public boost::totally_ordered<TypeId, boost::totally_ordered<TypeId, std::type_info> > {
		private:
			/// @brief Dummy empty type, used to indicate an empty typeid.
			class NullType {};
		public:
			/// @brief default constructor - constructs an "empty" typeid.
			TypeId() : _typeinfo(null_type_ptr()) {}

			/// @brief constructor from type_info reference (return type of typeid operator)
			TypeId(std::type_info const & ti) : _typeinfo(&ti) {}

			/// @brief Templated constructor using boost::mpl::identity as a wrapper.
			template<typename T>
			TypeId(boost::mpl::identity<T> const&) : _typeinfo(&typeid(T)) {}

			/// @brief templated static factory method - easier than nesting a typeid call.
			template<typename T>
			static TypeId create() {
				return TypeId(typeid(T));
			}

			/// @brief Some name, with no guarantee of uniqueness or usefulness.
			const char * name() const {
				if (empty()) {
					return "";
				} else {
					return get().name();
				}
			}

			/** @brief Ordering method, based on std::type_info's before ordering method.

				Used by the nonmember operator<()

				Arbitrary, except that for non-empty a and b,a.before(a)
				is always false, and a.before(b) implies !(b.before(a))
				and a != b

			*/
			bool before(std::type_info const& other) const {
				return get().before(other);
			}

			bool before(TypeId const& other) const {
				return get().before(other.get());
			}

			std::type_info const & get() const {
				return *_typeinfo;
			}

			std::type_info const * getPointer() const {
				return _typeinfo;
			}

			bool empty() const {
				return getPointer() == null_type_ptr();
			}

			static std::type_info const * null_type_ptr() {
				return &typeid(NullType);
			}
		private:

			std::type_info const * _typeinfo;

	};

	inline bool operator<(TypeId const& lhs, TypeId const& rhs) {
		return lhs.before(rhs);
	}

	inline bool operator<(TypeId const& lhs, std::type_info const& rhs) {
		return lhs.before(rhs);
	}

	inline bool operator>(std::type_info const& lhs, TypeId const& rhs) {
		return rhs.before(lhs);
	}

	inline bool operator==(TypeId const& lhs, TypeId const& rhs) {
		return lhs.get() == rhs.get();
	}

	inline bool operator==(TypeId const& lhs, std::type_info const& rhs) {
		return lhs.get() == rhs;
	}

	template<typename StreamType>
	StreamType & operator<<(StreamType & s, TypeId const& rhs) {
		s << "[TypeId: " << rhs.name() << "]";
		return s;
	}
} // end of namespace util

#endif // INCLUDED_TypeId_h_GUID_db8ba085_2a20_480f_bea4_90d9ca6a4a3c


#ifndef VLK_UTIL_HPP
#define VLK_UTIL_HPP

#include <type_traits>

namespace vlk
{
	//Using `typename` keyword for templates like this is a C++17 extension
	
	/*!
	 * \brief Is specialization template class
	 *
	 * Evaluates to <tt>std::true_type</tt> if template parameter <tt>T</tt> is a specialization of template class <tt>Primary</tt>. Otherwise evaluates to <tt>std::false_type</tt>
	 */
	template <class T, template<class...> class Primary>
	struct IsSpecialization : std::false_type {};

	template <template <class...> class Primary, class... Args>
	struct IsSpecialization<Primary<Args...>, Primary> : std::true_type {};
}

#endif

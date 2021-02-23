
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

	/*!
	 * \copydoc vlk::IsSpecialization
	 */
	template <template <class...> class Primary, class... Args>
	struct IsSpecialization<Primary<Args...>, Primary> : std::true_type {};

	/*!
	 * \brief Extract parameter template class
	 *
	 * Extracts the template parameter of a template type <tt>T</tt>.
	 * If <tt>T</tt> is not a template, <tt>type<tt> evaluates to <tt>T</tt>.
	 * If <tt>T</tt> is a template of the form <tt>T<S></tt>, then <tt>type</tt> evaluates to <tt>S</tt>.
	 */
	template <typename T>
	struct ExtractParameter { typedef T type; };

	/*!
	 * \copydoc vlk::ExtractParameter
	 */
	template <template <typename> typename T, typename S>
	struct ExtractParameter<T<S>> { typedef S type; };
}

#endif

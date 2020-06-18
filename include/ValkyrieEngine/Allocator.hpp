#pragma once
#include "ValkyrieEngine/ValkyrieDefs.hpp"

#include <functional>

namespace vlk
{
	/*!
	 * \brief Ensures that type \c Alloc has a function called \c Allocate that returns a \c void*
	 */
	template <typename Alloc>
	class HasAllocateFunc
	{
		template <typename U, void* (*)()> class SFINAE {};
		template <typename U> static constexpr std::true_type Test(SFINAE<U, &U::Allocate>*);
		template <typename U> static constexpr std::false_type Test(...);

		public:
		static constexpr bool Value = decltype(Test<Alloc>(nullptr))::value;
	};

	/*!
	 * \brief Ensures that type \c Alloc has a function called \c Deallocate that takes a \c void* as an argument and returns \c void
	 */
	template <typename Alloc>
	class HasDeallocateFunc
	{
		template <typename U, void (*)(void*)> class SFINAE {};
		template <typename U> static constexpr std::true_type Test(SFINAE<U, &U::Deallocate>*);
		template <typename U> static constexpr std::false_type Test(...);

		public:
		static constexpr bool Value = decltype(Test<Alloc>(nullptr))::value;
	};

	/*!
	 * \brief Ensures that type \c Alloc has a function called \c ForEach that takes a function as an argument.
	 * This function should return void and should take a \c Param* as an argument.
	 *
	 * \tparam Alloc The allocator type this class is assessing
	 * \tparam Param the type of object this allocator is allocating
	 *
	 * For example, if <tt>Alloc = MyAllocator<MyType></tt> then <tt>Param = MyType</tt>
	 */
	template <typename Alloc, typename Param>
	class HasForEachFunc
	{
		template <typename U, typename V, void (*)(std::function<void(V*)>)> class SFINAE {};
		template <typename U, typename V> static constexpr std::true_type Test(SFINAE<U, V, &U::ForEach>*);
		template <typename U, typename V> static constexpr std::false_type Test(...);

		public:
		static constexpr bool Value = decltype(Test<Alloc, Param>(nullptr))::value;
	};

	/*!
	 * \brief Ensures that type \c Alloc has a function called \c ForEach that takes two functions as arguments:
	 * A modifier function that returns void and takes a \c Param* as an argument
	 * A filter function that returns a bool and takes a <tt>const Param*</tt> as an argument
	 *
	 * \tparam Alloc The allocator type this class is assessing
	 * \tparam Param the type of object this allocator is allocating
	 *
	 * For example, if <tt>Alloc = MyAllocator<MyType></tt> then <tt>Param = MyType</tt>
	 */
	template <typename Alloc, typename Param>
	class HasFilterForEachFunc
	{
		template <typename T, typename U, void (*)(std::function<void(U*)>, std::function<bool(const U*)>)> class SFINAE {};
		template <typename T, typename U> static constexpr std::true_type Test(SFINAE<T, U, &T::ForEach>*);
		template <typename T, typename U> static constexpr std::false_type Test(...);

		public:
		static constexpr bool Value = decltype(Test<Alloc, Param>(nullptr))::value;
	};

	/*!
	 * \brief Ensures that type \c Alloc has a function called \c Count that returns a \c Size
	 */
	template <typename Alloc>
	class HasCountFunc
	{
		template <typename T, Size (*)()> class SFINAE {};
		template <typename T> static constexpr std::true_type Test(SFINAE<T, &T::Count>*);
		template <typename T> static constexpr std::false_type Test(...);

		public:
		static constexpr bool Value = decltype(Test<Alloc>(nullptr))::value;
	};

	/*!
	 * \brief Ensures that type \c Alloc has a function called \c Count that takes a \c std::function<bool(Param*)> and returns a \c Size
	 */
	template <typename Alloc, typename Param>
	class HasFilterCountFunc
	{
		template <typename T, typename U, Size (*)(std::function<bool(const U*)>)> class SFINAE {};
		template <typename T, typename U> static constexpr std::true_type Test(SFINAE<T, U, &T::Count>*);
		template <typename T, typename U> static constexpr std::false_type Test(...);

		public:
		static constexpr bool Value = decltype(Test<Alloc, Param>(nullptr))::value;
	};

	/*!
	 * \brief Assesses wether type \c Alloc is a valid allocator for type \c Param
	 * Valkyrie Engine allocators serve a distinct purpose to STL allocators, their memory
	 * allocation is handled strictly internally and they keep track of every active allocation. 
	 *
	 * Allocators must have the following functions defined:
	 * \code{.cpp}
	 *
	 * //Allocates space for a new instance and returns a void* to it.
	 * //Does not construct the instance.
	 * [[nodiscard]] static void* Allocate();
	 *
	 * //Frees the memory allocation of an instance located at the provided pointer.
	 * //Does not destruct the instance.
	 * static void Deallocate(void* ptr);
	 *
	 * //Performs a modifier function on every active allocation
	 * static void ForEach(std::function<void(Param*)> func);
	 *
	 * //Performs a modifier function on every active allocation that matches a filter
	 * static void ForEach(std::function<void(Param*)> func, std::function<bool(const Param*)> filter);
	 *
	 * //Returns the number of active allocations
	 * static Size Count();
	 *
	 * //Returns the number of active allocations that match a filter
	 * static Size Count(std::function<bool(const Param*)>);
	 *
	 * \tparam Alloc The allocator type this class is assessing
	 * \tparam Param the type of object this allocator is allocating
	 *
	 * For example, if <tt>Alloc = MyAllocator<MyType></tt> then <tt>Param = MyType</tt>
	 */
	template <typename Alloc, typename Param>
	class IsAllocator
	{
		public:
		static constexpr bool Value = 
			HasAllocateFunc<Alloc>::Value && 
			HasDeallocateFunc<Alloc>::Value && 
			HasForEachFunc<Alloc, Param>::Value && 
			HasFilterForEachFunc<Alloc, Param>::Value &&
			HasCountFunc<Alloc>::Value &&
			HasFilterCountFunc<Alloc, Param>::Value;
	};
}

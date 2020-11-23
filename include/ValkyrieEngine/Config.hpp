/*!
 * \file Config.hpp
 * \brief Defines various symbols to make valkyrie engine more compatable with older C++ standards, see each of the symbols for a detailed description
 */

#ifndef VLK_CONFIG_HPP
#define VLK_CONFIG_HPP

namespace vlk
{
	#ifdef __cplusplus
		/*!
		 * \def VLK_CXX14_CONSTEXPR
		 * \brief A macro used to denote an inline function or variable that satisfies the C++14 requirements for <tt>constexpr</tt>
		 *
		 * This may be used to avoid compiler errors for functions that meet <tt>constexpr</tt> requirements in C++14, but may not meet those of earlier versions.
		 *
		 * Expands to <tt>constexpr</tt> on C++14 and later, otherwise, the macro is empty.
		 */

		/*!
		 * \def VLK_CXX17_CONSTEXPR
		 * \brief A macro used to denote an inline function or variable that satisfies the C++17 requirements for <tt>constexpr</tt>
		 *
		 * This may be used to avoid compiler errors for functions that meet <tt>constexpr</tt> requirements in C++17, but may not meet those of earlier versions.
		 *
		 * Expands to <tt>constexpr</tt> on C++17 and later, otherwise, the macro is empty.
		 */

		/*!
		 * \def VLK_CXX20_CONSTEXPR
		 * \brief A macro used to denote an inline function or variable that satisfies the C++20 requirements for <tt>constexpr</tt>
		 *
		 * This may be used to avoid compiler errors for functions that meet <tt>constexpr</tt> requirements in C++20, but may not meet those of earlier versions.
		 *
		 * Expands to <tt>constexpr</tt> on C++20 and later, otherwise, the macro is empty.
		 */

		/*!
		 * \def VLK_IS_CONSTANT_EVALUATED
		 * \brief A macro used to determine whether something is being evaluated in a <tt>constexpr</tt> environment.
		 *
		 * Use this in conjunction with a <b>regular</b> (non-constexpr) <tt>if</tt> statement to enable different branches of code at compile time and runtime.
		 *
		 * This macro expands to <tt>std::is_constant_evaluated()</tt> if supported, otherwise expands to <tt>false</tt>.
		 *
		 * This macro will require the inclusion of the <tt>\<type_traits\></tt> header when used in order function properly.
		 *
		 * <tt>std::is_constant_evaluated</tt> is not standard until C++20.
		 *
		 * \sa VLK_CXX20_CONSTEXPR
		 */

		/*!
		 * \def VLK_CONSTEXPR_IF
		 * \brief A macro used to denote a <tt>constexpr if</tt> expression.
		 *
		 * Use this to evaluate an <tt>if</tt> statement at compile-time without causing compiler errors if <tt>if constexpr</tt> statements are not supported.
		 *
		 * This macro will expand to <tt>if constexpr</tt> if the expression is supported in the current C++ standard, otherwise expands to <tt>if</tt>.
		 *
		 * <tt>constexpr if</tt> statements are not standard until C++17.
		 *
		 * \code{.cpp}
		 * VLK_CXX14_CONSTEXPR bool SomeCondition()
		 * {
		 *     return true;
		 * }
		 *
		 * void Foo()
		 * {
		 *     // Statement is only constexpr in C++17 and above, but is well-formed in all versions.
		 *     VLK_CONSTEXPR_IF(SomeCondition())
		 *     {
		 *         // Block is evaluated
		 *     }
		 *     else
		 *     {
		 *         // Block is not evaluated
		 *     }
		 * }
		 * \endcode
		 */

		/*!
		 * \def VLK_FALLTHROUGH
		 * \brief A macro used to denote an intentional fallthrough within a <tt>switch</tt> statement.
		 *
		 * This macro will expand to <tt>[[fallthrough]]</tt> if the expression is supported in the current C++ standard, otherwise, the macro is empty.
		 *
		 * <tt>[[fallthrough]]</tt> attributes are not standard until C++17.
		 *
		 * \sa VLK_CXX17_CONSTEXPR
		 *
		 * \code{.cpp}
		 * void Foo(int i)
		 * {
		 *     switch (i)
		 *     {
		 *         case 1:
		 *         case 3:
		 *             Bar();
		 *             VLK_FALLTHROUGH; // No warning generated on fallthrough
		 *		   default:
		 *             Baz();
		 *             break;
		 *     }
		 * }
		 * \endcode
		 */

		/*!
		 * \def VLK_NODISCARD
		 * \brief A macro used to denote a function whose return value should not be discarded.
		 *
		 * This macro will expand to <tt>[[nodiscard]]</tt> if the expression is supported in the current C++ standard, otherwise, the macro is empty.
		 *
		 * <tt>[[nodiscard]]</tt> attributes are not standard until C++17.
		 *
		 * \sa VLK_NODISCARD_MSG(msg)
		 *
		 * \code{.cpp}
		 * VLK_NODISCARD
		 * int* MakeInt()
		 * {
		 *     return new int();
		 * }
		 * \endcode
		 *
		 * \def VLK_NODISCARD_MSG(msg)
		 * \brief A macro used to denote a function whose return value should not be discarded.
		 * \param msg A warning message to send to the compiler if the return value is discarded.
		 *
		 * This macro will expand to <tt>[[nodiscard(msg)]]</tt> if the expression is supported in the current C++ standard, otherwise, the macro will be identical to #VLK_NODISCARD.
		 *
		 * <tt>[[nodiscard(msg)]]</tt> attributes are not standard until C++20.
		 *
		 * \sa VLK_NODISCARD
		 *
		 * \code{.cpp}
		 * VLK_NODISCARD_MSG("Causes memory leaks")
		 * int* MakeInt()
		 * {
		 *     return new int();
		 * }
		 * \endcode
		 */

		/*!
		 * \def VLK_SHARED_MUTEX_TYPE
		 * \brief A macro used to denote the prefered shared mutex type in the current standard.
		 *
		 * This macro will expand to <tt>std::shared_mutex</tt> if the expression is supported in the current C++ standard, otherwise, the macro expands to <tt>std::shared_timed_mutex</tt>
		 * This macro will require the inclusion of the <tt><shared_mutex></tt> header in order to function properly.
		 *
		 * <tt>std::shared_mutex</tt> is not standard until C++17.
		 *
		 * \code{.cpp}
		 * VLK_SHARED_MUTEX_TYPE mtx;
		 * std::shared_lock slock(mtx);
		 *
		 * ...
		 *
		 * slock.unlock();
		 * \endcode
		 */

		/*!
		 * \def VLK_STATIC_ASSERT(cond)
		 * \brief A macro used to assert a condition at compile-time.
		 * \param cond A compile-time condition to evaluate.
		 *
		 * This macro will expand to <tt>static_assert(cond)</tt> if the expression is supported in the current C++ standard, otherwise, the macro will be identical to VLK_STATIC_ASSERT_MSG(cond, "").
		 *
		 * <tt>static_assert(bool)</tt> is not standard until C++17.
		 *
		 * \sa VLK_STATIC_ASSERT_MSG(cond, msg)
		 *
		 * \code{.cpp}
		 * VLK_STATIC_ASSERT(std::is_same<int, float>::value); // Prevents compilation
		 * \endcode
		 *
		 * \def VLK_STATIC_ASSERT_MSG(cond, msg)
		 * \brief A macro used to assert a condition at compile-time.
		 * \param cond A compile-time condition to evaluate
		 * \param msg A warning message to send to the compiler if the assertion fails.
		 *
		 * This macro will expand to <tt>static_assert(cond, msg)</tt> if the expression is supported in the current C++ standard, otherwise, the macro will be empty.
		 *
		 * <tt>static_assert(bool, const char*)</tt> is not standard until C++11.
		 *
		 * \sa VLK_STATIC_ASSERT(cond)
		 *
		 * \code{.cpp}
		 * VLK_STATIC_ASSERT_MSG(std::is_same<int, float>::value, "I just really didn't want this to compile.");
		 * \endcode
		 */
		#if   __cplusplus >= 202002L					//C++20
			#define VLK_CXX14_CONSTEXPR					constexpr
			#define VLK_CXX17_CONSTEXPR					constexpr
			#define VLK_CXX20_CONSTEXPR					constexpr

			#define VLK_IS_CONSTANT_EVALUATED			false
			#define VLK_CONSTEXPR_IF					if constexpr
			#define VLK_STATIC_ASSERT(cond)				static_assert(cond)
			#define VLK_STATIC_ASSERT_MSG(cond, msg)	static_assert(cond, msg)

			#define VLK_FALLTHROUGH						[[fallthrough]]
			#define VLK_NODISCARD						[[nodiscard]]
			#define VLK_NODISCARD_MSG(msg)				[[nodiscard(msg)]]

			#define VLK_SHARED_MUTEX_TYPE				std::shared_mutex

		#elif __cplusplus >= 201703L					//C++17
			#define VLK_CXX14_CONSTEXPR					constexpr
			#define VLK_CXX17_CONSTEXPR					constexpr
			#define VLK_CXX20_CONSTEXPR

			#define VLK_IS_CONSTANT_EVALUATED			false
			#define VLK_CONSTEXPR_IF					if constexpr
			#define VLK_STATIC_ASSERT(cond)				static_assert(cond)
			#define VLK_STATIC_ASSERT_MSG(cond, msg)	static_assert(cond, msg)

			#define VLK_FALLTHROUGH						[[fallthrough]]
			#define VLK_NODISCARD						[[nodiscard]]
			#define VLK_NODISCARD_MSG(msg)				[[nodiscard]]

			#define VLK_SHARED_MUTEX_TYPE				std::shared_mutex

		#elif __cplusplus >= 201402L					//C++14
			#define VLK_CXX14_CONSTEXPR					constexpr
			#define VLK_CXX17_CONSTEXPR
			#define VLK_CXX20_CONSTEXPR

			#define VLK_IS_CONSTANT_EVALUATED			false
			#define VLK_CONSTEXPR_IF					if
			#define VLK_STATIC_ASSERT(cond)				static_assert(cond, "")
			#define VLK_STATIC_ASSERT_MSG(cond, msg)	static_assert(cond, msg)

			#define VLK_FALLTHROUGH
			#define VLK_NODISCARD
			#define VLK_NODISCARD_MSG(msg)

			#define VLK_SHARED_MUTEX_TYPE				std::shared_timed_mutex

		#else
			#error C++14 or greater is required.
		#endif
	#else
		#error C++ compiler is required.
	#endif

	/*!
	 * \def VLK_ENABLE_TRACE_LOGGING
	 * \brief A macro used to enable trace-level debug logging, should expand to either <tt>true</tt> or <tt>false</tt>.
	 *
	 * This can have a substantial inpact on performance, so unless you are debugging something engine-side, it is best left turned off.
	 *
	 * You should enable this by passing an appropriate flag to your compiler, enabling it in your own code is not guaranteed to work.
	 *
	 * \code{.cpp}
	 * void Foo()
	 * {
	 *     if (VLK_ENABLE_TRACE_LOGGING)
	 *     {
	 *	       vlk::Log<vlk::LogLevel::Trace>("Entered function Foo()");
	 *     }
	 *
	 *     ...
	 * }
	 * \endcode
	 */
	#ifndef VLK_ENABLE_TRACE_LOGGING
		#define VLK_ENABLE_TRACE_LOGGING false
	#endif

	/*!
	 * \def VLK_IS_DEBUG
	 * \brief A macro used to determine whether debug-only code should be compiled.
	 *
	 * This macro depends on the <tt>NDEBUG</tt> macro and will expand to <tt>true</tt> if <tt>NDEBUG</tt> is **undefined** and <tt>false</tt> otherwise.
	 *
	 * \code{.cpp}
	 * void Bar()
	 * {
	 *     VLK_CONSTEXPR_IF (VLK_IS_DEBUG)
	 *     {
	 *         // Also works
	 *     }
	 * }
	 * \endcode
	 */
	#ifndef NDEBUG
		#define VLK_IS_DEBUG true
	#else
		#define VLK_IS_DEBUG false
	#endif
}

#endif //VLK_CONFIG_H

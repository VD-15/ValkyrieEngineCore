/*!
 * \file ValkyrieDefs.hpp
 * \brief Provides primitive typedefs
 */

#ifndef VLK_DEF_HPP
#define VLK_DEF_HPP

#include <cstdint>
#include "ValkyrieEngine/Config.hpp"

namespace vlk
{
	/*!
	 * \brief Unsigned 8-bit integer
	 */
	typedef std::uint8_t	UByte;

	/*!
	 * \brief Signed 8-bit integer
	 */
	typedef std::int8_t		Byte;

	/*!
	 * \brief Unsigned 16-bit integer
	 */
	typedef std::uint16_t	UShort;

	/*!
	 * \brief Signed 16-bit integer
	 */
	typedef std::int16_t	Short;

	/*!
	 * \brief Unsigned 32-bit integer
	 */
	typedef std::uint32_t	UInt;

	/*!
	 * \brief Signed 32-bit integer
	 */
	typedef std::int32_t	Int;

	/*!
	 * \brief Unsigned 64-bit integer
	 */
	typedef std::uint64_t	ULong;

	/*!
	 * \brief Signed 64-bit integer
	 */
	typedef std::int64_t	Long;

	/*!
	 * \brief Platform-dependent size type
	 */
	typedef std::size_t		Size;

	/*!
	 * \brief 32-bit floating-point number
	 */
	typedef float			Float;

	/*!
	 * \brief 64-bit floating-point number
	 */
	typedef double			Double;
}

#endif

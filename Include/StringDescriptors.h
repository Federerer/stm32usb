/*
 * StringDescriptors.h
 *
 *  Created on: 9 sie 2017
 *      Author: 08fed
 */

#ifndef USB_STRINGDESCRIPTORS_H_
#define USB_STRINGDESCRIPTORS_H_

#include <utility>
#include <array>

namespace USBLib
{

namespace
{
constexpr char charConverter(const char c, int i)
{
	return i % 2 ? (char) 0 : c;
}

template<unsigned N, typename T, T ... Nums>
constexpr const std::array<char, 2 * N + 2> stringDescriptorHelper(
		const char (&str)[N], std::integer_sequence<T, Nums...>)
{
	return
	{	(char)2*N+2,(char)3,charConverter(str[Nums/2], Nums)...};
}
}

template<unsigned N>
constexpr const std::array<char, 2 * N + 2> toStringDescriptor(
		const char (&str)[N])
{
	return stringDescriptorHelper(str,
			std::make_integer_sequence<unsigned, N * 2>());
}

} /* namespace stringDescriptors */

#endif /* USB_STRINGDESCRIPTORS_H_ */

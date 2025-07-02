/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:49:32 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:27:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * int ft_printf(const char *format, ...)
 * The function ft_printf() writes the output under the control of a
 * format string that specifies how subsequent arguments are converted for
 * output. The format string is composed of zero or more directives: 
 * ordinary characters `not %` which are copied unchanged to the output stream;
 * and conversion specifications, each of which results in fetching zero or
 * more subsequent arguments. Each conversion specification is introduced by
 * the character `%`, and ends with a `conversion specifier`. 
 * In between there may be (in this order) zero or more flags, an optional
 * minimum `field width`, an optional `precision`.
 * 
 * @param format the format string
 * @param ... the arguments to be formatted
 * @return the number of characters printed
 * @note
 * @note- The format string must be a null-terminated string.
 * @note- The format string may contain format specifiers starting 
 * with % which are
 * @note replaced by the values of arguments that follow the format string.
 * @note ### Flags (width, precisions, etc):
 * @note `#`		The value should be converted to an "alternate form". 
 * @note 		For (x, X, p),
 * @note 		a non-zero result has the string prefixed 
 * @note 		with 0x or 0X respectively prepended to it
 * @note `0`		The value should be zero-padded. 
 * @note 		For d, i, p, u, x, X conversions, the converted
 * @note 		value is padded on the left with zeros rather than blanks. 		
 * @note `-`  	a blank should be left before a positive number
 * @note  		produced by a signed conversion.	
 * @note `' '`
 * @note `+`	A sign (+ or -) should always be placed before a number produced
 * @note 		by a signed conversion. by default only - sign is used
 * @note `*`	The width is not specified in the format string, 
 * but as an additional
 * @note 		integer value argument preceding the argument that 
 * has to be formatted.
 * @note `.`	The precision is not specified in the format string, 
 * but as an additional
 * @note 		integer value argument preceding the argument 
 * that has to be formatted.
 * 
 * @note ### FORMAT SPECIFIERS:
 * 
 * @note `c`	Character
 * @note `s`	String of characters
 * @note `p`	Pointer address
 * @note `d`	Decimal number (base 10)
 * @note `i`	Integer
 * @note `u`	Unsigned decimal (integer) number
 * @note `x`	Hexadecimal number (with lowercase letters)
 * @note `X`	Hexadecimal number (with uppercase letters)
 * @note `%`	A % followed by another % character will 
 * write a single % to the stream.
 */
int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		result;

	if (!format)
		return (-1);
	va_start(args, format);
	result = ft_process_format(format, args);
	va_end(args);
	return (result);
}

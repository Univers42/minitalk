/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_width_hex_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:15:25 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:25:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Prints a left-aligned hexadecimal number or a zero value with 
 * precision and width handling.
 *
 * This function formats an unsigned integer as a hexadecimal 
 * string while considering
 * precision and width rules. It can print a zero value with a 
 * specified precision and
 * properly align the output based on the LEFT flag.
 *
 * @param num The unsigned integer to be printed as a hexadecimal number.
 * @param hex_len The length of the hexadecimal number
 * @param prefix_len The length of the prefix
 * @param flags The flags structure controlling formatting behavior.
 * @return The total number of characters printed.
 *
 * @note The function checks if uppercase letters should be used based 
 * on 'X' specifier.
 * @note The prefix ("0x" or "0X") is printed only if the HASH flag is 
 * set and the number is nonzero.
 * @note The hexadecimal number is printed with the required precision.
 * @note Any additional width is padded with spaces after printing the 
 * number (left-aligned).
 * @note If the precision is 0 or -1, only width padding is applied.
 * @note If the LEFT flag is set, precision is printed first, 
 * followed by width padding.
 * @note If the LEFT flag is not set, width padding is applied first, 
 * followed by precision.
 */
int	ft_print_hex_zero_with_dot(unsigned int num, t_flags flags, int upper)
{
	int	count;

	(void)num;
	count = 0;
	if (flags.precision == 0 || flags.precision == -1)
		return (ft_print_width(flags.width, 0, 0));
	if (flags.flags_bits & LEFT_FLAG)
	{
		count += ft_print_hex_precision(0, flags.precision, upper);
		count += ft_print_width(flags.width, flags.precision, 0);
	}
	else
	{
		count += ft_print_width(flags.width, flags.precision, 0);
		count += ft_print_hex_precision(0, flags.precision, upper);
	}
	return (count);
}

/**
 * Prints a left-aligned hexadecimal number with optional prefix, width, 
 * and precision.
 *
 * This function formats an unsigned integer as a hexadecimal string 
 * and prints it
 * left-aligned according to the specified formatting flags. 
 * It handles optional prefixes ("0x" or "0X"), precision, and width padding.
 *
 * @param num The unsigned integer to be printed as a hexadecimal number.
 * @param hex_len The length of the hexadecimal number
 * @param prefix_len The length of the prefix
 * @param flags The flags structure controlling formatting behavior.
 * @return The total number of characters printed.
 *
 * @note The function checks if uppercase letters should be used 
 * based on 'X' specifier.
 * @note The prefix ("0x" or "0X") is printed only if the HASH flag is 
 * set and the number is nonzero.
 * @note The hexadecimal number is printed with the required precision.
 * @note Any additional width is padded with spaces after printing 
 * the number
 */
int	ft_print_hex_left(unsigned int num, int hex_len,
					int prefix_len, t_flags flags)
{
	int	count;
	int	uppercase;

	count = 0;
	uppercase = 0;
	if (flags.spec == 'X')
		uppercase = 1;
	if ((flags.flags_bits & HASH_FLAG) && num != 0)
		count += (int)ft_print_hex_prefix(uppercase);
	count += ft_print_hex_precision(num, flags.precision, uppercase);
	count += ft_print_width(flags.width - prefix_len, hex_len, 0);
	return (count);
}

/**
 * Prints a right-aligned hexadecimal number with optional prefix, 
 * width, and precision.
 *
 * This function formats an unsigned integer as a hexadecimal 
 * string, applying
 * specific formatting rules based on the provided flags. 
 * It handles optional prefixes
 * ("0x" or "0X"), zero padding, width specifications, and precision.
 *
 * @param num The unsigned integer to be printed as a hexadecimal number.
 * @param hex_len The length of the hexadecimal number
 * @param prefix_len The length of the prefix
 * @param flags The flags structure controlling formatting behavior.
 * @return The total number of characters printed.
 *
 * @note The function checks if uppercase letters should be used based on 
 * 'X' specifier.
 * @note If the ZERO flag is set and the DOT (precision) flag is not set,
 *       padding is done with zeros instead of spaces.
 * @note The prefix ("0x" or "0X") is printed only if the HASH flag is set 
 * and the number is nonzero.
 * @note If zero padding is applied, it occurs before the number and prefix
 */
int	ft_print_hex_right(unsigned int num, int hex_len,
					int prefix_len, t_flags flags)
{
	int	count;
	int	uppercase;

	count = 0;
	uppercase = 0;
	if (flags.spec == 'X')
		uppercase = 1;
	if ((flags.flags_bits & ZERO_FLAG) && !(flags.flags_bits & DOT_FLAG))
	{
		if ((flags.flags_bits & HASH_FLAG) && num != 0)
			count += (int)ft_print_hex_prefix(uppercase);
		count += ft_print_width(flags.width - prefix_len, hex_len, 1);
	}
	else
	{
		count += ft_print_width(flags.width - prefix_len, hex_len, 0);
		if ((flags.flags_bits & HASH_FLAG) && num != 0)
			count += (int)ft_print_hex_prefix(uppercase);
	}
	count += ft_print_hex_precision(num, flags.precision, uppercase);
	return (count);
}

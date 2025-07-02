/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_wrappers_two_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 21:01:37 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:26:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Converts and prints a lower-case hexadecimal
 * value based on the provided flags.
 *
 * @param type The conversion type (ignored in this function).
 * @param flags The flags to apply during conversion and printing.
 * @param args The variable argument list containing the value to convert.
 * @return The number of characters printed.
 */
int	ft_convert_hex_lower(char type, t_flags flags, va_list args)
{
	(void)type;
	return (ft_print_hex_format(va_arg(args, unsigned int), 0, flags));
}

/**
 * Converts an unsigned integer to a hexadecimal string representation 
 * in uppercase.
 *
 * @param type The conversion type specifier (ignored in this function).
 * @param flags The formatting flags to apply.
 * @param args The variable argument list.
 * @return The number of characters printed.
 */
int	ft_convert_hex_upper(char type, t_flags flags, va_list args)
{
	(void)type;
	return (ft_print_hex_format(va_arg(args, unsigned int), 1, flags));
}

/**
 * Converts a pointer argument to a string and prints it according 
 * to the given flags.
 *
 * @param type The conversion specifier character.
 * @param flags The flags to apply during the conversion and printing.
 * @param args The variable argument list containing the pointer argument.
 * @return The number of characters printed.
 * @note the pointer is cast to an `unsigned long` before being printed. 
 * @note because the size of a pointer can vary depending on the system 
 * @note architecture. converting the pointer to an `unsigned long` ensures
 * @note that the pointer is printed correctly.
 * @note in this specific line of code `flags.flags_bits &= ~DOT_FLAG;`
 * the bitwise AND operator is used to clear the DOT_FLAG bit in the
 * `flags_bits` field of the `flags` structure. The `~` operator is used to
 * invert the bits of the DOT_FLAG constant. The result is that the DOT_FLAG
 * bit is cleared in the `flags_bits` field.
 */
int	ft_convert_ptr(char type, t_flags flags, va_list args)
{
	void	*ptr;

	(void)type;
	ptr = va_arg(args, void *);
	if (!ptr && (flags.flags_bits & DOT_FLAG) && flags.precision == 0)
		flags.flags_bits &= (unsigned int) ~DOT_FLAG;
	return (ft_print_ptr((unsigned long)ptr, flags));
}

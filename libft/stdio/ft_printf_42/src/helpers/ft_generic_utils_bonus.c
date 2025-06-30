/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_generic_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:28:30 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:58:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Prints a single character and returns 1
 */
int	ft_print_c(char c)
{
	return (write(STDOUT_FILENO, &c, 1), 1);
}

/**
 * Calculates the number of digits in a number with specified base
 * 
 * @param n The number to check
 * @param base The number base (e.g., 10 for decimal, 16 for hex)
 * @return The number of digits
 * @note  
 */
int	ft_nbr_len(size_t n, int base)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= (size_t)base;
		len++;
	}
	return (len);
}

/**
 * Simple character padding - prints the specified character n times
 */
int	ft_print_char_padding(char c, int count)
{
	int	total;

	total = 0;
	while (count > 0)
	{
		total += ft_print_c(c);
		count--;
	}
	return (total);
}

/**
 * ##### Checks if a character is a valid format specifier
 * @param c The character to check
 * @return 1 if it's a type specifier, 0 otherwise
 */
int	ft_istype(int c)
{
	return (c == 'c' || c == 's' || c == 'p'
		|| c == 'd' || c == 'i' || c == 'u'
		|| c == 'x' || c == 'X' || c == '%'
		|| c == 'o');
}

/**
 * ##### Handles invalid format specifiers by printing them as regular
 * characters
 * 
 * @param type The invalid format type character
 * @param flags The formatting flags
 * @return Number of characters printed
 */
int	ft_handle_invalid_format(char type, t_flags flags)
{
	int	count;

	count = 0;
	if (flags.flags_bits & LEFT_FLAG)
		count += ft_print_c(type);
	count += ft_print_width(flags.width - 1, 1, 0);
	if (!(flags.flags_bits & LEFT_FLAG))
		count += ft_print_c(type);
	return (count);
}

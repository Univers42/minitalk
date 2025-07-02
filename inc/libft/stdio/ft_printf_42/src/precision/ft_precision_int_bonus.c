/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_precision_int_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 06:16:43 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:26:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Handles the case of printing zero with precision 0.
 *
 * This function prints a number considering precision 0 cases and handles
 * sign-related formatting based on the flags.
 *
 * @param n The integer to be printed.
 * @param flags The flags structure controlling formatting behavior.
 * @return The total number of characters printed.
 *
 * @note If the PLUS or SPACE flag is set, or if the number is negative, 
 * the sign is handled.
 * @note If the LEFT flag is set, the sign is printed first, followed 
 * by width padding.
 * @note Otherwise, padding is applied first, followed by the sign.
 * @note If no special conditions apply, only width padding is printed.
 */
int	ft_handle_zero_precision(int n, t_flags flags)
{
	int	count;
	int	sign_len;

	count = 0;
	sign_len = ft_calc_sign_len(n, flags);
	if ((flags.flags_bits & PLUS_FLAG)
		|| (flags.flags_bits & SPACE_FLAG) || n < 0)
	{
		if (flags.flags_bits & LEFT_FLAG)
		{
			count += ft_print_sign(n, flags);
			count += ft_print_width(flags.width - sign_len, 0, 0);
		}
		else
		{
			count += ft_print_width(flags.width - sign_len, 0, 0);
			count += ft_print_sign(n, flags);
		}
		return (count);
	}
	return (ft_print_width(flags.width, 0, 0));
}

/**
 * Prints an unsigned integer with precision padding.
 *
 * This function prints an unsigned integer while ensuring the specified 
 * precision.
 * If precision is greater than the number length, leading zeros are added.
 *
 * @param n The unsigned integer to be printed.
 * @param precision The required precision (minimum number length).
 * @return The total number of characters printed.
 *
 * @note If the number is 0 and precision is 0, nothing is printed.
 * @note The function first prints leading zeros if needed, 
 * then prints the number.
 */
int	ft_print_unsigned_precision(unsigned int n, int precision)
{
	int	count;
	int	len;

	count = 0;
	if (n == 0 && precision == 0)
		return (0);
	len = ft_nbr_len(n, BASE_DEC);
	count += ft_print_width(precision, len, 1);
	count += ft_print_unsigned_nbr(n);
	return (count);
}

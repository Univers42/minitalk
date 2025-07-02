/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_int_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:02:07 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:26:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Prints the sign of a number according to flags
 * 
 * @param n The number to check for sign
 * @param flags The flags struct containing sign modifiers
 * @return Number of characters printed
 */
int	ft_print_sign(int n, t_flags flags)
{
	if (n < 0)
		return (ft_print_c('-'));
	else if (flags.flags_bits & PLUS_FLAG)
		return (ft_print_c('+'));
	else if (flags.flags_bits & SPACE_FLAG)
		return (ft_print_c(' '));
	return (0);
}

/**
 * Prints an unsigned integer
 * 
 * @param n The unsigned integer to print
 * @return Number of characters printed
 */
int	ft_print_unsigned_nbr(unsigned int n)
{
	int	count;

	count = 0;
	if (n >= BASE_DEC)
		count += ft_print_unsigned_nbr(n / BASE_DEC);
	count += ft_print_c((char)('0' + (n % BASE_DEC)));
	return (count);
}

/**
 * Calculates the length needed for sign character
 * 
 * @param n The number to check
 * @param flags The flags struct with sign options
 * @return 1 if sign will be printed, 0 otherwise
 */
int	ft_calc_sign_len(int n, t_flags flags)
{
	int	sign_len;

	sign_len = 0;
	if (n < 0)
		sign_len = 1;
	else if (flags.flags_bits & PLUS_FLAG)
		sign_len = 1;
	else if (flags.flags_bits & SPACE_FLAG)
		sign_len = 1;
	return (sign_len);
}

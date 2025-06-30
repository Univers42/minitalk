/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_width_int_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:50:21 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:25:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * @brief Calculate the length of the sign character
 * 
 * @param n The number to check
 * @param flags The flags struct with sign options
 * @return 1 if sign will be printed, 0 otherwise
 */
int	ft_print_int_ralign_std(int n, t_flags fl, unsigned int un, int len)
{
	int	count;
	int	sign_len;

	count = 0;
	sign_len = ft_calc_sign_len(n, fl);
	count += ft_print_width(fl.width, len + sign_len, 0);
	count += ft_print_sign(n, fl);
	count += ft_print_width(fl.precision, ft_nbr_len(un, BASE_DEC), 1);
	if (!((fl.flags_bits & DOT_FLAG) && fl.precision == 0 && n == 0))
		count += ft_print_unsigned_nbr(un);
	return (count);
}

/**
 * @brief Calculate the length of the sign character
 * 
 * @param n The number to check
 * @param flags The flags struct with sign options
 * @return 1 if sign will be printed, 0 otherwise
 */
int	ft_print_int_ralign_zero(int n, t_flags fl, unsigned int un, int len)
{
	int	count;
	int	sign_len;

	count = 0;
	sign_len = ft_calc_sign_len(n, fl);
	count += ft_print_sign(n, fl);
	count += ft_print_width(fl.width, len + sign_len, 1);
	count += ft_print_width(fl.precision, ft_nbr_len(un, BASE_DEC), 1);
	if (!((fl.flags_bits & DOT_FLAG) && fl.precision == 0 && n == 0))
		count += ft_print_unsigned_nbr(un);
	return (count);
}

/**
 * @brief Calculate the length of the sign character
 * 
 * 
 * @param n The number to check
 * @param flags The flags struct with sign options
 * @return 1 if sign will be printed, 0 otherwise
 * 
 */
int	ft_print_int_lalign(int n, t_flags fl, unsigned int un, int len)
{
	int	count;
	int	sign_len;

	count = 0;
	sign_len = ft_calc_sign_len(n, fl);
	count += ft_print_sign(n, fl);
	count += ft_print_width(fl.precision, ft_nbr_len(un, BASE_DEC), 1);
	if (!((fl.flags_bits & DOT_FLAG) && fl.precision == 0 && n == 0))
		count += ft_print_unsigned_nbr(un);
	count += ft_print_width(fl.width, len + sign_len, 0);
	return (count);
}

/**
 * Prints an integer with right alignment based on the given flags.
 *
 * @param n The integer to be printed.
 * @param fl The flags specifying the formatting options.
 * @param un The unsigned integer value (for handling negative numbers).
 * @param len The length of the integer value.
 * @return The number of characters printed.
 * @note 
 */
int	ft_print_int_ralign(int n, t_flags fl, unsigned int un, int len)
{
	if ((fl.flags_bits & ZERO_FLAG) && !(fl.flags_bits & DOT_FLAG))
		return (ft_print_int_ralign_zero(n, fl, un, len));
	else
		return (ft_print_int_ralign_std(n, fl, un, len));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_width_ptr_extra_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 06:17:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:25:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Routes to the appropriate pointer right-alignment function
 */
int	ft_print_ptr_right(unsigned long n, t_flags flags,
						int ptr_len, int hex_len)
{
	if ((flags.flags_bits & ZERO_FLAG) && !(flags.flags_bits & DOT_FLAG))
		return (ft_print_ptr_right_zero(n, flags, hex_len));
	else
		return (ft_print_ptr_right_std(n, flags, ptr_len));
}

/**
 * Prints pointer with right alignment using zero padding
 */
int	ft_print_ptr_right_zero(unsigned long n, t_flags flags, int hex_len)
{
	int	count;

	count = 0;
	count += (int)ft_print_hex_prefix(0);
	count += ft_print_width(flags.width - 2, hex_len, 1);
	count += ft_print_hex_value(n, 0);
	return (count);
}

/**
 * Prints pointer with standard right alignment
 */
int	ft_print_ptr_right_std(unsigned long n, t_flags flags, int ptr_len)
{
	int	count;

	count = 0;
	count += ft_print_width(flags.width, ptr_len, 0);
	count += (int)ft_print_hex_prefix(0);
	count += (int)ft_print_ptr_hex_precision(n, flags.precision);
	return (count);
}

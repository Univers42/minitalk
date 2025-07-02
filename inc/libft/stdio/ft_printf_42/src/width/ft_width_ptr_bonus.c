/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_width_ptr_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 06:17:13 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:25:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

int	ft_print_null_ptr(t_flags flags)
{
	if ((flags.flags_bits & DOT_FLAG) && flags.precision == 0)
		return (ft_print_width(flags.width, 0, 0));
	if (flags.flags_bits & LEFT_FLAG)
		return (ft_print_null_ptr_left(flags));
	else
		return (ft_print_null_ptr_right(flags));
}

/**
 * Prints null pointer with left alignment
 */
int	ft_print_null_ptr_left(t_flags flags)
{
	int	count;

	count = 0;
	count += (int)write(1, "(nil)", 5);
	if (flags.width > 5)
		count += ft_print_width(flags.width, 5, 0);
	return (count);
}

/**
 * Prints null pointer with right alignment
 */
int	ft_print_null_ptr_right(t_flags flags)
{
	int	count;

	count = 0;
	if (flags.width > 5)
		count += ft_print_width(flags.width, 5, 0);
	count += (int)write(1, "(nil)", 5);
	return (count);
}

/**
 * Prints pointer with left alignment
 */
int	ft_print_ptr_left(unsigned long n, t_flags flags, int ptr_len)
{
	int	count;

	count = 0;
	count += (int)ft_print_hex_prefix(0);
	count += ft_print_ptr_hex_precision(n, flags.precision);
	count += ft_print_width(flags.width, ptr_len, 0);
	return (count);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:50:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:26:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * This function formats and prints an unsigned integer `%u`while handlind
 * @param n The number to be printed
 * @param flags the format flags
 * @return the number of characaters to be printed
 * @note breakdown of the function:
 * @note case 1:
 */
int	ft_print_unsigned(unsigned int n, t_flags flags)
{
	int	count;
	int	len;

	count = 0;
	if ((flags.flags_bits & DOT_FLAG)
		&& (flags.precision == 0 || flags.precision == -1) && n == 0)
		return (ft_print_width(flags.width, 0, 0));
	len = ft_get_unsigned_len(n, flags);
	if (flags.flags_bits & LEFT_FLAG)
	{
		count += ft_print_unsigned_precision(n, flags.precision);
		count += ft_print_width(flags.width, len, 0);
	}
	else
	{
		if ((flags.flags_bits & ZERO_FLAG)
			&& !(flags.flags_bits & DOT_FLAG))
			count += ft_print_width(flags.width, len, 1);
		else
			count += ft_print_width(flags.width, len, 0);
		count += ft_print_unsigned_precision(n, flags.precision);
	}
	return (count);
}

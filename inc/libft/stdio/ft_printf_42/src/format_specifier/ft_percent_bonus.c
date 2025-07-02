/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_percent_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:23:25 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:27:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Handles printing percent sign with formatting
 * The percent sign is a special case that doesn't require a conversion.
 * @param flags The formatted flags
 * @return Number of characters printed
 * @note The functions is splitted into two parts, to keep the code clean 
 * @note The first part handles the left alignment case
 * @note The second part handles the right alignment case
 */
int	ft_print_percent(t_flags flags)
{
	int	count;

	count = 0;
	if (flags.flags_bits & LEFT_FLAG)
	{
		count += ft_print_c('%');
		count += ft_print_width(flags.width - 1, 0, 0);
	}
	else
	{
		if (flags.flags_bits & ZERO_FLAG)
			count += ft_print_width(flags.width - 1, 0, 1);
		else
			count += ft_print_width(flags.width - 1, 0, 0);
		count += ft_print_c('%');
	}
	return (count);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_format_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:26:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Prints prefix components - character and/or string
 */
t_ssize	ft_print_prefix(char prefix_char, char *prefix_str, int prefix_str_len)
{
	t_ssize	count;

	count = 0;
	if (prefix_char)
		count += ft_print_c(prefix_char);
	if (prefix_str)
		count += write(1, prefix_str, (size_t)prefix_str_len);
	return (count);
}

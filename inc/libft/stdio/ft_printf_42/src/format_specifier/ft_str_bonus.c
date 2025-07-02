/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:50:36 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:27:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"
/**
 * @brief Get the string to be printed and its length
 * @param str The string to be printed
 * @param flags The format flags
 * @return the string to be printed and its length
 * @note breakdown of the function:
 * @note case 1 : Precision is `.0` or `.`, so print only width spaces
 * @note case 2 : NULL string + precision < ft_strlen("(null)"), 
 * so print only width spaces
 * @note case 3 : Get the correct string to be printed
 */
int	ft_print_str(const char *str, t_flags flags)
{
	const char	*print_str;
	int			string_len;

	if ((flags.flags_bits & DOT_FLAG)
		&& (flags.precision == 0 || flags.precision == -1))
		return (ft_print_width(flags.width, 0, 0));
	if (!str && (flags.flags_bits & DOT_FLAG) && flags.precision < LEN_STR_NULL)
		return (ft_print_width(flags.width, 0, 0));
	get_print_info(str, flags, &print_str, &string_len);
	return (print_aligned_str(print_str, string_len, flags));
}

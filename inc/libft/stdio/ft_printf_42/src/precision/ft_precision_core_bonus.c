/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_precision_core_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:50:00 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:26:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Prints a string with precision limit
 * 
 * @param str String to print
 * @param precision Maximum characters to print (-1 means no limit)
 * @return Number of characters printed
 */
int	ft_print_strprec(const char *str, int precision)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (!str)
		str = "(null)";
	if (precision < 0)
		precision = (int)ft_strlen(str);
	while (str[i] && i < precision)
	{
		count += ft_print_c(str[i]);
		i++;
	}
	return (count);
}

/**
 * This function, `get_print_info`, takes in a string `str`,
 * a structure of flags `flasg`,
 * It is responsible for determining the information  needed
 * for printing the string, such as the string to print and its length
 * 
 * @param str the 
 * @param flags the flags structure
 * @param print_str the
 * @param print_len the length of the string to print
 * @note first we ensure the `print_str` pointer is set to a
 * @note we check if the `DOT_FLAG` is set in the flags structure and 
 * @note if the precision is typically used to specify the maximum number
 * of characters to be printed
 * @note  the code is ensuring that the number of characters to be printed 
 * does not exceed the specified precision
 * @note . If the condition is true, it means that the precision 
 * @note is less than the length of the string, and the code can proceed 
 * with printing only the specified number of characters. 
 * @note If the condition is false, it means that the precision is 
 * greater than or equal to the length of the string, 
 * @note and the code can print the entire string.
 * @return 
*/
void	get_print_info(const char *str, t_flags flags,
	const char **print_str, int *print_len)
{
	if (str)
		*print_str = str;
	else
		*print_str = "(null)";
	*print_len = (int)ft_strlen(*print_str);
	if ((flags.flags_bits & DOT_FLAG) && flags.precision < *print_len)
		*print_len = flags.precision;
}

/**
 * Calculates the length to use for unsigned numbers with precision
 */
int	ft_get_unsigned_len(unsigned int n, t_flags flags)
{
	int	len;

	len = ft_nbr_len(n, BASE_DEC);
	if ((flags.flags_bits & DOT_FLAG) && flags.precision == 0 && n == 0)
		return (0);
	if ((flags.flags_bits & DOT_FLAG) && flags.precision > len)
		len = flags.precision;
	return (len);
}

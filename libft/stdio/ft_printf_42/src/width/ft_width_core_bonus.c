/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_width_core_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:50:47 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:25:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Prints padding characters to achieve specified width
 * 
 * @param width Total width to fill
 * @param content_len Length of content already printed or to be printed
 * @param zero Flag to determine if padding should be '0' or ' '
 * @return Number of characters printed
 */
int	ft_print_width(int width, int content_len, int zero)
{
	int	count;

	count = 0;
	while (width > content_len)
	{
		if (zero)
			count += ft_print_c('0');
		else
			count += ft_print_c(' ');
		width--;
	}
	return (count);
}

/**
 * Prints a string with alignment and width handling.
 *
 * This function prints a given string while applying alignment and width rules
 * based on the provided flags. It supports left-aligned and right-aligned 
 * formatting.
 *
 * @param str The string to be printed.
 * @param len The length of the string to print.
 * @param flags The flags structure controlling formatting behavior.
 * @return The total number of characters printed.
 *
 * @note If the LEFT flag is set, the string is printed first, 
 * followed by space padding.
 * @note If the LEFT flag is not set, padding is applied first, 
 * followed by the string.
 * @note The function respects the specified width and 
 * ensures proper formatting.
 */
int	print_aligned_str(const char *str, int len, t_flags flags)
{
	int	count;

	count = 0;
	if (flags.flags_bits & LEFT_FLAG)
	{
		count += ft_print_strprec(str, len);
		count += ft_print_width(flags.width, len, 0);
	}
	else
	{
		count += ft_print_width(flags.width, len, 0);
		count += ft_print_strprec(str, len);
	}
	return (count);
}

/**
 * Prints a zero with width handling, considering left alignment 
 * and zero padding.
 *
 * This function prints a single '0' character while applying width 
 * and alignment rules
 * based on the provided flags. It handles left-alignment, 
 * zero-padding, and regular
 * space padding as necessary.
 *
 * @param flags The flags structure controlling formatting behavior.
 * @return The total number of characters printed.
 *
 * @note If the LEFT flag is set, the '0' is printed first, 
 * followed by space padding.
 * @note If the LEFT flag is not set, padding is applied first (zero or space), 
 * followed by '0'.
 * @note If the ZERO flag is set and LEFT is not set, padding is
 * done with zeros.
 * @note Otherwise, regular space padding is applied.
 */
int	ft_print_hex_zero_regular(t_flags flags)
{
	int	count;

	count = 0;
	if (flags.flags_bits & LEFT_FLAG)
	{
		count += ft_print_c('0');
		count += ft_print_width(flags.width - 1, 0, 0);
	}
	else
	{
		if (flags.flags_bits & ZERO_FLAG)
			count += ft_print_width(flags.width - 1, 0, 1);
		else
			count += ft_print_width(flags.width - 1, 0, 0);
		count += ft_print_c('0');
	}
	return (count);
}

/**
 * Handles width formatting, including negative width (left alignment)
 * 
 * @param width The specified width value
 * @param flags Pointer to the flags struct to update
 * @return void
 * @note The purpose of this function is to handle the width formatting 
 */
void	ft_handle_width(int width, t_flags *flags)
{
	if (width < 0)
	{
		flags->flags_bits |= LEFT_FLAG;
		flags->width = -width;
	}
	else
		flags->width = width;
}

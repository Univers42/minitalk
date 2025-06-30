/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag_wrappers_two_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:00:00 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:26:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Sets the plus flag in the given flags structure and clears the space flag.
 *
 * @param flags The flags structure to modify.
 * @param args  The variable argument list (unused in this function).
 */
void	handle_plus(t_flags *flags, va_list args)
{
	(void)args;
	set_flag_bit('+', flags);
	flags->flags_bits &= (unsigned int) ~SPACE_FLAG;
}

/**
 * Handles the '*' flag in the format specifier.
 *
 * This function sets the flag bit for the '*' flag in the given
 * `flags` structure and handles the width specifier
 * by retrieving the width value from the variable argument list `args`.
 *
 * @param flags The pointer to the `t_flags` structure.
 * @param args The variable argument list.
 */
void	handle_star(t_flags *flags, va_list args)
{
	set_flag_bit('*', flags);
	ft_handle_width(va_arg(args, int), flags);
}

/**
 * Sets the space flag in the given flags structure.
 *
 * This function is a handler for the space flag in the
 * ft_printf function. It sets the space flag bit in the flags structure,
 * indicating that a space should be added before positive signed conversions.
 *
 * @param flags The flags structure to modify.
 * @param args  The variable argument list (unused in this function).
 */
void	handle_space(t_flags *flags, va_list args)
{
	(void)args;
	set_flag_bit(' ', flags);
}

/**
 * Sets the flag bit for the '#' flag.
 *
 * This function is responsible for setting the flag bit for 
 * the '#' flag in the given `flags` structure.
 * The '#' flag is used to prefix the output value with 
 * the corresponding prefix (e.g., "0x" for hexadecimal values).
 *
 * @param flags The pointer to the `t_flags` structure that holds flag info
 * @param args  The `va_list` object containing the variable arguments.
 */
void	handle_hash(t_flags *flags, va_list args)
{
	(void)args;
	set_flag_bit('#', flags);
}

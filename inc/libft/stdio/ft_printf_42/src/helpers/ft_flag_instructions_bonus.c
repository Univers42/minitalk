/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag_instructions_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:00 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:26:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/* Helper for left-aligned zero with sign */
int	ft_handle_zero_left_sign(t_flags flags)
{
	int	count;

	count = 0;
	if (flags.flags_bits & PLUS_FLAG)
		count += ft_print_c('+');
	else
		count += ft_print_c(' ');
	count += ft_print_width(flags.width - 1, 0, 0);
	return (count);
}

/**
 * handles the printing of a sign (+ or space) and adjusts the width
 * for a numnber based on the provided flags, in a way that ensure the sign
 * is printed on the right side, following a zero-padding approach.
 * @param flags A structure containing the formatting flags
 * @return the number of bytes printed
 * ## Logic Flow
 * 
 * padding: The function ensure that the total width is respected by adjusting
 * the width `flags.width -1`using `ft_print_width`. the `-1`is to account
 * for the sign that will be printed separately.
 * Depending on whether the `PLUS_FLAG`is set, the function either prints a `+`
 * sign or a space using `ft_print_c`. 
 * 
 */
static int	ft_handle_zero_right_sign(t_flags flags)
{
	int	count;

	count = 0;
	count += ft_print_width(flags.width - 1, 0, 0);
	if (flags.flags_bits & PLUS_FLAG)
		count += ft_print_c('+');
	else
		count += ft_print_c(' ');
	return (count);
}

/**
 * Handles formatting for zero values with sign and width considerations.
 *
 * This function determines the appropriate formatting when printing 
 * the number zero, 
 * considering the presence of sign-related flags 
 * (`PLUS_FLAG` or `SPACE_FLAG`) and alignment flags.
 *
 * @param flags The formatting flags structure controlling 
 * how zero should be printed.
 * @param has_sign A boolean-like integer indicating if 
 * a sign should be considered.
 * @return The total number of characters printed.
 *
 * @note If a sign is required (`PLUS_FLAG` or `SPACE_FLAG` is set), 
 * the function
 *       determines whether to use left-aligned or right-aligned formatting.
 * @note If the `LEFT_FLAG` is set, the function calls 
 * `ft_handle_zero_left_sign` to
 *       handle left-aligned zero printing.
 * @note If the `LEFT_FLAG` is not set, 
 * `ft_handle_zero_right_sign` is used for
 *       right-aligned zero formatting.
 * @note If no sign is needed, the function simply prints width-padding spaces.
 */
int	ft_handle_common_zero(t_flags flags, int has_sign)
{
	int	sign_len;

	sign_len = 0;
	if (has_sign && ((flags.flags_bits & PLUS_FLAG)
			|| (flags.flags_bits & SPACE_FLAG)))
		sign_len = 1;
	if (sign_len > 0)
	{
		if (flags.flags_bits & LEFT_FLAG)
			return (ft_handle_zero_left_sign(flags));
		else
			return (ft_handle_zero_right_sign(flags));
	}
	return (ft_print_width(flags.width, 0, 0));
}

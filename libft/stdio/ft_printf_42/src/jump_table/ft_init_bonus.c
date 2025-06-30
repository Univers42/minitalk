/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 06:16:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:26:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * This is a function that initializes the lookup tables for the conversion
 * handlers. This function takes a pointer to a `t_lookup_tables`structure as 
 * a parameter. The convert table array is an array of function pointers of 
 * type `t_convert_func` . Each element in the `convert_table` array correspond
 * to a specific character that can be encountered in a format string. The
 * purpose of the `convert_table`is to map each character to the appropriate
 * conversion function. `%`, `c`, `s`, `d`. `i`,, `x`, `X`, `p`, and `u`.
 * The `funcs` array contains the corresponding covnersion functions for each 
 * character. 
 * @param tables a pointer to a `t_lookup_tables` structure
 * @return nothing
 * @note The `init_convert_table` function iterates over the `converters`
 * @note string and assigns the corresponding conversion function from the
 * `funcs` array to the `convert_table` array.
 * @note The character is cast to an `unsigned char` to ensure that it can
 * @note be used as an index in the array
 */
static void	init_convert_table(t_lookup_tables *tables)
{
	int				i;
	char			*converters;
	t_convert_func	funcs[9];

	converters = "%csdixXpu";
	funcs[0] = ft_convert_percent;
	funcs[1] = ft_convert_char;
	funcs[2] = ft_convert_str;
	funcs[3] = ft_convert_int;
	funcs[4] = ft_convert_int;
	funcs[5] = ft_convert_hex_lower;
	funcs[6] = ft_convert_hex_upper;
	funcs[7] = ft_convert_ptr;
	funcs[8] = ft_convert_unsigned;
	i = 0;
	while (converters[i])
	{
		tables->convert_table[(unsigned char)converters[i]] = funcs[i];
		i++;
	}
}

/** Overall the same purpose as `init_convert_table`
 * but with flag handers initialization. Overall the same
 */
static void	init_flag_table(t_lookup_tables *tables)
{
	int				i;
	char			*flags;
	t_flag_handler	funcs[8];

	flags = "-0.# +*";
	funcs[0] = handle_minus;
	funcs[1] = handle_zero;
	funcs[2] = handle_dot;
	funcs[3] = handle_hash;
	funcs[4] = handle_space;
	funcs[5] = handle_plus;
	funcs[6] = handle_star;
	funcs[7] = NULL;
	i = 0;
	while (flags[i])
	{
		tables->flag_table[(unsigned char)flags[i]] = funcs[i];
		i++;
	}
}

/**
 * Function to get lookup tables with lazy initialization
 * @return a pointer to a `t_lookup_tables` structure
 * @note This function is a wrapper for the tables that
 * @note we have initialized in the `init_*_table` functions
 * @note we use a design paradigm called `lazy initialization`
 * @note to ensure that the tables are only initialized once
 * @note and only when they are needed.
 */
t_lookup_tables	*get_tables(void)
{
	static t_lookup_tables	tables;
	static int				initialized = 0;

	if (initialized == 0)
	{
		ft_memset(&tables, 0, sizeof(t_lookup_tables));
		init_convert_table(&tables);
		init_flag_table(&tables);
		initialized = 1;
	}
	return (&tables);
}

/* Initialize the lookup tables - wrapper for compatibility */
void	ft_init_tables(void)
{
	get_tables();
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:39:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 12:59:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"

// Singleton accessor function

static void	init_specifier_table(t_parser_tables *tables)
{
	int	i;

	i = -1;
	while (++i < CHAR_TABLE_SIZE)
		tables->spec_table[i] = 0;
	tables->spec_table['s'] = spec_string;
	tables->spec_table['d'] = spec_decimal;
	tables->spec_table['i'] = spec_integer;
	tables->spec_table['u'] = spec_unsigned;
	tables->spec_table['x'] = spec_hex_lower;
	tables->spec_table['X'] = spec_hex_upper;
	tables->spec_table['o'] = spec_octal;
	tables->spec_table['c'] = spec_char;
	tables->spec_table['p'] = spec_pointer;
	tables->spec_table['f'] = spec_float;
	tables->spec_table['g'] = spec_float;
	tables->spec_table['G'] = spec_float;
}

static void	init_level_strings(t_parser_tables *tables)
{
	tables->level_strings[LOG_INFO] = "[INFO] ";
	tables->level_strings[LOG_WARNING] = "[WARNING] ";
	tables->level_strings[LOG_ERROR] = "[ERROR] ";
	tables->level_strings[LOG_DEBUG] = "[DEBUG] ";
	tables->level_strings[LOG_SUCCESS] = "[SUCCESS] ";
}

static void	init_flag_table(t_parser_tables *tables)
{
	int	i;

	i = -1;
	while (++i < CHAR_TABLE_SIZE)
		tables->flag_table[i] = 0;
	tables->flag_table['-'] = flag_left_align;
	tables->flag_table['+'] = flag_show_sign;
	tables->flag_table[' '] = flag_space_prefix;
	tables->flag_table['#'] = flag_alt_form;
	tables->flag_table['0'] = flag_zero_pad;
}

static void	init_state_table(t_parser_tables *tables)
{
	tables->state_transition_table[STATE_NORMAL] = action_normal;
	tables->state_transition_table[STATE_PERCENT] = action_percent;
	tables->state_transition_table[STATE_FLAGS] = action_flags;
	tables->state_transition_table[STATE_WIDTH] = action_width;
	tables->state_transition_table[STATE_PRECISION] = action_precision;
	tables->state_transition_table[STATE_LENGTH] = action_length;
	tables->state_transition_table[STATE_SPECIFIER] = action_specifier;
	tables->state_transition_table[STATE_ERROR] = action_error;
}

t_parser_tables	*get_parser_tables(void)
{
	static t_parser_tables	tables = {0};

	if (!tables.initialized)
	{
		tables.initialized = 1;
		init_char_table(&tables);
		init_state_table(&tables);
		init_flag_table(&tables);
		init_specifier_table(&tables);
		init_level_strings(&tables);
	}
	return (&tables);
}

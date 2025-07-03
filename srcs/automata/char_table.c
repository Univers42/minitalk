/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:52:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 02:08:09 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"

static void	init_digit_chars(t_parser_tables *tables)
{
	int	i;

	i = '0' - 1;
	while (++i <= '9')
		tables->char_class_table[i] |= CHAR_DIGIT;
}

static void	init_flag_chars(t_parser_tables *tables)
{
	tables->char_class_table['-'] |= CHAR_FLAG;
	tables->char_class_table['+'] |= CHAR_FLAG;
	tables->char_class_table[' '] |= CHAR_FLAG;
	tables->char_class_table['#'] |= CHAR_FLAG;
	tables->char_class_table['0'] |= CHAR_FLAG;
}

static void	init_spec_chars(t_parser_tables *tables)
{
	tables->char_class_table['s'] |= CHAR_SPEC;
	tables->char_class_table['d'] |= CHAR_SPEC;
	tables->char_class_table['i'] |= CHAR_SPEC;
	tables->char_class_table['u'] |= CHAR_SPEC;
	tables->char_class_table['x'] |= CHAR_SPEC;
	tables->char_class_table['X'] |= CHAR_SPEC;
	tables->char_class_table['o'] |= CHAR_SPEC;
	tables->char_class_table['c'] |= CHAR_SPEC;
	tables->char_class_table['p'] |= CHAR_SPEC;
	tables->char_class_table['f'] |= CHAR_SPEC;
	tables->char_class_table['g'] |= CHAR_SPEC;
	tables->char_class_table['G'] |= CHAR_SPEC;
}

static void	init_other_chars(t_parser_tables *tables)
{
	tables->char_class_table['l'] |= CHAR_LENGTH;
	tables->char_class_table['h'] |= CHAR_LENGTH;
	tables->char_class_table['L'] |= CHAR_LENGTH;
	tables->char_class_table['.'] |= CHAR_DOT;
	tables->char_class_table['%'] |= CHAR_PERCENT;
}

void	init_char_table(t_parser_tables *tables)
{
	int	i;

	i = -1;
	while (++i < CHAR_TABLE_SIZE)
		tables->char_class_table[i] = 0;
	init_digit_chars(tables);
	init_flag_chars(tables);
	init_spec_chars(tables);
	init_other_chars(tables);
}

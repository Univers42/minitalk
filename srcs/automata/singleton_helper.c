/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:43:02 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 01:59:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"

void	init_class(t_parser_tables *tables)
{
	tables->char_class_table['-'] |= CHAR_FLAG;
	tables->char_class_table['+'] |= CHAR_FLAG;
	tables->char_class_table[' '] |= CHAR_FLAG;
	tables->char_class_table['#'] |= CHAR_FLAG;
	tables->char_class_table['0'] |= CHAR_FLAG;
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
	tables->char_class_table['l'] |= CHAR_LENGTH;
	tables->char_class_table['h'] |= CHAR_LENGTH;
	tables->char_class_table['L'] |= CHAR_LENGTH;
	tables->char_class_table['.'] |= CHAR_DOT;
	tables->char_class_table['%'] |= CHAR_PERCENT;
}

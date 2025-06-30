/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_table.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/04 14:28:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_render.h"

static void	print_table_row(char **row, int cols)
{
	int	i;

	printf("│");
	i = 0;
	while (i < cols)
	{
		if (row && row[i])
			printf(" %-12s ", row[i]);
		else
			printf(" %-12s ", "");
		printf("│");
		i++;
	}
	printf("\n");
}

static void	print_table_data(char ***data, int rows, int cols)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		if (data[i])
			print_table_row(data[i], cols);
		i++;
	}
}

void	ft_print_table(char **headers, char ***data, int rows, int cols)
{
	if (!headers || !data || cols <= 0 || rows < 0)
		return ;
	print_table_border(cols, "┌", "─");
	print_table_row(headers, cols);
	print_table_border(cols, "├", "─");
	print_table_data(data, rows, cols);
	print_table_border(cols, "└", "─");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_table_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:10:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/04 14:22:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_render.h"

static void	print_border_char(char *corner)
{
	if (strcmp(corner, "┌") == 0)
		printf("┐\n");
	else if (strcmp(corner, "├") == 0)
		printf("┤\n");
	else
		printf("┘\n");
}

static void	print_separator_char(char *corner)
{
	if (strcmp(corner, "┌") == 0)
		printf("┬");
	else if (strcmp(corner, "├") == 0)
		printf("┼");
	else
		printf("┴");
}

void	print_table_border(int cols, char *corner, char *line)
{
	int	i;
	int	j;

	printf("%s", corner);
	i = 0;
	while (i < cols)
	{
		j = 0;
		while (j < 14)
		{
			printf("%s", line);
			j++;
		}
		if (i < cols - 1)
			print_separator_char(corner);
		i++;
	}
	print_border_char(corner);
}

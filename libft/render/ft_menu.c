/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_menu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:05:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/04 14:20:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_render.h"
#include "../stdio/ft_stdio.h"

void	ft_print_menu(char **options, int count, int selected)
{
	int	i;

	i = 0;
	printf("\n┌─ SELECT OPTION ─┐\n");
	while (i < count)
	{
		if (i == selected)
			printf("│ ► %-12s │\n", options[i]);
		else
			printf("│   %-12s │\n", options[i]);
		i++;
	}
	printf("└─────────────────┘\n");
}

static void	print_timeline_item(char *time, char *event, int is_last)
{
	if (is_last)
		ft_printf("%s└─ %s%s %s%s\n", CYAN, time, RESET, event, RESET);
	else
	{
		ft_printf("%s├─ %s%s %s%s\n", CYAN, time, RESET, event, RESET);
		ft_printf("%s│%s\n", CYAN, RESET);
	}
}

void	ft_print_timeline(char **events, char **times, int count)
{
	int	i;

	i = 0;
	ft_printf("\n%s── TIMELINE ──%s\n", CYAN, RESET);
	while (i < count)
	{
		print_timeline_item(times[i], events[i], (i == count - 1));
		i++;
	}
}

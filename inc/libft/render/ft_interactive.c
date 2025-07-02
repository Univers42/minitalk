/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interactive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/04 17:40:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_render.h"
#include "../stdio/ft_stdio.h"

void	ft_print_menu(char **options, int count, int selected)
{
	int	i;

	i = 0;
	ft_printf("\n%s┌─ SELECT OPTION ─┐%s\n", CYAN, RESET);
	while (i < count)
	{
		if (i == selected)
			ft_printf("%s│ ► %-12s │%s\n", GREEN, options[i], RESET);
		else
			ft_printf("%s│   %-12s │%s\n", WHITE, options[i], RESET);
		i++;
	}
	ft_printf("%s└─────────────────┘%s\n", CYAN, RESET);
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

static void	print_graph_row(int *values, int count, t_graph_params params)
{
	int	i;
	int	bar_height;

	i = 0;
	while (i < count)
	{
		bar_height = (values[i] * params.max_height) / params.max_val;
		if (bar_height >= params.row)
			ft_printf("%s█%s", GREEN, RESET);
		else
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
}

void	ft_print_graph(int *values, int count, int max_height)
{
	t_graph_params	params;
	int				i;

	params.max_val = 0;
	i = 0;
	while (i < count && values[i] > params.max_val)
		params.max_val = values[i++];
	params.row = max_height;
	while (params.row > 0)
	{
		print_graph_row(values, count, params);
		params.row--;
	}
	i = 0;
	while (i < count)
	{
		ft_printf("%d", values[i] % 10);
		i++;
	}
	ft_printf("\n");
}

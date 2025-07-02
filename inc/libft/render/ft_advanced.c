/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_advanced.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/04 13:56:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_render.h"

void	ft_print_notification(const char *title, const char *message,
			const char *type)
{
	const char	*color;

	if (!title || !message || !type)
		return ;
	color = GREEN;
	if (strcmp(type, "error") == 0)
		color = RED;
	else if (strcmp(type, "warning") == 0)
		color = YELLOW;
	printf("%s[%s] %s: %s%s\n", color, type, title, message, RESET);
}

void	ft_print_code_block(const char *code, const char *language)
{
	const char	*lang_name;

	if (!code)
		return ;
	lang_name = "Code";
	if (language)
		lang_name = language;
	printf("%s--- %s ---%s\n", CYAN, lang_name, RESET);
	printf("%s%s%s\n", DIM, code, RESET);
	printf("%s----------%s\n", CYAN, RESET);
}

void	ft_print_diff(const char *old_line, const char *new_line)
{
	if (!old_line || !new_line)
		return ;
	printf("%s- %s%s\n", RED, old_line, RESET);
	printf("%s+ %s%s\n", GREEN, new_line, RESET);
}

void	ft_print_emoji_status(const char *message, const char *emoji)
{
	if (!message || !emoji)
		return ;
	printf("%s %s%s %s%s\n", emoji, GREEN, message, RESET, emoji);
}

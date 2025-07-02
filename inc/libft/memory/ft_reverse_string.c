/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reverse_string.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:43:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/28 17:17:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../strings/ft_string.h"
#include "ft_memory.h"

void	ft_reverse_string(char *str)
{
	char	*start;
	char	*end;

	start = str;
	end = str + ft_strlen(str) - 1;
	if (!str || !*str)
		return ;
	while (start < end)
		ft_swap(start++, end--, sizeof(char));
}

//int main(void)
//{
//	char str1[] = "Hello, World!";
//	char str2[] = "42";
//	char str3[] = "";
//
//	printf("Original: \"%s\"\n", str1);
//	ft_reverse_string(str1);
//	printf("Reversed: \"%s\"\n\n", str1);
//
//	printf("Original: \"%s\"\n", str2);
//	ft_reverse_string(str2);
//	printf("Reversed: \"%s\"\n\n", str2);
//
//	printf("Original: \"%s\"\n", str3);
//	ft_reverse_string(str3);
//	printf("Reversed: \"%s\"\n\n", str3);
//
//	return 0;
//}
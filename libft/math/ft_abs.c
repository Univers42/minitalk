/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_abs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:31:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/04 13:50:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_math.h"

int	ft_abs(int x)
{
	int	mask;

	mask = x >> (sizeof(int) * 8 - 1);
	return (ft_add(x ^ mask, -mask));
}

//int main(int argc, char **argv)
//{
//	if (argc != 2)
//		return 1;
//	int x = atoi(argv[1]);
//	printf("abs(%d) = %d\n", x, ft_abs(x));
//	return 0;
//}
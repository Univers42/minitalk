/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:22:30 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/28 23:23:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int	ft_sqrt(int n)
{
	int	res;
	int	bit;
	int	neg;
	int	t;
	int	cond;

	res = 0;
	bit = 1 << 30;
	neg = n >> 31;
	n = n & ~neg;
	while (bit > n)
		bit >>= 2;
	while (bit != 0)
	{
		t = res + bit;
		cond = (n >= t);
		n -= t & -cond;
		res = ((res >> 1) + (bit & -cond)) | ((res >> 1) & ~(-cond));
		bit >>= 2;
	}
	return (res);
}

//int main(int argc, char **argv)
//{
//	if (argc != 2)
//		return 1;
//	int n = atoi(argv[1]);
//	printf("sqrt(%d) = %d\n", n, ft_sqrt(n));
//	return 0;
//}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:19:08 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 03:49:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * ?WIP 
 * let's trace through the function with the string 'ABC'
 * to understand how this work
 * `data = "ABC"` --> length = 3
 * checksum = 0 (32-bit integer : 0x00000000)
 * checksum = 0x00000000 = 00000000 00000000 00000000 00000000
 * data[0]  = 0x41       = 00000000 00000000 00000000 01000001

 * checksum ^= data[0] = 0x00000041 or (00000000 00000000 00000000 01000001
 checksum = (checksum << 1) | (checksum >> 31)
 left shift: 0x00000041 << 1 = 0x00000080 = 
									000000000 000000000 000000000 010000001
right shift: 0x00000041 >> 31 = 0x00000000

OR result : 0x00000082 | 0x00000000 = 0x000000082
B=0x00000180
C=0x00000386
 *  */
int	calculate_checksum(const char *data, int length)
{
	int	checksum;
	int	i;

	checksum = 0;
	i = 0;
	while (i < length)
	{
		checksum ^= data[i];
		checksum = (checksum << 1) | (checksum >> 31);
		i++;
	}
	return (checksum);
}

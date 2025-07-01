/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:56:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:47:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * need to distinguish between client and server because the data
 * will be stored and different, accross calls.
 */
t_client	*get_instance(t_client *set)
{
	static t_client	*client = NULL;

	if (set)
		client = set;
	return (client);
}

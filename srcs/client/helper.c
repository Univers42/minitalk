/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:03:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 15:09:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
singleton pattern is used to avoid pollute the codebase with global variables
that are unsafe due to their scope that every function can set.
leading to unsafe datas and potential corruptions. With the singleton,
we give on
 */
t_client *get_client(t_client *set)
{
    t_client *client = set;
    if (client)
        return (client);
    return (client);        
}

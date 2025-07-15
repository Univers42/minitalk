/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:16:35 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 03:53:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * check if the server pid is not from another source
 * in case someone by pass the system. just verification in case of
 * corruption trial
 */
int	validate_signal_source(t_server_state *server, pid_t sender_pid)
{
	if (server->pid != 0 && sender_pid != server->pid)
	{
		log_msg(LOG_WARNING, "Ignoring signal from unexpected PID: %d",
			sender_pid);
		return (0);
	}
	return (1);
}

/**
 * check if the process exist
 */
int	validate_process_exists(int pid)
{
	if (kill(pid, 0) == -1)
	{
		log_msg(LOG_ERROR, "Process PID %d is not accessible", pid);
		ft_printf("Error: Server process PID %d not found\n", pid);
		return (0);
	}
	return (1);
}

int	validate_ping_signal(t_server_state *server, siginfo_t *info)
{
	pid_t	my_pid;

	my_pid = getpid();
	if (info->si_pid == my_pid)
	{
		log_msg(LOG_ERROR, "Received signal from own process");
		exit(EXIT_FAILURE);
	}
	if (server->pid != 0 && info->si_pid != server->pid)
	{
		log_msg(LOG_WARNING, "Ignoring unexpected PID: %d (expected %d)",
			info->si_pid, server->pid);
		return (0);
	}
	if (server->transmission_active)
	{
		log_msg(LOG_DEBUG, "Ignoring ping during active transmission");
		return (0);
	}
	return (1);
}

/**
	we check the type of the PID, verify it it can be hooked
	then we convert it to the idenfication usable to our program
	in case of error use the state machine created
	and if all good just init
*/
void	validate_and_init(int argc, char **argv, t_client *data)
{
	t_parser_result	parse_result;

	parse_result = parse_arguments(argc, argv);
	if (parse_result != PARSER_SUCCESS)
		print_error_and_exit(parse_result);
	init_data(argv, data);
}

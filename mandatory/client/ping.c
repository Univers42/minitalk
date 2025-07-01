/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:48:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:48:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * Quick process existence check
 */
inline int ping_conn(pid_t pid)
{
    return (kill(pid, 0) == 0);
}

/**
 * Signal handler for server responses
 */
void handshake_protocol(int signum, siginfo_t *info, void *context)
{
    t_client *client = get_instance(NULL);
    (void)context;
    
    if (!client || info->si_pid != client->server_pid)
        return;
    
    client->server_ready = (signum == SERVER_READY);
}

/**
 * Wait for server response with timeout
 */
static int wait_server_response(int timeout_ms)
{
    t_client *client = get_instance(NULL);
    int elapsed = 0;
    const int interval = 10000; // 10ms
    
    while (elapsed < timeout_ms * 1000 && client)
    {
        if (client->server_ready)
            return (1);
        usleep(interval);
        elapsed += interval;
    }
    return (0);
}

/**
 * Handle connection with retries
 */
int handle_timeouts(pid_t pid)
{
    t_client *client = get_instance(NULL);
    int attempt = 0;
    
    if (!client || !ping_conn(pid))
        return (log_msg(LOG_ERROR, "Server unreachable"), 1);
    
    while (++attempt <= RETRIES)
    {
        client->server_ready = 0;
        
        if (kill(pid, SIGUSR1) == -1)
            return (log_msg(LOG_ERROR, "Signal send failed"), 1);
        
        log_msg(LOG_INFO, "Attempt %d: Waiting server response", attempt);
        
        if (wait_server_response(RETRY_TIMEOUT))
            return (log_msg(LOG_INFO, "Connected"), 0);
    }
    
    return (log_msg(LOG_ERROR, "Connection failed after %d attempts", RETRIES), 1);
}
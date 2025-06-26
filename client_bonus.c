/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:46:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 12:14:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_bonus.h"
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

t_bclient *get_client_instance(t_bclient *client);
int initialization(t_bclient *client, int argc, char **argv);
void handle_sig(int signal, siginfo_t *info, void *context);
void cypher_send(t_bclient *client);
/**
 * UNICODE AWARENESS
 * the previous implementation was using ASCII encoding,
 * provoking issues like that Sent SIGUSR1(10) for bit 7 of character '�'
 * This implementation will be UTF-8 aware, allowing to send any character
 * in the UTF-8 encoding. To be fully compliant with the UTF-8 encoding,
 * the client will need to handle multi-byte characters not char-by-char.
 * HANDSHAKE PROTOCOL
 * after the server receives each bit (or byte) it should send a signal
 * back to the client to confirm receipt of the signal.
 * The client should wait for this acknowledgement before sending the next bit.
 * This prevent data racing adn ensure synchronization
 * VAR_LENGTH CACHED PATTERN
 * Instead of sending on bit a a time and waiting for an acknoweledgement,
 * we can sebd a batch of bits and wait for an acknowledgement
 * this can speed up the communication, but we must ensure both client and
 * server are aware of the batch size.
 * 
 * ANY LENGTH MESSAGE
 */
int main(int argc, char **argv)
{
	t_bclient client;
	t_sigaction sa;

	if (!initialization(&client, argc, argv))
		return (ft_printf("the initialization has failed\n"), 1);
	get_client_instance(&client);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handle_sig;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		ft_printf("the signal is not found\n");
		return (1);
	}
	else if (sigaction(SIGUSR2, &sa, NULL))
	{
		ft_printf("The signal is not found \n");
		return (1);
	}
	cypher_send(&client);
	return (0);
}

char *read_stdin_dynamic(void)
{
    size_t cap = 4096;
    size_t len = 0;
    char *buf = malloc(cap);
    int c;
    if (!buf)
        return NULL;
    while ((c = getchar()) != EOF)
    {
        if (len + 2 > cap)
        {
            char *new_buf = ft_realloc(buf, cap, cap * 2);
            if (!new_buf)
            {
                free(buf);
                return NULL;
            }
            buf = new_buf;
            cap *= 2;
        }
        buf[len++] = (char)c;
    }
    buf[len] = '\0';
    return buf;
}

t_bclient *get_client_instance(t_bclient *set)
{
	static	t_bclient *client;

	if (set)
		client = set;
	return (client);
}

int initialization(t_bclient *client, int argc, char **argv)
{
    ft_memset(client, 0, sizeof(t_bclient));
    if (argc != 3)
    {
        ft_printf("./usage %s <srv_pid> <msg> !\n", *argv);
        return (0);
    }
    client->legacy_client.server_pid = ft_atoi(argv[1]);
    client->legacy_client.message = argv[2];
	return (1);
}

/**
 * unicode conversion
 * A codepoint is a unique number assigned to each character in teh unicode
 * standard. It represents a single Unicode character which may be encoded as 
 * more bytes in UTF-8
 * @param dst
 * @param codepoint unique number assigned to character
 * @note in C a codepoint is typically represented as a uint32_t 32 bit
 * @note UTF-8 ecnoding tunrs a codepoint into a 1-4 bytes as they are
 * @note if the message is already UTF-8, we can send the bytes directly.
 * @example 'hi'
 * 'h' -> Unicdode point: U+0068
 * 'i' -> Unicode point : U+0069
 * ```c
 * char buf[5];
 * size_t len = 0;
 * len += utf8_encode(buf + len, 'h');
 * len += utf8_encode(buf + len, 'i');
 * buf[len] ='\0';
 * ```
 */

void handle_sig(int signal, siginfo_t *info, void *context)
{
	t_bclient *client;
	(void)info;
	(void)context;

	client = get_client_instance(NULL);
	if (signal == SIGUSR1)
		client->ack = 1;
	else if (signal == SIGUSR2)
		ft_printf("signal received %d", SIGUSR2);
}

void handshake(t_bclient *client)
{
	(void)client;
	while (!client->ack)
		usleep(100);
	client->ack = 0;
}

/**
 * send a batch of bytes at a time then waits for acknowledgement
 * For each byte, send 8 bits (MSB first), then wait for ACK from server
 */
void	cypher_send(t_bclient *client)
{
	const char *msg = client->legacy_client.message;
	int pid = client->legacy_client.server_pid;
	int i, bit;
	unsigned char c;

	while (*msg)
	{
		c = (unsigned char)*msg;
		for (i = 0; i < 8; ++i)
		{
			bit = (c >> i) & 1;
			ft_printf("[CLIENT] Sending bit %d of char %c (byte %d, bit %d): %d\n", i, c, (int)(msg - client->legacy_client.message), i, bit);
			if (bit)
				kill(pid, SIGUSR1);
			else
				kill(pid, SIGUSR2);
			usleep(100);
			ft_printf("[CLIENT] Waiting for ACK after bit %d of byte %d\n", i, (int)(msg - client->legacy_client.message));
			handshake(client);
			ft_printf("[CLIENT] ACK received, sending next bit\n");
		}
		msg++;
	}
	// Send null terminator
	c = 0;
	for (i = 0; i < 8; ++i)
	{
		bit = (c >> i) & 1;
		ft_printf("[CLIENT] Sending bit %d of null terminator (byte END, bit %d): %d\n", i, i, bit);
		if (bit)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(100);
		ft_printf("[CLIENT] Waiting for ACK after bit %d of null terminator\n", i);
		handshake(client);
		ft_printf("[CLIENT] ACK received, sending next bit\n");
	}
}
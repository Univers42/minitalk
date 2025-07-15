/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_message.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:24:27 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 04:08:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

/**
 * @brief Finalizes message reception and cleans up resources
 * 
 * This function handles the completion of message reception by:
 * 1. Null-terminating the received message string
 * 2. Logging completion statistics
 * 3. Displaying the received message
 * 4. Cleaning up allocated memory
 * 5. Notifying the client of successful reception
 * 6. Resetting server state for new connections
 * 
 * @param client Pointer to the client state structure
 */
void	handle_complete_message(t_client_state *client)
{
	pid_t	client_pid;

	client->msg.message[client->msg_pos] = '\0';
	log_msg(LOG_SUCCESS, "Message reception complete:"
		"%d characters received", client->msg_pos);
	print_message(client);
	client_pid = client->client_pid;
	if (client->msg.message)
	{
		free(client->msg.message);
		client->msg.message = NULL;
	}
	send_completion_signal(client_pid);
	reset_client_state(client);
	log_msg(LOG_INFO, "Server released, ready for new connections");
}

/**
 * @brief Processes individual bits to reconstruct characters using bit manipulation
 * 
 * Mathematical Logic - Bit Position Calculation:
 * - Uses MSB-first (Most Significant Bit first) encoding
 * - Bit position mapping: 7, 6, 5, 4, 3, 2, 1, 0 (left to right)
 * - Left shift operation: bit_value << (7 - bit_pos)
 * - Bitwise OR accumulation: char_value |= shifted_bit
 * 
 * Mathematical Examples:
 * 
 * Example 1: Building character 'A' (ASCII 65 = 01000001₂)
 * - bit_pos=0: bit_value=0, shift=(7-0)=7, result=0<<7=0, char_value=0|0=0
 * - bit_pos=1: bit_value=1, shift=(7-1)=6, result=1<<6=64, char_value=0|64=64
 * - bit_pos=2: bit_value=0, shift=(7-2)=5, result=0<<5=0, char_value=64|0=64
 * - bit_pos=3: bit_value=0, shift=(7-3)=4, result=0<<4=0, char_value=64|0=64
 * - bit_pos=4: bit_value=0, shift=(7-4)=3, result=0<<3=0, char_value=64|0=64
 * - bit_pos=5: bit_value=0, shift=(7-5)=2, result=0<<2=0, char_value=64|0=64
 * - bit_pos=6: bit_value=0, shift=(7-6)=1, result=0<<1=0, char_value=64|0=64
 * - bit_pos=7: bit_value=1, shift=(7-7)=0, result=1<<0=1, char_value=64|1=65
 * - Final: char_value = 65 = 'A'
 * 
 * Example 2: Building character 'B' (ASCII 66 = 01000010₂)
 * - Bits: 0,1,0,0,0,0,1,0
 * - bit_pos=1: 1<<6=64, char_value=64
 * - bit_pos=6: 1<<1=2, char_value=64|2=66
 * - Final: char_value = 66 = 'B'
 * 
 * Modulo Logic for Character Completion:
 * - sig_count % 8 == 0: detects when 8 bits (1 byte) are received
 * - Triggers character storage and position increment
 * 
 * @param client Pointer to the client state structure
 * @param bit_value The bit value (0 or 1) to process
 * @param bit_pos The bit position within the current character (0-7)
 */
void	process_character(t_client_state *client, int bit_value, int bit_pos)
{
	client->char_value |= (bit_value << (7 - bit_pos));
	client->sig_count++;
	log_msg(LOG_DEBUG, "Bit %d/8: %d (char value: %d)",
		bit_pos + 1, bit_value, client->char_value);
	if (client->sig_count % 8 == 0)
	{
		client->msg.message[client->msg_pos] = client->char_value;
		log_msg(LOG_DEBUG, "Character complete: '%c'"
			" (ASCII: %d) at position %d/%d",
			client->char_value, client->char_value, client->msg_pos + 1,
			client->msg.size_message);
		client->msg_pos++;
		client->char_value = 0;
		if (client->msg_pos >= client->msg.size_message)
			handle_complete_message(client);
	}
}

/**
 * @brief Processes incoming signal bits to reconstruct message characters
 * 
 * Mathematical Logic - Bit Assembly:
 * - Bit position calculation: sig_count % 8 (determines position within byte)
 * - MSB-first bit ordering: positions 7,6,5,4,3,2,1,0 (high to low significance)
 * - Conditional bit setting: only sets bit if bit_value == 1
 * - Left shift for positioning: 1 << (7 - bit_pos)
 * - Bitwise OR for accumulation: char_value |= shifted_bit
 * 
 * Mathematical Examples:
 * 
 * Example 1: Receiving 'C' (ASCII 67 = 01000011₂)
 * Bit sequence: 0,1,0,0,0,0,1,1
 * 
 * - sig_count=0: bit_pos=0%8=0, bit_value=0, no bit set, char_value=0
 * - sig_count=1: bit_pos=1%8=1, bit_value=1, set bit at 7-1=6, char_value=0|(1<<6)=64
 * - sig_count=2: bit_pos=2%8=2, bit_value=0, no bit set, char_value=64
 * - sig_count=3: bit_pos=3%8=3, bit_value=0, no bit set, char_value=64
 * - sig_count=4: bit_pos=4%8=4, bit_value=0, no bit set, char_value=64
 * - sig_count=5: bit_pos=5%8=5, bit_value=0, no bit set, char_value=64
 * - sig_count=6: bit_pos=6%8=6, bit_value=1, set bit at 7-6=1, char_value=64|(1<<1)=66
 * - sig_count=7: bit_pos=7%8=7, bit_value=1, set bit at 7-7=0, char_value=66|(1<<0)=67
 * - sig_count=8: 8%8=0, character complete, store 'C' (67)
 * 
 * Bit Position Mapping (MSB-first):
 * | sig_count % 8 | Bit Position | Left Shift | Bit Value |
 * |---------------|--------------|-------------|-----------|
 * | 0             | 7            | 1 << 7 = 128| MSB       |
 * | 1             | 6            | 1 << 6 = 64 |           |
 * | 2             | 5            | 1 << 5 = 32 |           |
 * | 3             | 4            | 1 << 4 = 16 |           |
 * | 4             | 3            | 1 << 3 = 8  |           |
 * | 5             | 2            | 1 << 2 = 4  |           |
 * | 6             | 1            | 1 << 1 = 2  |           |
 * | 7             | 0            | 1 << 0 = 1  | LSB       |
 * 
 * Character Completion Logic:
 * - Modulo check: sig_count % 8 == 0 (every 8th signal)
 * - Triggers character storage and position advancement
 * - Resets char_value for next character
 * 
 * @param signum The signal number (SIGUSR1 or SIGUSR2)
 */
void	handle_msg(int signum)
{
	t_client_state	*client;
	const int		bit_value = get_bit_value(signum);
	int				bit_pos;

	client = get_client_instance();
	bit_pos = client->sig_count % 8;
	if (bit_pos == 0)
	{
		client->char_value = 0;
		log_msg(LOG_DEBUG, "Starting new character %d/%d",
			client->msg_pos + 1, client->msg.size_message);
	}
	if (bit_value == 1)
		client->char_value |= (1 << (7 - bit_pos));
	client->sig_count++;
	log_msg(LOG_DEBUG, "Bit %d/8: %d (bit_pos: %d, char value: %d)",
		bit_pos + 1, bit_value, 7 - bit_pos, client->char_value);
	if (client->sig_count % 8 == 0)
	{
		client->msg.message[client->msg_pos] = client->char_value;
		log_character_completion(client);
		client->msg_pos++;
		if (client->msg_pos >= client->msg.size_message)
			handle_complete_message(client);
	}
}

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/02 19:00:00 by dlesieur          #+#    #+#              #
#    Updated: 2025/07/15 00:09:42 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_SERVER = server
NAME_CLIENT = client

FLAG_DEBUG = 1
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./inc -I./inc/libft
CFLAGS += -DMINITALK_DEBUG=$(FLAG_DEBUG)
RM = rm -f

# Directories
LIBFT_DIR = inc/libft
SERVER_DIR = srcs/server
CLIENT_DIR = srcs/client
AUTOMATA_DIR = srcs/automata

# Source files
SERVER_SRCS = $(SERVER_DIR)/main.c $(SERVER_DIR)/server.c $(SERVER_DIR)/signals.c $(SERVER_DIR)/server_loop.c $(SERVER_DIR)/server_message.c \
				$(SERVER_DIR)/crook.c $(SERVER_DIR)/debug.c $(SERVER_DIR)/inits.c $(SERVER_DIR)/time.c $(SERVER_DIR)/check.c

CLIENT_SRCS = 	$(CLIENT_DIR)/main.c $(CLIENT_DIR)/bits.c $(CLIENT_DIR)/ping.c $(CLIENT_DIR)/parser.c $(CLIENT_DIR)/singleton.c $(CLIENT_DIR)/ping_utils.c \
				$(CLIENT_DIR)/time.c $(CLIENT_DIR)/inits.c $(CLIENT_DIR)/hook.c $(CLIENT_DIR)/handshake.c $(CLIENT_DIR)/debug.c $(CLIENT_DIR)/checksum.c $(CLIENT_DIR)/check.c \
				$(CLIENT_DIR)/validate.c

AUTOMATA_SRCS = $(AUTOMATA_DIR)/singleton.c $(AUTOMATA_DIR)/log.c $(AUTOMATA_DIR)/format_state.c \
				$(AUTOMATA_DIR)/spec.c $(AUTOMATA_DIR)/conversion.c $(AUTOMATA_DIR)/bufferization.c \
				$(AUTOMATA_DIR)/verify.c $(AUTOMATA_DIR)/format_helper.c $(AUTOMATA_DIR)/action.c \
				$(AUTOMATA_DIR)/action_extra.c $(AUTOMATA_DIR)/char_table.c  $(AUTOMATA_DIR)/utils.c \
				$(AUTOMATA_DIR)/spec2.c $(AUTOMATA_DIR)/singleton_helper.c $(AUTOMATA_DIR)/action_more.c

# Object files
SERVER_OBJS = $(SERVER_SRCS:.c=.o) $(AUTOMATA_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o) $(AUTOMATA_SRCS:.c=.o)

# Default rule
all: $(NAME_SERVER) $(NAME_CLIENT)

# Build libft
$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

# Build server
$(NAME_SERVER): $(LIBFT_DIR)/libft.a $(SERVER_OBJS) 
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJS) -L$(LIBFT_DIR) -lft

# Build client
$(NAME_CLIENT): $(LIBFT_DIR)/libft.a $(CLIENT_OBJS) 
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJS) -L$(LIBFT_DIR) -lft

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	$(RM) $(SERVER_OBJS) $(CLIENT_OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME_SERVER) $(NAME_CLIENT)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
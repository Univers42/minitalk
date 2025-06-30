# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 17:17:44 by dlesieur          #+#    #+#              #
#    Updated: 2025/06/30 17:55:28 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#=======================================#
#SETTABLES								#
#=======================================#
# Project
PROG_NAME= server client
PROJECT_DIR=.

# Directories
DIR_MANDATORY=$(PROJECT_DIR)/mandatory
DIR_BONUS=$(PROJECT_DIR)/bonus
LIBFT_DIR := libft
LIBFT_MAKE := $(MAKE) -C $(LIBFT_DIR)

# Sources
SRCS=$(DIR_MANDATORY)/client.c $(DIR_MANDATORY)/server.c $(DIR_MANDATORY)/utils.c
SRCS_BONUS=$(DIR_BONUS)/client_bonus.c $(DIR_BONUS/)/server_bonus.c $(DIR_BONUS)/utils_bonus.c

# implicit rules with conversion (convert .c -> .o)
OBJS=$(SRCS:.c=.o)
OBJS_BONUS=$(SRCS_BONUS:.c=.o)

ifdef BONUS
	OBJS_ACTUAL := $(OBJS_BONUS)
else
	OBJS_ACTUAL := $(OBJS)
endif

AR: ar rcs
RM: rm -rf

NAME=minitalk.a

#using simple assignement to immediate use
define LIBFT_DO
	$LIBFT_MAKE $(1)
endef

# Default target
all: $(NAME)

# Bonus mode
bonus: BONUS=1
bonus: all

# Rule to build the static lib
$(NAME): $(OBJS_ACTUAL)
	$(AR) $(NAME) $(OBJS_ACTUAL)

# Link targets for programs (server/client)
$(PROG_NAME): %: %.o $(NAME)
	$(CC) -o $@ $^ -L. -lft

build:
	$(LIBFT_MAKE)

clean: 
	$(RM) $(OBJS) $(OBJS_BONUS)
	$(call LIBFT_DO, fclean)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(PROG_NAME)
	$(call LIBFT_DO, fclean)

re: fclean all

.PHONY: all bonus build clean fclean re $(PROG_NAME)
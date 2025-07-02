# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 12:30:42 by dlesieur          #+#    #+#              #
#    Updated: 2025/06/15 18:11:31 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Static MAKEFILE includes
include build/colors.mk
include build/common.mk

# Library name
NAME = libft.a

# Directories
DATA_STRUCTURES = 	data_structures/queue data_structures/circular_linked_list data_structures/doubly_linked_list\
					data_structures/lists data_structures/vector
SRC_DIRS = ctype debug $(DATA_STRUCTURES) math memory render stdio stdlib strings sort

# Source files from all directories including ft_printf and gnl
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
PRINTF_SRCS = $(shell find stdio/ft_printf_42/src -name "*.c" 2>/dev/null)
GNL_SRCS = $(wildcard stdio/gnl/*.c)
ALL_SRCS = $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS)

# Object files with directory structure preserved
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(ALL_SRCS))

# Header files
HEADERS = $(wildcard *.h)

# Default rule: build the library
all: $(NAME)

# Rule to build the library
$(NAME): $(OBJS)
	@echo "$(BRIGHT_MAGENTA)📦 Archiving library...$(RESET)"
	@$(AR) $(NAME) $(OBJS)
	@echo "$(BRIGHT_GREEN)✓ $(NAME) created successfully!$(RESET)"

# Object compilation rule - simplified
$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "$(CYAN)Compiling$(RESET) %-40s $(GREEN)✓$(RESET)\n" "$(notdir $<)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Enhanced clean with visual feedback
clean:
	@echo "$(BRIGHT_RED)🧹 Cleaning object files...$(RESET)"
	@if [ -d "$(OBJ_DIR)" ]; then \
		echo "$(YELLOW)Removing $(OBJ_DIR)/ directory...$(RESET)"; \
		$(RM) -r $(OBJ_DIR); \
		echo "$(GREEN)✅ Object files cleaned$(RESET)"; \
	else \
		echo "$(DIM)No object files to clean$(RESET)"; \
	fi

# Enhanced fclean with visual feedback
fclean: clean
	@echo "$(BRIGHT_RED)🔥 Deep cleaning...$(RESET)"
	@if [ -f "$(NAME)" ]; then \
		echo "$(YELLOW)Removing library $(NAME)...$(RESET)"; \
		$(RM) $(NAME); \
		echo "$(GREEN)✅ Library removed$(RESET)"; \
	else \
		echo "$(DIM)No library to remove$(RESET)"; \
	fi

# Rebuild
re: fclean all

# Debug mode with enhanced feedback
debug: CFLAGS += -g3 -fsanitize=address
debug: 
	@echo "$(BRIGHT_YELLOW)🐛 DEBUG MODE ENABLED$(RESET)"
	@echo "$(YELLOW)Flags: $(CFLAGS)$(RESET)"
	@$(MAKE) re --no-print-directory
	@echo "$(BRIGHT_YELLOW)🔍 Debug build completed with AddressSanitizer$(RESET)"

.PHONY: all clean fclean re debug
NAME = minitalk
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC_SERVER = server.c
SRC_CLIENT = client.c
OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)
SERVER = server
CLIENT = client
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Colors
GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
RESET = \033[0m

all: $(SERVER) $(CLIENT)
	@echo "$(GREEN)Build successful!$(RESET)"

$(SERVER): $(OBJ_SERVER) $(LIBFT)
	@echo "$(BLUE)Linking server...$(RESET)"
	$(CC) $(CFLAGS) $(OBJ_SERVER) $(LIBFT) -o $(SERVER)

$(CLIENT): $(OBJ_CLIENT) $(LIBFT)
	@echo "$(BLUE)Linking client...$(RESET)"
	$(CC) $(CFLAGS) $(OBJ_CLIENT) $(LIBFT) -o $(CLIENT)

$(LIBFT):
	@echo "$(BLUE)Building libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

clean:
	@echo "$(RED)Cleaning objects...$(RESET)"
	rm -f $(OBJ_SERVER) $(OBJ_CLIENT)
	@if [ -d $(LIBFT_DIR) ]; then $(MAKE) -C $(LIBFT_DIR) clean 2>/dev/null || true; fi

fclean: clean
	@echo "$(RED)Full cleaning...$(RESET)"
	rm -f $(SERVER) $(CLIENT)
	@if [ -d $(LIBFT_DIR) ]; then $(MAKE) -C $(LIBFT_DIR) fclean 2>/dev/null || true; fi

re: fclean all

.PHONY: all clean fclean re


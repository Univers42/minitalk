NAME=minitalk.a
CC = cc
CFLAGS= -Wall -Wextra -Werror
OBJECTS= client.o server.o
AR = ar rcs $(OBJECTS)
PROGRAM_NAME = server
PROGRAM_NAME2 = client
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
edit: $(OBJECTS)
	cc -o edit $(OBJECTS)

$(OBJECTS):server.h
	client.o server.o
all:

bonus: pre_build $(BONUS_NAME)
	@echo "\n$(GREEN)$(BOLD)Bonus build successful!$(RESET)"

pre_build:
	@echo "$(CYAN)Starting build process...$(RESET)"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)bonus

$(NAME): $(LIBFT) $(OBJ_FILES)
	@echo "\n$(BLUE)Linking...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)$(NAME) created!$(RESET)"

$(BONUS_NAME):$(LIBFT) $(OBJ_BONUS)
	@echo "\n$(BLUE)"

test: all
	@echo "$(CYAN)Running tests...$(RESET)"
	@./tests/run_tests.sh

docs:
	@echo "$(CYAN)Generating documentation...$(RESET)"
	@doxygen Doxyfile

clean: $(OBJS)
	@echo "$(RED)Cleaning objects...$(RESET)"
	@$(RM) -r edit $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean $(NAME)
	@echo "$(RED)Full cleaning...$(RESET)"
	@$(RM) $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re : fclean all

.PHONY clean fclean re bonus debug profile docs test help pre_build


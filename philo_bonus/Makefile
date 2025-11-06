NAME			= philo_bonus
CC				= cc
CFLAGS			= -Wall -Wextra -Werror
MKDIR			= mkdir -p
RM				= rm -rf
LINKER  	    = -lpthread

# Colors

GREEN			= "\033[0;32m"
YELLOW			= "\033[1;33m"
RED				= "\033[0;31m"
RESET			= "\033[0m"

# Includes
INCLUDES_DIR 	= include
INCLUDES_FLAG 	= -I$(INCLUDES_DIR)
INCLUDES		= $(wildcard $(INCLUDES_DIR)/philosopher_bonus.h)

# Sources
SRCS_DIR		= src/
SRC_FILES		= main_bonus.c \
				  init_bonus.c \
				  utils_bonus.c \
				  simulation_bonus.c \
				  messages_bonus.c

# Objects
OBJS_DIR		= objs/
OBJ_FILES		= $(SRC_FILES:.c=.o)
OBJS			= $(addprefix $(OBJS_DIR), $(OBJ_FILES))


all : $(OBJS_DIR) $(NAME)

$(OBJS_DIR) :
	@$(MKDIR) $(OBJS_DIR)

$(NAME) : $(OBJS) Makefile
	@echo $(GREEN) " - Compiling $(NAME)..." $(RESET)
	@$(CC) $(CFLAGS) $(OBJS) $(LINKER) -o $(NAME)
	@echo $(YELLOW) " - Compiling FINISHED" $(RESET)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c $(INCLUDES)
	@$(CC) $(CFLAGS) $(INCLUDES_FLAG) -c $< -o $@	

clean :
	@$(RM) $(OBJS_DIR)
	@echo $(RED) " - Cleaned!" $(RESET)

fclean : clean
	@$(RM) $(NAME)
	@echo $(RED) " - Full Cleaned!" $(RESET)

bonus: all

re: fclean all

.PHONY: all clean fclean re
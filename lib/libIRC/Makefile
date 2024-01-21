# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fkhan <fkhan@student.42abudhabi.ae>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/01 20:03:39 by fkhan             #+#    #+#              #
#    Updated: 2024/01/21 18:18:54 by fkhan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libIRC.a

CXX			= c++
CXXFLAGS 	= -Wall -Werror -Wextra -std=c++98
INCFLAGS 	= -I$(INC_DIR) -I$(LIBFT_INC)

LIB			= ar -rcs

LIB_DIR		= ../

LIBFT_DIR	= $(LIB_DIR)/libft
LIBFT_INC	= $(LIBFT_DIR)/inc

INC_DIR		= inc
SRC_DIR		= src
BUILD_DIR	= .build

SRC			= $(wildcard $(SRC_DIR)/*.cpp)
INC			= $(wildcard $(INC_DIR)/*.hpp)
OBJ			= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# COLORS
GREEN 				= \033[0;32m
RED					= \033[0;31m
RESET 				= \033[0m

all: $(NAME)

$(NAME): $(BUILD_DIR) $(OBJ)
	@$(LIB) $(NAME) $(OBJ)
	@echo "\n$(NAME): $(GREEN)object files were created$(RESET)"
	@echo "$(NAME): $(GREEN)$(NAME) was created$(RESET)"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@echo "$(NAME): $(GREEN)$(BUILD_DIR) was created$(RESET)"

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp $(INC)
	@$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@
	@echo ".\cpp"

clean:
	@rm -rf $(BUILD_DIR)
	@echo "$(NAME): $(RED)object files were deleted$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME): $(RED)$(NAME) was deleted$(RESET)"

re: fclean all

.PHONY: all clean fclean re
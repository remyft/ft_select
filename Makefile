# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rfontain <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/28 20:50:45 by rfontain          #+#    #+#              #
#    Updated: 2018/11/20 05:53:11 by rfontain         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_select

CC = gcc -g3

RM = rm -rf

RED = "\x1b[0;31;40m"
RESET = "\x1b[1;37;40m"
GREY = "\x1b[1;30;40m"
GREEN = "\x1b[1;32;40m"

LIB_PATH = libft
LIB = $(LIB_PATH)/libft.a
LIB_LINK = -L $(LIB_PATH) -lft -lncurses

INC_DIR = inc
INCS = -I $(LIB_PATH)/ -I $(INC_DIR)

SRCS_DIR = src/
SRCS =	ft_select.c			\
		list_tool.c			\
		signal.c			\
		arrow_key.c			\
		key.c				\
		termcaps.c			\
		tools.c				\


OK =      $(GREEN)[OK]$(RESET)		

NEWLINE = $(shell echo "")

CFLAGS +=  -Wall -Wextra -Werror

OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	@mkdir -p $@

$(NAME): $(NEWLINE) $(OBJS) $(LIB)
	@$(CC) $^ -o $@ $(LIB_LINK)
	@echo ""
	@echo $(GREY)" Compilling" $(RESET) [ $(NAME) ] $(OK)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@echo $(RED)" ᚘ  "$(RESET) | tr -d '\n'
	$(CC) $(CFLAGS) $(INCS) -o $@ -c $< 

$(LIB):
	@echo ""
	@echo " " | tr -d '\n'
	@make -C $(LIB_PATH)

clean:
	@$(RM) $(OBJS_DIR)
	@make -C $(LIB_PATH) clean
	@echo $(GREY)" Cleaning :" $(RESET) [ $(NAME) ] $(OK)

fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIB_PATH) fclean
	@echo $(GREY)" Deleting.." $(RESET) [ $(NAME) ] $(OK)

reclean: clean
	@$(RM) $(NAME)
	@make -C $(LIB_PATH) fclean
	@echo $(GREY)" Deleting.." $(RESET) [ $(NAME) ] $(OK)
	@echo ""

re: reclean all

nn:
	norminette $(SRCS)
	norminette $(INC_DIR)

.PHONY: clean fclean all re

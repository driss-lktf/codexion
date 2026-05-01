# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#              #
#    Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= codexion
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread

SRC		= main.c \
		  parse_args.c \
		  parse_args2.c \
		  validate.c \
		  utils.c \
		  utils2.c \
		  init_sim.c \
		  init_coders.c \
		  heap.c \
		  heap_utils.c \
		  dongle.c \
		  dongle_acquire.c \
		  dongle_release.c \
		  coder_routine.c \
		  coder_actions.c \
		  scheduler.c \
		  monitor.c \
		  cleanup.c \
		  threads.c

OBJ		= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

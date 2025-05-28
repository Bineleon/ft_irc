RESET			= \e[0m\033[0m
GREEN			= \033[0;32m\e[1m

NAME			= ircserv

SRC_DIR			= srcs/
SRCS			= main.cpp Server.cpp Client.cpp
SRC				= ${addprefix ${SRC_DIR}, ${SRCS}}

INCLUDE_DIR		= includes/
INCLUDES		=  Server.hpp Client.hpp
INCLUDE			= ${addprefix ${INCLUDE_DIR}, ${INCLUDES}}

OBJS_DIR		= objects/
OBJS			= ${addprefix ${OBJS_DIR}, ${SRCS:.cpp=.o}}

DEPS_DIR		= dependances/
DEPS			= ${addprefix ${DEPS_DIR}, ${SRCS:.cpp=.d}}

CXX				= c++
CFLAGS			= -Wall -Werror -Wextra -std=c++98 -g -MMD -MP

RM				= rm -rf

${OBJS_DIR}%.o: ${SRC_DIR}%.cpp ${INCLUDE}
		@mkdir -p ${OBJS_DIR} ${DEPS_DIR}
		@${CXX} ${CFLAGS} -c $< -o $@ -MF ${DEPS_DIR}$*.d

${NAME}: ${OBJS} ${INCLUDE} Makefile
		@${CXX} ${CFLAGS} ${OBJS} -I ./ -o $@
		@echo "${GREEN}ircserv		: DONE!${RESET}"

all: ${NAME}

clean:
		@${RM} ${OBJS_DIR} ${DEPS_DIR}
		@echo "${GREEN}Clean		: DONE!${RESET}"


fclean: clean;
		@${RM} ${NAME}
		@echo "${GREEN}Full Clean	: DONE!${RESET}"

-include ${DEPS}

re: fclean all

.PHONY: all clean fclean re

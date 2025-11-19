###
# Variables
###


###
# Rules
###

build:
	@printf " $(YELLOW)$(BOLD)$(ITALIC)■$(RESET)  Building the project $(YELLOW)$(BOLD)$(ITALIC)$(RESET)\n"
	docker build -f backend/Dockerfile -t ft_transcendence .
run:
	docker run -d -p 3000:3000 -p 8000:8000 --name ft_transcendence ft_transcendence
stop:
	docker stop ft_transcendence || true
delete:
	docker rm ft_transcendence || true
clean: stop
	docker rmi ft_transcendence || true
re: stop delete build run

deploy:
	docker-compose up --build





###
# Colors
###


BLACK		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m
GRAY		=	\033[90m

BOLD		=	\033[1m
ITALIC		=	\033[3m

RESET		=	\033[0m
LINE_CLR	=	\33[2K\r






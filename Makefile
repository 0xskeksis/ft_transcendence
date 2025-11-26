###
# Variables
###


###
# Rules
###

build:
	@printf " $(YELLOW)$(BOLD)$(ITALIC)■$(RESET)  Building the project $(YELLOW)$(BOLD)$(ITALIC)$(RESET)\n"
	docker compose build
up:
	docker compose up -d
down:
	docker compose down --remove-orphans
re: down build up

clean: down
	docker image prune -f

# WARNING: delete all the images, this command can erase the database
clean-images:
	docker system prune -a --volumes





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






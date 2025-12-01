/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:47:55 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/30 16:04:06 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <Gamedef.h>
#include <list>
#include <map>
#include <utility>
#include <random>
#include <napi.h>

#define SETGET(type, name, Name) 											\
	public: 																\
		inline void set##Name(const type &value) { this->name = value; }	\
		inline type get##Name(void) const { return this->name; }			\
	private:																\
		type name


static inline int random(int x, int y)
{
	static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(x,y);

	return dist(rng);
}

struct Ball
{
	Ball(): x(0.5), y(0.5), dx(BALL_SPEED_NORM * random(1,2)*2-3), dy(0){};
	double x;
	double y;
	double dx;
	double dy;
	void reset() {x = 0.5; y = 0.5; dx = 0; dy = 0;}
};

#define LEFT 1
#define RIGHT 2

#define UP 2
#define DOWN 1
#define SET 3

typedef enum
{
	CREATED = 0,
	STARTED = 1,
	FINISHED = 2
} GameStatus;
class Game;
void pad_collision(Game &game);
Napi::Object GetGameData(const Napi::CallbackInfo& info);
class Game
{
	SETGET(double, lpad, Lpad);
	SETGET(double, rpad, Rpad);
	SETGET(GameStatus, status, Status);
	public:
		Game(): ball(Ball()), id(-1)
		{
			score = std::make_pair(0, 0);
			lpad = 0.5;
			rpad = 0.5;
			status = CREATED;
		}
		Game(int id): ball(Ball()), id(id)
		{
			score = std::make_pair(0, 0);
			lpad = 0.5;
			rpad = 0.5;
			status = CREATED;
		}
		~Game() {}

		const std::pair<int, int>	getScore() const { return score; }
		void						addScore(bool player) {player ? score.first++ : score.second++;};
		Ball						getBall() const { return ball; }
		void						movePaddle(int id, int op, int opt = 0);
		void						update(int input0, int input1);
		void						setBallPos(double x, double y) { ball.x = x; ball.y = y; }
		void						restart(int side);
		friend void					pad_collision(Game &game);
		friend Napi::Object GetGameData(const Napi::CallbackInfo& info);

	private:
		Ball ball;
		int id;
		std::pair<int, int> score;
};

extern std::map<int, Game> gameMap;

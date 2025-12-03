/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:22:01 by ellanglo          #+#    #+#             */
/*   Updated: 2025/12/03 14:11:14 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <Game.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <Gamedef.h>

void Game::movePaddle(int id, int op, int opt)
{
	if (!op)
		return;

	double *pad = &this->rpad;
	if (id == LEFT)
		pad = &this->lpad;

	switch (op)
	{
		case SET:
			{
				*pad = opt;
				return;
			}
		case DOWN:
			{
				if (*pad + MOVE_OFFSET < 1 - MOVE_OFFSET)
					*pad += MOVE_OFFSET;
				return;
			}
		case UP:
			{
				if (*pad - MOVE_OFFSET > -0.025)
					*pad -= MOVE_OFFSET;
				return;
			}
	}
}

static int inPaddle(double x, double y, double &padY_out, Game &game)
{
    bool sideRight = x > 0.5;
    double pad_y = sideRight ? game.getRpad() : game.getLpad();
    double pad_x = sideRight ? (1.0 - R_PAD_OFFSET_N - PADDLE_W_N) : (L_PAD_OFFSET_N);

    double pad_half_h = PADDLE_H_N / 2.0;

    double pad_top    = pad_y - pad_half_h;
    double pad_bottom = pad_y + pad_half_h;
    double pad_left   = pad_x;
    double pad_right  = pad_x + PADDLE_W_N;

    double closestX = std::clamp(x, pad_left, pad_right);
    double closestY = std::clamp(y, pad_top, pad_bottom);

    double dx = x - closestX;
    double dy = y - closestY;

    if (dx * dx + dy * dy <= BALL_RADIUS_N * BALL_RADIUS_N)
	{
        padY_out = pad_y;
        return sideRight ? 1 : -1;
    }
    return 0;
}

static double calc_angle(double ball_y, double pad_y)
{
    double rel = (ball_y - pad_y) / (PADDLE_H_N / 2.0);
    rel = std::clamp(rel, -1.0, 1.0);
    return rel * MAX_REFLECT;
}

void pad_collision(Game &game)
{
	//Ball game.ball = game.getBall();
	static const int step = 6;
	for (int i = 0; i < step; i++)
	{
		double padY = 0.0;
		int side = inPaddle(game.ball.x, game.ball.y, padY, game);
		if (side != 0)
		{
			double theta = calc_angle(game.ball.y, padY);
			double speed = std::hypot(game.ball.dx, game.ball.dy);
			speed = std::min(0.03, speed*1.2);
			game.ball.dx = -static_cast<double>(side) * speed * std::cos(theta);
			game.ball.dy = speed * std::sin(theta);
			double pad_x = (side == 1) ? (1.0 - R_PAD_OFFSET_N - PADDLE_W_N) : (L_PAD_OFFSET_N);
			if (side == -1)
				game.ball.x = pad_x + PADDLE_W_N + BALL_RADIUS_N + 1e-6;
			else
				game.ball.x = pad_x - BALL_RADIUS_N - 1e-6;
			std::cout << "Ball speed : " << speed << "\n";
		}
		game.ball.x += game.ball.dx / step;
		game.ball.y += game.ball.dy / step;
	}
}

void Game::update(int input0, int input1)
{
    movePaddle(LEFT, input0);
    movePaddle(RIGHT, input1);

	pad_collision(*this);
	if (ball.y <= 0.014 || ball.y >= 1 - 0.014)
		ball.dy = -ball.dy;

	if (ball.x > 1 - R_PAD_OFFSET_N - PADDLE_W_N)
		restart(RIGHT);
	else if (ball.x < L_PAD_OFFSET_N + PADDLE_W_N)
		restart(LEFT);
	if (std::max(score.first, score.second) == 5)
		status = FINISHED;
}

void Game::restart(int side)
{
	ball.reset();	
	switch (side)
	{
		case LEFT: score.second++; side = -1; break;
		case RIGHT: score.first++; side = 1; break;
	}
	lpad = 0.5;
	rpad = 0.5;
	ball.dx = BALL_SPEED_NORM * side;
}

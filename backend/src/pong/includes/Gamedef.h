/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gamedef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:21:46 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/25 15:53:23 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once
#include <cmath>

#define DESIGN_W        1920.0
#define DESIGN_H        1080.0
#define PADDLE_W_N      (10.0 / DESIGN_W)
#define PADDLE_H_N      (90.0 / DESIGN_H)
#define BALL_RADIUS_N   (12.0 / DESIGN_H)
#define L_PAD_OFFSET_N  (20.0 / DESIGN_W)
#define R_PAD_OFFSET_N  (20.0 / DESIGN_W)
#define PX_W(ratio, width)   ((int)((ratio) * (width)  + 0.5))
#define PX_H(ratio, height)  ((int)((ratio) * (height) + 0.5))
#define PX_MIN(ratio, w, h)  ((int)((ratio) * ((w<h)?w:h) + 0.5))
#define MAX_REFLECT		60 * M_PI / 180
#define BALL_SPEED_NORM 0.01
#define MOVE_OFFSET 0.025

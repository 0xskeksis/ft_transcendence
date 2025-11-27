// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   pong.controllers.js                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2025/11/10 20:07:25 by ellanglo          #+#    #+#             //
//   Updated: 2025/11/27 15:56:03 by ellanglo         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
import { createRequire } from "module";
const require = createRequire(import.meta.url);

const addon = require("../pong/build/Release/addon.node");

export const GetRandomData = addon.GetRandomData;
export const Update = addon.Update;
export const GetGameData = addon.GetGameData;
export const SetBallPos = addon.SetBallPos;
export const StartGame = addon.StartGame;
export const EndGame = addon.EndGame;

let g_last_id = 0;
let g_game_map = new Map();
let wrap = { game: null };

function Game(id1, id2)
{
	this.player1_id = id1;
	this.player2_id = id2;
	this.id = g_last_id;
	g_last_id++;
	this.linput = 0;
	this.rinput = 0;
	this.status = 0;
}

async function getGame(id, reply)
{
	wrap.game = g_game_map.get(id);
	let game = wrap.game;
	if (!game)
		return reply.status(400).send({error: "Game does not exist"});
	if (game.status == 2)
		return reply.status(400).send({error: "Game is already finished"});
	return 0;
}

export async function tick()
{
    g_game_map.forEach((value, key, map) => 
	{
		if (value.status == 1)
		{
			let gameData = Update(value);
			value.status = gameData.status;
			if (value.status == 2)
			{
				let score = [gameData.lscore, gameData.rscore];
				//store score in blockchain
			}
		}
	});
    setTimeout(tick, 1000 / 30);
}

export async function getGameData(request, reply)
{
	const game_id = Number(request.query.game_id);
	const rpl = await getGame(game_id, reply);
	if (rpl !== 0)
		return rpl;
	let data = GetGameData(wrap.game.id);
	return reply.send(data);
}

export async function sendInput(request, reply)
{
	const { game_id, input, side } = request.body;
	const rpl = await getGame(game_id, reply);
	if (rpl !== 0)
		return rpl;
	let game = wrap.game;
	if (side == 0)
		game.linput = input;
	else
		game.rinput = input;
	return 1;
}

export async function setBallPos(request, reply)
{
	const { x, y, game_id } = request.body;
	SetBallPos(x, y, id);
	return 1;
}

export async function createGame(request, reply)
{
	const { player1_id, player2_id} = request.body;
	const game = new Game(player1_id, player2_id);
	g_game_map.set(game.id, game);
	return reply.send({id:game.id});
}

export async function startGame(request, reply)
{
	const { game_id } = request.body;
	const rpl = await getGame(game_id, reply);
	if (rpl !== 0)
		return rpl;
	let game = wrap.game;
	game.status = 1;
	StartGame(game.id);	
	return 1;
}

export async function endGame(request, reply)
{
	const { game_id } = request.body;
	const rpl = await getGame(game_id, reply);
	if (rpl !== 0)
		return rpl;
	let game = wrap.game;
	game.status = 2;
	EndGame(game.id);	
	return 1;
}

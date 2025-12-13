// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   pong.controllers.js                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2025/11/10 20:07:25 by ellanglo          #+#    #+#             //
//   Updated: 2025/12/12 17:46:03 by ellanglo         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
import { createRequire } from "module";
const require = createRequire(import.meta.url);

const addon = require("../pong/build/Release/addon.node");

export const Update = addon.Update;
export const GetGameData = addon.GetGameData;
export const SetBallPos = addon.SetBallPos;
export const StartGame = addon.StartGame;
export const EndGame = addon.EndGame;
export const NewGame = addon.NewGame;

let g_last_game_id = 0;
let g_game_map = new Map();
let g_last_tournament_id = 0;
let g_tournament_map = new Map();
let game_wrap = { game: null };
let tournament_wrap = { tournament: null };

const AFK_TIME = 30

function Game()
{
	this.player1_id = -1;
	this.player2_id = -1;
	this.id = g_last_game_id;
	g_last_game_id++;
	this.linput = 0;
	this.rinput = 0;
	this.status = 0;
	this.tournament_id = -1;
	this.del = 0;
	this.last_linput_change = Date.now();
	this.last_rinput_change = Date.now();
	this.winner = null;
}

function Tournament()
{
	this.players_id = [];
	this.id = g_last_tournament_id;
	g_last_tournament_id++;
	this.game_1 = null;
	this.game_2 = null;
	this.game_3 = null;
	this.game_3_id = -1;
	this.game_3_lside = -1;
	this.status = 0;
	this.winner = -1; 
}

async function getGame(id, reply)
{
	game_wrap.game = g_game_map.get(id);
	let game = game_wrap.game;
	if (!game)
		return reply.status(400).send({error: "Game does not exist"});
	return 0;
}

async function getTournament(id, reply)
{
	tournament_wrap.tournament = g_tournament_map.get(id);
	let tournament = tournament_wrap.tournament;
	if (!tournament)
		return reply.status(400).send({error: "Tournament does not exist"});
	return 0;
}

async function newGame()
{
	let game = new Game();
	g_game_map.set(game.id, game);
	NewGame(game.id);
	return game;
}

function getWinner(game, gameData)
{
	const score = [gameData.lscore, gameData.rscore];
	let winner;
	if (score[0] > score[1])
		winner = game.player1_id;
	else
		winner = game.player2_id;
	return winner;
}

function inputStale(lastChange) 
{
    return (Date.now() - lastChange) > AFK_TIME * 1000;
}

function inputTiming(game)
{
	if (inputStale(game.last_linput_change))
	{
		game.winner = game.player2_id;
		game.status = 2;
		EndGame(game.id);
	}
	if (inputStale(game.last_rinput_change))
	{
		game.winner = game.player1_id;
		game.status = 2;
		EndGame(game.id);
	}
}

export async function tick()
{
    g_game_map.forEach(async (value, key, map) => 
	{
		if (value.status == 1)
		{
			inputTiming(value);
			const gameData = Update(value);
			value.status = gameData.status;
			let winner = null;
			if (value.winner == null)
				winner = getWinner(value, gameData);
			else
				winner = value.winner;
			if (value.status == 2)
			{
				if (value.tournament_id >= 0)
				{
					await getTournament(value.tournament_id, null);
					let tournament = tournament_wrap.tournament;
					if (tournament.game_3.player1_id == -1)
					{
						tournament.game_3.player1_id = winner;
						tournament.game_3_lside = winner;
					}
					else if (tournament.game_3.id != value.id)
					{
						tournament.game_3.player2_id = winner;
						tournament.game_3.status = 1;
						StartGame(tournament.game_3.id);
					}
					else
					{
						tournament.winner = winner;
						tournament.status = 2;
					}
				}
			}
		}
		else if (value.status == 2)
		{
			if (value.del == 100)
				g_game_map.delete(value.id)
			else
				value.del++;
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
	return reply.send(GetGameData(game_wrap.game.id));
}

export async function getOwner(request, reply)
{
	const game_id = Number(request.query.game_id);
	const rpl = await getGame(game_id, reply);
	if (rpl !== 0)
		return rpl;
	return reply.send({owner: game_wrap.game.player1_id});
}
export async function sendInput(request, reply)
{
	const { game_id, input, side } = request.body;
	const rpl = await getGame(game_id, reply);
	if (rpl !== 0)
		return rpl;
	let game = game_wrap.game;
	if (input != (side == 0 ? game.linput : game.rinput))
	{
		if (side == 0)
		{
			game.linput = input;
			game.last_linput_change = Date.now();
		}
		else
		{
			game.rinput = input;
			game.last_rinput_change = Date.now();
		}
	}
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
	const { player_id } = request.body;
	let game = await newGame();
	game.player1_id = player_id;
	return reply.send({id:game.id});
}

export async function joinGame(request, reply)
{
	const { game_id, player_id } = request.body;
	const rpl = await getGame(game_id, reply);
	if (rpl !== 0)
		return rpl;
	let game = game_wrap.game;
	if (game.player1_id == player_id)
		return reply.status(400).send({error:"Player aready in game"});
	game.player2_id = player_id;
	return 1;
}

export async function startGame(request, reply)
{
	const { game_id } = request.body;
	const rpl = await getGame(game_id, reply);
	if (rpl !== 0)
		return rpl;
	let game = game_wrap.game;
	if (game.status != 0)
		return reply.status(400).send({error:"Game already started or ended"});
	if (game.player2_id == -1)
		return reply.status(400).send({error:"You can't start a game alone"});
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
	let game = game_wrap.game;
	game.status = 2;
	EndGame(game.id);	
	return 1;
}

export async function createTournament(request, reply)
{
	let tournament = new Tournament();
	g_tournament_map.set(tournament.id, tournament);
	tournament.game_1 = await newGame();
	tournament.game_2 = await newGame();
	tournament.game_3 = await newGame();
	tournament.game_3_id = tournament.game_3.id;
	tournament.game_3.tournament_id = tournament.id;
	reply.send({id:tournament.id});
	return reply;
}

export async function joinTournament(request, reply)
{
	const {tournament_id, player_id} = request.body;
	const rpl = await getTournament(tournament_id, reply);
	if (rpl !== 0)
		return rpl;
	let tournament = tournament_wrap.tournament;
	if (tournament.players_id.length == 4)
		return reply.status(400).send({error: "Tournament is already full"});
	if (player_id in tournament.players_id)
		return reply.status(400).send({error: "User already in tournament"});
	tournament.players_id.push(player_id);
	if (tournament.players_id.length <= 2)
		return reply.send({id:tournament.game_1.id, side:(tournament.players_id.length-1)%2});
	else
		return reply.send({id:tournament.game_2.id, side:(tournament.players_id.length-1)%2});
}

export async function startTournament(request, reply)
{
	const {tournament_id} = request.body;
	const rpl = await getTournament(tournament_id, reply);
	if (rpl !== 0)
		return rpl;
	let tournament = tournament_wrap.tournament;
	if (tournament.players_id.length != 4)
		return reply.status(400).send({error: "Can't start tournament with less than 4 players"})
	let game_1 = tournament.game_1;
	let game_2 = tournament.game_2;
	game_1.player1_id = tournament.players_id[0];
	game_1.player2_id = tournament.players_id[1];
	game_1.tournament_id = tournament_id;
	game_1.status = 1;
	StartGame(game_1.id);
	game_2.player1_id = tournament.players_id[2];
	game_2.player2_id = tournament.players_id[3];
	game_2.tournament_id = tournament_id;
	game_2.status = 1;
	StartGame(game_2.id);
	tournament.status = 1;
}

export async function getTournamentData(request, reply)
{
	const tournament_id = Number(request.query.tournament_id);
	const rpl = await getTournament(tournament_id, reply);
	if (rpl !== 0)
		return rpl;
	return reply.send(tournament_wrap.tournament);
}

export async function getTournamentOwner(request, reply)
{
	const tournament_id = Number(request.query.tournament_id);
	const rpl = await getTournament(tournament_id, reply);
	if (rpl !== 0)
		return rpl;
	return reply.send({id:tournament_wrap.tournament.players_id[0]});
}

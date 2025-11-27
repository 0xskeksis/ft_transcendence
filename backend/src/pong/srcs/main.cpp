/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:47:38 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/27 17:08:18 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <Game.hpp>
#include <iostream>
#include <map>
#include <napi.h>
#include <random>

std::map<int, Game> gameMap;

Napi::Object GetRandomData(const Napi::CallbackInfo& info) 
{
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,100);

    Napi::Env env = info.Env();
	Napi::Object data = Napi::Object::New(env);

	data.Set("score", dist(rng));
	data.Set("lpos", dist(rng)/100.);
	data.Set("rpos", dist(rng)/100.);
	data.Set("bposx", dist(rng)/100.);
	data.Set("bposy", dist(rng)/100.);

    return data;
}

Napi::Object GetGameData(const Napi::CallbackInfo& info)
{
	int id = info[0].As<Napi::Number>();
    Napi::Env env = info.Env();
	Napi::Object data = Napi::Object::New(env);

	Game &game = gameMap[id];
	data.Set("score", game.getScore().first);
	data.Set("lpos", game.getLpad());
	data.Set("rpos", game.getRpad());
	data.Set("bposx", game.getBall()->x);
	data.Set("bposy", game.getBall()->y);
	std::cout << "GetGameData on game with id: " << id << "\n";

	return data;
}

Napi::Object Update(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    Napi::Object gameObj = info[0].As<Napi::Object>();

    int id     = gameObj.Get("id").As<Napi::Number>().Int32Value();
    int linput = gameObj.Get("linput").As<Napi::Number>().Int32Value();
    int rinput = gameObj.Get("rinput").As<Napi::Number>().Int32Value();
	std::cout << "Updating game with id: " << id << "\n";
    Game &game = gameMap[id];
    game.update(linput, rinput);
    Napi::Object returnData = Napi::Object::New(env);
    returnData.Set("status", Napi::Number::New(env, (int)game.getStatus()));
    auto score = game.getScore();
    returnData.Set("lscore", Napi::Number::New(env, score.first));
    returnData.Set("rscore", Napi::Number::New(env, score.second));
    return returnData;
}

Napi::Value SetBallPos(const Napi::CallbackInfo& info)
{
	double x = info[0].As<Napi::Number>();
	double y = info[1].As<Napi::Number>();
	int id = info[2].As<Napi::Number>();

	Game &game = gameMap[id];
	game.setBallPos(x, y);
	return info.Env().Null();
}

Napi::Value NewGame(const Napi::CallbackInfo& info)
{
	int id = info[0].As<Napi::Number>();

	gameMap[id] = Game(id);
	std::cout << "NewGame with id: " << id << "\n";
	return info.Env().Null();
}

Napi::Value StartGame(const Napi::CallbackInfo& info)
{
	int id = info[0].As<Napi::Number>();

	gameMap[id].setStatus(STARTED);
	std::cout << "StartGame with id: " << id << "\n";
	return info.Env().Null();
}

Napi::Value EndGame(const Napi::CallbackInfo& info)
{
	int id = info[0].As<Napi::Number>();

	gameMap[id].setStatus(FINISHED);
	return info.Env().Null();
}

#define EXPORT(name) exports.Set(#name, Napi::Function::New(env, name))
Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
	EXPORT(GetRandomData);
	EXPORT(Update);
	EXPORT(GetGameData);
	EXPORT(SetBallPos);
	EXPORT(NewGame);
	EXPORT(StartGame);
	EXPORT(EndGame);
    return exports;
}

NODE_API_MODULE(pong, Init);

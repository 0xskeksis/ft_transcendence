/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:47:38 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/27 15:57:42 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <Game.hpp>
#include <map>
#include <napi.h>
#include <random>
#include <iostream>

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
	double id = info[0].As<Napi::Number>();
    Napi::Env env = info.Env();
	Napi::Object data = Napi::Object::New(env);

	Game &game = gameMap[id];
	data.Set("score", game.getScore().first);
	data.Set("lpos", game.getLpad());
	data.Set("rpos", game.getRpad());
	data.Set("bposx", game.getBall()->x);
	data.Set("bposy", game.getBall()->y);

	return data;
}

Napi::Object Update(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    // Validation d'entrée
	/*
    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "Update expects one object argument")
            .ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }*/

    Napi::Object gameObj = info[0].As<Napi::Object>();
	/*
    if (!gameObj.Has("id") || !gameObj.Has("linput") || !gameObj.Has("rinput")) {
        Napi::TypeError::New(env, "Object must have id, linput and rinput")
            .ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
	*/

    int id     = gameObj.Get("id").As<Napi::Number>().Int32Value();
    int linput = gameObj.Get("linput").As<Napi::Number>().Int32Value();
    int rinput = gameObj.Get("rinput").As<Napi::Number>().Int32Value();
	/*
    std::cerr << "[addon] Update called id=" << id 
              << " linput=" << linput 
              << " rinput=" << rinput << std::endl;

    // Vérifier que gameMap contient l'id
    */
	auto it = gameMap.find(id);
	/*
    if (it == gameMap.end()) {
        std::string msg = "Game id not found: " + std::to_string(id);
        std::cerr << "[addon] " << msg << std::endl;
        Napi::Error::New(env, msg).ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
	*/

    Game &game = it->second;

    // Exécution de la logique jeu (aucun try/catch)
    game.update(linput, rinput);

    //std::cerr << "[addon] update done, preparing return object" << std::endl;

    Napi::Object returnData = Napi::Object::New(env);
    returnData.Set("status", Napi::Number::New(env, (int)game.getStatus()));

    auto score = game.getScore();
    returnData.Set("lscore", Napi::Number::New(env, score.first));
    returnData.Set("rscore", Napi::Number::New(env, score.second));

    //std::cerr << "[addon] returning success object" << std::endl;

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
	return info.Env().Null();
}

Napi::Value StartGame(const Napi::CallbackInfo& info)
{
	int id = info[0].As<Napi::Number>();

	gameMap[id].setStatus(STARTED);
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

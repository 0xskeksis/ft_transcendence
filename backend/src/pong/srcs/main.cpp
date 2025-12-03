/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:47:38 by ellanglo          #+#    #+#             */
/*   Updated: 2025/12/03 14:11:56 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <Game.hpp>
#include <map>
#include <napi.h>

std::map<int, Game> gameMap;

Napi::Object GetGameData(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    int id = info[0].As<Napi::Number>().Int32Value();

    auto it = gameMap.find(id);

    Game &game = it->second;

    Napi::Object data = Napi::Object::New(env);
    auto score = game.getScore();
	int score_mask = score.first + (score.second << 4);
    data.Set("score", Napi::Number::New(env, score_mask));
    data.Set("lpos", Napi::Number::New(env, game.getLpad()));
    data.Set("rpos", Napi::Number::New(env, game.getRpad()));
    data.Set("bposx", Napi::Number::New(env, game.ball.x));
    data.Set("bposy", Napi::Number::New(env, game.ball.y));
	data.Set("status", Napi::Number::New(env, (int)game.status));

    return data;
}

Napi::Object Update(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    Napi::Object gameObj = info[0].As<Napi::Object>();

    int id     = gameObj.Get("id").As<Napi::Number>().Int32Value();
    int linput = gameObj.Get("linput").As<Napi::Number>().Int32Value();
    int rinput = gameObj.Get("rinput").As<Napi::Number>().Int32Value();

    auto it = gameMap.find(id);

    Game &game = it->second;
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
    Napi::Env env = info.Env();

	double x = info[0].As<Napi::Number>().DoubleValue();
    double y = info[1].As<Napi::Number>().DoubleValue();
    int id = info[2].As<Napi::Number>().Int32Value();

    auto it = gameMap.find(id);

    Game &game = it->second;
    game.setBallPos(x, y);
    return env.Null();
}

Napi::Value NewGame(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

	int id = info[0].As<Napi::Number>().Int32Value();

    gameMap[id] = Game(id);
    return env.Null();
}

Napi::Value StartGame(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

	int id = info[0].As<Napi::Number>().Int32Value();

    auto it = gameMap.find(id);

    it->second.setStatus(STARTED);
    return env.Null();
}

Napi::Value EndGame(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

	int id = info[0].As<Napi::Number>().Int32Value();

    auto it = gameMap.find(id);

    it->second.setStatus(FINISHED);
    return env.Null();
}

#define EXPORT(name) exports.Set(#name, Napi::Function::New(env, name))
Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
	EXPORT(Update);
	EXPORT(GetGameData);
	EXPORT(SetBallPos);
	EXPORT(NewGame);
	EXPORT(StartGame);
	EXPORT(EndGame);
    return exports;
}

NODE_API_MODULE(pong, Init);

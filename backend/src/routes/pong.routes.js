// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   pong.routes.js                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2025/11/28 16:09:53 by stetrel           #+#    #+#             //
//   Updated: 2025/11/28 16:48:52 by stetrel          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

import {getGameData, sendInput, setBallPos, createGame, startGame} from '../controllers/pong.controllers.js'
import {authenticateToken} from '../middlewares/auth.middleware.js'


//TODO: Verify JWT to access all the routes
async function pong_routes (fastify, options){
	fastify.get('/pong/get-data', 
		{
			preHandler: authenticateToken
		},
		async (request, reply) => {
		getGameData(request, reply);
	})

	fastify.post('/pong/post-input',
		{
			preHandler: authenticateToken
		},
		async (request, reply) => {
		sendInput(request, reply);
	})

	fastify.post('/pong/debug/set-ball', 
		{
			preHandler: authenticateToken
		},
		async (request, reply) => {
		setBallPos(request, reply);
	})

	fastify.post('/pong/create-game', 
		{
			preHandler: authenticateToken
		},
		async (request, reply) => {
		createGame(request, reply);
	})

	fastify.post('/pong/start-game',
		{
			preHandler: authenticateToken
		},
		async (request, reply) =>{
		startGame(request, reply);
	})
}

export default pong_routes

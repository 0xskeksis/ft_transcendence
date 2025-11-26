import {getGameData, sendInput, setBallPos, createGame, startGame} from '../controllers/pong.controllers.js'

async function pong_routes (fastify, options){
	fastify.get('/pong/get-data', async (request, reply) => {
		getGameData(request, reply);
	})

	fastify.post('/pong/post-input', async (request, reply) => {
		sendInput(request, reply);
	})

	fastify.post('/pong/debug/set-ball', async (request, reply) => {
		setBallPos(request, reply);
	})

	fastify.post('/pong/create-game', async (request, reply) => {
		createGame(request, reply);
	})

	fastify.post('/pong/start-game', async (request, reply) =>{
		startGame(request, reply);
	})
}

export default pong_routes

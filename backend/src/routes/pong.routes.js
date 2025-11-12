import {sendData, getData} from '../controllers/pong.controllers.js'

async function pong_routes (fastify, options){
	fastify.get('/pong/get', async (request, reply) => {
		sendData(request, reply);
	})

	fastify.post('/pong/post', async (request, reply) => {
		getData(request, reply);
	})
}

export default pong_routes

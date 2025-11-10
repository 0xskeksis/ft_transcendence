import {sendData} from '../controllers/pong.controllers.js'

async function pong_routes (fastify, options){
	fastify.get('/pong', async (request, reply) => {
		sendData(request, reply);
	})
}

export default pong_routes

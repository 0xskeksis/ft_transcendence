import {registerUser, verifyUser} from '../controllers/auth.controllers.js'

async function routes (fastify, options){
	fastify.get('/', async (request, reply) => {
		return {hello: "world"}
	})

	fastify.post('/register', async (request, reply) => {
		registerUser(request, reply);
	})
	
	fastify.post('/login', async (request, reply) => {
		verifyUser(request, reply);
	})
}

export default routes

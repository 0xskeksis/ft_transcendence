import {registerUser, verifyUser} from '../controllers/auth.controllers.js'
import {checkRequestIntegrity} from '../middlewares/auth.middleware.js'

async function routes (fastify, options){
	fastify.get('/', async (request, reply) => {
		return {hello: "world"}
	})
	fastify.post(
		'/register',
		{
			preHandler: checkRequestIntegrity(['email', 'password', 'username'])
		},
		async (request, reply) => {
		registerUser(request, reply);
	})
	
	fastify.get(
	'/login',
	{
		preHandler: checkRequestIntegrity(['email', 'password'])
		
	},
	async (request, reply) => {
		verifyUser(request, reply);
	})
}

export default routes

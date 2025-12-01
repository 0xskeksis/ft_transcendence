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
		(request, reply) => {
		registerUser(request, reply);
	})
	
	fastify.post(
	'/login',
	{
		preHandler: checkRequestIntegrity(['username', 'password'])
		
	},
	(request, reply) => {
		verifyUser(request, reply);
	})
}

export default routes

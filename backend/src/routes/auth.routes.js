import {registerUser, verifyUser} from '../controllers/auth.controllers.js'
import {checkRequestIntegrity} from '../middlewares/auth.middleware.js'
import {get_google_secret} from '../controllers/2fa.controllers.js'

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
		preHandler: checkRequestIntegrity(['username', 'password', 'token'])
		
	},
	(request, reply) => {
		verifyUser(request, reply);
	})

	fastify.post(
		'/get-secret',
		{
			preHandler: checkRequestIntegrity(['username', 'password'])
		},
		(request, reply) => {
			get_google_secret(request, reply);
		}
	);
}

export default routes

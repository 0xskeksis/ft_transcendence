import {registerUser, verifyUser} from '../controllers/auth.controllers.js'
import {checkRequestIntegrity} from '../middlewares/auth.middleware.js'
import {setup2fa, enable2fa, disable2fa, verify2fa} from '../controllers/2fa.controllers.js'

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

	fastify.post(
		'/2fa/setup', (request, reply) => {
			setup2fa(request, reply);
		})
	fastify.post(
		'/2fa/verify', (request, reply) => {
			verify2fa(request, reply);
		})
	fastify.post(
		'/2fa/enable', (request, reply) => {
			enable2fa(request, reply);
		})
	fastify.post(
		'/2fa/disable', (request, reply) => {
			disable2fa(request, reply);
		})
}

export default routes

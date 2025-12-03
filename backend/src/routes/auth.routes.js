import {registerUser, verifyUser} from '../controllers/auth.controllers.js'
import {checkRequestIntegrity} from '../middlewares/auth.middleware.js'
import {get_google_secret} from '../controllers/2fa.controllers.js'
import jwt from 'jsonwebtoken';


import dotenv from 'dotenv'
dotenv.config()

async function routes (fastify, options){
	fastify.get('/', async (request, reply) => {
		const code = request.query.code;
		if (!code) 
			return reply.send('No code provided');

		const response = await fetch('https://github.com/login/oauth/access_token', {
			method: 'POST',
			headers: { 'Content-Type': 'application/json', 'Accept': 'application/json' },
			body: JSON.stringify({
				client_id: process.env.GITHUB_CLIENT_ID,
				client_secret: process.env.GITHUB_CLIENT_SECRET,
				code,
				redirect_uri: 'https://localhost:3000/'
			})
		});

		const data = await response.json(); // { access_token, token_type, scope }
		const accessToken = data.access_token;

		// Récupérer les infos utilisateur
		const userRes = await fetch('https://api.github.com/user', {
			headers: { Authorization: `token ${accessToken}` }
		});
		const githubUser = await userRes.json();
		if (!githubUser)
			return 

		// Crée un JWT/session pour ton frontend
		const jwtToken = jwt.sign({ id: githubUser.id, login: githubUser.login },
			process.env.JWT_SECRET,
			{ expiresIn: "1000h" }

		);
		return reply.redirect(`https://localhost:5173/`)
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

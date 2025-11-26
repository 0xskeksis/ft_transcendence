import Fastify from 'fastify';
import routes from './routes/auth.routes.js'
import pong_routes from './routes/pong.routes.js'
import {createDatabase} from './controllers/auth.controllers.js'
import {tick} from './controllers/pong.controllers.js'

const LOGIN_PORT = 3000
const API_PORT = 8000

createDatabase();

// --- Serveur pour le login ---
const loginServer = Fastify();
loginServer.register(routes);
loginServer.listen({ port: LOGIN_PORT }, (err, address) => {
	if (err) {
		loginServer.log.error(err);
		process.exit(1);
	}
	loginServer.log.info(`Login server listening on ${address}`);
});

// --- Serveur pour l'API ---
const apiServer = Fastify();
apiServer.register(pong_routes);
apiServer.listen({ port: API_PORT }, (err, address) => {
	if (err) {
		apiServer.log.error(err);
		process.exit(1);
await fastify.listen({ port: PORT, host: '0.0.0.0' }, function (err, address){
	if (err){
		fastify.log(err)
		process.exit(1)
	}
	tick();
	apiServer.log.info(`API server listening on ${address}`);
});

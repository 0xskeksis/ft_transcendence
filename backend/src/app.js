import Fastify from 'fastify';
import routes from './routes/auth.routes.js'
import pong_routes from './routes/pong.routes.js'
import {createDatabase} from './controllers/auth.controllers.js'
import {tick} from './controllers/pong.controllers.js'
import fs from 'fs'

const APP_PORT = 3000
const API_PORT = 8000

createDatabase();

const httpsOptions = {
  key: fs.readFileSync('/certs/key.pem'),
  cert: fs.readFileSync('/certs/cert.pem')
};

// --- Serveur pour le login ---
const loginServer = Fastify({https: httpsOptions});
loginServer.register(routes);
loginServer.listen({ port: APP_PORT, host: '0.0.0.0'}, (err, address) => {
	if (err) {
		loginServer.log.error(err);
		process.exit(1);
	}
	loginServer.log.info(`Login server listening on ${address}`);
});

// --- Serveur pour l'API ---
const apiServer = Fastify();
apiServer.register(pong_routes);
apiServer.listen({ port: API_PORT, host: '0.0.0.0'}, (err, address) => {
	if (err) {
		apiServer.log.error(err);
		process.exit(1);
	}
	tick();
})

// await Fastify.listen({ port: APP_PORT, host: '0.0.0.0' }, function (err, address){
// 	if (err){
// 		Fastify.log(err)
// 		process.exit(1)
// 	}
// 	apiServer.log.info(`API server listening on ${address}`);
// });

import Fastify from 'fastify';
import routes from './routes/auth.routes.js'
import {createDatabase} from './controllers/auth.controllers.js'

const PORT = 3000

createDatabase();

const fastify = Fastify();

fastify.register(routes)

await fastify.listen({ port: PORT, host: '0.0.0.0' }, function (err, address){
	if (err){
		fastify.log(err)
		process.exit(1)
	}
	fastify.log.info(`server listening on ${address}`)
});

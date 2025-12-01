import db from "../db/db.js"
import {checkPassword} from '../utils/utils.controllers.js'
import { authenticator } from 'otplib';



//TODO:
// 	fastify.post(
// 		'/2fa/verify', (request, reply) => {
// 			verify2fa(request, reply);	//TODO
// 		})
// 	fastify.post(
// 		'/2fa/enable', (request, reply) => {
// 			enable2fa(request, reply);	//TODO
// 		})
// 	fastify.post(
// 		'/2fa/disable', (request, reply) => {
// 			disable2fa(request, reply);	//TODO
// 		})
// }
//

export async function verify2fa(request, reply){
	const { username, token } = request.body ?? {};
	if (!username || !token)
		return reply.status(400).send({ error: "Missing Fields" });

	let stmt = db.prepare(
		"SELECT google_secret, two_fa FROM users WHERE username = ?"
	);
	const user = stmt.get(username);
	if (!user || !user.two_fa)
		return reply.status(400).send({ error: "2FA not enabled" });

	const is_valid = authenticator.verify({
		token,
		secret: user.google_secret
	});

	if (!is_valid)
		return reply.status(400).send({ error: "Invalid 2FA code" });

	return reply.send({ success: true });
}

export async function setup2fa(request, reply){
	const { username, password } = request.body ?? {};
	if (!username || !password)
		return reply.status(400).send({ error: "Missing Fields" });

	let stmt = db.prepare("SELECT id, username, email, password FROM users WHERE username = ?");
	const user = stmt.get(username);
	if (!user)
		return reply.status(400).send({ error: "Wrong Credentials" });

	if (!(await checkPassword(user, password)))
		return reply.status(400).send({ error: "Invalid Password" });

	const new_secret = authenticator.generateSecret();
	const keyUri = authenticator.keyuri(username, "TEST", new_secret);

	stmt = db.prepare(
		"UPDATE users SET two_fa = 0, google_secret = ? WHERE id = ?"
	);
	stmt.run(new_secret, user.id);

	return reply.send({
		keyUri,
		secret: new_secret
	});
}

export async function enable2fa(request, reply){
	const { username, token } = request.body ?? {};
	if (!username || !token)
		return reply.status(400).send({ error: "Missing Fields" });
	let stmt = db.prepare(
		"SELECT id, google_secret FROM users WHERE username = ?"
	);
	const user = stmt.get(username);
	if (!user || !user.google_secret)
		return reply.status(400).send({ error: "2FA not initialized" });
	const is_valid = authenticator.verify({
		token: token,
		secret: user.google_secret
	});
	if (!is_valid)
		return reply.status(400).send({ error: "Invalid 2FA code" });
	stmt = db.prepare("UPDATE users SET two_fa = 1 WHERE id = ?");
	stmt.run(user.id);
	return reply.send({ success: true });
}

export async function disable2fa(request, reply){
	const { username, password, token } = request.body ?? {};
	if (!username || !password || !token)
		return reply.status(400).send({ error: "Missing Fields" });

	let stmt = db.prepare(
		"SELECT id, password, google_secret FROM users WHERE username = ?"
	);
	const user = stmt.get(username);
	if (!user)
		return reply.status(400).send({ error: "Wrong Credentials" });

	if (!(await checkPassword(user, password)))
		return reply.status(400).send({ error: "Invalid Password" });

	const is_valid = authenticator.verify({
		token,
		secret: user.google_secret
	});

	if (!is_valid)
		return reply.status(400).send({ error: "Invalid 2FA code" });

	stmt = db.prepare(
		"UPDATE users SET two_fa = 0, google_secret = NULL WHERE id = ?"
	);
	stmt.run(user.id);

	return reply.send({ success: true });
}

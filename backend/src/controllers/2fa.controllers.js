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

export async function verify2fa(username, token){
	let stmt = db.prepare(
		"SELECT google_secret FROM users WHERE username = ?"
	);
	const user = stmt.get(username);

	const is_valid = await authenticator.verify({
		token,
		secret: user.google_secret,
		window: 1
	});
	console.log("SERVER TOTP NOW =", authenticator.generate(user.google_secret));

	if (!is_valid)
		return 0;
	return 1;
}

export async function setup2fa(username){
	
	const new_secret = authenticator.generateSecret();
	const keyUri = authenticator.keyuri(username, "ft_transcendence", new_secret);

	let stmt = db.prepare(
		"UPDATE users SET google_secret = ? WHERE username = ?"
	);
	stmt.run(new_secret, username);

	return {
		uriKey: keyUri,
		secret: new_secret
	};
}

export async function get_google_secret(request, reply){
	const {username, password} = request.body ?? {};
	if (!username || !password)
		return reply.status(400).send({ error: "Missing Fields" })

	const stmt = db.prepare("SELECT id, username, email, password, google_secret FROM users WHERE username = ?");
	const user = stmt.get(username);

	if (!user)
		return reply.status(401).send({ error: "Invalid Credentials" });

	const good = await checkPassword(user, password);
	if (!good)
		return reply.status(401).send({ error: "Invalid Credentials" });
	return reply.send({ secret: user.google_secret});
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
	const { username, token } = request.body ?? {};
	if (!username || !token)
		return reply.status(400).send({ error: "Missing Fields" });

	let stmt = db.prepare(
		"SELECT id, password, google_secret FROM users WHERE username = ?"
	);
	const user = stmt.get(username);
	if (!user)
		return reply.status(400).send({ error: "Wrong Credentials" });

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

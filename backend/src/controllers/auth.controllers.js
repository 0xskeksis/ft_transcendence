import Database from 'better-sqlite3'
import bcrypt from 'bcrypt'
import db from "../db/db.js"
import {findUserByEmail, checkPassword} from "../utils/utils.controllers.js"
import jwt from 'jsonwebtoken';

const SALT_ROUNDS = 10;
const SECRET = "8fh328h78&fgdshfgsj"
const MIN_PASSWORD_LENGTH = 1;

const	blacklist = [];
blacklist.push("admin", "root", "support", "system", "moderator", "staff")

export async function createDatabase(){
	const query = `
			CREATE TABLE IF NOT EXISTS users (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			username TEXT NOT NULL UNIQUE,
			email TEXT NOT NULL UNIQUE,
			password TEXT NOT NULL
		);

			CREATE TABLE IF NOT EXISTS users_stats (
			id INTEGER NOT NULL,
			games_played INTEGER NOT NULL,
			games_wins INTEGER NOT NULL,
			FOREIGN KEY (id) REFERENCES users(id) ON DELETE CASCADE
		);
	`;
	db.exec(query);
}

export async function registerUser(request, reply){
	const {username, email, password} = request.body ?? {};
	const re = /^[^@\s]+@[^@\s]+\.[^@\s]+$/


	if (!username || !email || !password)
		return reply.status(400).send({error: "Missing Field in Controllers"});
	if (blacklist.some(word => username.includes(word)))
		return reply.status(400).send({error: "This username is strickly forbidden"});
	if (password.length < MIN_PASSWORD_LENGTH)
		return reply.status(400).send({error: "Bad password length"});
	if (re.test(email) == false)
		return reply.status(400).send({error: "Invalid Email: ", email});
	try {
		const hashedPass = await bcrypt.hash(password, SALT_ROUNDS);
		const stmt  = db.prepare(
			"INSERT INTO users (username, email, password) VALUES (?, ?, ?)");

		const info = stmt.run(username, email, hashedPass);

		const userId = info.lastInsertRowid;
		db.prepare("INSERT INTO users_stats (id, games_played, games_wins) VALUES (?, 0, 0)").run(userId);

		// return reply.send({success: true, message: "User successfuly created !" });
		//debug db
		const rows = db.prepare("SELECT * FROM users").all()
		return reply.send({users: rows});
		// return {success: true, message: "User successfuly created !" };
	}catch (err) {
		if (err.code === "SQLITE_CONSTRAINT_UNIQUE" || err.code == "SQLITE_CONSTRAINT"){
			return reply.status(400).send({error: "Failed to create the users"})
		}
		console.log(err);
		return reply.status(500).send({error: "Internal Error"})
	}

}

export async function verifyUser(request, reply){
	try {
		const { email, password} = request.body ?? {};

		if (!email || !password)
			return reply.status(400).send({ error: "Missing Field" });

		const stmt = db.prepare("SELECT id, username, email, password FROM users WHERE email = ?");
		const user = stmt.get(email);

		if (!user)
			return reply.status(401).send({ error: "Invalid Credentials" });

		const good = await checkPassword(user, password);
		if (!good)
			return reply.status(500).send({ error: "Invalid Credentials" });
		const token = await jwt.sign(
			{ id: user.id, email: user.email, username: user.username },
			SECRET,
			{ expiresIn: "1h" }
		);
		const obj = {
			success: true,
			message: "User successfully logged in!",
			token,
		}
		console.log(obj)
		return reply.send(obj);
	}catch(e){
		console.log(e);
		return reply.code(500).send({ error: "Internal server error" });
	}
}

export async function deleteUser(request, reply){
	const {username, email, password} = request.body;
	
	if (!username || !email || !password)
		return response.status(400).send({error: "Missing Field"});

	const stmt = db.prepare("SELECT id, username, email, password FROM users WHERE email = ?")
	const user = stmt.get(email);

	if (!user)
		return reply.status(401).send({error: "Invalid Credentials"})
	if (!checkPassword(user, password))
		return reply.status(401).send({error: "Invalid Credentials"})
	stmt = db.prepare("DELETE FROM users WHERE email = ?");
	stmt.run(email)


	return {success: true, message: "You are no longer register on this website !"};
}

export async function changePassword(request, reply) {
	const { email, oldPassword, newPassword } = request.body;

	if (!email || !oldPassword || !newPassword)
		return reply.status(400).send({ error: "Missing Field" });

	const stmt = db.prepare("SELECT * FROM users WHERE email = ?");
	const user = stmt.get(email);

	if (!user)
		return reply.status(404).send({ error: "User not found" });

	const match = await bcrypt.compare(oldPassword, user.password);
	if (!match)
		return reply.status(401).send({ error: "Invalid old password" });

	const hashedNew = await bcrypt.hash(newPassword, SALT_ROUNDS);
	const updateStmt = db.prepare("UPDATE users SET password = ? WHERE email = ?");
	updateStmt.run(hashedNew, email);

	return { success: true, message: "Password successfully changed" };
}

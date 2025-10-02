import Database from 'better-sqlite3'
import bcrypt from 'bcrypt'
import db from "../db/db.js"
import {findUserByEmail, checkPassword} from "../utils/utils.controllers.js"

const SALT_ROUNDS = 10;

const MIN_PASSWORD_LENGTH = 12;


const	blacklist = [];
blacklist.push("admin", "root", "support", "system", "moderator", "staff",
				"nazi")

export async function createDatabase(){
	const query = `
		CREATE TABLE IF NOT EXISTS users (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		username TEXT NOT NULL UNIQUE,
		email TEXT NOT NULL UNIQUE,
		password TEXT NOT NULL)
`
	db.exec(query);
}

export async function registerUser(request, reply){
	const {username, email, password} = request.body;
	const re = /^[^@\s]+@[^@\s]+\.[^@\s]+$/


	if (!username || !email || !password)
		return reply.status(400).send({error: "Missing Field"});
	if (blacklist.some(word => username.includes(word)))
		return reply.status(400).send({error: "This username is strickly forbidden"});
	if (password.length < MIN_PASSWORD_LENGTH)
		return reply.status(400).send({error: "Bad password length"});
	if (re.test(email) == false)
		return reply.status(400).send({error: "Invalid Email: ", email});
	try {
		const hashedPass = await bcrypt.hash(password, SALT_ROUNDS);
		const stmt  = db.prepare(
			"INSERT INTO users (username, email, password) VALUES (?, ?, ?)",
			[username, email, hashedPass]);

		stmt.run(username, email, hashedPass);

		return {success: true, message: "User successfuly created !" };
	}catch (err) {
		console.log("SQLite error :", err);
		if (err.code === "SQLITE_CONSTRAINT_UNIQUE" || err.code == "SQLITE_CONSTRAINT"){
			return reply.status(400).send({error: "Failed to create the users"})
		}
		return reply.status(500).send({error: "Internal Error"})
	}
}

export async function verifyUser(request, reply){
	const {email, password} = request.body;

	if (!email || !password){
		return response.status(400).send({error: "Missing Field"});
	}
	const stmt = db.prepare("SELECT id, username, email, password FROM users WHERE email = ?")
	const user = stmt.get(email);

	if (!user)
		return reply.status(401).send({error: "Invalid Credentials"})
	if (checkPassword(user, password))
		return reply.status(401).send({error: "Invalid Credentials"})

	
		
	return {success: true, message: "User successfuly logged in !" };
}

export async function deleteUser(request, reply){
	const {username, email, password} = request.body;
	
	if (!username || !email || !password)
		return response.status(400).send({error: "Missing Field"});

	const stmt = db.prepare("SELECT id, username, email, password FROM users WHERE email = ?")
	const user = stmt.get(email);

	if (!user)
		return reply.status(401).send({error: "Invalid Credentials"})
	if (checkPassword(user, password))
		return reply.status(401).send({error: "Invalid Credentials"})
	//const	stmt = db.prepare("DELETE id where email = ?");
	//stmt.exec(email)


	return {success: true, message: "You are no longer register on this website !"};
}

export async function changePassword(request, reply){
	const {username, email, password} = request.body;

	if (!username || !email || !password){
		return response.status(400).send({error: "Missing Field"});

	


	return {success: true, message: "Password succesfuly changed"};
	}
}

import Database from 'better-sqlite3'
import db from "../db/db.js"
import bcrypt from 'bcrypt'

export async function findUserByEmail(email){
	const stmt = db.prepare("SELECT * FROM users WHERE email = ?");
	return stmt.get(email);
}

export async function checkPassword(user, PlainPassword){
	return await bcrypt.compare(plainedPassword, user.password);
}



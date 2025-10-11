import jwt from 'jsonwentoken'
import dotenv from 'dotenv'

dotenv.config()
const secret = process.env.JWT_SECRET || "dev_secret_key"

export function authenticateToken(request, reply, next) {
	const	authHeader = request.headers["authorization"];
	const	token = authHeader && authHeader.split(" ")[1];

	if (!token)
		return reply.status(401).send({error: "Missing Token"});

	try{
		const decoded = jwt.verify(token, SECRET);
		request.user = decoded;
		next();
	}catch (err) {
		return reply.status(403).send({ error: "Invalid or expired token" });
	}
}

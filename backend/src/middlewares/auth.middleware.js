import jwt from 'jsonwebtoken'
import dotenv from 'dotenv'

dotenv.config()


export function authenticateToken(request, reply, done){
	const	authHeader = request.headers["authorization"];
	const	token = authHeader && authHeader.split(" ")[1];

	if (!token)
		return reply.status(401).send({error: "Missing Token"});

	try{
		const decoded = jwt.verify(token, process.env.JWT_SECRET);
		request.user = decoded;
	}catch (err) {
		return reply.status(403).send({ error: "Invalid or expired token" });
	}
	done()
}

export function checkRequestIntegrity(requireFields){
	return (request, reply, done) => {
		if (!request.body) {
            return reply.status(501).send({ error: "Missing body in middleware" });
        }
		for (const field of requireFields){
			if (!(field in request.body))
				return reply.status(403).send({error: "Missing field"})
		}
		done();
	}
}

import express from 'express';
import uuid4 from "uuid4";

class Session {
	constructor(userId, sessionId) {
		this.userId = userId;
		this.session = sessionId;
	}
}

export function sessionRouter(userRepository, redisClient) {
	let router = express.Router();

	router.post('/', async (req, res) => {
		// check if request is valid
		if (req.body.username && req.body.password) {
			let userDoc = await userRepository.findOne({ username: req.body.username });

			// check if user is valid
			if (!userDoc) {
				res.sendStatus(400);
				return;
			}

			// check password is correct
			if (req.body.password != userDoc.password) {
				res.sendStatus(403);
				return;
			}

			// remove old session of user
			const previousSessionKey = `sessionsIdsByUserId:${userDoc._id}`;
			const previousSessionId = await redisClient.get(previousSessionKey);
			if (previousSessionId) {
				await redisClient.del(`sessions:${previousSessionId}`);
			}

			// generate session id
			const sessionId = uuid4();
			const sessionKey = `sessions:${sessionId}`;
			const session = new Session(userDoc._id, sessionId);

			try {
				await redisClient.set(sessionKey, JSON.stringify(session));
				await redisClient.expire(sessionKey, 10);
				await redisClient.set(previousSessionKey, sessionId);
			} catch (error) {
				console.error("Error communicating with Redis:", error);
				res.sendStatus(500);
				return;
			}

			res.json(session);

		} else {
			// bad request
			res.sendStatus(400);
		}
	});

	return router;
}
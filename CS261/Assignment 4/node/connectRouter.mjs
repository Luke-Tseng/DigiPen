import express from 'express';
import crypto from 'crypto';
import { ObjectId } from 'mongodb';

const secret = 'cs261secret';
const gameport = 4200;

export function connectRouter(userRepository, redisClient) {
	let router = express.Router();

	router.post('/', async (req, res) => {
		const game_type = req.body.game_type;

		try {
			const sessionKey = `sessions:${req.body.session}`;
			const sessionString = await redisClient.get(sessionKey);
			const session = JSON.parse(sessionString);

			// check if gametype is valid
			if (!game_type) {
				res.sendStatus(400);
				return;
			}

			// check if session is valid
			if (!session) {
				res.sendStatus(401);
				return;
			}

			const userid = session.userId;
			// check if id is valid
			if (!ObjectId.isValid(userid)) {
				res.sendStatus(404);
				return;
			}

			let oid = new ObjectId(userid);
			let user = await userRepository.findOne({ '_id': oid });

			// check if userDoc is valid
			if (!user) {
				res.sendStatus(404);
				return;
			}

			// Calculate the plaintext token
			let plaintextToken = user.username + user.avatar + game_type + secret;
			let encodedToken = crypto.createHash('sha256').update(plaintextToken).digest('base64');

			// Create output object
			const output = {
				username: user.username,
				avatar: user.avatar,
				game_port: gameport,
				token: encodedToken
			};

			return res.json(output);
		} catch (error) {
			console.error("Error communicating with Redis:", error);
			res.sendStatus(500);
			return;
		}
	});

	return router;
}

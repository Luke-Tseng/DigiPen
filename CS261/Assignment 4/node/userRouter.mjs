import express from 'express';
import { CreateProfile, PublicProfile, FullProfile } from './user.mjs';
import { ObjectId } from 'mongodb';

export function userRouter(userRepository, redisClient) {
	let router = express.Router();

	// Create user
	router.post('/', async (req, res) => {
		if (req.body.username &&
			req.body.password &&
			req.body.avatar) {
			let count = await userRepository.countDocuments({ username: req.body.username });
			if (count > 0) {
				res.sendStatus(409);
				return;
			}
			let user = CreateProfile(req.body);
			await userRepository.insertOne(user);
			res.json(FullProfile(user));
		} else {
			res.sendStatus(400);
		}
	});

	// Update user
	router.put('/:id', async (req, res) => {
		try {
			const sessionKey = `sessions:${req.body.session}`;
			const sessionString = await redisClient.get(sessionKey);
			const session = JSON.parse(sessionString);

			// check if session is valid
			if (!session) {
				res.sendStatus(401);
				return;
			}
			// check if id is valid
			if (!ObjectId.isValid(req.params.id)) {
				res.sendStatus(404);
				return;
			}

			let oid = new ObjectId(req.params.id);
			let userDoc = await userRepository.findOne({ '_id': oid });
			// check if user is valid
			if (!userDoc) {
				res.sendStatus(404);
				return;
			}
			// check if user is owner
			if (session.userId === userDoc._id.toString()) {
				let update = {};
				if (req.body.username) {
					update.username = req.body.username;
				}
				if (req.body.password) {
					update.password = req.body.password;
				}
				if (req.body.avatar) {
					update.avatar = req.body.avatar;
				}

				let updatedDoc = await userRepository.findOneAndUpdate(
					{ '_id': oid },
					{ $set: update },
					{ returnDocument: 'after' });

				res.json(FullProfile(updatedDoc));
			} else {
				res.sendStatus(403);
				return;
			}
		} catch (error) {
			console.error("Error communicating with Redis:", error);
			res.sendStatus(500);
			return;
		}
	});

	// Get by id
	router.get('/:id', async (req, res) => {
		try {
			const sessionKey = `sessions:${req.body.session}`;
			const sessionString = await redisClient.get(sessionKey);
			const session = JSON.parse(sessionString);

			// check if session is valid
			if (!session) {
				res.sendStatus(401);
				return;
			}
			// check if id is valid
			if (!ObjectId.isValid(req.params.id)) {
				res.sendStatus(404);
				return;
			}

			let oid = new ObjectId(req.params.id);
			let userDoc = await userRepository.findOne({ '_id': oid });
			// check if userDoc is valid
			if (!userDoc) {
				res.sendStatus(404);
				return;
			}

			// check if user is owner
			if (session.userId === req.params.id) {
				res.json(FullProfile(userDoc));
			} else {
				res.json(PublicProfile(userDoc));
			}
		} catch (error) {
			console.error("Error communicating with Redis:", error);
			res.sendStatus(500);
			return;
		}
	});

	// Get by username
	router.get('/', async (req, res) => {
		// check if username is valid
		if (!req.query.username) {
			res.sendStatus(400);
			return;
		}

		try {
			const sessionKey = `sessions:${req.body.session}`;
			const sessionString = await redisClient.get(sessionKey);
			const session = JSON.parse(sessionString);

			// check if session is valid
			if (!session) {
				res.sendStatus(401);
				return;
			}

			let userDoc = await userRepository.findOne({ username: req.query.username });
			
			// check if user is valid
			if (!userDoc) {
				res.sendStatus(404);
				return;
			}

			// check if user is owner
			if (session.userId === userDoc._id.toString()) {
				res.json(FullProfile(userDoc));
			} else {
				res.json(PublicProfile(userDoc));
			}
		} catch (error) {
			console.error("Error communicating with Redis:", error);
			res.sendStatus(500);
			return;
		}
	});

	return router;
}
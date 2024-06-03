import express from 'express';
import base64url from 'base64url';

class User {
	constructor(username, password, avatar) {
		this.id = base64url(username);
		this.username = username;
		this.password = password;
		this.avatar = avatar;
	}
}

export function userRouter(userRepository, activeSessions) {
	let router = express.Router();

	router.post('/', (req, res) => {
		// check if request is valid
		if (req.body.username && req.body.password && req.body.avatar) {
			let newUser = new User(req.body.username,
				req.body.password,
				req.body.avatar);
			if (newUser.id in userRepository) {
				// user already exists
				res.sendStatus(409);
				return;
			}
			userRepository[newUser.id] = newUser;
			res.json(newUser);
		} else {
			// bad request
			res.sendStatus(400);
		}
	});

	router.put('/:id', (req, res) => {
		const user = userRepository[req.params.id];
		const sessionid = req.body.session;
		const session = activeSessions[sessionid];

		// check if session is valid
		if (!session) {
			res.sendStatus(401);
			return;
		}

		// check if user is valid
		if (!user) {
			res.sendStatus(404);
			return;
		}

		// check if user is owner
		if (session.userId === user.id) {
			// update info
			user.username = req.body.username;
			user.password = req.body.password;
			user.avatar = req.body.avatar;
			user.id = base64url(user.username);
			res.json(user);
		} else {
			res.sendStatus(403);
			return;
		}
	});

	router.get('/:id', (req, res) => {
		const user = userRepository[req.params.id];
		const sessionid = req.body.session;
		const session = activeSessions[sessionid];

		// check if session is valid
		if (!session) {
			res.sendStatus(401);
			return;
		}

		// check if user is valid
		if (!user) {
			res.sendStatus(404);
			return;
		}

		// check if user is owner
		if (session.userId === req.params.id) {
			res.json(user);
		} else {
			res.json({
				id: user.id,
				username: user.username,
				avatar: user.avatar
			});
		}
	});

	router.get('/', (req, res) => {
		const { username } = req.query;
		const sessionid = req.body.session;
		const session = activeSessions[sessionid];

		// check if username is valid
		if (!username) {
			res.sendStatus(400);
			return;
		}

		// check if session is valid
		if (!session) {
			res.sendStatus(401);
			return;
		}

		const user = userRepository[base64url(username)];

		// check if user is valid
		if (!user) {
			res.sendStatus(404);
			return;
		}

		// check if user is owner
		if (session.userId === user.id) {
			res.json(user);
		} else {
			res.json({
				id: user.id,
				username: user.username,
				avatar: user.avatar
			});
		}
	});
	return router;
}
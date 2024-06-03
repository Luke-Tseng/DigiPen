import express from 'express';
import base64url from 'base64url';
import uuid4 from "uuid4";

class Session {
	constructor(userId, sessionId) {
		this.userId = userId;
		this.session = sessionId;
	}
}

export function sessionRouter(userRepository, activeSessions) {
	let router = express.Router();
	router.post('/', (req, res) => {
		// check if request is valid
		if (req.body.username && req.body.password) {
			const userid = base64url(req.body.username);
			const user = userRepository[userid];
			// check if user is valid
			if (!user) {
				res.sendStatus(400);
				return;
			}
			// check password is correct
			if (req.body.password != userRepository[userid].password)
			{
				res.sendStatus(403);
				return;
			}

			// delete old sessions of this user
			Object.keys(activeSessions).forEach((sessionid) => {
				const userID = activeSessions[sessionid].userId;
				if (userID === userid) {
					delete activeSessions[sessionid];
				}
			});

			// generate session id
			const sessionid = uuid4();
			uuid4.valid(sessionid);
			const newSession = new Session(userid, sessionid);
			activeSessions[newSession.session] = newSession;
			res.json(newSession);

		} else {
			// bad request
			res.sendStatus(400);
		}
	});
	return router;
}
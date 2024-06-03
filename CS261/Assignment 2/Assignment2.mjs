import express from 'express';
import { userRouter } from './userRouter.mjs';
import { sessionRouter } from './sessionRouter.mjs';

const port = 3100;
const apiVersion = 'v1';
// store our users & active sessions in in-memory dictionaries
// (we'll replace with databases later)
const userRepository = {};
const activeSessions = {};
const app = express();

app.use(express.json());
app.use(express.text());
// the users routes:
app.use(`/api/${apiVersion}/users`, userRouter(userRepository, activeSessions));
// the sessions routes:
app.use(`/api/${apiVersion}/login`, sessionRouter(userRepository, activeSessions));
app.listen(port);

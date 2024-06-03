import express from 'express';
import { userRouter } from './userRouter.mjs';
import { sessionRouter } from './sessionRouter.mjs';
import { connectRouter } from './connectRouter.mjs';
import { MongoClient } from 'mongodb';
import redis from 'redis';

// connect to mongodb
const connString = 'mongodb://localhost:27017';
const mongoClient = new MongoClient(connString);
await mongoClient.connect();
console.log("Mongo Connection Successful");
const db = mongoClient.db('assignment4');
const userRepository = db.collection('users');
await userRepository.createIndex(['username', 'id'], { unique: true });

// connect to redis 
const redisClient = redis.createClient();
redisClient.on('error', (e) => console.log(e));
(async () => {
    await redisClient.connect();
})();


const port = 3100;
const apiVersion = 'v1';
const app = express();

app.use(express.json());
app.use(express.text());
// the users routes:
app.use(`/api/${apiVersion}/users`, userRouter(userRepository, redisClient));
// the sessions routes:
app.use(`/api/${apiVersion}/login`, sessionRouter(userRepository, redisClient));
// the connect routes:
app.use(`/api/${apiVersion}/connect`, connectRouter(userRepository, redisClient));

app.listen(port);

import express from 'express';
import { userRouter } from './userRouter.mjs';
import { sessionRouter } from './sessionRouter.mjs';
import { connectRouter } from './connectRouter.mjs';
import { MongoClient } from 'mongodb';
import redis from 'redis';

// connect to mongodb
const connString = process.env.Mongo_connection_string;
const mongoClient = new MongoClient(connString);
await mongoClient.connect();
console.log("Mongo Connection Successful");
const db = mongoClient.db(process.env.Mongo_database_name);
const userRepository = db.collection(process.env.Mongo_collection_name);
await userRepository.createIndex(['username', 'id'], { unique: true });


// connect to redis 
const redisClient = redis.createClient(process.env.Redis_port, process.env.Redis_address);
redisClient.on('error', (e) => console.log(e));
(async () => {
    await redisClient.connect();
})();

const port = process.env.Listen_port;
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

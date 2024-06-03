module.exports = {
  apps: [{
    name: 'Assignment6',
    script: 'Assignment6.mjs',
    instances: 1,
    autorestart: true,
    max_memory_restart: '1G',
    watch: false,
    env: {
      Listen_port: 3100,
      Mongo_connection_string: 'mongodb://localhost:27017',
      Mongo_database_name: 'assignment6',
      Mongo_collection_name: 'users',
      Redis_address: '127.0.0.1',
      Redis_port: 6379,
      Redis_expiration_duration: 10,
      Game_port: 4200,
      Shared_secret: 'cs261secret'
    }
  }
  ]
};

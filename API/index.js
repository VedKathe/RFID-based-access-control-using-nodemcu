
// Import required packages
const express = require('express');
const mysql = require('mysql');
const app = express();
const cors = require('cors');
const bodyParser = require('body-parser');
app.use(bodyParser.json());
app.use(cors());

// MySQL configuration
const dbConfig = {
  host: 'localhost',
  user: 'root',
  password: 'root',
  database: 'user',
};

// Create a MySQL connection pool
const pool = mysql.createPool(dbConfig);

// Middleware to handle MySQL connection
app.use((req, res, next) => {
  req.db = pool;
  next();
});

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html'); // Assuming your HTML file is named "index.html" and is in the same directory as this script
});

// Define a route that queries the database
app.get('/users', (req, res) => {
  const db = req.db;

  db.query('SELECT * FROM rfids', (err, results) => {
    if (err) {
      console.error(err);
      res.status(500).send('Error querying the database');
    } else {
      var data = [];
      results.forEach((result) => {
        data.push(result.id);
      });
      res.json(data);
    }
  });
});

// Define a route that queries the database
app.get('/getusers', (req, res) => {
  const db = req.db;

  db.query('SELECT * FROM usedata', (err, results) => {
    if (err) {
      console.error(err);
      res.status(500).send('Error querying the database');
    } else {
      res.json(results);
    }
  });
});

// Define a route for handling POST requests
app.post('/setusers', (req, res) => {
  const db = req.db;
  const { id } = req.body; // Assuming the request body contains a id and datetime

  if (!id) {
    res.status(400).json({ error: 'Both id and datetime are required' });
    return;
  }

  const insertQuery = 'INSERT INTO usedata (id, datetime) VALUES (?, NOW())';
  const values = [id];

  db.query(insertQuery, values, (err, result) => {
    if (err) {
      console.error(err);
      res.status(500).send('Error storing data in the database');
    } else {
      res.status(201).json({ message: 'Data stored successfully' });
    }
  });
});


// Start the Express server
const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Server is listening on port ${PORT}`);
});

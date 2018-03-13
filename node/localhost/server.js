const express = require('express');
const bodyParser = require('body-parser');

const config = require('./config');
const decryptSkey = require('./decryptSkey');
const decryptMinutiae = require('./decryptMinutiae');

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.use(decryptSkey.decrypt, decryptMinutiae.decrypt);

app.use('/', (req, res) => {
	res.send('Hello World!');
});

app.listen(config.port, () => {
	console.log(`Server is up at port ${config.port}`);
});

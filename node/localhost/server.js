const express = require('express');
const bodyParser = require('body-parser');

const config = require('./config');
const decryptSkey = require('./decryptSkey');
const decryptMinutiae = require('./decryptMinutiae');
const logger = require('./logger');

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.use(logger.reqBody);
app.use(decryptSkey.decrypt, decryptMinutiae.decrypt);
app.use(logger.reqBody);

app.use('/', (req, res) => {
	res.send({
		message: 'Decryption Success',
		minutiae: req.body.minutiae,
	});
});

app.listen(config.port, () => {
	console.log(`Server is up at port ${config.port}`);
});

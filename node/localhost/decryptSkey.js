const crypto = require('crypto');
const config = require('./config');

exports.decrypt = (req, res, next) => {
	try {
		req.body.decSkey = crypto.privateDecrypt({
			key: config.prk,
			padding: crypto.constants.RSA_PKCS1_PADDING,
		}, Buffer.from(req.body.eSkey, 'hex'));
	} catch (err) {
		res.status(400).send('Failed to decrypt session key !!');
	}
	next();
};

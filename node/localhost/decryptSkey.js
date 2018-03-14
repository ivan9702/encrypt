const crypto = require('crypto');
const config = require('./config');

exports.decrypt = (req, res, next) => {
	req.body.decSkey = crypto.privateDecrypt({
		key: config.prk,
		padding: crypto.constants.RSA_PKCS1_PADDING,
	}, Buffer.from(req.body.eSkey, 'hex'));
	next();
};

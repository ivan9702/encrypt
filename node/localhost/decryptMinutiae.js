const crypto = require('crypto');

exports.decrypt = (req, res, next) => {
	const decipher = crypto.createDecipheriv('aes256', req.body.decSkey, Buffer.from(req.body.iv, 'hex'));
	decipher.setAutoPadding(auto_padding=false);

	const decrypted = decipher.update(Buffer.from(req.body.encMinutiae, 'hex'));
	decipher.final();
	req.body.minutiae = decrypted.toString('hex');
	next();
};

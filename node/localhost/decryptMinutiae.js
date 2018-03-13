const crypto = require('crypto');

exports.decrypt = (req, res, next) => {
	try {
		const decipher = crypto.createDecipheriv('aes256', req.body.decSkey, Buffer.from(req.body.iv, 'hex'));
		decipher.setAutoPadding(auto_padding=false);

		const decrypted = decipher.update(Buffer.from(req.body.encMinutiae, 'hex'));
		decipher.final();
		req.body.minutiae = decrypted.toString('hex');
	} catch (err) {
		res.status(400).send('Failed to decrypt minutiae !!');
	}
	next();
};

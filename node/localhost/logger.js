exports.reqBody = (options) => {
	return (req, res, next) => {
		if (options === 'bfDecrypt') {
			console.log('====== Before Decrypt ======');
			console.log(`fpIndex: ${req.body.fpIndex}`);
			console.log(`userId: ${req.body.userId}`);
			console.log(`encMinutiae: ${req.body.encMinutiae}`);
			console.log(`\neSkey: ${req.body.eSkey}`);
			console.log(`\niv: ${req.body.iv}`);
		} else {
			console.log('====== After  Decrypt ======');
			console.log(`minutiae: ${req.body.minutiae}`);
		}
		next();
	};
};

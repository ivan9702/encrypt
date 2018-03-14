exports.reqBody = (req, res, next) => {
  console.log('============================');
  console.log(`fpIndex: ${req.body.fpIndex}`);
  console.log(`userId: ${req.body.userId}`);

  if (!req.body.hasOwnProperty('minutiae')) {
    console.log(`encMinutiae: ${req.body.encMinutiae}`);
    console.log(`\neSkey: ${req.body.eSkey}`);
    console.log(`\niv: ${req.body.iv}`);
  } else {
    console.log(`minutiae: ${req.body.minutiae}`);
  }
	next();
};


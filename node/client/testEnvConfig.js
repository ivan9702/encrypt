const testEnvConfig = {};

const testUrl = 'http://localhost:3000';
const addFpRoute = '/api/addFP';
const identifyRoute = '/api/identifyFP';
const deleteFpRoute = '/api/deleteFP';

testEnvConfig.env = process.env.TEST_ENV || 'non-secure';
testEnvConfig.minutiaeStr = '464d520020323000000000f000000118015400c500c501000000642380960054566440780056076480540067106480a30079e864805a007a026480430081146480da0094d76480a60097666480ce0099646480d000a16a64809200b06f6480ce00b46d64809300baed6480b500c2ed64405700c9876480aa00d7df64809200d96a64806400dbfd64404a00de8d6480e600de6664803f00df056480b800e46064809600e7e86480e700eef864808700f26d6440bc00fedd64408c0105db6440680105f46440af0118d664405e0119896480c40121d96480ba0124596480ac0130d064809a0133c564805e0137b26400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000';

testEnvConfig.addFpUrl = testUrl + addFpRoute;
testEnvConfig.identifyUrl = testUrl + identifyRoute;
testEnvConfig.deleteFpUrl = testUrl + deleteFpRoute;

testEnvConfig.fpIndex = '1';
testEnvConfig.userId = '30001';

module.exports = testEnvConfig;
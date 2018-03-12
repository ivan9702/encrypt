const ffi = require('ffi');
const ref = require('ref');

const key = Buffer.from([
  0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE,
  0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
  0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7,
  0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4,
]);
const keyPtr = ref.refType('char');

const iv = Buffer.from([
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
]);
const ivPtr = ref.refType('char');

const minutiaeStr = '464d520020323000000000f000000118015400c500c501000000642380960054566440780056076480540067106480a30079e864805a007a026480430081146480da0094d76480a60097666480ce0099646480d000a16a64809200b06f6480ce00b46d64809300baed6480b500c2ed64405700c9876480aa00d7df64809200d96a64806400dbfd64404a00de8d6480e600de6664803f00df056480b800e46064809600e7e86480e700eef864808700f26d6440bc00fedd64408c0105db6440680105f46440af0118d664405e0119896480c40121d96480ba0124596480ac0130d064809a0133c564805e0137b26400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000';
const minutiaeBuf = Buffer.from(minutiaeStr, 'hex');
const minutiaePtr = ref.refType('char');

const encryptedMinutiae = Buffer.alloc(512);
const encryptedMinutiaePtr = ref.refType('char');

const keyPath = '../../testKeyPairs/keyfile-pub.key';

const encryptedSkey = Buffer.alloc(256);
const encryptedSkeyPtr = ref.refType('char');

const encryptDll = ffi.Library('./encrypt', {
  'gen_aes_encrypted_minutiae': ['int', [ minutiaePtr, ivPtr, keyPtr, encryptedMinutiaePtr ]],
  'gen_rsa_encrypted_skey': ['int', [ keyPtr, 'String', encryptedSkeyPtr ]]
});

encryptDll.gen_aes_encrypted_minutiae(minutiaeBuf, iv, key, encryptedMinutiae);
console.log('encryptedMinutiae: ', encryptedMinutiae.toString('hex'));

encryptDll.gen_rsa_encrypted_skey(key, keyPath, encryptedSkey);
console.log('encryptedSkey: ', encryptedSkey.toString('hex'));

const ffi = require('ffi');
const ref = require('ref');
const request = require('request');

const encryptConfig = require('./config');

const keyPtr = ref.refType('char');
const ivPtr = ref.refType('char');

const minutiaeBuf = Buffer.from(encryptConfig.minutiaeStr, 'hex');
const minutiaePtr = ref.refType('char');

const encryptedMinutiae = Buffer.alloc(512);
const encryptedMinutiaePtr = ref.refType('char');

const encryptedSkey = Buffer.alloc(256);
const encryptedSkeyPtr = ref.refType('char');

const encryptDll = ffi.Library('./encrypt', {
  'gen_aes_encrypted_minutiae': ['int', [ minutiaePtr, ivPtr, keyPtr, encryptedMinutiaePtr ]],
  'gen_rsa_encrypted_skey': ['int', [ keyPtr, 'String', encryptedSkeyPtr ]]
});

encryptDll.gen_aes_encrypted_minutiae(minutiaeBuf, encryptConfig.iv, encryptConfig.key, encryptedMinutiae);
console.log('\nencryptedMinutiae:\n', encryptedMinutiae.toString('hex'));

encryptDll.gen_rsa_encrypted_skey(encryptConfig.key, encryptConfig.keyPath, encryptedSkey);
console.log('\nencryptedSkey:\n', encryptedSkey.toString('hex'));

request.post( {
  url: `${encryptConfig.addFpUrl}`,
  form: {
    fpIndex: `${encryptConfig.fpIndex}`,
    encMinutiae: `${encryptedMinutiae.toString('hex')}`,
    userId: `${encryptConfig.userId}`,
    eSkey: `${encryptedSkey.toString('hex')}`,
    iv: `${encryptConfig.iv.toString('hex')}`
  },
}, (err, res, body) => {
  console.log('error: ', err);
  console.log('statusCode: ', res && res.statusCode);
  console.log('body: ', body);
});

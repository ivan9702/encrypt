const ffi = require('ffi');
const ref = require('ref');
const request = require('request');

const encryptConfig = require('./encryptConfig');
const testEnvConfig = require('./testEnvConfig');

const keyPtr = ref.refType('char');
const ivPtr = ref.refType('char');

const requestCb = (err, res, body) => {
  console.log('error: ', err);
  console.log('statusCode: ', res && res.statusCode);
  console.log('body: ', body);
};

const minutiaeBuf = Buffer.from(testEnvConfig.minutiaeStr, 'hex');
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

if ( 'secure' === testEnvConfig.env ) {
  request.post( {
    url: `${testEnvConfig.addFpUrl}`,
    form: {
      fpIndex: `${testEnvConfig.fpIndex}`,
      encMinutiae: `${encryptedMinutiae.toString('hex')}`,
      userId: `${testEnvConfig.userId}`,
      eSkey: `${encryptedSkey.toString('hex')}`,
      iv: `${encryptConfig.iv.toString('hex')}`
    },
  }, requestCb);
} else {
  request.post( {
    url: `${testEnvConfig.addFpUrl}`,
    form: {
      fpIndex: `${testEnvConfig.fpIndex}`,
      minutiae: `${testEnvConfig.minutiaeStr}`,
      userId: `${testEnvConfig.userId}`,
    },
  }, requestCb);
}

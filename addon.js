var sealjs = require('node-cmake')('sealjs');

/*
  Initialize your SEAL context, with various security parameters.
  SEALContext expects 3 parameters:
    - Polynomial degree modulus (should be power of two)
        - Increase for higher security, less performance
    - Coefficient modulus (must be 128, 192, or 256)
        - Increase for higher security, less noise budget
    - Plaintext modulus
        - Increase for bigger plaintext space, less noise budget
  For a thorough guide on the meaning of these parameters, check
  out the official SEAL examples.
*/
const context = new sealjs.SEALContext(2048, 128, 65536);

// Generate a public/secret key pair
const keygen = new sealjs.KeyGenerator(context);
const pk = new sealjs.PublicKey(keygen);
const sk = new sealjs.SecretKey(keygen);

// These keys can be saved as a binary file
pk.save('public.key');

// These files can then be read as a PublicKey/SecretKey
const pk2 = new sealjs.PublicKey("public.key");

// Using a public key, generate an encryptor
const encryptor = new sealjs.Encryptor(context, pk);

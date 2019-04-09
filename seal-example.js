var seal = require('node-cmake')('sealjs');

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
const context = new seal.SEALContext(2048, 128, 65536);

// Generate a public/secret key pair
const keygen = new seal.KeyGenerator(context);
const pk = new seal.PublicKey(keygen);
const sk = new seal.SecretKey(keygen);

// These keys can be saved as a binary file
pk.save('public.key');

// These files can then be read as a PublicKey/SecretKey
const pk2 = new seal.PublicKey("public.key");

/* 
   Encoders are used to turn integers into a Plaintext object
   Plaintext objects store a polynomial representation of the integer
   Must provide a plaintext modulus as an argument
*/
const encoder = new seal.Encoder(65536);

// Let's encode some numbers!
const plaintexts = [
    new seal.Plaintext(encoder, 24),
    new seal.Plaintext(encoder, -9),
    new seal.Plaintext(encoder, 1)
];

// Encryptor is used to turn Plaintext into Ciphertext
const encryptor = new seal.Encryptor(context, pk);

// Encrypt each of our plaintexts
const ciphertexts = plaintexts.map((plain) => {
    return new seal.Ciphertext(encryptor, plain);
});

// Ciphertexts can be saved and loaded from files
ciphertexts[0].save("cipher.seal");
const new_cipher = new seal.Ciphertext("cipher.seal");

// Evaluator is used to perform homomorphic computations on Ciphertext
const evaluator = new seal.Evaluator(context);

// Let's try adding two numbers
// The function addInPlace overwrites the first argument
evaluator.addInPlace(ciphertexts[0], ciphertexts[1]);
evaluator.addInPlace(ciphertexts[0], ciphertexts[2]);

// If we have the secret key, we can recover the Plaintext using a Decryptor
const decryptor = new seal.Decryptor(context, sk);

// The Plaintext class can also accept a decryptor/ciphertext pair
const result = new seal.Plaintext(decryptor, ciphertexts[0]);

// Decode the Plaintext object into an integer
console.log(encoder.decode(result));

////////////////////

// Let's try it a different way
const plaintexts2 = [
    new seal.Plaintext(encoder, 24),
    new seal.Plaintext(encoder, -9),
    new seal.Plaintext(encoder, 1)
];

// Encrypt each of our plaintexts
const ciphertexts2 = plaintexts.map((plain) => {
    return new seal.Ciphertext(encryptor, plain);
});

// You can also sum an array of ciphertexts using addMany
const sum = new seal.Ciphertext();
evaluator.addMany(ciphertexts2, sum);

const result2 = new seal.Plaintext(decryptor, sum);
console.log(encoder.decode(result2));

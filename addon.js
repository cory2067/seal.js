var sealjs = require('node-cmake')('sealjs');

const context = new sealjs.SEALContext(2048, 128, 65536);
const keygen = new sealjs.KeyGenerator(context);
const pk = new sealjs.PublicKey(keygen);
const sk = new sealjs.SecretKey(keygen);

pk.save('public.key')
sk.save('secret.key')

/*
var obj = new addon.MyObject(10);
console.log( obj.plusOne() ); // 11
console.log( obj.plusOne() ); // 12
console.log( obj.plusOne() ); // 13

console.log( obj.multiply().value() ); // 13
console.log( obj.multiply(10).value() ); // 130

var newobj = obj.multiply(-1);
console.log( newobj.value() ); // -13
console.log( obj === newobj ); // false

console.log(obj)

var a = {meme: "hi there im pasing stuf"};
console.log(obj.lookAt(newobj))
*/

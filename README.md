# seal.js
JavaScript bindings for SEAL homomorphic encryption library

# Setup
- Download SEAL 3.0, add the following line to the CMakeLists.txt before installing: `set(CMAKE_POSITION_INDEPENDENT_CODE ON)`
- Run `npm install`
- Run `npm run generate-cmake`
- Run `npm run rebuild`
- To try example, run `node .`

# Usage
View the example at `seal-example.js` for usage of this node addon. The examples showcase all currently-supported features.

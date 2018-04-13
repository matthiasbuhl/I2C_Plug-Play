# I2C_Plug-Play

The folders offer demo codes, that were used to prove a plug-and-play solution for I2C. They enable smart sensors to connect to a node/gate through I2C and be configured with respect to an allocated i2c address and a description of the provided services to the network. 

As indicated, the codes are a work in progress and currently only demonstrate the mechanism to discover and describe connected devices. The intended transfer of a JSON-formatted description couldn't be tested sufficiently, because the Arduino I2C-Wire Library didn't allow for datatransfer bigger than 32byte. 

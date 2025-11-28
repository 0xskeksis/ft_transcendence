import "@nomicfoundation/hardhat-toolbox";
import dotenv from "dotenv";

dotenv.config();

export default {
  solidity: "0.8.24",
  networks: {
    fuji: {
      url: process.env.AVALANCHE_FUJI_RPC,
      accounts: [process.env.PRIVATE_KEY_WALLET],
    },
  },
};

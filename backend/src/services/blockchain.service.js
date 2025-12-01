import { ethers } from "ethers";
import dotenv from "dotenv";

dotenv.config();

const CONTRACT_ADDRESS = "0x88Ba24e34dd5e581832C685142aA97fC12eFe2aE";

const ABI = [
    "function saveScore(uint256 _gameId, uint256 _p1Id, uint256 _p2Id, uint256 _s1, uint256 _s2) external"
];

let contract = null;

try {
    const provider = new ethers.JsonRpcProvider(process.env.AVALANCHE_FUJI_RPC);
    const wallet = new ethers.Wallet(process.env.PRIVATE_KEY_WALLET, provider);
    contract = new ethers.Contract(CONTRACT_ADDRESS, ABI, wallet);
    console.log("✅ Blockchain connectée !");
} catch (e) {
    console.error("❌ Erreur connexion Blockchain :", e);
}

export async function saveScoreToBlockchain(gameId, p1Id, p2Id, score1, score2) {
    if (!contract) return;
    try {
        console.log(`🔗 Envoi Blockchain : Game ${gameId} | ${score1}-${score2}`);
        const tx = await contract.saveScore(gameId, p1Id, p2Id, score1, score2);
        console.log(`⏳ Transaction envoyée ! Hash : ${tx.hash}`);
        console.log(`👉 Voir sur Snowtrace : https://testnet.snowtrace.io/tx/${tx.hash}`);
        return tx.hash;
    } catch (err) {
        console.error("❌ Erreur transaction :", err);
        return null;
    }
}

import { getGameData, sendInput, setBallPos, createGame, startGame } from '../controllers/pong.controllers.js';
import { saveScoreToBlockchain } from '../services/blockchain.service.js';

async function pong_routes (fastify, options) {
    
    fastify.get('/pong/get-data', async (req, rep) => getGameData(req, rep));
    fastify.post('/pong/post-input', async (req, rep) => sendInput(req, rep));
    fastify.post('/pong/debug/set-ball', async (req, rep) => setBallPos(req, rep));
    fastify.post('/pong/create-game', async (req, rep) => createGame(req, rep));
    fastify.post('/pong/start-game', async (req, rep) => startGame(req, rep));

    fastify.get('/pong/test-blockchain', async (request, reply) => {
        console.log("🧪 Test manuel de la Blockchain lancé...");
        
        // Simulation : Game 999 | J1: 42 | J2: 21 | Score: 10-5
        const txHash = await saveScoreToBlockchain(999, 42, 21, 10, 5);
        
        if (txHash) {
            return { 
                status: "Succès", 
                hash: txHash, 
                link: `https://testnet.snowtrace.io/tx/${txHash}` 
            };
        } else {
            return reply.status(500).send({ error: "Erreur Blockchain" });
        }
    })
}

export default pong_routes;

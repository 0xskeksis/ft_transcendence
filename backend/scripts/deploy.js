import hre from "hardhat";

async function main() {
  console.log("Déploiement en cours sur Avalanche Fuji...");
  const TournamentStorage = await hre.ethers.getContractFactory("TournamentStorage");
  const storage = await TournamentStorage.deploy();
  await storage.waitForDeployment();
  console.log(`✅ Contrat déployé ! Adresse : ${storage.target}`);
}

main().catch((error) => {
  console.error(error);
  process.exit(1);
});

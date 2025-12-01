import Database from 'better-sqlite3'
import path from 'path'
import fs from 'fs'

let dbPath = '/app/data/database.db';

if (!fs.existsSync('/app/data')) {
    dbPath = path.resolve(process.cwd(), '../data/sqlite/database.db');
    console.log(`💻 Mode Local détecté. Base de données : ${dbPath}`);
}

const dbDir = path.dirname(dbPath);
if (!fs.existsSync(dbDir)){
    fs.mkdirSync(dbDir, { recursive: true });
}

const db = new Database(dbPath)
db.pragma("journal_mode = WAL")
db.pragma("foreign_keys = ON")

export default db;

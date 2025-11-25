import Database from 'better-sqlite3'
import path from 'path'

const dbPath = path.resolve('/app/data/database.db'); // chemin absolu


const db = new Database('/data/database.db')
db.pragma("journal_mode = WAL") //Performances reasons
db.pragma("foreign_keys = ON")

export default db;

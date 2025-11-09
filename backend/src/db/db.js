import Database from 'better-sqlite3'
const db = new Database('db/database.db')
db.pragma("journal_mode = WAL") //Performances reasons
db.pragma("foreign_keys = ON")

export default db;

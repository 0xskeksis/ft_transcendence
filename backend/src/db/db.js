import Database from 'better-sqlite3'
import path from 'path'



const db = new Database('/app/data/database.db')
db.pragma("journal_mode = WAL") //Performances reasons
db.pragma("foreign_keys = ON")

export default db;

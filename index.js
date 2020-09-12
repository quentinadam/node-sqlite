"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const sqlite = require('./build/Release/sqlite');
class Database {
    constructor(name) {
        this.handle = sqlite.open(name);
    }
    query(sql, ...parameters) {
        return sqlite.query(this.handle, sql, parameters);
    }
    close() {
        sqlite.close(this.handle);
    }
}
exports.default = Database;

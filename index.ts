const sqlite = require('./build/Release/sqlite');

export default class Database {

  private handle;

  constructor(name: string) {
    this.handle = sqlite.open(name);
  }

  query<T extends { [key: string]: string | number | null | Buffer } = { [key: string]: string | number | null | Buffer }>(sql: string, ...parameters: (string | number | boolean | Buffer | null | undefined)[]) {
    return sqlite.query(this.handle, sql, parameters) as T[];
  }

  close() {
    sqlite.close(this.handle);
  }
}

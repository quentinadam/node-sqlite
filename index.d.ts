export default class Database {
  constructor(name: string);
  query(sql: string, ...parameters: (string | number | boolean | Buffer | null | undefined)[]): {
    [key: string]: string | number | null | Buffer;
  }[];
  close(): void;
}

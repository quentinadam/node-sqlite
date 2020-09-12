/// <reference types="node" />
export default class Database {
    private handle;
    constructor(name: string);
    query<T extends {
        [key: string]: string | number | null | Buffer;
    } = {
        [key: string]: string | number | null | Buffer;
    }>(sql: string, ...parameters: (string | number | boolean | Buffer | null | undefined)[]): T[];
    close(): void;
}

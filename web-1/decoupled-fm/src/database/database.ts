const DB_NAME = 'DecoupledDB';
const DB_VERSION = 1;
const SONGS_STORE_NAME = 'songs';

export interface Song {
  id: number; // Optional because it's auto-incremented by IndexedDB
  name: string; // Original filename
  title: string; // Extracted title
  artist: string; // Extracted artist
  album?: string; // Extracted album
  duration?: number; // Duration in seconds
  data: ArrayBuffer;
  addedAt: Date;
}

export const openDatabase = (): Promise<IDBDatabase> => {
  return new Promise((resolve, reject) => {
    const request: IDBOpenDBRequest = indexedDB.open(DB_NAME, DB_VERSION);

    request.onupgradeneeded = (event) => {
      const db: IDBDatabase = (event.target as IDBOpenDBRequest).result;
      if (!db.objectStoreNames.contains(SONGS_STORE_NAME)) {
        db.createObjectStore(SONGS_STORE_NAME, { keyPath: 'id', autoIncrement: true })
      }
    }

    request.onsuccess = (event: Event) => {
      resolve((event.target as IDBOpenDBRequest).result);
    }

    request.onerror = (event: Event) => {
      console.error('IndexedDB error:', (event.target as IDBRequest).error);
      reject('Failed to open IndexedDB');
    };
  })
};

export const addSongToDB = async (songData: Omit<Song, 'id' | 'addedAt'>): Promise<void> => {
  const db: IDBDatabase = await openDatabase();
  return new Promise((resolve, reject) => {
    const transaction: IDBTransaction = db.transaction([SONGS_STORE_NAME], 'readwrite');
    const store: IDBObjectStore = transaction.objectStore(SONGS_STORE_NAME);
    const request: IDBRequest = store.add({ ...songData, addedAt: new Date() });

    request.onsuccess = () => resolve();

    request.onerror = (_: Event) => reject("Failed to add song");
  });
};

export const getSongsFromDB = async(): Promise<Song[]> => {
  const db: IDBDatabase = await openDatabase();
  return new Promise((resolve, reject) => {
    const transaction: IDBTransaction = db.transaction([SONGS_STORE_NAME], 'readonly');
    const store: IDBObjectStore = transaction.objectStore(SONGS_STORE_NAME);
    const request: IDBRequest<Song[]> = store.getAll();

    request.onsuccess = (event: Event) => resolve((event.target as IDBRequest<Song[]>).result);

    request.onerror = (_: Event) => reject("Failed to get songs");
  })
};

export const deleteSongFromDB = async (id: number): Promise<void> => {
  const db: IDBDatabase = await openDatabase();
  return new Promise((resolve, reject) => {
    const transaction: IDBTransaction = db.transaction([SONGS_STORE_NAME], 'readonly');
    const store: IDBObjectStore = transaction.objectStore(SONGS_STORE_NAME);
    const request: IDBRequest = store.delete(id);

    request.onsuccess = () => resolve();

    request.onerror = (_: Event) => reject("Failed to delete song");
  });
};
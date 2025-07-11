const DB_NAME = 'DecoupledDB';
const DB_VERSION = 2;
const SONGS_STORE_NAME = 'songs';
const PLAYLISTS_STORE_NAME = 'playlists';

export interface Song {
  id: number;
  name: string; // Original filename
  title: string; // Extracted title
  artist: string; // Extracted artist
  album?: string; // Extracted album
  duration?: number; // Duration in seconds
  imageUrl?: string;
  data: ArrayBuffer;
  addedAt: Date;
}

export interface Playlist {
  id: number;
  name: string;
  songIds: number[];
  createdAt: Date;
}

export const openDatabase = (): Promise<IDBDatabase> => {
  return new Promise((resolve, reject) => {
    const request: IDBOpenDBRequest = indexedDB.open(DB_NAME, DB_VERSION);

    request.onupgradeneeded = (event) => {
      const db: IDBDatabase = (event.target as IDBOpenDBRequest).result;

      if (!db.objectStoreNames.contains(SONGS_STORE_NAME)) {
        db.createObjectStore(SONGS_STORE_NAME, { keyPath: 'id', autoIncrement: true });
      }

      if (!db.objectStoreNames.contains(PLAYLISTS_STORE_NAME)) {
        db.createObjectStore(PLAYLISTS_STORE_NAME, { keyPath: 'id', autoIncrement: true });
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
    const transaction: IDBTransaction = db.transaction(
      [SONGS_STORE_NAME, PLAYLISTS_STORE_NAME],
      'readwrite'
    );

    // Delete from songs store
    const songsStore: IDBObjectStore = transaction.objectStore(SONGS_STORE_NAME);
    const songsRequest: IDBRequest = songsStore.delete(id);

    // Remove from all playlists
    const playlistsStore: IDBObjectStore = transaction.objectStore(PLAYLISTS_STORE_NAME);
    const playlistsRequest: IDBRequest = playlistsStore.getAll();

    playlistsRequest.onsuccess = () => {
      const playlists = playlistsRequest.result;
      const updatePromises: Promise<void>[] = [];

      playlists.forEach((playlist: Playlist) => {
        if (playlist.songIds.includes(id)) {
          const updatedPlaylist = {
            ...playlist,
            songIds: playlist.songIds.filter((songId: number) => songId !== id)
          };
          
          const updateRequest = playlistsStore.put(updatedPlaylist);
          updatePromises.push(
            new Promise<void>((res, rej) => {
              updateRequest.onsuccess = () => res();
              updateRequest.onerror = () => rej();
            })
          );
        }
      });

      // Wait for all playlist updates to complete
      Promise.all(updatePromises)
        .then(() => {
          // Resolve after both song deletion and playlist updates complete
          transaction.oncomplete = () => resolve();
        })
        .catch(() => reject("Failed to update playlists"));
    };

    songsRequest.onerror = () => reject("Failed to delete song");
    playlistsRequest.onerror = () => reject("Failed to load playlists");
  });
};

export const addPlaylistToDB = async (name: string): Promise<void> => {
  const db: IDBDatabase = await openDatabase();
  return new Promise((resolve, reject) => {
    const transaction: IDBTransaction = db.transaction([PLAYLISTS_STORE_NAME], 'readwrite');
    const store: IDBObjectStore = transaction.objectStore(PLAYLISTS_STORE_NAME);
    const request: IDBRequest = store.add({ name, songIds: [], createdAt: new Date() }); // New playlist starts with no songs

    request.onsuccess = () => {
      console.log('Playlist added to IndexedDB:', name);
      resolve();
    };

    request.onerror = (event: Event) => {
      console.error('Error adding playlist to IndexedDB:', (event.target as IDBRequest).error);
      reject('Failed to add playlist');
    };
  });
};

export const getPlaylistsFromDB = async (): Promise<Playlist[]> => {
  const db: IDBDatabase = await openDatabase();
  return new Promise((resolve, reject) => {
    const transaction: IDBTransaction = db.transaction([PLAYLISTS_STORE_NAME], 'readonly');
    const store: IDBObjectStore = transaction.objectStore(PLAYLISTS_STORE_NAME);
    const request: IDBRequest<Playlist[]> = store.getAll();

    request.onsuccess = (event: Event) => {
      resolve((event.target as IDBRequest<Playlist[]>).result);
    };

    request.onerror = (event: Event) => {
      console.error('Error getting playlists from IndexedDB:', (event.target as IDBRequest).error);
      reject('Failed to get playlists');
    };
  });
};

export const updatePlaylistInDB = async (playlist: Playlist): Promise<void> => {
  const db: IDBDatabase = await openDatabase();
  return new Promise((resolve, reject) => {
    const transaction: IDBTransaction = db.transaction([PLAYLISTS_STORE_NAME], 'readwrite');
    const store: IDBObjectStore = transaction.objectStore(PLAYLISTS_STORE_NAME);
    const request: IDBRequest = store.put(playlist); // Use put to update existing or add new

    request.onsuccess = () => {
      console.log('Playlist updated in IndexedDB:', playlist.name);
      resolve();
    };

    request.onerror = (event: Event) => {
      console.error('Error updating playlist in IndexedDB:', (event.target as IDBRequest).error);
      reject('Failed to update playlist');
    };
  });
};

export const deletePlaylistFromDB = async (id: number): Promise<void> => {
  const db: IDBDatabase = await openDatabase();
  return new Promise((resolve, reject) => {
    const transaction: IDBTransaction = db.transaction([PLAYLISTS_STORE_NAME], 'readwrite');
    const store: IDBObjectStore = transaction.objectStore(PLAYLISTS_STORE_NAME);
    const request: IDBRequest = store.delete(id);

    request.onsuccess = () => {
      console.log('Playlist deleted from IndexedDB:', id);
      resolve();
    };

    request.onerror = (event: Event) => {
      console.error('Error deleting playlist from IndexedDB:', (event.target as IDBRequest).error);
      reject('Failed to delete playlist');
    };
  });
};
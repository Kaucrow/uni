import { auth, db } from "./firebase";
import { onAuthStateChanged } from "firebase/auth";
import { arrayUnion, doc, getDoc, setDoc, updateDoc } from "firebase/firestore";
import type { NoteType } from "./types";

let uid: string | null = null;

let authListenerInitialized = false;

export function initAuthListener() {
  if (authListenerInitialized) return;

  onAuthStateChanged(auth, (user) => {
    uid = user ? user.uid : null;
    console.log('User ID updated:', uid);
  });

  authListenerInitialized = true;
}

// Initialize the listener when this module loads
initAuthListener();

export const initializeUserDocument = async (userData: any) => {
  if (!uid) {
    throw Error("No authenticated user");
  }

  const userDocRef = doc(db, "users", uid);

  try {
    // Check if document exists
    const docSnap = await getDoc(userDocRef);
    
    if (!docSnap.exists()) {
      // Document doesn't exist - create it
      await setDoc(userDocRef, {
        ...userData,
        createdAt: new Date(),
        lastLogin: new Date()
      });
      console.log("New user document created");
    } else {
      // Document exists - update last login
      await updateDoc(userDocRef, { lastLogin: new Date(), });
      console.log("Existing user document updated");
    }
  } catch (err) {
    console.error("Error handling user document:", err);
  }
}

export const createNote = async (color: string) => {
  if (!uid) {
    throw Error("No authenticated user");
  }

  const userDocRef = doc(db, "users", uid);

  try {
    const userDoc = await getDoc(userDocRef);

    if (!userDoc.exists()) {
      throw new Error("User document not found");
    }

    const userData = userDoc.data();
    const currentNotes = userData?.notes || [];
    const newId = currentNotes.length.toString(); // Use array length as ID

    const newNote = {
      id: newId,
      title: "",
      content: "",
      color,
      dtCreated: new Date(),
    };

    await updateDoc(userDocRef, {
      notes: arrayUnion(newNote),
    });
  } catch (err) {
    console.error("Error adding note: ", err);
  }
};

export const getNotes = async (): Promise<Array<NoteType>> => {
  if (!uid) {
    throw new Error("No authenticated user");
  }

  const userDocRef = doc(db, "users", uid);

  try {
    const userDoc = await getDoc(userDocRef);

    if (!userDoc.exists()) {
      throw new Error("User document not found");
    }

    const userData = userDoc.data();

    const notes = userData?.notes || [];

    const processedNotes = notes.map((note: any) => ({
      ...note,
      dtCreated: note.dtCreated.toDate()
    }));

    return processedNotes as NoteType[];
  } catch (err) {
    console.error("Error fetching notes: ", err);
    throw err;
  }
};

export const saveNote = async (
  id: number,
  title: string,
  content: string
) => {
  if (!uid) {
    throw new Error("No authenticated user");
  }

  const userDocRef = doc(db, "users", uid);

  try {
    const userDoc = await getDoc(userDocRef);

    if (!userDoc.exists()) {
      throw new Error("User document not found");
    }

    const userData = userDoc.data();
    const notes = userData.notes || [];

    const noteIndex = notes.findIndex((note: any) => note.id === id);

    if (noteIndex === -1) {
      throw new Error(`Note with id ${id} not found`);
    }

    const updatedNote = {
      ...notes[noteIndex],
      title,
      content
    };

    const updatedNotes = [...notes];
    updatedNotes[noteIndex] = updatedNote;

    await updateDoc(userDocRef, {
      notes: updatedNotes
    });
  } catch (err) {
    console.error("Error saving note: ", err);
  }
}

export const deleteNote = async (id: number) => {
  if (!uid) {
    throw new Error("No authenticated user");
  }

  const userDocRef = doc(db, "users", uid);

  try {
    const userDoc = await getDoc(userDocRef);
    if (!userDoc.exists()) {
      throw new Error("User document not found");
    }

    const userData = userDoc.data();
    let notes: NoteType[] = userData.notes || [];

    // Filter out the note to delete and adjust IDs
    notes = notes
      .filter(note => note.id !== id) // Remove the deleted note
      .map(note => ({
        ...note,
        id: note.id > id ? note.id - 1 : note.id // Decrement higher IDs
      }));

    await updateDoc(userDocRef, { notes });
  } catch (err) {
    console.error("Error deleting note:", err);
  }
};
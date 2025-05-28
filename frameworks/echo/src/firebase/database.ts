import { auth, db } from "./firebase";
import { onAuthStateChanged } from "firebase/auth";
import { arrayUnion, doc, getDoc, setDoc, updateDoc } from "firebase/firestore";

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

export const createNote = async () => {
  if (!uid) {
    throw Error("No authenticated user");
  }

  const userDocRef = doc(db, "users", uid);

  try {
    const docSnap = await getDoc(userDocRef);

    const newNote = {
      content: "Hello World",
      dtCreated: new Date(),
    };

    if (docSnap.exists()) {
      await updateDoc(userDocRef, {
        notes: arrayUnion(newNote),
      })
    } else {
      throw Error("User has no document");
    }
  } catch (err) {
    console.error("Error adding note: ", err);
  }
};
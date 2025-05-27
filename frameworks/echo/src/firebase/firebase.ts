import { initializeApp } from "firebase/app";
import { getAuth } from "firebase/auth";
import { getFirestore } from "firebase/firestore";

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyAboIUdGMacI43CZjaUIR8YTJPkKQ9v0uQ",
  authDomain: "echoes-bd8a0.firebaseapp.com",
  projectId: "echoes-bd8a0",
  storageBucket: "echoes-bd8a0.firebasestorage.app",
  messagingSenderId: "981899224862",
  appId: "1:981899224862:web:b33f587de3fa5500cf0b0f",
  measurementId: "G-TR99HPHBT3"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const auth = getAuth(app);
const db = getFirestore(app);

export { app, auth, db };
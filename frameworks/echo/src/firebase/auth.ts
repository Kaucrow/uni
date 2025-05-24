import { auth } from "./firebase";

import {
  createUserWithEmailAndPassword,
  signInWithEmailAndPassword,
  GoogleAuthProvider,
  sendPasswordResetEmail,
  signInWithPopup,
  updatePassword,
  sendEmailVerification,
  reauthenticateWithCredential,
  deleteUser,
  AuthCredential,
  type User
} from "firebase/auth";

export const doCreateUserWithEmailAndPassword = async (
  email: string,
  password: string
) => {
  return createUserWithEmailAndPassword(auth, email, password);  
};

export const doSignInWithEmailAndPassword = (
  email: string,
  password: string
) => {
  console.log('signing in');
  return signInWithEmailAndPassword(auth, email, password);
};

export const doSignInWithGoogle = async () => {
  const provider = new GoogleAuthProvider();
  const result = await signInWithPopup(auth, provider);
  return result;
};

export const doSignOut = () => {
  return auth.signOut();
};

export const doReauthenticateWithCredential = async (
  user: User | null,
  credential: AuthCredential
) => {
  if (!user) {
    return Promise.reject(new Error("No authenticated user"));
  }
  return reauthenticateWithCredential(user, credential);
}

export const doPasswordReset = (email: string) => {
  return sendPasswordResetEmail(auth, email);
};

export const doPasswordChange = (password: string) => {
  if (!auth.currentUser) {
    return Promise.reject(new Error("No authenticated user"));
  }
  return updatePassword(auth.currentUser, password);
};

export const doDeleteAccount = async (user: User | null) => {
  if (!user) {
    return Promise.reject(new Error("No authenticated user"));
  }
  return deleteUser(user);
}

export const doSendEmailVerification = () => {
  if (!auth.currentUser) {
    return Promise.reject(new Error("No authenticated user"));
  }
  return sendEmailVerification(auth.currentUser, {
    url: `${window.location.origin}/home`,
  })
}
import {
  BrowserRouter,
  Routes,
  Route,
  Outlet
} from 'react-router-dom';

import { AuthProvider } from './contexts/authContext';

import Navbar from './layouts/Navbar';

import NotFound from './pages/not-found/NotFound';
import Home from './pages/home/Home';
import Login from './pages/auth/login/Login';
import Register from './pages/auth/register/Register';
import Profile from './pages/profile/Profile';
import Notes from './pages/notes/Notes';

import "./App.css";

const App = () => {
  return (
    <AuthProvider>
      <BrowserRouter>
        <Routes>
          <Route element={(
            <div>
              <Navbar />
              <Outlet />
            </div>
          )}>
            <Route
              path="/"
              element={ <Home /> }
            />
            <Route
              path="/auth/login"
              element={ <Login /> }
            />
            <Route
              path="/auth/register"
              element={ <Register /> }
            />
            <Route
              path="/user"
              element={ <Profile /> }
            />
            <Route
              path="/notes"
              element={ <Notes /> }
            />
            <Route
              path="/about"
              element={ <h1 style={{ height:'4px' }}>About</h1> }
            />
            <Route
              path="*"
              element={ <NotFound /> }
            />
          </Route>
        </Routes>
    </BrowserRouter>
  </AuthProvider>
  )
}

export default App
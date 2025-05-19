import {
  BrowserRouter,
  Routes,
  Route,
  Outlet
} from 'react-router-dom';

import Navbar from './layouts/Navbar';

import NotFound from './pages/not-found/NotFound';
import Home from './pages/home/Home';

import "./App.css";

const App = () => {
  return (
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
  )
}

export default App
import {
  BrowserRouter,
  Routes,
  Route,
  Outlet
} from 'react-router-dom';

import Home from './pages/home/Home';

import './App.css'

const App = () => {
  return (
    <BrowserRouter>
      <Routes>
        <Route element={(
          <div>
            <Outlet />
          </div>
        )}>
          <Route
            path="/"
            element={ <Home/> }
          />
        </Route>
      </Routes>
    </BrowserRouter>
  )
}

export default App
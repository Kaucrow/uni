import app from './app.js';
import { server } from './constants/constants.js';

app.listen(server.port, () => {
  console.log(`Server listening on port ${server.port}`);
});
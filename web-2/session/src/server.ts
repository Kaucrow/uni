import app from './app.ts';
import { PORT } from './config/constants.ts';

app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});
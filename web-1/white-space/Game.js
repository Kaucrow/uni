import { adjustCanvas } from "./canvas.js";
import { WhiteSpace } from "./rooms/WhiteSpace.js";
import { AudioPlayer } from "./AudioPlayer.js";

adjustCanvas();

export const audioPlayer = new AudioPlayer();

const room = new WhiteSpace();
let lastTime = 0;

async function gameLoop() {
  const currentTime = performance.now();
  const deltaTime = Math.min(((currentTime - lastTime) / 1000), 1/30); // Convert to seconds

  room.update(deltaTime);

  lastTime = currentTime;

  requestAnimationFrame(gameLoop);
}

document.addEventListener('DOMContentLoaded', () => {
  gameLoop();
});

window.addEventListener('resize', () => {
  adjustCanvas();
  room.adjustCanvas();
});
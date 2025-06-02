import { adjustCanvas } from "./canvas.js";
import { Room } from "./Room.js";

adjustCanvas();

let audioContext = null;
let bgMusicSource = null;
let isAudioPlaying = false;

// Set up audio toggle
await setupAudioToggle();

async function setupAudioToggle() {
  const audioToggle = document.getElementById('audioToggle');
  
  audioToggle.addEventListener('click', async () => {
    if (!audioContext) {
      audioContext = new (window.AudioContext || window.webkitAudioContext)();
    }

    const icon = document.getElementById("audioToggleIcon");

    if (isAudioPlaying) {
      // Stop audio
      bgMusicSource.stop();
      bgMusicSource = null;
      icon.innerHTML = `<g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8" fill="#059cf7" opacity="0.1" stroke-width="0"></polygon><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8"></polygon><line x1="17" y1="9.5" x2="22" y2="14.5"></line><line x1="17" y1="14.5" x2="22" y2="9.5"></line></g>`;
    } else {
      // Start audio
      try {
        const playSound = await loadAndLoopSound(room.bgMusic);
        bgMusicSource = playSound();
        icon.innerHTML = `<g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8" fill="#059cf7" opacity="0.1" stroke-width="0"></polygon><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8"></polygon><path d="M17.72,5.55a7,7,0,0,1,0,12.9"></path><path d="M17.12,9.88a3,3,0,0,1,0,4.24"></path></g>`
      } catch (error) {
        console.error("Audio error:", error);
        icon.innerHTML = `<g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8" fill="#059cf7" opacity="0.1" stroke-width="0"></polygon><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8"></polygon><line x1="17" y1="9.5" x2="22" y2="14.5"></line><line x1="17" y1="14.5" x2="22" y2="9.5"></line></g>`;
        return;
      }
    }
    isAudioPlaying = !isAudioPlaying;
  });
}

async function loadAndLoopSound(data) {
  const response = await fetch(data.url);
  const arrayBuffer = await response.arrayBuffer();
  const audioBuffer = await audioContext.decodeAudioData(arrayBuffer);

  return () => {
    const source = audioContext.createBufferSource();
    source.buffer = audioBuffer;
    source.connect(audioContext.destination);
    source.loop = true;
    source.loopEnd = data.loopEnd;
    source.start();
    return source;
  };
}

const room = new Room();
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
})

window.addEventListener('resize', () => {
  adjustCanvas();
  room.adjustCanvas();
})
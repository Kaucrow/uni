// Canvas setup
export const canvas = document.getElementById('gameCanvas');
export const ctx = canvas.getContext('2d');

export function adjustCanvas() {
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;

  const displayWidth = canvas.clientWidth;
  const displayHeight = canvas.clientHeight;

  const dpr = window.devicePixelRatio || 1;
  canvas.width = displayWidth * dpr;
  canvas.height = displayHeight * dpr;

  ctx.scale(dpr, dpr);

  ctx.imageSmoothingEnabled = false;
  ctx.msImageSmoothingEnabled = false;
  ctx.mozImageSmoothingEnabled = false;
  ctx.webkitImageSmoothingEnabled = false;
}
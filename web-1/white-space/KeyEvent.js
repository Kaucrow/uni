export class KeyEvent {
  constructor(key, callback) {
    this.enabled = true;
    this.key = key;
    this.callback = callback;
  }
}
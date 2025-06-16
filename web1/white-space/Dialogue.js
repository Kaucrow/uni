export class Dialogue {
  static SPEED = {
    FAST: 450,
    NORMAL: 300,
    SLOW: 150,
    VERYSLOW: 50,
    ULTRASLOW: 25,
  };

  constructor(text, speed) {
    if (typeof text !== 'string') throw new Error('Type of dialogue text must be `string` you dummy');

    this.text = text;
    this.speed = speed;
  }
}
export class AudioPlayer {
  constructor() {
    this.audioContext = null;
    this.buffers = new Map(); // Stores both arrayBuffers and decoded audioBuffers
    this.activeSources = new Map();
    this.pausedSources = new Map();
    this.globalVolume = 1.0;
    this.isMuted = false;
    this.isAudioPlaying = false;
    this.pendingOperations = new Map();
    
    // Audio toggle elements
    this.audioToggle = document.getElementById('audioToggle');
    this.audioToggleIcon = document.getElementById('audioToggleIcon');
    
    // SVG icons
    this.muteIcon = `<g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8" fill="#059cf7" opacity="0.1" stroke-width="0"></polygon><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8"></polygon><line x1="17" y1="9.5" x2="22" y2="14.5"></line><line x1="17" y1="14.5" x2="22" y2="9.5"></line></g>`;
    this.unmuteIcon = `<g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8" fill="#059cf7" opacity="0.1" stroke-width="0"></polygon><polygon points="2 8 2 16 6 16 13 21 13 3 6 8 2 8"></polygon><path d="M17.72,5.55a7,7,0,0,1,0,12.9"></path><path d="M17.12,9.88a3,3,0,0,1,0,4.24"></path></g>`;

    this.#setupAudioToggle();
  }

  #setupAudioToggle() {
    this.audioToggle.addEventListener('click', async () => {
      try {
        // Initialize AudioContext on first click if needed
        if (!this.audioContext) {
          this.audioContext = new (window.AudioContext || window.webkitAudioContext)();
          await this.#processPendingOperations();
        }

        // Handle suspended state
        if (this.audioContext.state === 'suspended') {
          await this.audioContext.resume();
        }

        if (this.isAudioPlaying) {
          // Pause all audio
          this.pauseAll();
          this.audioToggleIcon.innerHTML = this.muteIcon;
        } else {
          // Resume playback
          this.resumeAll();
          this.audioToggleIcon.innerHTML = this.unmuteIcon;
        }
      } catch (error) {
        console.error("Audio toggle error:", error);
        this.isAudioPlaying = false;
        this.audioToggleIcon.innerHTML = this.muteIcon;
      }
    });
  }

  async #processPendingOperations() {
    // Decode all preloaded sounds
    const decodePromises = [];
    this.buffers.forEach((value, key) => {
      if (!value.decoded && value.arrayBuffer) {
        decodePromises.push(
          this.audioContext.decodeAudioData(value.arrayBuffer)
            .then(audioBuffer => {
              this.buffers.set(key, { audioBuffer, decoded: true });
            })
            .catch(error => {
              console.error(`Failed to decode sound ${key}:`, error);
            })
        );
      }
    });

    await Promise.all(decodePromises);

    const operationsToProcess = Array.from(this.pendingOperations.values());
    this.pendingOperations.clear();

    // Execute pending play operations
    for (const op of operationsToProcess) {
      try {
        await op.callback();
      } catch (error) {
        console.error("Pending operation failed:", error);
      }
    }
  }

  #playInternal(key, { loop, volume, loopStart, loopEnd, offset = 0 } = {}) {
    if (this.isMuted) return null;

    const bufferData = this.buffers.get(key);
    if (!bufferData?.audioBuffer) {
      throw new Error(`Sound ${key} not decoded`);
    }

    this.stop(key); // Stop any existing playback

    const source = this.audioContext.createBufferSource();
    const gainNode = this.audioContext.createGain();
    
    source.buffer = bufferData.audioBuffer;
    source.loop = loop;
    if (loopEnd) {
      source.loopStart = loopStart;
      source.loopEnd = loopEnd;
    }

    gainNode.gain.value = volume * this.globalVolume;
    
    source.connect(gainNode);
    gainNode.connect(this.audioContext.destination);
    
    source.start(0, offset); // Start from specified offset
    
    const sourceId = Symbol();
    this.activeSources.set(key, { source, gainNode, id: sourceId, startTime: this.audioContext.currentTime, offset });
    
    if (!loop) {
      source.onended = () => {
        if (this.activeSources.get(key)?.id === sourceId) {
          this.activeSources.delete(key);
        }
      };
    }

    return sourceId;
  }

  async load(key, url) {
    if (this.buffers.has(key)) return;

    try {
      const response = await fetch(url);
      const arrayBuffer = await response.arrayBuffer();
  
      if (this.audioContext) {
        const audioBuffer = await this.audioContext.decodeAudioData(arrayBuffer);
        this.buffers.set(key, { audioBuffer, decoded: true });
      } else {
        this.buffers.set(key, { arrayBuffer, decoded: false });
      }
    } catch (error) {
      console.error(`Failed to preload sound ${key}:`, error);
      throw error;
    }
  }

  playOrQueue(key, { loop = false, volume = 1.0, loopStart = 0, loopEnd } = {}) {
    if (!this.audioContext) {
      // Queue the play operation if no context exists yet
      return new Promise((resolve) => {
        const callback = () => {
          this.pendingOperations.delete(key); // Remove when executed
          resolve(this.playOrQueue(key, { loop, volume, loopStart, loopEnd }));
        };

        this.pendingOperations.set(key, {
          callback,
          cancel: () => resolve(null) // Resolve with null if cancelled
        });
      });
    }

    return this.#playInternal(key, { loop, volume, loopStart, loopEnd });
  }

  play(key, { loop = false, volume = 1.0, loopStart = 0, loopEnd } = {}) {
    if (!this.audioContext || !this.buffers.has(key)) {
      return null;
    }

    const bufferData = this.buffers.get(key);
    if (!bufferData.decoded) {
      // If not decoded but we have context, decode synchronously
      try {
        const audioBuffer = this.audioContext.decodeAudioData(bufferData.arrayBuffer);
        this.buffers.set(key, { audioBuffer, decoded: true });
        return this.#playInternal(key, { loop, volume, loopStart, loopEnd });
      } catch (error) {
        console.error(`Failed to decode sound ${key}:`, error);
        return null;
      }
    }

    return this.#playInternal(key, { loop, volume, loopStart, loopEnd });
  }

  pauseAll() {
    // Record current playback positions before stopping
    this.activeSources.forEach((sourceInfo, key) => {
      const elapsed = this.audioContext.currentTime - sourceInfo.startTime;
      const newOffset = sourceInfo.offset + elapsed;
      this.pausedSources.set(key, {
        loop: sourceInfo.source.loop,
        volume: sourceInfo.gainNode.gain.value / this.globalVolume,
        loopStart: sourceInfo.source.loopStart,
        loopEnd: sourceInfo.source.loopEnd,
        offset: newOffset
      });
      
      // Stop the source but keep its info for resuming
      sourceInfo.source.stop();
      sourceInfo.source.disconnect();
      sourceInfo.gainNode.disconnect();
    });

    this.activeSources.clear();
    this.isAudioPlaying = false;
  }

  resumeAll() {
    this.pausedSources.forEach((params, key) => {
      this.#playInternal(key, params);
    });
    this.pausedSources.clear();
    this.isAudioPlaying = true;
  }

  stop(key) {
    // Stop active playback
    const sources = this.activeSources.get(key);
    if (sources) {
      try {
        sources.source.stop();
        sources.source.disconnect();
        sources.gainNode.disconnect();
      } catch (e) {
        console.warn(`Error stopping sound ${key}:`, e);
      }
      this.activeSources.delete(key);
    }
    
    // Clear any paused state
    this.pausedSources.delete(key);

    // Cancel any pending operations for this key
    const pendingOp = this.pendingOperations.get(key);
    if (pendingOp) {
      pendingOp.cancel();
      this.pendingOperations.delete(key);
    }
  }

  stopAll() {
    // Completely stop all playback (don't track for resume)
    this.activeSources.forEach((sourceInfo, key) => {
      sourceInfo.source.stop();
      sourceInfo.source.disconnect();
      sourceInfo.gainNode.disconnect();
    });
    this.activeSources.clear();
    this.pausedSources.clear();
    this.pendingOperations.clear();
    this.isAudioPlaying = false;
  }

  setVolume(key, volume) {
    const sources = this.activeSources.get(key);
    if (sources) {
      sources.gainNode.gain.value = volume * this.globalVolume;
    }
  }

  setGlobalVolume(volume) {
    this.globalVolume = Math.max(0, Math.min(1, volume));
    this.activeSources.forEach(sources => {
      sources.gainNode.gain.value = sources.gainNode.gain.value / (sources.gainNode.gain.value / this.globalVolume);
    });
  }

  toggleMute() {
    this.isMuted = !this.isMuted;
    if (this.isMuted) {
      this.setGlobalVolume(0);
    } else {
      this.setGlobalVolume(1.0);
    }
    return this.isMuted;
  }

  isPlaying(key) {
    return this.activeSources.has(key);
  }
}
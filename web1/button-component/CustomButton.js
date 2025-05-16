class CustomButton extends HTMLElement {
  constructor() {
    super();

    // Create a shadow root
    this.attachShadow({ mode: 'open' });

    // Set up template
    this.shadowRoot.innerHTML = `
      <style>
        button {
          padding: 10px 20px;
          border: none;
          border-radius: 4px;
          font-size: 16px;
          cursor: pointer;
          transition: all 0.2s ease;
        }

        .primary {
          background-color: #4CAF50;
          color: white;
        }
        
        .primary:hover {
          background-color: #45a049;
        }
        
        .secondary {
          background-color: #f44336;
          color: white;
        }
        
        .secondary:hover {
          background-color: #d32f2f;
        }
        
        .disabled {
          opacity: 0.6;
          cursor: not-allowed;
        }
      </style>

      <button class="primary">
        <slot></slot>
      </button>
    `;
  }
  
  static get observedAttributes() {
    return ['variant', 'disabled'];
  }
  
  attributeChangedCallback(name, oldValue, newValue) {
    if (name === 'variant') {
      const button = this.shadowRoot.querySelector('button');
      button.className = newValue || 'primary';   // Uses 'primary' as default if newValue is empty/null
    }

    if (name === 'disabled') {
      const button = this.shadowRoot.querySelector('button');
      if (newValue !== null) {
        button.classList.add('disabled');
        button.disabled = true;
      } else {
        button.classList.remove('disabled');
        button.disabled = false;
      }
    }
  }
}

// Define the custom element
customElements.define('custom-button', CustomButton);
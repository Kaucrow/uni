class MyCheckbox extends HTMLElement {
    constructor() {
        super();
        this.attachShadow({ mode: 'open' });

        // Define template
        const template = document.createElement('template');
        template.innerHTML = `
            <style>
                input {
                    width: 20px;
                    height: 20px;
                    cursor: pointer;
                }
            </style>
            <input type="checkbox">
        `;

        this.shadowRoot.appendChild(template.content.cloneNode(true));

        // Bind elements
        this.checkbox = this.shadowRoot.querySelector('input');

        // Define property with getter and setter
        Object.defineProperty(this, 'check', {
            get: () => this.checkbox.checked,
            set: (value) => {
                this.checkbox.checked = value;
                this.handleChange();
            }
        });

        // Event listener for change
        this.checkbox.addEventListener('change', () => {
            this.handleChange();
        });
    }

    handleChange() {
        console.log("Checkbox state changed to:", this.check);
        this.dispatchEvent(new Event('change'));
    }
}

customElements.define('my-checkbox', MyCheckbox);
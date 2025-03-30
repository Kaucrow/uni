var x = 32;

class MyCheckbox extends HTMLElement {
    constructor() {
        super();
        const shadow = this.attachShadow({ mode: 'open' });

        const template = document.createElement('template');
        template.innerHTML = `
            <style>
                      p {
            color: red;
            font-weight: bold;
        }

            </style>
                    <input type="checkbox">
        <p>Checkbox 1</p>

        `;

        shadow.appendChild(template.content.cloneNode(true));

        this.input = this.shadowRoot.querySelector('input');

Object.defineProperty(this, 'check', {
    get: () => this.input.checked,
    set: (value) => {
        this.input.checked = value;
    }
});this.check = true;this.other = true;
    }
}

customElements.define('my-checkbox', MyCheckbox);

var y = 42;
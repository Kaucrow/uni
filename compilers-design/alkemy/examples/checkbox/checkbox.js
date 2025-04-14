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

            input {
                cursor: pointer;
            }
        }

            </style>
                    <input type="checkbox">

        `;

        shadow.appendChild(template.content.cloneNode(true));

        this.other = true;this.input = this.shadowRoot.querySelector('input');

Object.defineProperty(this, 'check', {
    get: () => this.input.checked,
    set: (value) => {
        this.input.checked = value;
    }
});this.check = false;

        this.input.addEventListener('change', () => {
            if (this.check) {
            console.log('The feature was enabled');
        } else {
            console.log('The feature was disabled');
        }

});
    }
}

customElements.define('my-checkbox', MyCheckbox);

var y = 42;
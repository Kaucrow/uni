class MyComponent extends HTMLElement {
    constructor() {
        super();
        const shadow = this.attachShadow({ mode: 'open' });

        const template = document.createElement('template');
        template.innerHTML = `
            <style>
                p { color: red; font-weight: bold; }
            </style>
            <p>Hello from MyComponent!</p>
        `;

        shadow.appendChild(template.content.cloneNode(true));
    }
}

customElements.define('my-component', MyComponent);
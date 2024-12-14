class ContextMenu {
  constructor(container) {
    // Get the container to put the menu in
    this.container = document.querySelector(container);
    // Stores the items in the menu's main bar
    this.menuItems = [];
    // Create the menu's main bar
    this.createMenu();
  }

  addMenuItem(label, submenu = []) {
    const item = { label, submenu };
    this.menuItems.push(item);
    this.renderMenu();
  }

  createMenu() {
    const menuBar = document.createElement('div');
    menuBar.classList.add('menu-bar');
    this.container.appendChild(menuBar);
  }

  renderMenu() {
    const menuBar = this.container.querySelector('.menu-bar');
    // Prevent duplicate items
    menuBar.innerHTML = '';

    this.menuItems.forEach((item) => {
      const menuItem = document.createElement('div');
      menuItem.classList.add('menu-item');
      menuItem.textContent = item.label;

      if (item.submenu.length > 0) {
        const submenu = this.createSubmenu(item.submenu);
        menuItem.appendChild(submenu);

        menuItem.addEventListener('mouseover', () => submenu.classList.add('show'));
        menuItem.addEventListener('mouseout', () => submenu.classList.remove('show'));
      }

      menuBar.appendChild(menuItem);
    });
  }

  createSubmenu(submenuItems) {
    const submenu = document.createElement('div');
    submenu.classList.add('submenu');

    submenuItems.forEach((subItem) => {
      const subMenuItem = document.createElement('div');
      subMenuItem.classList.add('submenu-item');
      subMenuItem.textContent = subItem.label;

      if (subItem.submenu && subItem.submenu.length > 0) {
        const nestedSubmenu = this.createSubmenu(subItem.submenu);
        subMenuItem.appendChild(nestedSubmenu);

        subMenuItem.addEventListener('mouseover', () => nestedSubmenu.classList.add('show'));
        subMenuItem.addEventListener('mouseout', () => nestedSubmenu.classList.remove('show'));
      }

      subMenuItem.addEventListener('click', subItem.action || (() => {}));
      submenu.appendChild(subMenuItem);
    });

    return submenu;
  }
}

// Add the styles
const link = document.createElement("link");
link.rel = "stylesheet";
link.href = "./styles.css";

document.head.appendChild(link);
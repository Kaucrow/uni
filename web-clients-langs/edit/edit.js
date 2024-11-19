/* Creates an edit component and returns the following:
 * - element: The HTML element
 * - getValue(): Returns the value of the edit's input
 * - isValid(): Returns true if the input is valid and false otherwise */
function Edit(labelText = 'Input some text', validationType = 'textStrict') {
  // Validation regex patterns
  const patterns = {
    text: /^.*/s,   // Any character except newlines
    textStrict: /^[a-zA-Z0-9 ]+$/,  // Letters, numbers, and spaces
    email: /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/,  // Basic email regex
    password: /^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d]{8,}$/,  // At least 8 characters, 1 letter, 1 number
  };

  // Create the container
  const container = document.createElement('div');
  container.className = 'edit';

  // Style the container
  container.style.fontFamily = 'Arial, sans-serif';
  container.style.position = 'relative';
  container.style.width = '400px';
  container.style.display = 'flex';

  // Create the label
  const label = document.createElement('label');
  label.setAttribute('for', 'textInput');
  label.textContent = labelText;
  label.style.position = 'absolute';
  label.style.left = '10px';
  label.style.top = '50%';
  label.style.transform = 'translateY(-50%)';
  label.style.transition = 'all 0.3s ease';
  label.style.padding = '0 5px';
  label.style.pointerEvents = 'none';
  label.style.color = '#404040';

  // Create the input
  const input = document.createElement('input');
  input.type = validationType === 'password' ? 'password' : 'text';
  input.id = 'textInput';
  input.placeholder = '';
  input.style.color = '#d3c2c3';
  input.style.background = '#121212'; 
  input.style.padding = '10px';
  input.style.fontSize = '16px';
  input.style.width = '100%';
  input.style.border = '1px solid #404040';
  input.style.borderRadius = '4px';
  input.style.outline = 'none';
  input.style.transition = 'all 0.3s ease';

  // Bring the input back to its original state
  // after unfocusing it or erasing it
  const resetInput = () => {
    label.style.top = '50%';
    label.style.left = '10px';
    label.style.fontSize = '16px';
    label.style.color = '#404040';
    input.style.borderColor = '#404040';
  }

  // Input focus event listener
  input.addEventListener('focus', () => {
    label.style.top = '-15px';
    label.style.left = '5px';
    label.style.fontSize = '12px';
    label.style.color = '#b7976a';
    input.style.borderColor = '#b7976a';
  });

  // Input unfocus event listener
  input.addEventListener('blur', () => {
    if (!input.value) {
      resetInput();
    }
    validateInput();
  });

  // Validate the input based on the selected validation type
  const validateInput = () => {
    const pattern = patterns[validationType];
    const isValid = pattern.test(input.value);
    if (!isValid && input.value !== '') {
      input.style.borderColor = '#f44336';  // Invalid input color
    } else {
      input.style.borderColor = '#404040';  // Valid input color
    }
    return isValid;
  };

  // Create the erase button
  const erase = document.createElement('button');
  erase.innerHTML = 'Erase';
  erase.style.borderRadius = '10px';
  erase.style.marginTop = '5px';
  erase.style.marginBottom = '5px';
  erase.style.marginLeft = '10px';
  erase.style.padding = '5px 15px';
  erase.style.backgroundColor = '#f44336';
  erase.style.color = 'white';
  erase.style.border = 'none';
  erase.style.cursor = 'pointer';

  // Erase input event listener
  erase.addEventListener('click', () => {
    input.value = '';
    resetInput();
  });

  // Append elements to the container
  container.appendChild(label);
  container.appendChild(input);
  container.appendChild(erase);

  // Return the complete component
  return {
    element: container,
    getValue: () => input.value,
    isValid: () => validateInput(),
  };
}
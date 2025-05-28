import './Input.css';

interface InputProps {
  input: string,
  label?: string,
  id?: string,
  required?: boolean,
  type?: React.HTMLInputTypeAttribute,
  err?: boolean,
  setInput: React.Dispatch<React.SetStateAction<string>>,
}

const Input: React.FC<InputProps> = ({
  input = "",
  label = "",
  id = "input",
  required = false,
  type = "text",
  err = false,
  setInput,
}) => {
  return (
    <div className="input-container">
      <input
        required={required}
        type={type}
        className={`input-field ${err ? "input-error" : ""}`}
        id={id}
        autoComplete="off"
        value={input}
        onChange={(e) => setInput(e.target.value)}
        placeholder='fdashflkdsah'
      />
      <label htmlFor={id} className="input-label">{label}</label>
    </div>
  )
}

export default Input
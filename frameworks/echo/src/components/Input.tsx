import './Input.css';

interface InputProps {
  input: string,
  label?: string,
  id?: string,
  err?: boolean,
  setInput: React.Dispatch<React.SetStateAction<string>>,
}

const Input: React.FC<InputProps> = ({
  input = "",
  label = "",
  id = "input",
  err = false,
  setInput,
}) => {
  return (
    <div className="input-container">
      <input
        required
        type="text"
        className={`input-field ${err ? "input-error" : ""}`}
        id={id}
        autoComplete="off"
        value={input}
        onChange={(e) => setInput(e.target.value)}
      />
      <label htmlFor={id} className="input-label">{label}</label>
    </div>
  )
}

export default Input
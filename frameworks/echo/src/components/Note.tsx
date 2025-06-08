import './Note.css';
import { useState, useEffect, useRef } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faChevronLeft, faTrash, faFloppyDisk } from '@fortawesome/free-solid-svg-icons';
import { darken } from 'polished';
import { saveNote, deleteNote } from '../firebase/database';

interface NoteProps {
  id: number;
  color: string;
  title: string;
  content: string;
  isExpanded: boolean;
  onClick: () => void;
  onDelete: () => void;
}

const Note = ({ id, color, title, content, isExpanded, onClick, onDelete }: NoteProps) => {
  const [noteContent, setNoteContent] = useState<string>(content);
  const [noteTitle, setNoteTitle] = useState<string>(title);
  const [isEditingTitle, setIsEditingTitle] = useState<boolean>(false);
  const [lastSavedContent, setLastSavedContent] = useState(content);
  const [lastSavedTitle, setLastSavedTitle] = useState(title);
  const wasExpandedRef = useRef(isExpanded);

  // Track changes to reset when needed
  useEffect(() => {
    if (wasExpandedRef.current && !isExpanded) {
      // Just collapsed - reset to last saved values
      setNoteContent(lastSavedContent);
      setNoteTitle(lastSavedTitle);
    }
    wasExpandedRef.current = isExpanded;
  }, [isExpanded, lastSavedContent, lastSavedTitle]);

  const handleTextareaChange = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
    setNoteContent(e.target.value);
  };

  const handleTitleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setNoteTitle(e.target.value);
  };

  const handleSave = async (e: React.MouseEvent) => {
    e.stopPropagation();
    try {
      await saveNote(id, noteTitle, noteContent);
      // Update our last saved values
      setLastSavedContent(noteContent);
      setLastSavedTitle(noteTitle);
      onClick(); // Collapse the note
    } catch (error) {
      console.error('Failed to save note:', error);
    }
  };

  const handleDelete = async (e: React.MouseEvent) => {
    e.stopPropagation();
    try {
      await deleteNote(id);
      onClick(); // Collapse the note
      onDelete(); // Refresh the notes
    } catch (error) {
      console.error('Failed to save note:', error);
    }
  };

  const handleGoBack = (e: React.MouseEvent) => {
    e.stopPropagation();
    onClick();
  };

  const handleTitleClick = (e: React.MouseEvent) => {
    if (isExpanded) {
      e.stopPropagation();
      setIsEditingTitle(true);
    }
  };

  return (
    <div 
      className={`note ${isExpanded ? 'note-expanded' : 'note-small'}`}
      style={{ backgroundColor: color }}
      onClick={!isExpanded ? onClick : undefined}
    >
      {isExpanded && (
        <button 
          className="note-back-button"
          onClick={handleGoBack}
        >
          <FontAwesomeIcon className="note-btn-icon" icon={faChevronLeft} />
          Back
        </button>
      )}
      
      <div className="note-title-container">
        {isExpanded && isEditingTitle ? (
          <input
            type="text"
            className="note-title-input"
            value={noteTitle}
            onChange={handleTitleChange}
            onBlur={() => setIsEditingTitle(false)}
            autoFocus
            style={{
              borderBottom: `2px solid ${darken(0.1, color)}`,
            }}
          />
        ) : (
          <h3
            className="note-title"
            onClick={handleTitleClick}
            style={{
              borderBottom: `2px solid ${darken(0.1, color)}`,
            }}
          >
            {isExpanded ? noteTitle : lastSavedTitle}
          </h3>
        )}
      </div>

      {isExpanded ? (
        <div className="note-content-expanded">
          <textarea 
            className="note-textarea"
            spellCheck={false}
            value={noteContent}
            onChange={handleTextareaChange}
          />
          <div className="note-actions">
            <button
              className="note-button"
              onClick={handleDelete}
            >
              <FontAwesomeIcon className="note-btn-icon" icon={faTrash} />
              Delete
            </button>
            <button
              className="note-button"
              onClick={handleSave}
            >
              <FontAwesomeIcon className="note-btn-icon" icon={faFloppyDisk} />
              Save
            </button>
          </div>
        </div>
      ) : (
        <div>
          <div className="note-separator-preview"></div>
          <p className="note-content-preview">{lastSavedContent}</p>
        </div>
      )}
    </div>
  );
};

export default Note;
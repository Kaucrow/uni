import './Notes.css';
import { useEffect, useState } from 'react';
import { Navigate } from 'react-router-dom';
import { Flipper, Flipped } from 'react-flip-toolkit';
import FloatingDropdown from '../../components/FloatingDropdown';
import Note from '../../components/Note';
import type { NoteType } from '../../firebase/types';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faNoteSticky } from '@fortawesome/free-solid-svg-icons';
import { faPencil } from '@fortawesome/free-solid-svg-icons';
import { createNote, getNotes } from '../../firebase/database';
import { useAuth } from '../../hooks/useAuth';

const Notes = () => {
  const [notes, setNotes] = useState<NoteType[]>([]);
  const { userLoggedIn } = useAuth();

  if (!userLoggedIn) {
    return <Navigate to={'/'} replace />
  }

  const fetchNotes = async () => {
    try {
      const fetchedNotes = await getNotes();
      setNotes(fetchedNotes);
    } catch (err) {
      console.error(err);
    }
  }

  useEffect(() => {
    fetchNotes();
  }, []);
  
  const [layoutIndex, setLayoutIndex] = useState(0);
  const layouts = [
    { cols: 4, class: 'grid-cols-4' },
    { cols: 3, class: 'grid-cols-3' },
    { cols: 2, class: 'grid-cols-2' },
    { cols: 1, class: 'grid-cols-1' },
  ];

  const currentLayout = layouts[layoutIndex];

  const [expandedNoteId, setExpandedNoteId] = useState<number | null>(null);

  const handleNoteClick = (id: number) => {
    setExpandedNoteId(id === expandedNoteId ? null : id);
  };

  const changeLayout = () => {
    setLayoutIndex((prevIndex) => (prevIndex + 1) % layouts.length);
  };

  const getNoteColor = (color: string): string => {
    const colorMap: Record<string, string> = {
      red: "#e67e80",
      green: "#a7c080",
      blue: "#7fbbb3",
      yellow: "#dbbc7f"
    };

    return colorMap[color.toLowerCase()] || '#ffffff';
  }

  const handleCreateNote = async (color: string) => {
    try {
      await createNote(color);
      await fetchNotes();
      console.log('Note created')
    } catch (err) {
      console.error(`Error on note creation ${err}`);
    }
  }

  const handleDeleteNote = async () => {
    try {
      await fetchNotes();
    } catch (err) {
      console.error(`Error on note deletion ${err}`);
    }
  }

  const dropdownOptions = [
  {
    id: 'create-note',
    label: 'Create Note',
    icon: <FontAwesomeIcon icon={faNoteSticky} />,
    subOptions: [
      {
        id: 'create-red',
        label: 'Red Note',
        onClick: () => handleCreateNote('red')
      },
      {
        id: 'create-green',
        label: 'Green Note',
        onClick: () => handleCreateNote('green')
      },
      {
        id: 'create-blue',
        label: 'Blue Note',
        onClick: () => handleCreateNote('blue')
      },
      {
        id: 'create-yellow',
        label: 'Yellow Note',
        onClick: () => handleCreateNote('yellow')
      }
    ]
  }];

  return (
    <div className="container">
      <FloatingDropdown
        triggerContent={<FontAwesomeIcon icon={faPencil} className="dropdown-trigger-icon"/>}
        options={dropdownOptions}
        position="top-left"
      />

      <Flipper flipKey={currentLayout.class}>
        <div className={`grid-container ${currentLayout.class}`}>
          {notes.map(note => (
            <Flipped key={note.id} flipId={note.id}>
              <div>
                <Note
                  id={note.id}
                  color={getNoteColor(note.color)}
                  title={note.title}
                  content={note.content}
                  isExpanded={expandedNoteId === note.id}
                  onClick={() => handleNoteClick(note.id)}
                  onDelete={() => handleDeleteNote()}
                />
              </div>
            </Flipped>
          ))}
        </div>
      </Flipper>

      <button 
        onClick={changeLayout}
        className="layout-button"
      >
        <svg xmlns="http://www.w3.org/2000/svg" className="button-icon" fill="none" viewBox="0 0 24 24" stroke="currentColor">
          <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M4 6h16M4 12h16M4 18h16" />
        </svg>
        <span className="button-text">Change Layout</span>
      </button>
    </div>
  );
};

export default Notes;
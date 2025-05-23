import './NoteGrid.css'
import { useState } from 'react';
import { Flipper, Flipped } from 'react-flip-toolkit';

interface Note {
  id: number;
  title: string;
  content: string;
  color: string;
}

const NoteGrid: React.FC = () => {
  const [gridSize, setGridSize] = useState<number>(6);
  const [notes] = useState<Note[]>([
    { id: 1, title: 'Note 1', content: 'This is the first note', color: '#ffffff' },
    { id: 2, title: 'Note 2', content: 'This is the second note', color: '#ffffff' },
    { id: 3, title: 'Note 3', content: 'This is the third note', color: '#ddddff' },
    { id: 4, title: 'Note 4', content: 'This is the fourth note', color: '#ffffdd' },
    { id: 5, title: 'Note 5', content: 'This is the fifth note', color: '#ffddff' },
    { id: 6, title: 'Note 6', content: 'This is the sixth note', color: '#ddffff' },
    { id: 7, title: 'Note 7', content: 'This is the seventh note', color: '#ffeedd' },
    { id: 8, title: 'Note 8', content: 'This is the eighth note', color: '#ddffee' },
    { id: 9, title: 'Note 9', content: 'This is the ninth note', color: '#eeddff' },
    { id: 10, title: 'Note 10', content: 'This is the tenth note', color: '#ffeeff' },
    { id: 11, title: 'Note 11', content: 'This is the eleventh note', color: '#ffffee' },
    { id: 12, title: 'Note 12', content: 'This is the twelfth note', color: '#eeffff' },
  ]);

  const cycleGridSize = () => {
    setGridSize(prev => {
      if (prev === 6) return 4;
      if (prev === 4) return 3;
      return 6;
    });
  };

  return (
    <div className="note-grid-container">
      <Flipper
        flipKey={notes.map(note => note.id).join('') + gridSize}
        spring="gentle"
        staggerConfig={{
          default: {
            speed: 0.9
          }
        }}
      >
      <div className={`note-grid grid-size-${gridSize}`}>
        {notes.map(note => (
          <Flipped key={note.id} flipId={note.id}>
            <div className="note" style={{ backgroundColor: note.color }}>
              <h3>{note.title}</h3>
              <p>{note.content}</p>
            </div>
          </Flipped>
        ))}
      </div>
      </Flipper>
      <button className="floating-button" onClick={cycleGridSize}>
        {gridSize} per row
      </button>
    </div>
  );
};

export default NoteGrid;
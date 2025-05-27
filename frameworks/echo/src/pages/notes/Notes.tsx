import { useState } from 'react';
import { Flipper, Flipped } from 'react-flip-toolkit';
import FloatingDropdown from '../../components/FloatingDropdown';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faNoteSticky } from '@fortawesome/free-solid-svg-icons';
import { createNote } from '../../firebase/database';

const Notes = () => {
  const [items] = useState([
    { id: 1, color: 'bg-red-500' },
    { id: 2, color: 'bg-blue-500' },
    { id: 3, color: 'bg-green-500' },
    { id: 4, color: 'bg-yellow-500' },
    { id: 5, color: 'bg-purple-500' },
    { id: 6, color: 'bg-pink-500' },
    { id: 7, color: 'bg-indigo-500' },
    { id: 8, color: 'bg-orange-500' },
    { id: 9, color: 'bg-teal-500' },
    { id: 10, color: 'bg-gray-500' },
    { id: 11, color: 'bg-red-400' },
    { id: 12, color: 'bg-blue-400' },
  ]);

  const [layoutIndex, setLayoutIndex] = useState(0);
  const layouts = [
    { cols: 6, class: 'grid-cols-6' },
    { cols: 4, class: 'grid-cols-4' },
    { cols: 3, class: 'grid-cols-3' },
    { cols: 1, class: 'grid-cols-1' },
  ];

  const currentLayout = layouts[layoutIndex];

  const changeLayout = () => {
    setLayoutIndex((prevIndex) => (prevIndex + 1) % layouts.length);
  };

  const handleCreateNote = async () => {
    await createNote();
    console.log('Note created');
  }

  const dropdownOptions = [
    {
      id: 'create',
      label: 'Create Note',
      icon: <FontAwesomeIcon icon={faNoteSticky} />,
      onClick: handleCreateNote
    }
  ]

  return (
    <div className="p-8 max-w-7xl mx-auto relative min-h-screen">
      <FloatingDropdown
        triggerContent={<div></div>}
        options={dropdownOptions}
        position="top-left"
      />
      <h1 className="text-2xl font-bold mb-6">Grid Layout Animation Tester</h1>
      
      <div className="mb-6">
        <p className="text-gray-600">
          Current layout: {currentLayout.cols} columns
        </p>
      </div>

      <Flipper flipKey={currentLayout.class}>
        <div className={`grid ${currentLayout.class} gap-4`}>
          {items.map(item => (
            <Flipped key={item.id} flipId={item.id}>
              <div 
                className={`${item.color} h-32 rounded-lg flex items-center justify-center text-white font-bold text-xl shadow-md`}
              >
                {item.id}
              </div>
            </Flipped>
          ))}
        </div>
      </Flipper>

      <button 
        onClick={changeLayout}
        className="fixed bottom-6 right-6 px-6 py-3 bg-gray-800 text-white rounded-full hover:bg-gray-700 transition shadow-lg flex items-center justify-center"
      >
        <svg xmlns="http://www.w3.org/2000/svg" className="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
          <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M4 6h16M4 12h16M4 18h16" />
        </svg>
        <span className="ml-2">Change Layout</span>
      </button>

      <div className="mt-8 text-gray-600">
        <p className="mb-2">Click the floating button to see FLIP animations as the layout changes.</p>
        <p>Uses <code>react-flip-toolkit</code> for smooth animations.</p>
      </div>
    </div>
  );
};

export default Notes;
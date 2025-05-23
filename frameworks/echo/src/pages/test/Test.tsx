import { useState } from 'react';
import { Flipper, Flipped } from 'react-flip-toolkit';

const GridFlipperTester = () => {
  const [items, setItems] = useState([
    { id: 1, color: 'bg-red-500' },
    { id: 2, color: 'bg-blue-500' },
    { id: 3, color: 'bg-green-500' },
    { id: 4, color: 'bg-yellow-500' },
    { id: 5, color: 'bg-purple-500' },
    { id: 6, color: 'bg-pink-500' },
  ]);

  const shuffleItems = () => {
    setItems(prevItems => {
      const newItems = [...prevItems];
      // Simple shuffle algorithm
      for (let i = newItems.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [newItems[i], newItems[j]] = [newItems[j], newItems[i]];
      }
      return newItems;
    });
  };

  const resetItems = () => {
    setItems([
      { id: 1, color: 'bg-red-500' },
      { id: 2, color: 'bg-blue-500' },
      { id: 3, color: 'bg-green-500' },
      { id: 4, color: 'bg-yellow-500' },
      { id: 5, color: 'bg-purple-500' },
      { id: 6, color: 'bg-pink-500' },
    ]);
  };

  return (
    <div className="p-8 max-w-4xl mx-auto">
      <h1 className="text-2xl font-bold mb-6">Grid Flipper Animation Tester</h1>
      
      <div className="flex gap-4 mb-6">
        <button 
          onClick={shuffleItems}
          className="px-4 py-2 bg-gray-800 text-white rounded hover:bg-gray-700 transition"
        >
          Shuffle Items
        </button>
        <button 
          onClick={resetItems}
          className="px-4 py-2 bg-gray-200 rounded hover:bg-gray-300 transition"
        >
          Reset Order
        </button>
      </div>

      <Flipper flipKey={items.map(item => item.id).join('')}>
        <div className="grid grid-cols-3 gap-4">
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

      <div className="mt-8 text-gray-600">
        <p className="mb-2">Click "Shuffle Items" to see FLIP animations as elements change positions.</p>
        <p>Uses <code>react-flip-toolkit</code> for smooth animations.</p>
      </div>
    </div>
  );
};

export default GridFlipperTester;
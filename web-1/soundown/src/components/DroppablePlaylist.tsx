import { useDroppable } from "@dnd-kit/core";

interface DroppablePlaylistProps {
  id: number;
  children: React.ReactNode;
}

const DroppablePlaylist: React.FC<DroppablePlaylistProps> = ({ id, children }) => {
  const { setNodeRef, isOver } = useDroppable({
    id: id,
    data: { type: 'playlist', playlistId: id },
  });

  const isCurrentlyOver = isOver;

  return (
    <div
      ref={setNodeRef}
      className={`relative flex flex-row p-2 rounded-md transition-all duration-200 items-center ${
        isCurrentlyOver
          ? 'bg-blue-100 dark:bg-blue-900 ring-2 ring-blue-500'
          : 'hover:bg-neutral-100 dark:hover:bg-neutral-700'
      }`}
    >
      {children}
      {isCurrentlyOver && (
        <div className="absolute inset-0 border-2 border-dashed border-blue-500 rounded-md pointer-events-none"></div>
      )}
    </div>
  );
};

export default DroppablePlaylist;
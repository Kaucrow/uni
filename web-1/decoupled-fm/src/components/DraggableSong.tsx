import { type Song } from "@/database/database";
import { useDraggable } from "@dnd-kit/core";

interface DraggableSongProps {
  song: Song;
  children: React.ReactNode;
}

const DraggableSong: React.FC<DraggableSongProps> = ({ song, children }) => {
  const { attributes, listeners, setNodeRef, isDragging } = useDraggable({
    id: song.id,
    data: { type: 'song', song: song },
  });

  const style = {
    opacity: isDragging ? 0.4 : 1,
    cursor: 'grab',
    transition: 'opacity 0.15s ease-in-out',
  };

  return (
    <div ref={setNodeRef} style={style} {...listeners} {...attributes}>
      {children}
    </div>
  );
};

export default DraggableSong;
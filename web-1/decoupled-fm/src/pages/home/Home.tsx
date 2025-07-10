import React, { useState } from 'react';
import ThemeToggle from '@/components/ThemeToggle';
import DroppablePlaylist from '@/components/DroppablePlaylist';
import DraggableSong from '@/components/DraggableSong';
import { type Song } from '@/database/database';

// FontAwesome icons
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import {
  faPlay,
  faPause,
  faForwardStep,
  faBackwardStep,
  faVolumeHigh,
  faShuffle,
  faRotate,
  faCircle,
  faMagnifyingGlass,
  faPlus,
  faHouse,
} from '@fortawesome/free-solid-svg-icons'

// Shadcn UI components
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Card, CardContent, CardHeader, CardTitle, CardDescription } from '@/components/ui/card';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Slider } from '@/components/ui/slider';
import { Separator } from '@/components/ui/separator';
import { ScrollArea } from '@/components/ui/scroll-area';

// dnd-kit imports
import {
  DndContext,
  rectIntersection,
  KeyboardSensor,
  PointerSensor,
  useSensor,
  useSensors,
  DragOverlay,
  type DragEndEvent,
  type UniqueIdentifier,
} from '@dnd-kit/core';

interface Playlist {
  id: string;
  name: string;
  songIds: number[];
}

// --- Mock Data (Typed) ---
const mockSongs: Song[] = [
  { id: 1, title: 'Analog Dreams', artist: 'Synthwave Savior', duration: 3, name: 'a', data: new ArrayBuffer(), addedAt: new Date() },
  { id: 2, title: 'Cybernetic Heartbeat', artist: 'Data Echo', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date() },
  { id: 3, title: 'Neon Highway', artist: 'Retro Rider', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date() },
  { id: 4, title: 'Ghost in the Machine', artist: 'Virtual Virtuoso', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date() },
  { id: 5, title: 'Echoes of Tomorrow', artist: 'Future Funk', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date()},
  { id: 6, title: 'Electric Serenity', artist: 'Dream Weaver', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date()},
  { id: 7, title: 'Pixelated Paradise', artist: 'Game Core', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date()},
  { id: 8, title: 'Binary Bliss', artist: 'Code Harmony', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date()},
  { id: 9, title: 'Circuit Symphony', artist: 'Logic Loop', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date()},
  { id: 10, title: 'Neural Network Blues', artist: 'AI Artist', duration: 1, name: 'a', data: new ArrayBuffer(), addedAt: new Date()},
];

const initialPlaylists: Playlist[] = [
  { id: 'p1', name: 'Chillwave Vibes', songIds: [1, 3] },
  { id: 'p2', name: 'Deep Cyberpunk', songIds: [2, 4] },
  { id: 'p3', name: 'Morning Code', songIds: [5] },
];

const Home: React.FC = () => {
  const [currentSong, setCurrentSong] = useState<Song | null>(null);
  const [isPlaying, setIsPlaying] = useState<boolean>(false);
  const [volume, setVolume] = useState<number>(50);
  const [searchResults, setSearchResults] = useState<Song[]>(mockSongs);
  const [playlists, setPlaylists] = useState<Playlist[]>(initialPlaylists);
  const [selectedPlaylist, setSelectedPlaylist] = useState<Playlist | null>(null);
  const [activeId, setActiveId] = useState<UniqueIdentifier | null>(null);
  const [activeTab, setActiveTab] = useState<string>("songs"); // State to manage active tab

  const draggedSong = activeId ? mockSongs.find(s => s.id === activeId) : null;

  const sensors = useSensors(
    useSensor(PointerSensor, { activationConstraint: { distance: 8 } }),
    useSensor(KeyboardSensor)
  );

  const handleSearch = (event: React.ChangeEvent<HTMLInputElement>) => {
    const query = event.target.value.toLowerCase();
    const filteredSongs = mockSongs.filter(song =>
      song.title.toLowerCase().includes(query) || song.artist.toLowerCase().includes(query)
    );
    setSearchResults(filteredSongs);
  };

  const playSong = (song: Song) => {
    setCurrentSong(song);
    setIsPlaying(true);
    console.log(`Playing: ${song.title} by ${song.artist}`);
  };

  const togglePlayPause = () => {
    setIsPlaying(!isPlaying);
  };

  const handleDragStart = (event: { active: { id: UniqueIdentifier; }; }) => {
    setActiveId(event.active.id);
  };

  const handleDragEnd = (event: DragEndEvent) => {
    const { active, over } = event;
    setActiveId(null);

    if (active && over) {
      const draggedSongData = active.data.current?.song as Song;
      const targetType = over.data.current?.type;
      const targetPlaylistId = over.data.current?.playlistId;

      if (draggedSongData && targetType === 'playlist' && targetPlaylistId) {
        setPlaylists((prevPlaylists) => {
          const updatedPlaylists = prevPlaylists.map((playlist) => {
            if (playlist.id === targetPlaylistId) {
              if (!playlist.songIds.some(id => id === draggedSongData.id)) {
                return {
                  ...playlist,
                  songIds: [...playlist.songIds, draggedSongData.id],
                };
              }
            }
            return playlist;
          });
          return updatedPlaylists;
        });

        if (selectedPlaylist && selectedPlaylist.id === targetPlaylistId) {
          setSelectedPlaylist((prevSelected) => {
            if (prevSelected && !prevSelected.songIds.some(id => id === draggedSongData.id)) {
              return {
                ...prevSelected,
                songs: [...prevSelected.songIds, draggedSongData.id],
              };
            }
            return prevSelected;
          });
        }

        console.log(`Dropped song "${draggedSongData.title}" onto playlist "${targetPlaylistId}"`);
      }
    }
  };

  return (
    <DndContext
      sensors={sensors}
      collisionDetection={rectIntersection}
      onDragStart={handleDragStart}
      onDragEnd={handleDragEnd}
    >
      <div
        className="
          w-300
          flex flex-col
          bg-gradient-to-br from-neutral-50 to-neutral-200
          dark:from-neutral-950 dark:to-neutral-800
          text-neutral-900 dark:text-neutral-50
          transition-colors duration-300 ease-in-out
        "
      >
        {/* Header / Nav */}
        <header
          className="
            w-full
            flex justify-between items-center
            py-4 px-6
            bg-neutral-200/80 dark:bg-neutral-800/80
            shadow-lg
            transition-colors duration-300 ease-in-out
            z-10
            flex-shrink-0
          "
        >
          <h1 className="text-3xl font-bold tracking-tight">
            DecoupledFM
          </h1>
          <nav className="flex items-center space-x-4">
            <a
              href="/"
              className="
                text-lg font-medium
                hover:text-blue-600 dark:hover:text-blue-400
                transition-colors duration-200
              "
            >
              Home
            </a>
            <ThemeToggle />
          </nav>
        </header>

        {/* Main Content Area */}
        <main className="flex flex-1 overflow-hidden p-6 gap-6">
          {/* Left Sidebar - Navigation & Playlists */}
          <Card className="w-1/4 flex flex-col p-4 flex-shrink-0">
            <CardHeader className="flex-shrink-0">
              <CardTitle>Navigation</CardTitle>
            </CardHeader>
            <CardContent className="flex flex-col space-y-3 overflow-hidden items-start">
              <a href="/">
                <Button variant="ghost">
                  <FontAwesomeIcon icon={faHouse} />
                  Home
                </Button>
              </a>
              <Separator className="flex-shrink-0" />
              <CardTitle className="mt-4 flex-shrink-0">Playlists</CardTitle>
              <ScrollArea className="flex-grow pr-2">
                {playlists.map((playlist) => (
                  <DroppablePlaylist key={playlist.id} id={playlist.id}>
                    <Button
                      variant="ghost"
                      className="justify-start w-full"
                      onClick={() => {
                        setSelectedPlaylist(playlist);
                        setActiveTab("playlists"); // Automatically switch to playlists tab
                      }}
                    >
                      <FontAwesomeIcon icon={faCircle} />
                      {playlist.name}
                    </Button>
                  </DroppablePlaylist>
                ))}
              </ScrollArea>
              <Button className="mt-4 flex-shrink-0">
                <FontAwesomeIcon icon={faPlus}/>
                Create New Playlist
              </Button>
            </CardContent>
          </Card>

          {/* Main Content - Search & Playlists */}
          <div className="flex-1 flex flex-col gap-6 overflow-hidden">
            {/* Search Bar */}
            <Card className="p-4 w-full">
              <CardHeader>
                <CardTitle>Search for Music</CardTitle>
                <CardDescription>Find your next favorite track.</CardDescription>
              </CardHeader>
              <CardContent>
                <div className="flex space-x-2">
                  <Input
                    type="text"
                    placeholder="Search songs, artists..."
                    onChange={handleSearch}
                    className="flex-grow"
                  />
                  <Button>
                    <FontAwesomeIcon icon={faMagnifyingGlass} className="w-5 h-5"/>
                  </Button>
                </div>
              </CardContent>
            </Card>

            {/* Song List / Playlist View */}
            <Card className="flex-1 flex flex-col overflow-hidden">
              <Tabs value={activeTab} onValueChange={setActiveTab} className="flex flex-col flex-1 overflow-hidden">
                <CardHeader className="flex-shrink-0 pb-0">
                  <TabsList>
                    <TabsTrigger value="songs">Songs</TabsTrigger>
                    <TabsTrigger value="playlists">View Playlist</TabsTrigger>
                  </TabsList>
                </CardHeader>

                <CardContent className="flex-grow pt-0 pb-4 overflow-hidden">
                  <TabsContent value="songs" className="flex flex-col h-full">
                    <CardTitle className="flex-shrink-0">Available Songs</CardTitle>
                    <CardDescription className="flex-shrink-0">Drag a song into a playlist on the left.</CardDescription>
                    {/* Add a Song*/ }
                    <div className="flex w-full">
                      <Button className="mt-4">
                        <FontAwesomeIcon icon={faPlus} />
                        Add a Song
                      </Button>
                    </div>
                    {/* Available Songs */}
                    <ScrollArea className="flex-grow max-h-full pr-2 mt-4">
                      {searchResults.length > 0 ? (
                        searchResults.map((song) => (
                          <DraggableSong key={song.id} song={song}>
                            <div className="flex items-center justify-between p-3 border-b last:border-b-0 dark:border-neutral-700 hover:bg-neutral-100 dark:hover:bg-neutral-700 rounded-md transition-colors">
                              <div className="text-left">
                                <p className="font-medium">{song.title}</p>
                                <p className="text-sm text-neutral-600 dark:text-neutral-400">{song.artist}</p>
                              </div>
                              <div className="flex items-center space-x-3">
                                <span className="text-sm text-neutral-500 dark:text-neutral-400">{song.duration}</span>
                                <Button size="sm" onClick={() => playSong(song)}>
                                  <FontAwesomeIcon icon={faPlay} />
                                </Button>
                              </div>
                            </div>
                          </DraggableSong>
                        ))
                      ) : (
                        <p className="text-center text-neutral-500 dark:text-neutral-400 mt-8">No songs found.</p>
                      )}
                    </ScrollArea>
                  </TabsContent>

                  <TabsContent value="playlists" className="flex flex-col h-full">
                    {selectedPlaylist ? (
                      <>
                        <CardTitle className="flex-shrink-0">{selectedPlaylist.name}</CardTitle>
                        <CardDescription className="flex-shrink-0">{selectedPlaylist.songIds.length} songs</CardDescription>
                        <ScrollArea className="flex-grow pr-2 mt-4">
                          {selectedPlaylist.songIds.map((songId) => {
                            const song = mockSongs.find(s => s.id === songId);
                            if (!song) return null;

                            return (
                              <div key={song.id} className="flex items-center justify-between p-3 border-b last:border-b-0 dark:border-neutral-700 hover:bg-neutral-100 dark:hover:bg-neutral-700 rounded-md transition-colors">
                                <div className="text-left">
                                  <p className="font-medium">{song.title}</p>
                                  <p className="text-sm text-neutral-600 dark:text-neutral-400">{song.artist}</p>
                                </div>
                                <Button size="sm" onClick={() => playSong(song)}>
                                  <FontAwesomeIcon icon={faPlay} />
                                </Button>
                              </div>
                            )
                          })}
                          {selectedPlaylist.songIds.length === 0 && (
                             <p className="text-center text-neutral-500 dark:text-neutral-400 mt-8">Drag songs here to add them to this playlist!</p>
                          )}
                        </ScrollArea>
                        <Button className="mt-4 w-full flex-shrink-0">Play All</Button>
                      </>
                    ) : (
                      <p className="text-center text-neutral-500 dark:text-neutral-400 mt-8">Select a playlist from the sidebar to view its songs.</p>
                    )}
                  </TabsContent>
                </CardContent>
              </Tabs>
            </Card>
          </div>
        </main>

        {/* Music Player Bar (Fixed at Bottom) */}
        <footer
          className="
            sticky bottom-0 left-0 right-0
            w-full
            bg-neutral-200/90 dark:bg-neutral-800/90
            backdrop-blur-sm
            border-t border-neutral-200 dark:border-neutral-700
            p-4 flex items-center justify-between
            shadow-lg
            transition-colors duration-300 ease-in-out
            z-20
            flex-shrink-0
          "
        >
          <div className="flex items-center space-x-4 w-1/3">
            {currentSong ? (
              <>
                <div className="w-12 h-12 bg-neutral-200 dark:bg-neutral-700 rounded-md flex-shrink-0 flex items-center justify-center">
                  <span className="text-neutral-500 dark:text-neutral-400">🎶</span>
                </div>
                <div>
                  <p className="font-semibold">{currentSong.title}</p>
                  <p className="text-sm text-neutral-600 dark:text-neutral-400">{currentSong.artist}</p>
                </div>
              </>
            ) : (
              <p className="text-neutral-500 dark:text-neutral-400">No song playing</p>
            )}
          </div>

          <div className="flex-1 flex flex-col items-center max-w-lg">
            <div className="flex items-center space-x-4 mb-2">
              <Button variant="ghost" size="icon">
                <FontAwesomeIcon icon={faBackwardStep} />
              </Button>
              <Button size="lg" onClick={togglePlayPause}>
                {isPlaying ? (
                  <FontAwesomeIcon icon={faPause} />
                ) : (
                  <FontAwesomeIcon icon={faPlay} />
                )}
              </Button>
              <Button variant="ghost" size="icon">
                <FontAwesomeIcon icon={faForwardStep} />
              </Button>
            </div>
            <Slider
              defaultValue={[0]}
              max={100}
              step={1}
              className="w-full"
              aria-label="Progress"
            />
            <div className="flex justify-between w-full text-xs text-neutral-500 dark:text-neutral-400 mt-1">
              <span>0:00</span>
              <span>{currentSong ? currentSong.duration : '0:00'}</span>
            </div>
          </div>

          <div className="flex items-center space-x-3 w-1/3 justify-end">
            <Button variant="ghost" size="icon">
              <FontAwesomeIcon icon={faVolumeHigh} />
            </Button>
            <Slider
              defaultValue={[volume]}
              max={100}
              step={1}
              onValueChange={(val: number[]) => setVolume(val[0])}
              className="w-24"
              aria-label="Volume"
            />
            <Button variant="ghost" size="icon">
              <FontAwesomeIcon icon={faShuffle} />
            </Button>
            <Button variant="ghost" size="icon">
              <FontAwesomeIcon icon={faRotate} />
            </Button>
          </div>
        </footer>
      </div>

      <DragOverlay>
        {draggedSong ? (
          <div className="bg-white dark:bg-neutral-800 rounded-md shadow-lg p-3 flex items-center space-x-3 border border-blue-500 dark:border-blue-400">
            <div className="w-8 h-8 bg-neutral-200 dark:bg-neutral-700 rounded-sm flex-shrink-0 flex items-center justify-center">
              <span className="text-neutral-500 dark:text-neutral-400 text-lg">🎵</span>
            </div>
            <div className="text-left">
              <p className="font-medium text-neutral-900 dark:text-neutral-50">{draggedSong.title}</p>
              <p className="text-sm text-neutral-600 dark:text-neutral-400">{draggedSong.artist}</p>
            </div>
          </div>
        ) : null}
      </DragOverlay>
    </DndContext>
  );
};

export default Home;
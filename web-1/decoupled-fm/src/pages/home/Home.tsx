import { useState, useEffect, useCallback, useRef } from 'react';
import ThemeToggle from '@/components/ThemeToggle';
import DroppablePlaylist from '@/components/DroppablePlaylist';
import DraggableSong from '@/components/DraggableSong';
import { addSongToDB, getSongsFromDB, type Song } from '@/database/database';

// Music metadata
import { parseBlob, type IAudioMetadata } from 'music-metadata';

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
  faMusic,
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

const initialPlaylists: Playlist[] = [
  { id: 'p1', name: 'Chillwave Vibes', songIds: [1, 3] },
  { id: 'p2', name: 'Deep Cyberpunk', songIds: [2, 4] },
  { id: 'p3', name: 'Morning Code', songIds: [5] },
];

const Home: React.FC = () => {
  const fileInputRef = useRef<HTMLInputElement>(null);
  const [storedSongs, setStoredSongs] = useState<Song[]>([]);
  const [currentSong, setCurrentSong] = useState<Song | null>(null);
  const [isPlaying, setIsPlaying] = useState<boolean>(false);
  const [volume, setVolume] = useState<number>(50);
  const [searchQuery, setSearchQuery] = useState<string>('');
  const [searchResults, setSearchResults] = useState<Song[]>(storedSongs);
  const [playlists, setPlaylists] = useState<Playlist[]>(initialPlaylists);
  const [selectedPlaylist, setSelectedPlaylist] = useState<Playlist | null>(null);
  const [activeId, setActiveId] = useState<UniqueIdentifier | null>(null);
  const [activeTab, setActiveTab] = useState<string>("songs"); // State to manage active tab
  const audioRef = useRef<HTMLAudioElement>(new Audio()); // Audio element for playback

  const draggedSong = activeId ? storedSongs.find(s => s.id === activeId) : null;

  const sensors = useSensors(
    useSensor(PointerSensor, { activationConstraint: { distance: 8 } }),
    useSensor(KeyboardSensor)
  );

  // Function to load and display songs from IndexedDB
  const loadSongs = useCallback(async () => {
    try {
      const songs: Song[] = await getSongsFromDB();
      setStoredSongs(songs);

      // Conditionally update searchResults based on searchQuery
      if (searchQuery) {
        const filtered = songs.filter(song =>
          (song.title?.toLowerCase().includes(searchQuery.toLowerCase()) ||
          song.artist?.toLowerCase().includes(searchQuery.toLowerCase()) ||
          song.album?.toLowerCase().includes(searchQuery.toLowerCase()) ||
          song.name.toLowerCase().includes(searchQuery.toLowerCase()))
        );
        setSearchResults(filtered);
      } else {
        // If no search query, display all songs
        setSearchResults(songs);
      }
    } catch (error: any) {
      console.error(`Error loading songs: ${error}`);
    }
  }, []); // Dependency on searchQuery to re-run filtering if query changes

  useEffect(() => {
    loadSongs();
  }, [loadSongs]);

  // Handles the click on the "Add a Song" button
  const handleAddSongButtonClick = (): void => {
    fileInputRef.current?.click(); // Programmatically click the hidden file input
  };

  const handleSearch = (event: React.ChangeEvent<HTMLInputElement>): void => {
    const query = event.target.value;
    setSearchQuery(query);

    if (query) {
      const filtered = storedSongs.filter(song =>
        (song.title?.toLowerCase().includes(query.toLowerCase()) ||
        song.artist?.toLowerCase().includes(query.toLowerCase()) ||
        song.album?.toLowerCase().includes(query.toLowerCase()))
      );
      setSearchResults(filtered);
    } else {
      // If search query is empty, show all stored songs
      setSearchResults(storedSongs);
    }
  };

  const handlePlaySong = (song: Song): void => {
    setCurrentSong(song);
    setIsPlaying(true);

    if (currentSong && currentSong.id === song.id) {
      // If the same song is playing, pause it
      audioRef.current.pause();
      setIsPlaying(false);
    } else {
      // Create a Blob from the ArrayBuffer and then a URL
      const audioBlob: Blob = new Blob([song.data], { type: 'audio/mpeg' }); // Adjust MIME type if necessary
      const audioUrl: string = URL.createObjectURL(audioBlob);

      audioRef.current.src = audioUrl;
      setCurrentSong(song); // Set current playing song before play()
      audioRef.current.play();

      audioRef.current.onended = () => {
        setCurrentSong(null);
        URL.revokeObjectURL(audioUrl); // Clean up the URL after playback
      };

      audioRef.current.onerror = (e: string | Event) => {
        console.error('Audio playback error:', e);
        setIsPlaying(false);
        URL.revokeObjectURL(audioUrl);
      };
    }
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

  // Handles the file selection from the input
  const handleFileChange = async (event: React.ChangeEvent<HTMLInputElement>): Promise<void> => {
    const file: File | undefined = event.target.files?.[0];
    if (!file) {
      return;
    }

    // Check if the file is an audio file
    if (!file.type.startsWith('audio/')) {
      //setMessage('Please select an audio file.');
      return;
    }

    //setLoading(true);
    //setMessage(`Adding "${file.name}"...`);

    const reader: FileReader = new FileReader();

    reader.onload = async (e: ProgressEvent<FileReader>) => {
      try {
        const arrayBuffer: ArrayBuffer = e.target?.result as ArrayBuffer; // The audio data as ArrayBuffer
        console.log('ArrayBuffer obtained from FileReader:', arrayBuffer);
        console.log('ArrayBuffer byteLength:', arrayBuffer.byteLength);
        let songTitle: string | undefined = file.name; // Default to filename
        let songArtist: string | undefined;
        let songAlbum: string | undefined;
        let songDuration: number | undefined;
        let songImageUrl: string | undefined; // New state for image URL

        try {
          // Attempt to parse metadata using music-metadata
          const metadata: IAudioMetadata = await parseBlob(file);
          if (metadata.common.title) {
            songTitle = metadata.common.title;
          }
          if (metadata.common.artist) {
            songArtist = metadata.common.artist;
          }
          if (metadata.common.album) {
            songAlbum = metadata.common.album;
          }
          if (metadata.format.duration) {
            songDuration = metadata.format.duration;
          }

          // Extract album art
          if (metadata.common.picture && metadata.common.picture.length > 0) {
            const picture = metadata.common.picture[0];
            // music-metadata provides data as Uint8Array, convert to Blob then Data URL
            const imageBlob = new Blob([picture.data], { type: picture.format });
            const imageDataUrl = await new Promise<string>((resolve, reject) => {
              const imageReader = new FileReader();
              imageReader.onloadend = () => resolve(imageReader.result as string);
              imageReader.onerror = reject;
              imageReader.readAsDataURL(imageBlob);
            });
            songImageUrl = imageDataUrl;
          }

          //setMessage(`Extracted metadata for "${songTitle || file.name}".`);
        } catch (metadataError) {
          console.warn('Could not extract metadata from file:', metadataError);
          //setMessage(`Could not extract metadata from "${file.name}". Adding with filename.`);
        }

        const songDataToStore: Omit<Song, 'id' | 'addedAt'> = {
          name: file.name, // Keep original filename
          data: arrayBuffer,
          title: songTitle,
          artist: songArtist ?? 'Unknown Artist',
          album: songAlbum,
          duration: songDuration,
          imageUrl: songImageUrl, // Store the extracted image URL
        };

        await addSongToDB(songDataToStore);
        //setMessage(`"${songTitle || file.name}" added successfully!`);
        loadSongs(); // Reload the list of songs, which will now re-filter searchResults
      } catch (error: any) {
        //setMessage(`Failed to add "${file.name}": ${error}`);
      } finally {
        //setLoading(false);
        // Clear the file input to allow selecting the same file again if needed
        event.target.value = ''; // Reset file input
      }
    };

    reader.onerror = () => {
      //setMessage('Error reading file.');
      //setLoading(false);
      event.target.value = ''; // Reset file input
    };

    reader.readAsArrayBuffer(file); // Read the file as an ArrayBuffer
  };

  // --- Utility Function for Duration Formatting ---
  const formatDuration = (seconds?: number): string => {
    if (seconds === undefined || isNaN(seconds) || seconds < 0) {
      return 'N/A';
    }

    const hours = Math.floor(seconds / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    const remainingSeconds = Math.floor(seconds % 60);

    const formattedMinutes = minutes < 10 && hours > 0 ? `0${minutes}` : `${minutes}`;
    const formattedSeconds = remainingSeconds < 10 ? `0${remainingSeconds}` : `${remainingSeconds}`;

    if (hours > 0) {
      return `${hours}:${formattedMinutes}:${formattedSeconds}`;
    } else {
      return `${minutes}:${formattedSeconds}`;
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
                    <input
                      type="file"
                      accept="audio/*" // Accepts any audio file type
                      ref={fileInputRef}
                      onChange={handleFileChange}
                      className="hidden" // Hide the input visually
                    />
                    {/* Add a Song*/ }
                    <div className="flex w-full">
                      <Button onClick={handleAddSongButtonClick} className="mt-4">
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
                                <span className="text-sm text-neutral-500 dark:text-neutral-400">{formatDuration(song.duration)}</span>
                                <Button size="sm" onClick={() => handlePlaySong(song)}>
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
                            const song = storedSongs.find(s => s.id === songId);
                            if (!song) return null;

                            return (
                              <div key={song.id} className="flex items-center justify-between p-3 border-b last:border-b-0 dark:border-neutral-700 hover:bg-neutral-100 dark:hover:bg-neutral-700 rounded-md transition-colors">
                                <div className="text-left">
                                  <p className="font-medium">{song.title}</p>
                                  <p className="text-sm text-neutral-600 dark:text-neutral-400">{song.artist}</p>
                                </div>
                                <Button size="sm" onClick={() => handlePlaySong(song)}>
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
          <div className="flex items-center space-x-4 w-1/3 ml-4">
            {currentSong ? (
              <>
                <div className="w-12 h-12 bg-neutral-200 dark:bg-neutral-700 rounded-md flex-shrink-0 flex items-center justify-center">
                  {currentSong.imageUrl ? (
                    <div className="w-16 h-16 rounded-lg overflow-hidden flex-shrink-0 shadow-md">
                      <img
                        src={currentSong.imageUrl}
                        alt="Album Art"
                        className="w-full h-full object-contain"
                      />
                    </div>
                  ) : (
                    <div className="w-16 h-16 rounded-md bg-neutral-200 dark:bg-neutral-700 flex items-center justify-center flex-shrink-0 shadow-md">
                      <FontAwesomeIcon icon={faMusic} className="text-3xl text-neutral-500 dark:text-neutral-400" />
                    </div>
                  )}
                </div>
                <div className="ml-4 text-left">
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
              <span>{currentSong ? formatDuration(currentSong.duration) : '0:00'}</span>
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
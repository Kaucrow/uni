import { useState, useEffect, useCallback, useRef } from 'react';
import ThemeToggle from '@/components/ThemeToggle';
import DroppablePlaylist from '@/components/DroppablePlaylist';
import DraggableSong from '@/components/DraggableSong';
import LogoIcon from '@/components/LogoIcon';

// IndexedDB
import {
  addSongToDB,
  getSongsFromDB,
  addPlaylistToDB,
  deleteSongFromDB,
  getPlaylistsFromDB,
  updatePlaylistInDB,
  type Song,
  type Playlist,
  deletePlaylistFromDB
} from '@/database/database';

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
  faTrash,
  faCompactDisc,
  faBars,
} from '@fortawesome/free-solid-svg-icons'

// Shadcn UI components
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Card, CardContent, CardHeader, CardTitle, CardDescription } from '@/components/ui/card';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Slider } from '@/components/ui/slider';
import { Separator } from '@/components/ui/separator';
import { ScrollArea } from '@/components/ui/scroll-area';
import { Checkbox } from '@/components/ui/checkbox';

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

const Home: React.FC = () => {
  const [storedSongs, setStoredSongs] = useState<Song[]>([]);
  const [currentSong, setCurrentSong] = useState<Song | null>(null);
  const [isPlaying, setIsPlaying] = useState<boolean>(false);
  const [volume, setVolume] = useState<number>(50);
  const [activeTab, setActiveTab] = useState<string>("songs"); // State to manage active tab
  const audioRef = useRef<HTMLAudioElement>(new Audio()); // Audio element for playback
  const fileInputRef = useRef<HTMLInputElement>(null);

  // Drag and Drop functionality
  const [activeId, setActiveId] = useState<UniqueIdentifier | null>(null);

  // Playlist functionality
  const [playlists, setPlaylists] = useState<Playlist[]>([]);
  const [isCreatingPlaylist, setIsCreatingPlaylist] = useState<boolean>(false);
  const [newPlaylistName, setNewPlaylistName] = useState<string>('');
  const [selectedPlaylist, setSelectedPlaylist] = useState<Playlist | null>(null);

  // Playlist playing functionality
  const [currentSongIndex, setCurrentSongIndex] = useState<number | null>(null);
  const [isPlaylistMode, setIsPlaylistMode] = useState<boolean>(false);
  const [playingPlaylist, setPlayingPlaylist] = useState<Song[] | null>(null);

  // Search functionality
  const [searchQuery, setSearchQuery] = useState<string>('');
  const [searchResults, setSearchResults] = useState<Song[]>(storedSongs);

  // Playback progress
  const [sliderValue, setSliderValue] = useState<number>(0);

  // Checkbox selection
  const [checkedSongs, setCheckedSongs] = useState<number[]>([]); // Array of song IDs
  const [checkedPlaylists, setCheckedPlaylists] = useState<number[]>([]); // Array of playlist IDs

  const draggedSong = activeId ? storedSongs.find(s => s.id === activeId) : null;

  const sensors = useSensors(
    useSensor(PointerSensor, { activationConstraint: { distance: 8 } }),
    useSensor(KeyboardSensor)
  );

  // Keep selectedPlaylist in sync with playlists
  useEffect(() => {
    if (selectedPlaylist) {
      // Find the updated version of the selected playlist
      const updatedPlaylist = playlists.find(p => p.id === selectedPlaylist.id);
      setSelectedPlaylist(updatedPlaylist || null);
    }
  }, [playlists]);

  // Sync audio volume when volume state changes
  useEffect(() => {
    if (audioRef.current) {
      audioRef.current.volume = volume / 150;
    }
  }, [volume]);

  // Function to load and display songs from IndexedDB
  const loadSongs = useCallback(async () => {
    try {
      const songs: Song[] = await getSongsFromDB();
      setStoredSongs(songs);
      setCheckedSongs([]);

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
  }, []);

  // Function to load and display playlists from IndexedDB
  const loadPlaylists = useCallback(async () => {
    try {
      const fetchedPlaylists: Playlist[] = await getPlaylistsFromDB();
      setPlaylists(fetchedPlaylists);
    } catch (error: any) {
      console.error("Error loading playlists:", error);
    }
  }, []);

  useEffect(() => {
    loadSongs();
    loadPlaylists();
  }, [loadSongs, loadPlaylists]);

  // --- Audio Playback Event Handlers ---
  const handleTimeUpdate = useCallback(() => {
    if (audioRef.current.duration > 0) {
      const progress = (audioRef.current.currentTime / audioRef.current.duration) * 100;
      setSliderValue(progress);
    }
  }, []);

  const handleLoadedMetadata = useCallback(() => {
    setSliderValue(0);
  }, []);

  const handleAudioEnded = useCallback(() => {
    console.log(isPlaylistMode);
    console.log(playingPlaylist);
    console.log(currentSongIndex);
    if (isPlaylistMode && playingPlaylist && currentSongIndex !== null) {
      const nextIndex = currentSongIndex + 1;

      if (nextIndex < playingPlaylist.length) {
        setCurrentSongIndex(nextIndex);
        handlePlaySong(playingPlaylist[nextIndex], selectedPlaylist ?? undefined);
      } else {
        // Playlist ended
        endPlayback();
      }
    } else {
      // Single-song mode
      endPlayback();
    }
  }, [isPlaylistMode, playingPlaylist, currentSongIndex]); 

  const endPlayback = () => {
    setCurrentSong(null);
    setIsPlaying(false);
    setSliderValue(0);
    // Clean up Blob URL if it was created
    if (audioRef.current.src.startsWith('blob:')) {
      URL.revokeObjectURL(audioRef.current.src);
    }
    audioRef.current.src = ''; // Clear source
  }

  useEffect(() => {
    const audio = audioRef.current;
    audio.addEventListener('timeupdate', handleTimeUpdate);
    audio.addEventListener('loadedmetadata', handleLoadedMetadata);
    audio.addEventListener('ended', handleAudioEnded);

    return () => {
      audio.removeEventListener('timeupdate', handleTimeUpdate);
      audio.removeEventListener('loadedmetadata', handleLoadedMetadata);
      audio.removeEventListener('ended', handleAudioEnded);
    }
  }, [handleTimeUpdate, handleLoadedMetadata, handleAudioEnded]);

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

  const handlePlaySong = (song: Song, playlist?: Playlist): void => {
    // Clean up previous Blob URL if any
    if (audioRef.current.src.startsWith('blob:')) {
      URL.revokeObjectURL(audioRef.current.src);
    }

    // Set playlist context if provided
    if (playlist) {
      setIsPlaylistMode(true);
    } else {
      setIsPlaylistMode(false); // Single-song mode
    }

    // Create a Blob from the ArrayBuffer and then a URL
    const audioBlob: Blob = new Blob([song.data], { type: 'audio/mpeg' }); // Adjust MIME type if necessary
    const audioUrl: string = URL.createObjectURL(audioBlob);

    audioRef.current.src = audioUrl;
    audioRef.current.currentTime = 0; // Reset playback to start
    setCurrentSong(song);
    audioRef.current.play();

    setIsPlaying(true);

    audioRef.current.onerror = (e: string | Event) => {
      console.error('Audio playback error:', e);
      setIsPlaying(false);
      URL.revokeObjectURL(audioUrl);
    };
  };

  const handlePlayPlaylist = (playlist: Playlist) => {
    const songsInPlaylist = getSongsFromPlaylist(playlist, storedSongs);
    if (songsInPlaylist.length === 0) return;
    
    setPlayingPlaylist(songsInPlaylist);
    setCurrentSongIndex(0);
    handlePlaySong(songsInPlaylist[0], playlist);
  };

  // Handles slider value change (seeking)
  const handleSliderChange = (value: number[]): void => {
    const newProgress = value[0];
    setSliderValue(newProgress);
    if (audioRef.current.duration > 0) {
      audioRef.current.currentTime = (newProgress / 100) * audioRef.current.duration;
    }
  };

  const togglePlayPause = () => {
    if (!currentSong) return;

    setIsPlaying(!isPlaying);

    if (isPlaying)
      audioRef.current.pause();
    else
      audioRef.current.play();
  };

  const handleDragStart = (event: { active: { id: UniqueIdentifier; }; }) => {
    setActiveId(event.active.id);
  };

  const handleDragEnd = async (event: DragEndEvent) => {
    const { active, over } = event;
    setActiveId(null); // Clear activeId after drag ends

    if (active && over) {
      const draggedSongData = active.data.current?.song as Song;
      const targetType = over.data.current?.type;
      const targetPlaylistId = over.data.current?.playlistId;

      if (draggedSongData && draggedSongData.id !== undefined && targetType === 'playlist' && targetPlaylistId !== undefined) {
        // Find the target playlist
        const targetPlaylistIndex = playlists.findIndex(p => p.id === targetPlaylistId);

        if (targetPlaylistIndex !== -1) {
          const targetPlaylist = { ...playlists[targetPlaylistIndex] }; // Create a mutable copy

          // Check if song is already in playlist
          if (!targetPlaylist.songIds.includes(draggedSongData.id)) {
            targetPlaylist.songIds = [...targetPlaylist.songIds, draggedSongData.id];

            // Update state
            setPlaylists((prevPlaylists) => {
              const updatedPlaylists = [...prevPlaylists];
              updatedPlaylists[targetPlaylistIndex] = targetPlaylist;
              return updatedPlaylists;
            });

            // Persist to IndexedDB
            try {
              await updatePlaylistInDB(targetPlaylist);
              //loadPlaylists(); // Reload playlists to ensure UI is consistent
            } catch (error) {
              console.error('Failed to update playlist in DB:', error);
            }
          }
        }
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
      return;
    }

    const reader: FileReader = new FileReader();

    reader.onload = async (e: ProgressEvent<FileReader>) => {
      try {
        const arrayBuffer: ArrayBuffer = e.target?.result as ArrayBuffer; // The audio data as ArrayBuffer
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
        } catch (metadataError) {
          console.warn('Could not extract metadata from file:', metadataError);
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
        loadSongs(); // Reload the list of songs, which will now re-filter searchResults
      } catch (error: any) {
        console.error(`Failed to add "${file.name}": ${error}`);
      } finally {
        // Clear the file input to allow selecting the same file again if needed
        event.target.value = '';
      }
    };

    reader.onerror = (e: Event) => {
      console.error(`Error reading file: ${e}`);
      event.target.value = ''; // Reset file input
    };

    reader.readAsArrayBuffer(file); // Read the file as an ArrayBuffer
  };

  const handleCreatePlaylist = async (): Promise<void> => {
    // Only attempt to create if there's a non-empty name and we are in creation mode
    if (!isCreatingPlaylist) {
      return;
    }

    if (!newPlaylistName) {
      setNewPlaylistName('');
      setIsCreatingPlaylist(false);
      return;
    }

    try {
      await addPlaylistToDB(newPlaylistName.trim());
      setNewPlaylistName('');
      setIsCreatingPlaylist(false);
      loadPlaylists(); // Reload playlists to update UI
    } catch (error: any) {
      console.error(`Failed to create playlist: ${error}`);
    }
  };

  // Handles checkbox change for song selection
  const handleSongCheckboxChange = (songId: number, isChecked: boolean): void => {
    setCheckedSongs(prevCheckedSongs => {
      if (isChecked) {
        return [...prevCheckedSongs, songId];
      } else {
        return prevCheckedSongs.filter((id: number) => id !== songId);
      }
    });
  };

  // Handles checkbox change for playlist selection
  const handlePlaylistCheckboxChange = (playlistId: number, isChecked: boolean): void => {
    setCheckedPlaylists(prevCheckedPlaylists => {
      if (isChecked) {
        return [...prevCheckedPlaylists, playlistId];
      } else {
        return prevCheckedPlaylists.filter((id: number) => id !== playlistId);
      }
    });
  };

  // Handles deleting all checked songs
  const handleDeleteSelectedSongs = async (): Promise<void> => {
    if (checkedSongs.length === 0) return;

    try {
      for (const songId of checkedSongs) {
        await deleteSongFromDB(songId);
        // If a deleted song was playing, stop playback
        if (currentSong && currentSong.id === songId) {
          audioRef.current.pause();
          audioRef.current.src = '';
          setCurrentSong(null);
          setIsPlaying(false);
        }
      }
      console.log(`${checkedSongs.length} songs deleted successfully!`);
      loadSongs(); // Reload songs and clear checkedSongs
      loadPlaylists(); // Reload playlists
    } catch (error: any) {
      console.error(`Failed to delete selected songs: ${error}`);
    }
  };

  const handleDeleteSelectedPlaylists = async (): Promise<void> => {
    if (checkedPlaylists.length === 0) return;

    try {
      for (const playlistId of checkedPlaylists) {
        await deletePlaylistFromDB(playlistId);
      }
      console.log(`${checkedPlaylists.length} playlists deleted successfully!`);
      setCheckedPlaylists([]); // clear checkedPlaylists
      loadPlaylists(); // Reload playlists
    } catch (error: any) {
      console.error(`Failed to delete selected playlists: ${error}`);
    }
  };

  // --- Utility Functions ---

  // Obtaining Songs from a Playlist
  const getSongsFromPlaylist = (playlist: Playlist, storedSongs: Song[]): Song[] => {
  return playlist.songIds
    .map(id => storedSongs.find(song => song.id === id))
    .filter((song): song is Song => song !== undefined); // Filter out undefined
  };

  // Duration Formatting
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
          flex flex-col
          rounded-md
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
            rounded-t-md
            flex justify-between items-center
            py-4 px-6
            bg-neutral-200/80 dark:bg-neutral-800/80
            shadow-lg
            transition-colors duration-300 ease-in-out
            z-10
            flex-shrink-0
          "
        >
          <div className="ml-2 flex flex-row items-center gap-0">
            <h1 className="title pt-2 mr-[-0.15em] text-4xl font-bold tracking-tight select-none text-custom-accent">
              S
            </h1>
            {LogoIcon} 
            <h1 className="title pt-2 ml-[-0.15em] text-4xl font-bold tracking-tight select-none text-custom-accent">
              undown
            </h1>
          </div>
          <nav className="flex items-center space-x-4">
            {/*
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
            */}
            <ThemeToggle />
          </nav>
        </header>

        {/* Main Content Area */}
        <main className="flex flex-1 overflow-hidden p-6 gap-6">
          {/* Left Sidebar - Current Song Info, Navigation & Playlists */}
          <div className="w-1/4 flex flex-col flex-shrink-0 gap-6">
            {/* Playing Song Info */}
            <Card className="flex flex-col flex-shrink-0 gap-0 p-4 min-h-92">
              <div className="aspect-square w-full border-1 flex flex-col items-center justify-center">
                {!currentSong || !currentSong.imageUrl ?
                  <FontAwesomeIcon icon={faCompactDisc} size="4x" className="text-neutral-300 dark:text-neutral-800"/>
                  :
                  <img
                    src={currentSong.imageUrl}
                    alt="Album Art"
                    className="w-full h-full object-contain"
                  />
                }
              </div>
              {currentSong &&
                <div>
                  <p className="mt-2">{currentSong.title}</p>
                  <p className="text-sm text-neutral-600 dark:text-neutral-400">{currentSong.artist}</p>
                  <p className="text-xs text-neutral-600 dark:text-neutral-400">{currentSong.album}</p>
                </div>
              }
            </Card>

            <Card className="h-full">
              <CardContent className="flex flex-col space-y-3 items-left">
                <div className="flex text-left pl-4 mb-6 text-custom-accent gap-2">
                  <FontAwesomeIcon icon={faBars}/>
                  <CardTitle>Navigation</CardTitle>
                </div>
                <a className="w-full" href="/">
                  <Button className="min-w-full justify-start text-left" variant="ghost">
                    <FontAwesomeIcon icon={faHouse} />
                    Home
                  </Button>
                </a>
                <Separator className="flex-shrink-0" />
                <div className="flex text-left pl-4 mt-2 mb-4 text-custom-accent gap-2">
                  <FontAwesomeIcon icon={faCompactDisc}/>
                  <CardTitle>Playlists</CardTitle>
                </div>
                <div className="w-full">
                  {playlists.map((playlist) => (
                    <div key={playlist.id} className="flex flex-row max-h-full w-full items-center">
                      <Checkbox
                        id={`checkbox-${playlist.id}`}
                        checked={checkedPlaylists.includes(playlist.id!)} // Use non-null assertion as id will exist
                        onCheckedChange={(checked) => handlePlaylistCheckboxChange(playlist.id!, checked as boolean)}
                        className="mr-3"
                      />
                      <div className="w-full">
                      <DroppablePlaylist key={playlist.id} id={playlist.id}>
                        <Button
                          variant="ghost"
                          className="justify-start w-full"
                          onClick={() => {
                            setSelectedPlaylist(playlist);
                            setActiveTab("playlists"); // Automatically switch to playlists tab
                          }}
                        >
                          <FontAwesomeIcon icon={faCircle} size="2xs" className="mt-[0.25rem]"/>
                          <p className="truncate font-medium text-base">
                            {playlist.name}
                          </p>
                        </Button>
                      </DroppablePlaylist>
                      </div>
                    </div>
                  ))}
                </div >

                <div className="flex justify-center w-full flex-shrink-0 mt-2 mb-2">
                  {!isCreatingPlaylist ? (
                    <div className="flex flex-col gap-2">
                      <Button onClick={() => setIsCreatingPlaylist(true)}>
                        <FontAwesomeIcon icon={faPlus}/>
                        Create New Playlist
                      </Button>
                      {checkedPlaylists.length > 0 &&
                        <Button variant="ghost" onClick={handleDeleteSelectedPlaylists}>
                          <FontAwesomeIcon className="text-destructive" icon={faTrash} size="2x"/>
                          <span className="text-destructive">Delete Selected</span>
                        </Button>
                      }
                    </div>
                  ) : (
                    <div className="text-left">
                      <Input
                        id="playlist-name"
                        type="text"
                        placeholder="Playlist name"
                        value={newPlaylistName}
                        onChange={(e) => setNewPlaylistName(e.target.value)}
                        onBlur={handleCreatePlaylist}
                        className="flex-grow"
                      />
                    </div>
                  )}
                </div>
              </CardContent>
            </Card>
          </div>

          {/* Main Content - Search & Playlists */}
          <div className="flex-1 flex flex-col gap-6 overflow-hidden">
            {/* Search Bar */}
            <Card className="p-4 w-full">
              <CardHeader>
                <CardTitle>Search for Music</CardTitle>
                <CardDescription>Find your next favorite track.</CardDescription>
              </CardHeader>
              <CardContent>
                <div className="relative flex space-x-2">
                  <FontAwesomeIcon
                    className="absolute left-3 top-2.5 text-custom-accent pointer-events-none"
                    icon={faMagnifyingGlass}
                  />
                  <Input
                    type="text"
                    placeholder="Search songs, artists..."
                    onChange={handleSearch}
                    className="pl-10"
                  />
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
                    <CardTitle>Available Songs</CardTitle>
                    <CardDescription className="flex-shrink-0">Drag a song into a playlist on the left.</CardDescription>

                    {/* Available Songs */}
                    <div className="flex-grow max-h-full mt-4">
                      {searchResults.length > 0 ? (
                        searchResults.map((song) => (
                          <div key={song.id} className="flex flex-row max-h-full w-full items-center">
                            <Checkbox
                              id={`checkbox-${song.id}`}
                              checked={checkedSongs.includes(song.id!)} // Use non-null assertion as id will exist
                              onCheckedChange={(checked) => handleSongCheckboxChange(song.id!, checked as boolean)}
                              className="mr-3"
                            />
                            <div className="flex-grow min-w-0">
                              <DraggableSong song={song}>
                                <div className="flex items-center justify-between w-full p-3 border-b last:border-b-0 dark:border-neutral-700 hover:bg-neutral-100 dark:hover:bg-neutral-700 rounded-md transition-colors">
                                  <div className="text-left min-w-0">
                                    <p className="font-medium truncate">{song.title}</p>
                                    <p className="text-sm text-neutral-600 dark:text-neutral-400 truncate">{song.artist}</p>
                                  </div>
                                  <div className="flex items-center space-x-3 flex-shrink-0">
                                    <span className="text-sm text-neutral-500 dark:text-neutral-400">
                                      {formatDuration(song.duration)}
                                    </span>
                                    <Button bgColor="custom-accent" size="sm" onClick={() => handlePlaySong(song)} className="flex-shrink-0">
                                      <FontAwesomeIcon icon={faPlay} />
                                    </Button>
                                  </div>
                                </div>
                              </DraggableSong>
                            </div>
                          </div>
                        ))
                      ) : (
                        <p className="text-center text-neutral-500 dark:text-neutral-400 mt-8">No songs found.</p>
                      )}
                    </div>
                    {/* Song Actions Row */ }
                    <div className="flex flex-row items-center justify-start gap-3 mt-4">
                      {/* Invisible File Selector */}
                      <input
                        type="file"
                        accept="audio/*" // Accepts any audio file type
                        ref={fileInputRef}
                        onChange={handleFileChange}
                        className="hidden" // Hide the input visually
                      />
                      {/* Add Song Button */}
                      <Button onClick={handleAddSongButtonClick}>
                        <FontAwesomeIcon icon={faPlus} />
                        Add a Song
                      </Button>
                      {/* Delete Song Button */}
                      {checkedSongs.length > 0 &&
                        <Button onClick={handleDeleteSelectedSongs} variant="ghost">
                          <FontAwesomeIcon size="2x" icon={faTrash} className="text-destructive"/>
                          <span className="text-destructive">Delete Selected</span>
                        </Button>
                      }
                    </div>
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
                                <Button bgColor="custom-accent" size="sm" onClick={() => handlePlaySong(song)}>
                                  <FontAwesomeIcon icon={faPlay} />
                                </Button>
                              </div>
                            )
                          })}
                          {selectedPlaylist.songIds.length === 0 && (
                            <p className="text-center text-neutral-500 dark:text-neutral-400 mt-8">Drag songs into the playlist to add them!</p>
                          )}
                        </ScrollArea>
                        <Button bgColor="custom-accent" onClick={() => handlePlayPlaylist(selectedPlaylist)} className="mt-4 w-full flex-shrink-0">
                          <FontAwesomeIcon icon={faPlay}/>
                          Play All
                        </Button>
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
            rounded-b-md
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
              <Button color="custom-accent" variant="ghost" size="icon">
                <FontAwesomeIcon icon={faBackwardStep} />
              </Button>
              <Button bgColor="custom-accent" size="lg" onClick={togglePlayPause}>
                {isPlaying ? (
                  <FontAwesomeIcon icon={faPause} />
                ) : (
                  <FontAwesomeIcon icon={faPlay} />
                )}
              </Button>
              <Button color="custom-accent" variant="ghost" size="icon">
                <FontAwesomeIcon icon={faForwardStep} />
              </Button>
            </div>
            <Slider
              defaultValue={[0]}
              max={100}
              step={1}
              value={[sliderValue]}
              onValueChange={handleSliderChange}
              className="w-full"
              aria-label="Progress"
              rangeClassName='bg-custom-accent'
              thumbClassName='bg-custom-accent border-transparent'
            />
            <div className="flex justify-between w-full text-xs text-neutral-500 dark:text-neutral-400 mt-1">
              <span>
                {currentSong ? (
                  currentSong.duration && 
                  audioRef.current.currentTime > currentSong.duration
                    ? formatDuration(currentSong.duration) // Clamp to metadata duration
                    : formatDuration(audioRef.current.currentTime)
                  ) : '0:00'}
                </span>
              <span>{currentSong ? formatDuration(currentSong.duration) : '0:00'}</span>
            </div>
          </div>

          <div className="flex items-center space-x-3 w-1/3 justify-end">
            <Button color="custom-accent" variant="ghost" size="icon">
              <FontAwesomeIcon icon={faVolumeHigh} />
            </Button>
            <Slider
              defaultValue={[volume]}
              max={100}
              step={1}
              onValueChange={(val: number[]) => setVolume(val[0])}
              className="w-24"
              aria-label="Volume"
              rangeClassName="bg-custom-accent"
              thumbClassName="bg-custom-accent border-transparent"
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
            <div className="w-8 h-8 bg-neutral-200 dark:bg-neutral-700 rounded-sm flex-shrink-0 flex items-center justify-center overflow-hidden">
              <img
                src={draggedSong.imageUrl}
                alt="Album Art"
                className="w-full h-full object-contain"
              />
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
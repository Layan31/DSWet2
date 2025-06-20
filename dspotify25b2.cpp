// You can edit anything you want in this file.
// However you need to implement all public DSpotify function, as provided below as a template

#include "dspotify25b2.h"

DSpotify::DSpotify() = default;

DSpotify::~DSpotify() = default;

StatusType DSpotify::addGenre(int genreId) {
    if (genreId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (genres.find(genreId)) {
        return StatusType::FAILURE;
    }
    try {
        genres.insert(genreId, new Genre(genreId));
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType DSpotify::addSong(int songId, int genreId) {
    if (songId <= 0 || genreId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Genre* genre = genres.find(genreId);

    if (!genre) {
        return StatusType::FAILURE;
    }
    // Song exists in the union-find structure <=> song belongs to a tree that has a root.
    if (songs.get_root_val(songId)) {
        return StatusType::FAILURE;
    }

    try {
        songs.insert(songId, new Song(songId, genreId));
        songs.update_path_from(songId, 1);
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    if (genre->num_songs == 0) {
        // This is the first song of the genre.
        genre->root_song = songs.get_root_val(songId); // O(1), traverse tree with 1 node
    }
    else {
        // This isn't the first song of the genre, need to merge it into the existing tree that belongs to this genre.
        songs.merge(genre->root_song->song_id, songId);
    }

    genre->num_songs++;

    return StatusType::SUCCESS;
}

StatusType DSpotify::mergeGenres(int genreId1, int genreId2, int genreId3) {
    if (genreId1 <= 0 || genreId2 <= 0 || genreId3 <= 0 ||
        genreId1 == genreId2 || genreId2 == genreId3 || genreId3 == genreId1) {
        return StatusType::INVALID_INPUT;
    }

    Genre* genre1 = genres.find(genreId1);
    Genre* genre2 = genres.find(genreId2);

    if (!genre1 || !genre2) {
        return StatusType::FAILURE;
    }

    StatusType genre3_status = addGenre(genreId3);

    if (genre3_status == StatusType::FAILURE || genre3_status == StatusType::ALLOCATION_ERROR) {
        return genre3_status;
    }

    Genre* genre3 = genres.find(genreId3);

    // If both genres are empty - no merging needed in the union find.
    if (genre1->num_songs == 0 && genre2->num_songs == 0) {
        return StatusType::SUCCESS;
    }

    Genre *mergedInto, *merged;
    if (genre1->num_songs >= genre2->num_songs) {
        mergedInto = genre1;
        merged = genre2;
    }
    else {
        mergedInto = genre2;
        merged = genre1;
    }

    // Unite the trees of both genres, if neither are empty
    if (genre1->num_songs != 0 && genre2->num_songs != 0) {
        songs.merge(mergedInto->root_song->song_id, merged->root_song->song_id);
    }

    // Move into the new genre.
    genre3->root_song = songs.get_root_val(mergedInto->root_song->song_id); // O(1), traverse up from root to itself
    genre3->root_song->genre_id = genreId3;
    genre3->num_songs = genre1->num_songs + genre2->num_songs;

    // All songs in genre3 had a genre change - update the r value of the root
    songs.update_path_from(genre3->root_song->song_id, 1);

    // Reset the previous genres.
    genre1->clear();
    genre2->clear();

    return StatusType::SUCCESS;
}

output_t<int> DSpotify::getSongGenre(int songId){
    if (songId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Song* root_song = songs.get_root_val(songId);

    if (!root_song) {
        return StatusType::FAILURE;
    }

    return root_song->genre_id;
}

output_t<int> DSpotify::getNumberOfSongsByGenre(int genreId){
    if (genreId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Genre* genre = genres.find(genreId);

    if (!genre) {
        return StatusType::FAILURE;
    }

    return genre->num_songs;
}

output_t<int> DSpotify::getNumberOfGenreChanges(int songId){
    if (songId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // If song with songId isn't in the union-find structure - path_to returns -1.
    int num_changes = songs.path_to(songId);

    if (num_changes == -1) {
        return StatusType::FAILURE;
    }

    return num_changes;
}

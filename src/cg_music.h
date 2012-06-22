#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <alsa/asoundlib.h>

/* Global audio handle */
snd_pcm_t                *cg_audio;
extern snd_pcm_uframes_t  cg_frames;
/* Music file */
OggVorbis_File            cg_music;
char                     *cg_audio_buffer;
int                       cg_audio_buffer_size;
FILE                     *cg_music_file;

void InitMusic(char *musicfilename);
void PlayingMusic(snd_pcm_t *handle);
void SyncPlay();
void FreeMusic();

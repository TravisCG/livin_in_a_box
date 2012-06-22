#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <alsa/asoundlib.h>
#include <math.h>

#include "cg_music.h"

snd_pcm_uframes_t cg_frames = 256;

void InitMusic(char *musicfilename){
   snd_pcm_hw_params_t  *hw_params;
   snd_pcm_sw_params_t  *sw_params;
   vorbis_info          *music_info;
   int                   dir = 0;
   int                   err;

   /* Alsa initialization */
   err = snd_pcm_open(&cg_audio, "default", SND_PCM_STREAM_PLAYBACK, 0);
   if(err < 0) printf("snd_pcm_open error %s\n", snd_strerror(err));

   snd_pcm_hw_params_malloc(&hw_params);
   snd_pcm_hw_params_any(cg_audio, hw_params);
   snd_pcm_hw_params_set_access(cg_audio, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
   snd_pcm_hw_params_set_format(cg_audio, hw_params, SND_PCM_FORMAT_S16_LE);

   /* Vorbis file */
   cg_music_file = fopen(musicfilename, "r");
   ov_open(cg_music_file, &cg_music, NULL, 0);
   music_info = ov_info(&cg_music, -1);

   /* Set alsa channels and rate */
   snd_pcm_hw_params_set_rate_near(cg_audio, hw_params, (unsigned int *)&music_info->rate, 0);
   snd_pcm_hw_params_set_channels(cg_audio, hw_params, music_info->channels);

   /* Set alsa buffer and period */
   snd_pcm_hw_params_get_period_size(hw_params, &cg_frames, &dir);
   cg_audio_buffer_size = cg_frames * music_info->channels * 2;/* 2 bytes/sample */
   cg_audio_buffer = (char *)malloc(cg_audio_buffer_size);

   snd_pcm_hw_params(cg_audio, hw_params);
   snd_pcm_hw_params_free(hw_params);

   /* Software settings */
   snd_pcm_sw_params_malloc(&sw_params);
   snd_pcm_sw_params_current(cg_audio, sw_params);
   snd_pcm_sw_params_set_start_threshold(cg_audio, sw_params, cg_audio_buffer_size);
   snd_pcm_sw_params_set_avail_min(cg_audio, sw_params, cg_frames);
   err = snd_pcm_sw_params(cg_audio, sw_params);
   if(err < 0) printf("software setting error:%s\n", snd_strerror(err));
   snd_pcm_sw_params_free(sw_params);

   /* All settings finished */
   err = snd_pcm_prepare(cg_audio);
}

void PlayingMusic(snd_pcm_t *handle){
int bytes_needed = cg_audio_buffer_size;
int pcm_index = 0;
int bitstream;
long sample_length;
snd_pcm_sframes_t ret;
      /* Read vorbis file */
      while(bytes_needed > 0){
         sample_length = ov_read(&cg_music, cg_audio_buffer + pcm_index, bytes_needed, 0, 2, 1, &bitstream);
         switch(sample_length){
            case 0:
               bytes_needed = 0;
               break;
            default:
               pcm_index    += sample_length;
               bytes_needed -= sample_length;
         }
      }
      /* put the data to alsa*/
      ret = snd_pcm_writei(handle, cg_audio_buffer, cg_frames);
      if(ret < 0){
         printf("Recover\n");
         ret = snd_pcm_recover(handle, ret, 0);
      }
      if(ret < 0) ret = snd_pcm_prepare(handle);
      if(ret < 0) printf("Underrun %s\n", snd_strerror(ret));
}

static void PlayingCallback(snd_async_handler_t *ahandler){
   snd_pcm_t *handle = snd_async_handler_get_pcm(ahandler);
   snd_pcm_sframes_t avail;

   avail = snd_pcm_avail_update(handle);
   if(avail < 0) printf("update error: %s\n", snd_strerror(avail));

   while(avail >= (signed int)cg_frames){
      PlayingMusic(handle);
      avail = snd_pcm_avail_update(handle);
   }

}

void AsyncPlay(){
   snd_async_handler_t *ahandler;
   int count;

   count = snd_async_add_pcm_handler(&ahandler, cg_audio, PlayingCallback, NULL);
   if(count < 0) printf("No async handler. Reason = %s %d\n", snd_strerror(count),count);

   /*
      We need this cicle to send enought data to the callback.
      Without this we get a broken pipe error message in the callback.
   */
   for(count = 0; count < 2; count++){
      PlayingMusic(cg_audio);
   }

   count = snd_pcm_start(cg_audio);/* Start playing */
   if(count < 0) printf("No music. Reason = %s %d\n",snd_strerror(count),count);
}

/* Use this in the case of PulseAudio */
void SyncPlay(){
   snd_pcm_sframes_t avail;

   avail = snd_pcm_avail_update(cg_audio);
   if(avail < 0){
      printf("update error: %s\n", snd_strerror(avail));
   }

   while(avail >= (signed int)cg_frames){
      PlayingMusic(cg_audio);
      avail = snd_pcm_avail_update(cg_audio);
   }
}

void FreeMusic(){
   free(cg_audio_buffer);
   snd_pcm_close(cg_audio);
   ov_clear(&cg_music);
}

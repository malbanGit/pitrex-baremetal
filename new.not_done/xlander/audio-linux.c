/*
 * Based on code from:
 *     https://stackoverflow.com/questions/8485553/alsa-tutorial-required
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include <alsa/asoundlib.h>

bool USE_AUDIO = true;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static char retro_snd_buf[16 * 1024];
static snd_pcm_t *alsa;

static bool play_retro;

static void *snd_thread(void *arg)
{
do_loop:
	pthread_mutex_lock(&mtx);
	while (!play_retro)
		pthread_cond_wait(&cond, &mtx);
	pthread_mutex_unlock(&mtx);

	snd_pcm_prepare(alsa);

	while (play_retro)
		snd_pcm_writei(alsa, retro_snd_buf, sizeof(retro_snd_buf));

	goto do_loop;

	return NULL;
}

void snd_stop(void)
{
	if (!USE_AUDIO || !play_retro)
		return;

	pthread_mutex_lock(&mtx);
	play_retro = false;
	snd_pcm_pause(alsa, 1);
	pthread_mutex_unlock(&mtx);
}

void snd_start(void)
{
	if (!USE_AUDIO || play_retro)
		return;

	pthread_mutex_lock(&mtx);
	play_retro = true;
	snd_pcm_pause(alsa, 0);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mtx);
}

void snd_init(void)
{
	int i;
	int err;
	pthread_t tid;
	const char device[] = "plug:default";

	if (!USE_AUDIO)
		return;

	err = snd_pcm_open(&alsa, device, SND_PCM_STREAM_PLAYBACK, 0);
	if (err) {
		fprintf(stderr, "snd: cannot open audio device %s (%s)\n",
			device, snd_strerror(err));
		exit(EXIT_FAILURE);
	}

	err = snd_pcm_set_params(alsa,
				 SND_PCM_FORMAT_U8,
				 SND_PCM_ACCESS_RW_INTERLEAVED,
				 1,
				 16000,
				 1,
				 100000);
	if (err) {
		fprintf(stderr, "snd: params error (%s)\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}

	/* This sounds a bit like a thruster! */
	for (i = 0; i < sizeof(retro_snd_buf); i++)
		retro_snd_buf[i] = random() & 0xff * 2 / 10;

	pthread_create(&tid, NULL, snd_thread, NULL);
	pthread_detach(tid);
}

void snd_destroy(void)
{
	if (!USE_AUDIO)
		return;

	snd_pcm_close(alsa);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

unsigned long seed_t = 0;
unsigned long final_t = 1 << 19;
int use_wav = 0;

void usage(const char *p) {

	fprintf(stderr, "\nusage: %s [-h] [-w] [-s seed]\n\n", p);

	fprintf(stderr, "  -h          display help\n");
	fprintf(stderr, "  -w          output WAV\n");
	fprintf(stderr, "  -s seed     set seed value (default: %lu)\n", seed_t);

	fputc('\n', stderr);
}

void sequence(unsigned long *t) {

	unsigned long f = (*t)++;

	 //f = (f * 5) & (f >> 7) | (f * 3) & (f * 4 >> 10);

	 f = ((f<<4) & (f>>4)) + ((f<<7) & (f>>5)) ^ ((f<<3) & (f>>6));

	putchar(f);
}

void print32(u_int32_t v) {
	ssize_t n = fwrite(&v, 1, sizeof(v), stdout);
	assert(n == sizeof(v));
}

void print16(u_int16_t v) {
	ssize_t n = fwrite(&v, 1, sizeof(v), stdout);
	assert(n = sizeof(v));
}

void output_wav_header(int rate, int bits, int channels, int samples) {

	int bytes = bits / 8;
	unsigned long length = channels * samples * bytes;

	assert(bits % 8 == 0);

	// write the wav header

	fputs("RIFF", stdout);      // magic
	print32(36 + length);       // chunksize
	fputs("WAVE", stdout);      // format

	fputs("fmt ", stdout);            // subchunk id
	print32(16);                      // subchunk size
	print16(1);                       // audioformat (pcm)
	print16(channels);                // channels
	print32(rate);                    // sample rate
	print32(rate * channels * bytes); // byte rate
	print16(channels * bytes);        // block size
	print16(bits);                    // bits per sample

	fputs("data", stdout);      // subchunk id
	print32(length);            // subchunk size
}

int main(int argc, char **argv) {

	unsigned long samples;
	unsigned long t;
	int rate = 8000;
	int channels = 1;
	int bits = 8;
	int c;

	setvbuf(stdout, NULL, _IONBF, 0);

	while ((c = getopt(argc, argv, "hws:")) != -1) {

		switch (c) {

			case 's':

				seed_t = atoi(optarg);
				break;

			case 'w':

				use_wav = 1;
				break;

			case '?':

				/* fall-thru */

			case 'h':

				usage(*argv);
				exit(EXIT_FAILURE);

		}

	}

	fprintf(stderr, "  seed: %lu\n", seed_t);
	fprintf(stderr, " final: %lu\n", final_t);
	fprintf(stderr, "buffer: %d\n", sizeof(t) * 8);
	fprintf(stderr, "format: %s %dhz %d-bit %s\n", use_wav ? ".WAV" : ".PCM", rate, bits, channels == 1 ? "mono" : "stereo");

	samples = final_t - seed_t;

	if(use_wav) {
		output_wav_header(rate, bits, channels, samples);
	}

	t = seed_t;

	while(t != final_t) 
		sequence(&t);

	exit(EXIT_SUCCESS);
}

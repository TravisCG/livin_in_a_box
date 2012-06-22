typedef struct {
   FILE *videofile;
   unsigned char *buffer;
}DepthVideo;

DepthVideo openDepthVideo(char *filename);
void closeDepthVideo(DepthVideo dv);
void rewindDepthVideo(DepthVideo dv);
void getNextFrame(DepthVideo dv);

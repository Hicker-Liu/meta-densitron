#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define SYS_INPUT_DIR	"/sys/class/input"
#define EV_SND		0x12
#define SND_BELL	0x01
#define SND_TONE	0x02

struct input_event {
	struct timeval time;
	unsigned short int type;
	unsigned short int code;
	signed int value;
};

static void help(void)
{
	printf("Usage: ./beep <freq> <duration> <repeat>\n");
}

static int findDevice(char* devicePath)
{
	char deviceNamePath [PATH_MAX] = { 0 };
	char deviceNameValue[NAME_MAX] = { 0 };
	DIR *sysdir;
	struct dirent *dir;

	sysdir = opendir(SYS_INPUT_DIR);

	if (sysdir)
	{
		while ((dir = readdir(sysdir)) != NULL)
		{
			if (( strcmp( dir->d_name, "." ) == 0 ) ||
				( strcmp( dir->d_name, "." ) == 0 ))
			{
				continue;
			}

			if ( strstr( dir->d_name, "event" ) > 0 )
			{
				memset(deviceNamePath, 0,sizeof(deviceNamePath));
				memset(deviceNameValue,0,sizeof(deviceNameValue));

				strcpy(deviceNamePath,SYS_INPUT_DIR);
				strcat(deviceNamePath,"/");
				strcat(deviceNamePath,dir->d_name);
				strcat(deviceNamePath,"/device/name");

				FILE *pfile;
				long filesize;

				pfile = fopen( deviceNamePath , "r");

				if (pfile)
				{
					fseek (pfile , 0 , SEEK_END);
					filesize = ftell (pfile);
					rewind (pfile);
					// copy the file into the buffer:
					fread (deviceNameValue,1,filesize,pfile);

					fclose(pfile);

					if (strcmp(deviceNameValue,"pwm-beeper") > 0)
					{
						strcpy(devicePath,"/dev/input/");
						strcat(devicePath,dir->d_name);

						fprintf(stdout," Path      : %s\n",devicePath);
						fprintf(stdout," Name      : %s",deviceNameValue);

						return 0;
					}
				}
				else
					fclose(pfile);
			}
		}

		closedir(sysdir);
	}

	return -1;
}

int main(int argc, char **argv)
{
	char devicePath [PATH_MAX] = { 0 };

	if (argc < 4) {
		help();
		return -1;
	}

	int freq = atoi(argv[1]);
	int duration = atoi(argv[2]);
	int repeat = atoi(argv[3]);

	if (findDevice(devicePath) != 0	)
	{
		fprintf(stderr,"ERROR: can't find beeper device\n");
		return -1;
	}

	int fd = open(devicePath, O_RDWR);

	if (fd < 0) {
		fprintf(stderr,"ERROR: can't open %s\n",devicePath);
		return -1;
	}

	fprintf(stdout," Frequency : %d Hz\n",freq);
	fprintf(stdout," Duration  : %d msec\n",duration);
	fprintf(stdout," repeat    : %d msec\n",repeat);

	if ( repeat > 0 )
	{
		while ( repeat-- )
		{
			struct input_event event;
			event.type = EV_SND;
			event.code = SND_TONE;
			event.value = freq;

			if (write(fd, &event, sizeof(event)) < 0)
			{
				goto error;
			}

			usleep( duration * 1000 );

			event.value = 0;

			if (write(fd, &event, sizeof(event)) < 0)
			{
				goto error;
			}

			usleep( duration * 500 );
		}
	}

	close(fd);

	return 0;

error:

	close(fd);
	return -1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <linux/input.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>

#define SYS_INPUT_DIR	"/sys/class/input"

#define PWM_DEFAULT_PERIOD	"1000000"

struct PwmDevice
{
	char export [PATH_MAX];
	char path   [PATH_MAX];
	char period [PATH_MAX];
	char duty   [PATH_MAX];
	char enable [PATH_MAX];

	int  percent;
	int  length;
};

struct EventDevice
{
	char name[NAME_MAX];
	char path[PATH_MAX];

	struct input_absinfo abs_x;
	struct input_absinfo abs_y;
};

struct HapticsDevice
{
	struct EventDevice event;
	struct PwmDevice   pwm;

	struct timeval timestamp;
	int    enable;

	pthread_mutex_t mutex;
};


int g_interrupted = 0;

static void exit_handler(int sig)
{
    g_interrupted = 1;
}

static void help(void)
{
	printf("Usage: ./haptic-feedback <input device name> <pwmchip number> <percent> <length>\n");
}

static int event_pick(char* targetName,char* targetPath)
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

					if (strcmp(deviceNameValue,targetName) > 0)
					{
						strcpy(targetPath,"/dev/input/");
						strcat(targetPath,dir->d_name);

						return EXIT_SUCCESS;
					}
				}
				else
					fclose(pfile);
			}
		}

		closedir(sysdir);
	}

	return EXIT_FAILURE;
}

static int checkPath(char* path)
{
	DIR* dir = opendir(path);

	if (dir)
	{
    	/* Directory exists. */
	    closedir(dir);
		return EXIT_SUCCESS;
	}
	 else if (ENOENT == errno)
	{
    	/* Directory does not exist. */
	}
	else
	{
    	/* opendir() failed for some other reason. */
	}

	return EXIT_FAILURE;
}

static int pwm_export(char* path)
{
	int fd = open(path,O_WRONLY);

	if (fd < 0 )
	{
		return EXIT_FAILURE;
	}

	if ( write(fd,"0",strlen("0")) <0 )
	{
		close(fd);
		return EXIT_FAILURE;
	}
	else
	{
		close(fd);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
};

static int pwm_configure(struct PwmDevice* pwm)
{
	int fd_period = open (pwm->period,O_WRONLY);
	int fd_duty   = open (pwm->duty,O_WRONLY);

	char buffer[65]= { 0 };
	long duty = 0;

	if ( fd_period < 0 )
		return EXIT_FAILURE;
	else
	{
		if (write(fd_period,PWM_DEFAULT_PERIOD,strlen(PWM_DEFAULT_PERIOD)) < 0 )
		{
			close(fd_period);
			return EXIT_FAILURE;
		}
		else
		{
			close(fd_period);
		}
	}

	if ( fd_duty < 0 )
		return EXIT_FAILURE;
	else
	{
		duty = atol(PWM_DEFAULT_PERIOD) * pwm-> percent;
		duty = duty / 100;

		sprintf(buffer,"%ld",duty);

		if (write(fd_duty,buffer,strlen(buffer)) < 0 )
		{
			close(fd_period);
			return EXIT_FAILURE;
		}
		else
		{
			close(fd_period);
		}
	}

	return EXIT_SUCCESS;
}

static int pwm_enable(struct PwmDevice* pwm)
{
	int fd_enable = open (pwm->enable,O_WRONLY);

	if ( fd_enable < 0 )
		return EXIT_FAILURE;
	else
	{
		if (write(fd_enable,"1",strlen("1")) < 0 )
		{
			close(fd_enable);
			return EXIT_FAILURE;
		}
		else
		{
			close(fd_enable);
		}
	}
}

static int pwm_disable(struct PwmDevice* pwm)
{
	int fd_enable = open (pwm->enable,O_WRONLY);

	if ( fd_enable < 0 )
		return EXIT_FAILURE;
	else
	{
		if (write(fd_enable,"0",strlen("0")) < 0 )
		{
			close(fd_enable);
			return EXIT_FAILURE;
		}
		else
		{
			close(fd_enable);
		}
	}
}

void* event_monitor(void* data)
{
	struct HapticsDevice* device = (struct HapticsDevice*)data;
	struct input_event ev;

	int fd;
	fd_set rdfs;

	/* Open Device */
	fd = open(device->event.path, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "%s is not a vaild device\n", device->event.path);
		goto err;
	}

	/* Print Device Name */
	/*
	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("Reading from:\n");
	printf("device file = %s\n", EVENT_DEVICE);
	printf("device name = %s\n", name);
	*/

	ioctl(fd,EVIOCGABS(ABS_X),&device->event.abs_x);
	/* Print ABS_X infotmation
	printf("x abs lastest value=%d\n",event->abs_x.value);
	printf("x abs min=%d\n",event->abs_x.minimum);
	printf("x abs max=%d\n",event->abs_x.maximum);
	*/
	ioctl(fd,EVIOCGABS(ABS_Y),&device->event.abs_y);
	/* Print ABS_Y infotmation
	printf("y abs lastest value=%d\n",event->abs_y.value);
	printf("y abs min=%d\n",event->abs_y.minimum);
	printf("y abs max=%d\n",event->abs_y.maximum);
	*/

	FD_ZERO(&rdfs);
	FD_SET(fd, &rdfs);

	while( g_interrupted != 1)
	{
		const size_t ev_size = sizeof(struct input_event);
		ssize_t size;

		/* TODO: use select() */
		select(fd + 1, &rdfs, NULL, NULL, NULL);

		size = read(fd, &ev, ev_size);
		if (size < ev_size)
		{
			fprintf(stderr, "Error size when reading\n");
			goto err;
		}

		if (ev.type == EV_ABS && (ev.code == ABS_X || ev.code == ABS_Y))
		{
			/* TODO: convert value to pixels */
//			printf("%s = %d\n", ev.code == ABS_X ? "X" : "Y", ev.value);

			double pixel;
			if (ev.code == ABS_X)
			{
				pixel = round((ev.value * 1920 ) /
				              (device->event.abs_x.maximum - device->event.abs_x.minimum) );

				printf("%s = %.1f pixels\n", "X" , pixel);
			}
			else
			{
				pixel = round((ev.value * 287 ) /
				              (device->event.abs_y.maximum - device->event.abs_y.minimum) );

				printf("%s = %.1f pixels\n", "Y" , pixel);
			}

			pthread_mutex_lock( &device->mutex);
			pwm_enable(&device->pwm);
			device->enable = 1;
			gettimeofday(&device->timestamp,NULL);
			pthread_mutex_unlock( &device->mutex);
		}
/*
		if (ev.type == EV_KEY && ev.code == BTN_TOUCH )
		{
			printf("BTN_TOUCH = %d\n",ev.value);
		}
*/
	}

	//return EXIT_SUCCESS;

err:
	close(fd);
	//return EXIT_FAILURE;
}

double time_difference(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void* motor_monitor(void* data)
{
	struct HapticsDevice* device = (struct HapticsDevice*)data;

	while( g_interrupted != 1)
	{
		pthread_mutex_lock( &device->mutex);
		if (device->enable)
		{
			struct timeval now;
			gettimeofday(&now,NULL);

			if (time_difference(device->timestamp,now) > device->pwm.length )
			{
				pwm_disable(&device->pwm);
				device->enable = 0;
			}
		}
		pthread_mutex_unlock( &device->mutex);

		usleep(5 * 1000);
		/*
		struct timevaldelay;
		delay.tv_sec =0;
		delay.tv_usec =20 * 1000; // 20 ms
		select(0, NULL,NULL, NULL, &delay);
		*/
	}
}

int main(int argc, char *argv[])
{
	struct HapticsDevice m_haptics;

	signal(SIGINT, exit_handler);
	signal(SIGTERM, exit_handler);

	if (argc < 5)
	{
		help();
		return -1;
	}

	if ((getuid ()) != 0)
	{
		fprintf(stderr, "You are not root! This may not work...\n");
		return EXIT_FAILURE;
	}

	sprintf(m_haptics.event.name,"%s",argv[1]);

	sprintf(m_haptics.pwm.export,"/sys/class/pwm/pwmchip%s/export",argv[2]);
	sprintf(m_haptics.pwm.path,  "/sys/class/pwm/pwmchip%s/pwm0",argv[2]);
	sprintf(m_haptics.pwm.period,"/sys/class/pwm/pwmchip%s/pwm0/period",argv[2]);
	sprintf(m_haptics.pwm.duty,  "/sys/class/pwm/pwmchip%s/pwm0/duty_cycle",argv[2]);
	sprintf(m_haptics.pwm.enable,"/sys/class/pwm/pwmchip%s/pwm0/enable",argv[2]);

	m_haptics.pwm.percent = atoi(argv[3]);
	m_haptics.pwm.length  = atoi(argv[4]);

	if ( event_pick(m_haptics.event.name,
	                m_haptics.event.path) != EXIT_SUCCESS )
	{
		fprintf(stderr,"HapticsDevice: event: Can't find '%s'\n",m_haptics.event.name);

		return EXIT_FAILURE;
	}
	else
	{
		fprintf(stdout,"HapticsDevice: event: name: %s\n",m_haptics.event.name);
		fprintf(stdout,"HapticsDevice: event: path: %s\n",m_haptics.event.path);
	}

	if (checkPath(m_haptics.pwm.path) != EXIT_SUCCESS)
	{
		if ( pwm_export(m_haptics.pwm.export) != EXIT_SUCCESS )
		{
			fprintf(stderr,"HapticsDevice: pwm: Can't export pwm node\n");
			return EXIT_FAILURE;
		}
	}

	if ( pwm_configure(&m_haptics.pwm) != EXIT_SUCCESS)
	{
		fprintf(stderr,"HapticsDevice: pwm: Can't configure pwm node\n");
		return EXIT_FAILURE;
	}
	else
		fprintf(stdout,"HapticsDevice: pwm: pwm0 configuration completed\n");

	m_haptics.enable = 0;
	pthread_mutex_init(&m_haptics.mutex,NULL);

	pthread_t event_thread;
	pthread_t motor_thread;
	pthread_create(&event_thread,NULL,event_monitor,&m_haptics);
	pthread_create(&motor_thread,NULL,motor_monitor,&m_haptics);

	pthread_join(event_thread,NULL);
	pthread_join(motor_thread,NULL);

	return EXIT_SUCCESS;
}

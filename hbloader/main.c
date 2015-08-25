#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define STDIN (0)
#define STDOUT (1)
#define CMD_WRITE 0xa7
#define CMD_JUMP 0xa9
#define RESP_ACK 0xa0
#define ACK_LEN (1)
#define WRBUF_LEN (64)
#define JMPADDR_LEN (2)

void setup_serial(int fd) {
	// set to blocking mode
	fcntl(fd, F_SETFL, 0);

	struct termios options;

	/*  get the current options */
	tcgetattr(fd, &options);

	/*
	 * 	 * Set the baud rates to B115200...
	 * 	 	 */

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	/*
	 * 	*No parity (8N1):
	 * 		*/
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	/*
	 * 	*Set charachter size
	 * 		*/
	options.c_cflag &= ~CSIZE; /*  Mask the character size bits */
	options.c_cflag |= CS8;    /*  Select 8 data bits */

	/*
	 * 	 * Enable the receiver and set local mode...
	 * 	 	 */

	/*  set raw input,0 char, 1 second timeout */
	options.c_cflag     |= (CLOCAL | CREAD);
	options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag     &= ~OPOST;
	options.c_cc[VMIN]  = ACK_LEN; //will block until receive this amount of bytes
	options.c_cc[VTIME] = 10; //Once recieves _PacketSize - start a time out for more data 
	//(We don't realy use it since we don't have flow control in our system)
	/*  set the options */
	tcsetattr(fd, TCSANOW, &options);

}

int main(int argc, char** argv) {
	char* devpath;
	int devfd;
	int nbytes, err;
	unsigned char cmd;
	char page;
	char buf[WRBUF_LEN];
	char chksum;
	char resp;
	char jmpaddr[JMPADDR_LEN];

	if (argc > 1) {
		devpath = argv[1];
		fprintf(stderr, "opening file: %s\n", devpath);
		devfd = open(devpath, O_RDWR | O_NOCTTY | O_NDELAY);
		setup_serial(devfd);
	}
	else {
		fprintf(stderr, "opening stdout\n");
		devfd = STDOUT;
	}

	if (-1 == devfd) {
		perror("failed to open device file");
		return 1;
	}

	err = 0;
	do {
		cmd = 0;
		nbytes = read(STDIN, &cmd, 1);
		if (nbytes < 0) {
			perror("failed to read command from stdin");
			err = 1;
			break;
		}
		if (nbytes == 0) {
			// reached EOF
			err = 0;
			break;
		}

		if (cmd == CMD_WRITE) {
			nbytes  = read(STDIN, &page, 1);
			nbytes += read(STDIN, &buf, WRBUF_LEN);
			nbytes += read(STDIN, &chksum, 1);
			if (nbytes != 1 + WRBUF_LEN + 1) {
				perror("failed to read stdin");
				err = 1;
				break;
			}

			fprintf(stderr, "writing to page 0x%x\n", page);

			nbytes  = write(devfd, &cmd, 1);
			nbytes += write(devfd, &page, 1);
			nbytes += write(devfd, &buf, WRBUF_LEN);
			nbytes += write(devfd, &chksum, 1);
			if (nbytes != 1 + 1 + WRBUF_LEN + 1) {
				perror("failed to write to device");
				err = 1;
				break;
			}

			read(devfd, &resp, 1);
			if (resp != RESP_ACK) {
				fprintf(stderr, "failed to write page %d to device (got 0x%02x)\n", page, resp);
				err = 1;
				break;
			}
		}
		else if (cmd == CMD_JUMP) {
			nbytes = read(STDIN, &jmpaddr, JMPADDR_LEN);
			if (nbytes != JMPADDR_LEN) {
				perror("failed to read stdin");
				err = 1;
				break;
			}

			fprintf(stderr, "jumping to 0x%02x%02x\n", jmpaddr[0], jmpaddr[1]);

			nbytes  = write(devfd, &cmd, 1);
			nbytes += write(devfd, &jmpaddr, JMPADDR_LEN);
			if (nbytes != 1 + JMPADDR_LEN) {
				perror("failed to write to device");
				err = 1;
				break;
			}

			read(devfd, &resp, 1);
			if (resp != RESP_ACK) {
				fprintf(stderr, "failed to write jump command to device (got 0x%x)\n", resp);
				err = 1;
				break;
			}
		}
		else {
			fprintf(stderr, "unrecognized command 0x%x\n", cmd);
			err = 1;
		}

		usleep(1000);
	} while (!err);

	close(devfd);
	return err;
}


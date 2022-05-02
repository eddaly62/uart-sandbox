#include <stdio.h>   /* I/O Definitions                    */
#include <unistd.h>  /* Standard Symbolic Constants        */
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <string.h>  /* String Manipulation Definitions    */
#include <errno.h>   /* Error Code Definitions             */

int main(int argc, char *argv[])
{
    char buf_rx[100];
    char buf_tx[100];
    const char *device = "/dev/ttymxc1";
    struct termios tty;
    int fd;
    int flags = O_RDWR | O_NOCTTY | O_NDELAY; /* O_RDWR Read/write access to the serial port */
                                              /* O_NOCTTY No terminal will control the process */
                                              /* O_NDELAY Use non-blocking I/O */

    /*------------------------------- Opening the Serial Port -------------------------------*/
    fd = open(device, flags);

    if(fd == -1){
        printf("Failed to open port!\n ");
        return -1;
    }

    /*---------- Serial port settings using the termios structure --------- */
    /* Settings (9600/8N1):
    Baud rate: 9600 baud
    Data bits: 8
    Parity bit: No
    Stop bit: 1
    Hardware Flow Control: No
    */

    tcgetattr(fd, &tty);    /* Get the current attributes of the Serial port */

    cfsetispeed(&tty, B9600); /* Set read speed as 9600 baud                       */
    cfsetospeed(&tty, B9600); /* Set write speed as 9600 baud                      */

    tty.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB)  */
    tty.c_cflag &= ~CSTOPB;   /* Clear CSTOPB, configuring 1 stop bit    */
    tty.c_cflag &= ~CSIZE;    /* Using mask to clear data size setting   */
    tty.c_cflag |=  CS8;      /* Set 8 data bits                         */
    tty.c_cflag &= ~CRTSCTS;  /* Disable Hardware Flow Control           */

    if((tcsetattr(fd, TCSANOW, &tty)) != 0){ /* Write the configuration to the termios structure*/
        printf("Error! Can't set attributes.\n ");
        return -1;
    }else{
        printf("All set! \n");
    }

    tcflush(fd, TCIFLUSH);

    strncpy(buf_tx, "Testing UART\n", sizeof(buf_tx)); 

    int result = write(fd,(char *)buf_tx,sizeof(buf_tx)); 
    if(result == -1){
        printf("Error: %s\n",strerror(errno));
        return -1;
    } else {
        printf("%d bytes sent\n", result);
    }

    usleep(186000);

    if (read(fd, buf_rx, sizeof(buf_rx)) == -1) { 
        printf("Error: %s\n",strerror(errno));
        return -1;
    }
    printf("buf_rx = %s\n", buf_rx);

    close(fd);

    return 0;
}
#include <stdio.h>   /* I/O Definitions                    */
#include <unistd.h>  /* Standard Symbolic Constants        */
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <string.h>  /* String Manipulation Definitions    */
#include <errno.h>   /* Error Code Definitions             */

int main(int argc, char *argv[]) {
    char buf_rx[100];
    char buf_tx[100];
    const char *first_uart = "/dev/ttymxc1";
    const char *second_uart = "/dev/ttymxc3";
    struct termios tty;
    int fd_first_uart;
    int fd_second_uart;
    int flags = O_RDWR | O_NOCTTY | O_NDELAY; /* O_RDWR Read/write access to the serial port */
    /* O_NOCTTY No terminal will control the process */
    /* O_NDELAY Use non-blocking I/O */

    /*------------------------------- Opening the Serial Ports -------------------------------*/
    fd_first_uart = open(first_uart, flags);
    fd_second_uart = open(second_uart, flags);

    if (fd_first_uart == -1 || fd_second_uart == -1) {
        printf("Failed to open port!\n");
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

    tcgetattr(fd_first_uart, &tty); /* Get the current attributes of the first serial port */

    cfsetispeed(&tty, B9600); /* Set read speed as 9600 baud                       */
    cfsetospeed(&tty, B9600); /* Set write speed as 9600 baud                      */

    tty.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB)  */
    tty.c_cflag &= ~CSTOPB; /* Clear CSTOPB, configuring 1 stop bit    */
    tty.c_cflag &= ~CSIZE; /* Using mask to clear data size setting   */
    tty.c_cflag |= CS8; /* Set 8 data bits                         */
    tty.c_cflag &= ~CRTSCTS; /* Disable Hardware Flow Control           */

    if ((tcsetattr(fd_first_uart, TCSANOW, &tty)) != 0) { /* Write the configuration to the termios structure*/
        printf("Error! Can't set attributes.\n");
        return -1;
    } else {
        printf("First UART all set!\n");
    }

    tcflush(fd_first_uart, TCIFLUSH);

    tcgetattr(fd_second_uart, &tty); /* Get the current attributes of the second serial port */

    cfsetispeed(&tty, B9600); /* Set read speed as 9600 baud                       */
    cfsetospeed(&tty, B9600); /* Set write speed as 9600 baud                      */

    tty.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB)  */
    tty.c_cflag &= ~CSTOPB; /* Clear CSTOPB, configuring 1 stop bit    */
    tty.c_cflag &= ~CSIZE; /* Using mask to clear data size setting   */
    tty.c_cflag |= CS8; /* Set 8 data bits                         */
    tty.c_cflag &= ~CRTSCTS; /* Disable Hardware Flow Control           */

    if ((tcsetattr(fd_second_uart, TCSANOW, &tty)) != 0) { /* Write the configuration to the termios structure*/
        printf("Error! Can't set attributes.\n");
        return -1;
    } else {
        printf("Second UART all set!\n");
    }

    tcflush(fd_second_uart, TCIFLUSH);

    strncpy(buf_tx, "Second UART writing to first UART\n", sizeof(buf_tx));

    int result = write(fd_second_uart, (char *) buf_tx, sizeof(buf_tx));
    if (result == -1) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    } else {
        printf("%d bytes sent to first UART\n", result);
    }

    usleep(1860000);

    if (read(fd_first_uart, buf_rx, sizeof(buf_rx)) == -1) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    } else {
        printf("First UART received: %s\n", buf_rx); /* Print content read from serial */
        strncpy(buf_tx, "First UART sending to second UART\n", sizeof(buf_tx));
        result = write(fd_first_uart, (char *) buf_tx, sizeof(buf_tx));
        if (result == -1) {
            printf("Error: %s\n", strerror(errno));
            return -1;
        } else {
            printf("%d bytes sent to second UART\n", result);
        }
    }

    usleep(1860000);

    if (read(fd_second_uart, buf_rx, sizeof(buf_rx)) == -1) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    } else {
        printf("Second UART received: %s\n", buf_rx); /* Print content read from serial */
    }

    close(fd_first_uart);
    close(fd_second_uart);

    return 0;
}
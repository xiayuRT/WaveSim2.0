/**
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 * @file       CartSPI.cpp
 * @brief      
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Aug-28-2023     Xiayu Z                         xiayu.zeng@runningtide.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 */

#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "SPI.hpp"

// SPI configuration
static const char *device = "/dev/spidev0.0";
static uint8_t spi_mode = 0;
static uint8_t bits_per_word = 8;
static uint32_t spi_speed = 500000;
static uint16_t spi_delay = 0;

// Send the error msg and terminate the program
static void pabort(const char *s){
    std::cout << s << std::endl; // output the error message to the terminal
    std::exit(EXIT_FAILURE);  // terminate the program
}

// Start the SPI and returns the spi_fd
int SPI_begin(){

    int ret = 0;
    int spi_fd;

    // search for the SPI device
    spi_fd = open(device, O_RDWR);
    if(spi_fd < 0)
        pabort("Can't open the device");


    // set the SPI mode
    spi_mode |= SPI_CS_HIGH;
    // spi_mode &= ~SPI_NO_CS;
    ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &spi_mode);
    if (ret == -1)
        pabort("can't set spi mode");

    
    // set the bits per word
    ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits_per_word);
    if (ret == -1)
        pabort("can't set bits per word");


    // set the SPI speed
    ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if (ret == -1)
        pabort("can't get max speed hz");

    return spi_fd;

}

// Disable the SPI
void SPI_end(int spi_fd){
    close(spi_fd);
}

// Write instruction
void reg_write(int spi_fd, uint16_t data){
    int ret;
    uint8_t tx[2];
    tx[0] = data >> 8;
    tx[1] = data | 0b00000000;

    struct spi_ioc_transfer tr ={
        .tx_buf = (unsigned long) tx,
        .rx_buf = (unsigned long) NULL,
        .len = 2,
        .delay_usecs = spi_delay,
        .bits_per_word = bits_per_word,
    };

    ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);

    if(ret < 1)
        pabort("can't send spi message!");
}

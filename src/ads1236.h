#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int main(void){
    int spi_fd = open("/dev/spi6.1", O_RDWR);
    if (spi_fd < 0)
    {
        perror("Failed to open an SPI Device");
        exit(1);
    }

    uint8_t spi_mode = SPI_MODE_0;
    uint8_t spi_bits_per_word = 8;
    uint32_t spi_speed_hz = 1000000;  // Set your desired speed here

    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) < 0) {
        perror("Failed to set SPI mode");
        exit(1);
    }

    if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word) < 0) {
        perror("Failed to set SPI bits per word");
        exit(1);
    }

    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed_hz) < 0) {
        perror("Failed to set SPI speed");
        exit(1);
    }

    uint8_t tx_buffer[] = {0x20, 0x02, 0xFF, 0xFF};  // Example transmit buffer
    uint8_t rx_buffer[sizeof(tx_buffer)];  // Receive buffer

    struct spi_ioc_transfer spi_transfer = {
        .tx_buf = (unsigned long)tx_buffer,
        .rx_buf = (unsigned long)rx_buffer,
        .len = sizeof(tx_buffer),
        .speed_hz = spi_speed_hz,
        .bits_per_word = spi_bits_per_word,
    };

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_transfer) < 0) {
        perror("SPI transfer failed");
        exit(1);
    }

    for(uint8_t i = 0; i < sizeof(rx_buffer)/sizeof(rx_buffer[0]); i++){
        printf("%d", rx_buffer[i]);
    }

    return 0;
}
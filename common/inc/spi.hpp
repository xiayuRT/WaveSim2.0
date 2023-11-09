/**
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 * @file       spi.hpp
 * @brief      the header file for spi.cpp
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Aug-28-2023     Xiayu Zeng                      xiayu.zeng@runningtide.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 */

#ifndef SPI_HPP
#define SPI_HPP

#include <cstdint>

/**
 * @brief Start the SPI configuration and returns the spi_fd           
 * @return int
 * @retval spi_fd        
 */
int  spi_begin();


/**
 * @brief disable the SPI
 * @param[in]spi_fd: output of the spi_begin, the spi file      
 */
void spi_end(int spi_fd);


/**
 * @brief Write instruction           
 * @param[in] spi_fd: output of the spi_begin, the spi file
 * @param[in] data: 16-bit data to send         
 */
void reg_write(int spi_fd, uint16_t data);

#endif
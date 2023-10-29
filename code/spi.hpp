/**
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE****************************
 * @file       BaseMotor.cpp
 * @brief      The instructions to control the base motor
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
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE****************************
 */

#ifndef SPI_HPP
#define SPI_HPP

#include <cstdint>

int  SPI_begin();
void SPI_end(int spi_fd);
void reg_write(int spi_fd, uint16_t data);

#endif
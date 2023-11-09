/**
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 * @file       cartmotor.cpp
 * @brief      the modified library from the arduino based high power stepper driver controller
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Aug-18-2023     Xiayu Zeng                      xiayu.zeng@runningtide.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 */

#include <cstdint>
#include <iostream>
#include <pigpio.h>
#include <chrono>
#include <thread>

#include "../inc/spi.hpp"
#include "../inc/cartmotor.hpp"


static void pabort(const char *s){
    // Send the error msg and terminate the program
    std::cout << s << std::endl; // output the error message to the terminal
    std::exit(EXIT_FAILURE);  // terminate the program
}


void drv8711_spi::write_reg(uint8_t address, uint16_t value){
    // read/write bit and register address are the first 4 bits of the first byte; data
    // is in the remaining 4 bits of the first byte combined with the second byte (12 bytes total)
    set_chip_select_pin();
    gpioWrite(cs_pin, 1);
    int spi_fd = spi_begin();
    reg_write(spi_fd, (address & 0b111) << 12 | (value & 0xFFF));
    spi_end(spi_fd);
    gpioWrite(cs_pin, 0);
}


void drv8711_spi::write_reg(hpsd_reg_addr address, uint16_t value){
    write_reg(static_cast<uint8_t>(address), value);
}


void drv8711_spi::set_chip_select_pin()
{
    if(!gpioInitialise()){pabort("Use 'sudo' to initialize the GPIO!\n");};
    gpioSetMode(cs_pin, PI_OUTPUT);
    gpioWrite(cs_pin, 0);
}


high_power_stepper_driver::high_power_stepper_driver(){
    ctrl   = 0xC10;
    torque = 0x1FF;
    off    = 0x030;
    blank  = 0x080;
    decay  = 0x110;
    stall  = 0x040;
    drive  = 0xA59;
}


void high_power_stepper_driver::reset_settings()
{
    ctrl   = 0xC10;
    torque = 0x1FF;
    off    = 0x030;
    blank  = 0x080;
    decay  = 0x110;
    stall  = 0x040;
    drive  = 0xA59;
    apply_settings();
}


void high_power_stepper_driver::apply_settings()
{
    write_torque();
    write_off();
    write_blank();
    write_decay();
    write_drive();
    write_stall();
    write_ctrl(); // Set others up before enabling the motor
}


void high_power_stepper_driver::enable_driver()
{
    ctrl |= (1 << 0);
    write_ctrl();
}


void high_power_stepper_driver::disable_driver()
{
    ctrl &= ~(1 << 0);
    write_ctrl();
}


void high_power_stepper_driver::set_direction(bool value)
{
    if (value)
    {
        ctrl |= (1 << 1);
    }
    else
    {
        ctrl &= ~(1 << 1);
    }
    write_ctrl();
}


bool high_power_stepper_driver::get_direction()
{
    return (ctrl >> 1) & 1;
}


void high_power_stepper_driver::step()
{
    driver.write_reg(hpsd_reg_addr::CTRL, ctrl | (1 << 2));
}


void high_power_stepper_driver::set_step_mode(hpsd_step_mode mode)
{
    // Pick 1/4 micro-step by default.
    uint8_t sm = 0b0010;

    switch (mode)
    {
        case hpsd_step_mode::MICRO_STEP_1:   sm = 0b0000; break;
        case hpsd_step_mode::MICRO_STEP_2:   sm = 0b0001; break;
        case hpsd_step_mode::MICRO_STEP_4:   sm = 0b0010; break;
        case hpsd_step_mode::MICRO_STEP_8:   sm = 0b0011; break;
        case hpsd_step_mode::MICRO_STEP_16:  sm = 0b0100; break;
        case hpsd_step_mode::MICRO_STEP_32:  sm = 0b0101; break;
        case hpsd_step_mode::MICRO_STEP_64:  sm = 0b0110; break;
        case hpsd_step_mode::MICRO_STEP_128: sm = 0b0111; break;
        case hpsd_step_mode::MICRO_STEP_256: sm = 0b1000; break;
    }

    ctrl = (ctrl & 0b111110000111) | (sm << 3);
    write_ctrl();
}


void high_power_stepper_driver::set_step_mode(uint16_t mode)
{
    set_step_mode(static_cast<hpsd_step_mode>(mode));
}


void high_power_stepper_driver::set_current_milliamps_36v4(uint16_t current)
{
    if (current > 8000) { current = 8000; }
    uint8_t isgain_bits = 0b11;
    uint16_t torque_bits = ((static_cast<uint32_t>(768) * current) / 6875);

    // Halve the gain and TORQUE until the TORQUE value fits in 8 bits.
    while (torque_bits > 0xFF)
    {
        isgain_bits--;
        torque_bits >>= 1;
    }

    ctrl = (ctrl & 0b110011111111) | (isgain_bits << 8);
    write_ctrl();
    torque = (torque & 0b111100000000) | torque_bits;
    write_torque();
}


void high_power_stepper_driver::set_decay_mode(hpsd_decay_mode mode)
{
    decay = (decay & 0b00011111111) | (static_cast<uint8_t>(mode) << 8);
    write_decay();
}


void high_power_stepper_driver::clear_status()
{
    driver.write_reg(hpsd_reg_addr::STATUS, 0);
}


void high_power_stepper_driver::write_ctrl()
{
    driver.write_reg(hpsd_reg_addr::CTRL, ctrl);
}


void high_power_stepper_driver::write_torque()
{
    driver.write_reg(hpsd_reg_addr::TORQUE, torque);
}


void high_power_stepper_driver::write_off()
{
    driver.write_reg(hpsd_reg_addr::OFF, off);
}


void high_power_stepper_driver::write_blank()
{
    driver.write_reg(hpsd_reg_addr::BLANK, blank);
}


void high_power_stepper_driver::write_decay()
{
    driver.write_reg(hpsd_reg_addr::DECAY, decay);
}


void high_power_stepper_driver::write_stall()
{
    driver.write_reg(hpsd_reg_addr::STALL, stall);
}


void high_power_stepper_driver::write_drive()
{
    driver.write_reg(hpsd_reg_addr::DRIVE, drive);
}



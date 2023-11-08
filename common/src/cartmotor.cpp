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

// Send the error msg and terminate the program
static void pabort(const char *s){
    std::cout << s << std::endl; // output the error message to the terminal
    std::exit(EXIT_FAILURE);  // terminate the program
}

// Write the specified value to a register
void DRV8711SPI::writeReg(uint8_t address, uint16_t value){
    // read/write bit and register address are the first 4 bits of the first byte; data
    // is in the temaining 4 bits of the first byte combined with the second byte (12 bytes total)
    setChipSelectPin();
    gpioWrite(cs_pin, 1);
    int spi_fd = SPI_begin();
    reg_write(spi_fd, (address & 0b111) << 12 | (value & 0xFFF));
    SPI_end(spi_fd);
    gpioWrite(cs_pin, 0);
}


// Write the specified value to a register
void DRV8711SPI::writeReg(HPSDRegAddr address, uint16_t value){
    writeReg((uint8_t) address, value);
}


// Initilialize the GPIO function and select the PIN
void DRV8711SPI::setChipSelectPin()
{
    if(!gpioInitialise()){pabort("Use 'sudo' to initialize the GPIO!\n");};
    gpioSetMode(cs_pin, PI_OUTPUT);
    gpioWrite(cs_pin, 0);
}


HighPowerStepperDriver::HighPowerStepperDriver(){
    // All settings set to power-on defaults
    ctrl   = 0xC10;
    torque = 0x1FF;
    off    = 0x030;
    blank  = 0x080;
    decay  = 0x110;
    stall  = 0x040;
    drive  = 0xA59;
}


/// Changes all of the driver's settings back to their default values.
///
/// It is good to call this near the beginning of your program to ensure that
/// there are no settings left over from an earlier time that might affect the
/// operation of the driver.
void HighPowerStepperDriver::resetSettings()
{
    ctrl   = 0xC10;
    torque = 0x1FF;
    off    = 0x030;
    blank  = 0x080;
    decay  = 0x110;
    stall  = 0x040;
    drive  = 0xA59;
    applySettings();
}


/// Re-writes the cached settings stored in this class to the device.
///
/// You should not normally need to call this function because settings are
/// written to the device whenever they are changed.  However, if
/// verifySettings() returns false (due to a power interruption, for
/// instance), then you could use applySettings() to get the device's settings
/// back into the desired state.
void HighPowerStepperDriver::applySettings()
{
    writeTORQUE();
    writeOFF();
    writeBLANK();
    writeDECAY();
    writeDRIVE();
    writeSTALL();
    writeCTRL(); // Set others up before enable the motor
}


/// Enables the driver (ENBL = 1).
void HighPowerStepperDriver::enableDriver()
{
    ctrl |= (1 << 0);
    writeCTRL();
}


/// Disables the driver (ENBL = 0).
void HighPowerStepperDriver:: disableDriver()
{
    ctrl &= ~(1 << 0);
    writeCTRL();
}


/// Sets the motor direction (RDIR).
///
/// Allowed values are 0 or 1.
///
/// You can use this command to control the direction of the stepper motor and
/// leave the DIR pin disconnected.
void HighPowerStepperDriver::setDirection(bool value)
{
    if (value)
    {
    ctrl |= (1 << 1);
    }
    else
    {
    ctrl &= ~(1 << 1);
    }
    writeCTRL();
}


/// Returns the cached value of the motor direction (RDIR).
///
/// This does not perform any SPI communication with the driver.
bool HighPowerStepperDriver::getDirection()
{
    return ctrl >> 1 & 1;
}


/// Advances the indexer by one step (RSTEP = 1).
///
/// You can use this command to step the stepper motor and leave the STEP pin
/// disconnected.
///
/// The driver automatically clears the RSTEP bit after it is written.
void HighPowerStepperDriver::step()
{
    driver.writeReg(HPSDRegAddr::CTRL, ctrl | (1 << 2));
}


/// Sets the driver's stepping mode (MODE).
///
/// This affects many things about the performance of the motor, including how
/// much the output moves for each step taken and how much current flows
/// through the coils in each stepping position.
///
/// If an invalid stepping mode is passed to this function, then it selects
/// 1/4 micro-step, which is the driver's default.
void HighPowerStepperDriver::setStepMode(HPSDStepMode mode)
{
    // Pick 1/4 micro-step by default.
    uint8_t sm = 0b0010;

    switch (mode)
    {
    case HPSDStepMode::MicroStep1:   sm = 0b0000; break;
    case HPSDStepMode::MicroStep2:   sm = 0b0001; break;
    case HPSDStepMode::MicroStep4:   sm = 0b0010; break;
    case HPSDStepMode::MicroStep8:   sm = 0b0011; break;
    case HPSDStepMode::MicroStep16:  sm = 0b0100; break;
    case HPSDStepMode::MicroStep32:  sm = 0b0101; break;
    case HPSDStepMode::MicroStep64:  sm = 0b0110; break;
    case HPSDStepMode::MicroStep128: sm = 0b0111; break;
    case HPSDStepMode::MicroStep256: sm = 0b1000; break;
    }

    ctrl = (ctrl & 0b111110000111) | (sm << 3);
    writeCTRL();
}


/// Sets the driver's stepping mode (MODE).
///
/// This version of the function allows you to express the requested
/// microstepping ratio as a number directly.
/// \see setStepMode(HPSDStepMode)
void HighPowerStepperDriver::setStepMode(uint16_t mode)
{
    setStepMode((HPSDStepMode)mode);
}


/// Sets the current limit for a High-Power Stepper Motor Driver 36v4.
///
/// The argument to this function should be the desired current limit in
/// milliamps.
void HighPowerStepperDriver::setCurrentMilliamps36v4(uint16_t current)
{
    if (current > 8000) { current = 8000; }
    uint8_t isgainBits = 0b11;
    uint16_t torqueBits = ((uint32_t)768  * current) / 6875;

    // Halve the gain and TORQUE until the TORQUE value fits in 8 bits.
    while (torqueBits > 0xFF)
    {
    isgainBits--;
    torqueBits >>= 1;
    }

    ctrl = (ctrl & 0b110011111111) | (isgainBits << 8);
    writeCTRL();
    torque = (torque & 0b111100000000) | torqueBits;
    writeTORQUE();
}


/// Sets the driver's decay mode (DECMOD).
void HighPowerStepperDriver::setDecayMode(HPSDDecayMode mode)
{
    decay = (decay & 0b00011111111) | (((uint8_t)mode & 0b111) << 8);
    writeDECAY();
}


/// Clears any status conditions that are currently latched in the driver.
void HighPowerStepperDriver::clearStatus()
{
    driver.writeReg(HPSDRegAddr::STATUS, 0);
}    


/// Writes the cached value of the CTRL register to the device.
void HighPowerStepperDriver::writeCTRL(){ 
    driver.writeReg(HPSDRegAddr::CTRL, ctrl);
}


/// Writes the cached value of the TORQUE register to the device.
void HighPowerStepperDriver::writeTORQUE()
{
    driver.writeReg(HPSDRegAddr::TORQUE, torque);
}


/// Writes the cached value of the OFF register to the device.
void HighPowerStepperDriver::writeOFF()
{
    driver.writeReg(HPSDRegAddr::OFF, off);
}


/// Writes the cached value of the BLANK register to the device.
void HighPowerStepperDriver::writeBLANK()
{
    driver.writeReg(HPSDRegAddr::BLANK, blank);
}


/// Writes the cached value of the DECAY register to the device.
void HighPowerStepperDriver::writeDECAY()
{
    driver.writeReg(HPSDRegAddr::DECAY, decay);
}


/// Writes the cached value of the STALL register to the device.
void HighPowerStepperDriver::writeSTALL()
{
    driver.writeReg(HPSDRegAddr::STALL, stall);
}


/// Writes the cached value of the DRIVE register to the device.
void HighPowerStepperDriver::writeDRIVE()
{
    driver.writeReg(HPSDRegAddr::DRIVE, drive);
}



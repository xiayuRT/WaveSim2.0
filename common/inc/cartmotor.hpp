/**
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 * @file       cartmotor.hpp
 * @brief      header file for cartmotor.cpp
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

#ifndef _CARTMOTOR_HPP
#define _CARTMOTOR_HPP

#include <cstdint>

enum class HPSDRegAddr : uint8_t
{
    CTRL   = 0x00,
    TORQUE = 0x01,
    OFF    = 0x02,
    BLANK  = 0x03,
    DECAY  = 0x04,
    STALL  = 0x05,
    DRIVE  = 0x06,
    STATUS = 0x07,
};

enum class HPSDStepMode : uint16_t
{
    MicroStep256 = 256,
    MicroStep128 = 128,
    MicroStep64  = 64,
    MicroStep32  = 32,
    MicroStep16  = 16,
    MicroStep8   = 8,
    MicroStep4   = 4,
    MicroStep2   = 2,
    MicroStep1   = 1,
};

enum class HPSDDecayMode : uint8_t
{
    Slow                = 0b000,
    SlowIncMixedDec     = 0b001,
    Fast                = 0b010,
    Mixed               = 0b011,
    SlowIncAutoMixedDec = 0b100,
    AutoMixed           = 0b101,
};

enum class HPSDStatusBit : uint8_t
{
    OTS    = 0,  // Overtemperature shutdown
    AOCP   = 1,  // Channel A overcurrent shutdown
    BOCP   = 2,  // Channel B overcurrent shutdown
    APDF   = 3,  // Channel A predriver fault
    BPDF   = 4,  // Channel B predriver fault
    UVLO   = 5,  // Undervoltage lockout
    STD    = 6,  // Stall detected
    STDLAT = 7,  // Latched stall detect
};

class DRV8711SPI
{
private:
    uint8_t cs_pin = 17;

public:
    void writeReg(uint8_t address, uint16_t value);     // Write the specified value to a register
    void writeReg(HPSDRegAddr address, uint16_t value); // Write the specified value to a register
    void setChipSelectPin();                            // Initilialize the GPIO function and select the PIN
};

class HighPowerStepperDriver
{
public:
    HighPowerStepperDriver();       // All settings set to power-on defaults
    void resetSettings();           // Changes all of the driver's settings back to their default values.
    void applySettings();           // Re-writes the cached settings stored in this class to the device.
    void enableDriver();            // Enables the driver (ENBL = 1).
    void disableDriver();           // Disables the driver (ENBL = 0).
    void setDirection(bool value);  // Sets the motor direction (RDIR).
    bool getDirection();            // Returns the cached value of the motor direction (RDIR).
    void step();                    // Advances the indexer by one step (RSTEP = 1).
    void setStepMode(HPSDStepMode mode);    // Sets the driver's stepping mode (MODE).
    void setStepMode(uint16_t mode);        // Sets the driver's stepping mode (MODE).
    void setCurrentMilliamps36v4(uint16_t current); // Sets the current limit for a High-Power Stepper Motor Driver 36v4.
    void setDecayMode(HPSDDecayMode mode);          // Sets the driver's decay mode (DECMOD).
    void clearStatus();                             // Clears any status conditions that are currently latched in the driver.

protected:
    uint16_t ctrl, torque, off, blank, decay, stall, drive;
    void writeCTRL();   // Writes the cached value of the CTRL register to the device.
    void writeTORQUE(); // Writes the cached value of the TORQUE register to the device.
    void writeOFF();    // Writes the cached value of the OFF register to the device.
    void writeBLANK();  // Writes the cached value of the BLANK register to the device.
    void writeDECAY();  // Writes the cached value of the DECAY register to the device.
    void writeSTALL();  // Writes the cached value of the STALL register to the device.
    void writeDRIVE();  // Writes the cached value of the DRIVE register to the device.

public:
    DRV8711SPI driver;
};

#endif  
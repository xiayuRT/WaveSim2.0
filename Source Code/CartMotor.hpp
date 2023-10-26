/**
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 * @file       CartMotor.hpp
 * @brief      Header file implemented with control instructions for high power stepper motor controller
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Aug-18-2023     Xiayu Z                         xiayu.zeng@runningtide.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 */

#ifndef _CARTMOTOR_HPP
#define _CARTMOTOR_HPP

#include <cstdint>
#include <iostream>
#include <pigpio.h>
#include <chrono>
#include <thread>

#include "spi.hpp"

// Send the error msg and terminate the program
static void pabort(const char *s){
    std::cout << s << std::endl; // output the error message to the terminal
    std::exit(EXIT_FAILURE);  // terminate the program
}

// Address of control and status registers
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

// Possible arguments to setStepMode().
enum class HPSDStepMode : uint16_t
{
  MicroStep256 = 256,
  MicroStep128 = 128,
  MicroStep64  =  64,
  MicroStep32  =  32,
  MicroStep16  =  16,
  MicroStep8   =   8,
  MicroStep4   =   4,
  MicroStep2   =   2,
  MicroStep1   =   1,
};

// Possible arguments to setDecayMode().
enum class HPSDDecayMode : uint8_t
{
  Slow                = 0b000,
  SlowIncMixedDec     = 0b001,
  Fast                = 0b010,
  Mixed               = 0b011,
  SlowIncAutoMixedDec = 0b100,
  AutoMixed           = 0b101,
};

// Bits that are set in the return value of readStatus() to indicate statusconditions.
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

// Low level functions for reading and writing from SPI with DRV8711
class DRV8711SPI{
private:
    uint8_t cs_pin = 17;

public:
    // Write the specified value to a register
    void writeReg(uint8_t address, uint16_t value){
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
    void writeReg(HPSDRegAddr address, uint16_t value){
        writeReg((uint8_t) address, value);
    }

    // Initilialize the GPIO function and select the PIN
    void setChipSelectPin()
    {
        if(!gpioInitialise()){
            pabort("Use 'sudo' to initialize the GPIO!\n");
        };
        gpioSetMode(cs_pin, PI_OUTPUT);
        gpioWrite(cs_pin, 0);
    }
};


// High level functions for motor control -> controller board
class HighPowerStepperDriver{

public:
    // the default constructor
    HighPowerStepperDriver(){
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
    void resetSettings()
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
    void applySettings()
    {
        writeTORQUE();
        writeOFF();
        writeBLANK();
        writeDECAY();
        writeDRIVE();
        writeSTALL();

        // CTRL is written last because it contains the ENBL bit, and we want to try
        // to have all the other settings correct first.  (For example, TORQUE
        // defaults to 0xFF (the maximum value), so it would be better to set a more
        // appropriate value if necessary before enabling the motor.)
        writeCTRL();
    }

    /// Enables the driver (ENBL = 1).
    void enableDriver()
    {
        ctrl |= (1 << 0);
        writeCTRL();
    }

    /// Disables the driver (ENBL = 0).
    void disableDriver()
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
    void setDirection(bool value)
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
    bool getDirection()
    {
        return ctrl >> 1 & 1;
    }

    /// Advances the indexer by one step (RSTEP = 1).
    ///
    /// You can use this command to step the stepper motor and leave the STEP pin
    /// disconnected.
    ///
    /// The driver automatically clears the RSTEP bit after it is written.
    void step()
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
    ///
    /// Example usage:
    /// ~~~{.cpp}
    /// sd.setStepMode(HPSDStepMode::MicroStep32);
    /// ~~~
    void setStepMode(HPSDStepMode mode)
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
    ///
    /// ~~~{.cpp}
    /// sd.setStepMode(32);
    /// ~~~
    ///
    /// \see setStepMode(HPSDStepMode)
    void setStepMode(uint16_t mode)
    {
        setStepMode((HPSDStepMode)mode);
    }

    /// Sets the current limit for a High-Power Stepper Motor Driver 36v4.
    ///
    /// The argument to this function should be the desired current limit in
    /// milliamps.
    ///
    /// WARNING: The 36v4 can supply up to about 4 A per coil continuously;
    /// higher currents might be sustainable for short periods, but can eventually
    /// cause the MOSFETs to overheat, which could damage them.  See the driver's
    /// product page for more information.
    ///
    /// This function allows you to set a current limit of up to 8 A (8000 mA),
    /// but we strongly recommend against using a current limit higher than 4 A
    /// (4000 mA) unless you are careful to monitor the MOSFETs' temperatures
    /// and/or restrict how long the driver uses the higher current limit.
    ///
    /// This function takes care of setting appropriate values for ISGAIN and
    /// TORQUE to get the desired current limit.
    void setCurrentMilliamps36v4(uint16_t current)
    {
        if (current > 8000) { current = 8000; }

        // From the DRV8711 datasheet, section 7.3.4, equation 2:
        //
        //   Ifs = (2.75 V * TORQUE) / (256 * ISGAIN * Risense)
        //
        // Rearranged:
        //
        //   TORQUE = (256 * ISGAIN * Risense * Ifs) / 2.75 V
        //
        // The 36v4 has an Risense of 30 milliohms, and "current" is in milliamps,
        // so:
        //
        //   TORQUE = (256 * ISGAIN * (30/1000) ohms * (current/1000) A) / 2.75 V
        //          = (7680 * ISGAIN * current) / 2750000
        //
        // We want to pick the highest gain (5, 10, 20, or 40) that will not
        // overflow TORQUE (8 bits, 0xFF max), so we start with a gain of 40 and
        // calculate the TORQUE value needed.
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
    ///
    /// Example usage:
    /// ~~~{.cpp}
    /// sd.setDecayMode(HPSDDecayMode::AutoMixed);
    /// ~~~
    void setDecayMode(HPSDDecayMode mode)
    {
        decay = (decay & 0b00011111111) | (((uint8_t)mode & 0b111) << 8);
        writeDECAY();
    }

    /// Clears any status conditions that are currently latched in the driver.
    ///
    /// WARNING: Calling this function clears latched faults, which might allow
    /// the motor driver outputs to reactivate.  If you do this repeatedly without
    /// fixing an abnormal condition (like a short circuit), you might damage the
    /// driver.
    void clearStatus()
    {
        driver.writeReg(HPSDRegAddr::STATUS, 0);
    }

    /// Clears any fault conditions that are currently latched in the driver.
    ///
    /// This function behaves the same as clearStatus(), except it only clears
    /// bits that indicate faults (STATUS bits 5:0).
    ///
    /// WARNING: Calling this function clears latched faults, which might allow
    /// the motor driver outputs to reactivate.  If you do this repeatedly without
    /// fixing an abnormal condition (like a short circuit), you might damage the
    /// driver.
    void clearFaults()
    {
        driver.writeReg(HPSDRegAddr::STATUS, ~0b00111111);
    }
    

protected:

    uint16_t ctrl, torque, off, blank, decay, stall, drive;

    /// Writes the cached value of the CTRL register to the device.
    void writeCTRL(){ driver.writeReg(HPSDRegAddr::CTRL, ctrl);}

    /// Writes the cached value of the TORQUE register to the device.
    void writeTORQUE()
    {
        driver.writeReg(HPSDRegAddr::TORQUE, torque);
    }

    /// Writes the cached value of the OFF register to the device.
    void writeOFF()
    {
        driver.writeReg(HPSDRegAddr::OFF, off);
    }

    /// Writes the cached value of the BLANK register to the device.
    void writeBLANK()
    {
        driver.writeReg(HPSDRegAddr::BLANK, blank);
    }

    /// Writes the cached value of the DECAY register to the device.
    void writeDECAY()
    {
        driver.writeReg(HPSDRegAddr::DECAY, decay);
    }

    /// Writes the cached value of the STALL register to the device.
    void writeSTALL()
    {
        driver.writeReg(HPSDRegAddr::STALL, stall);
    }

    /// Writes the cached value of the DRIVE register to the device.
    void writeDRIVE()
    {
        driver.writeReg(HPSDRegAddr::DRIVE, drive);
    }

    public:
        DRV8711SPI driver; 

};

void stepper_init(HighPowerStepperDriver& sd){
    sd.resetSettings();                          // clear the settings register
    sd.clearStatus();                            // clear the settings register
    sd.setDecayMode(HPSDDecayMode::AutoMixed);   // set the decay mode
    sd.setCurrentMilliamps36v4(2500);            // set the current limitation
    sd.setStepMode(HPSDStepMode::MicroStep64);  // set the micro-stepping mode
    sd.enableDriver();                           // enable the driver
}
#endif
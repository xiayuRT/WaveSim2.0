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

#ifndef CARTMOTOR_HPP
#define CARTMOTOR_HPP

#include <cstdint>

enum class hpsd_reg_addr : uint8_t
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

enum class hpsd_step_mode : uint16_t
{
    MICRO_STEP_256 = 256,
    MICRO_STEP_128 = 128,
    MICRO_STEP_64  = 64,
    MICRO_STEP_32  = 32,
    MICRO_STEP_16  = 16,
    MICRO_STEP_8   = 8,
    MICRO_STEP_4   = 4,
    MICRO_STEP_2   = 2,
    MICRO_STEP_1   = 1,
};

enum class hpsd_decay_mode : uint8_t
{
    SLOW                        = 0b000,
    SLOW_INC_MIXED_DEC          = 0b001,
    FAST                        = 0b010,
    MIXED                       = 0b011,
    SLOW_INC_AUTO_MIXED_DEC     = 0b100,
    AUTO_MIXED                  = 0b101,
};

enum class hpsd_status_bit : uint8_t
{
    OVER_TEMP_SHUTDOWN          = 0, 
    CHA_OVER_CURRENT_SHUTDOWN   = 1,  
    CHB_OVER_CURRENT_SHUTDOWN   = 2,  
    CHA_PREDRIVER_FAULT         = 3,  
    CHB_PREDRIVER_FAULT         = 4,  
    UNDER_VOLTAGE_LOCKOUT       = 5,  
    STALL_DETECTED              = 6,  
    LATCHED_STALL_DETECT        = 7,  
};

class drv8711_spi
{
private:
    uint8_t cs_pin = 17;

public:
    /**
     * @brief Write the specified value to an address
     * @param[in] address: 8-bit address
     * @param[in] value : 16-bit instruction 
     */ 
    void write_reg(uint8_t address, uint16_t value);    


    /**
     * @brief Write the specified value to a HPSD register
     * @param[in] address: drv8711 register address 
     * @param[in] value : 16-bit instruction 
     */   
    void write_reg(hpsd_reg_addr address, uint16_t value);


    /**
     * @brief Initialize the GPIO function and select the PIN
     */      
    void set_chip_select_pin();                            
};

class high_power_stepper_driver
{
public:
    high_power_stepper_driver();       


    /**
     * @brief Changes all of the driver's settings back to their default values.
     */      
    void reset_settings();     


    /**
     * @brief Re-writes the cached settings stored in this class to the device.
     */            
    void apply_settings();  


    /**
     * @brief Disables the driver (ENBL = 1).
     */           
    void enable_driver();   


    /**
     * @brief Disables the driver (ENBL = 0).
     */          
    void disable_driver();    


     /**
     * @brief Returns the cached value of the motor direction (RDIR). This does not perform any SPI communication with the driver.
     * @param[in] direction, 0/1 You can use this command to control the direction of the stepper motor and leave the DIR pin disconnected.
     */         
    void set_direction(bool value);  


    /**
     * @brief  Returns the cached value of the motor direction (RDIR). This does not perform any SPI communication with the driver.
     * @return bool
     * @retval true -> forward, false -> backward
     */    
    bool get_direction();    


    /**
     * @brief  Advances the indexer by one step (RSTEP = 1). You can use this command to step the stepper motor and leave the STEP pin disconnected.
     */           
    void step();    


    /**
     * @brief  Sets the driver's stepping mode (MODE).
     * @param[in] mode: set up step mode
     */                 
    void set_step_mode(hpsd_step_mode mode);  


    /**
     * @brief Sets the driver's decay mode (DECMOD). This version of the function allows you to express the requested microstepping ratio as a number directly.
     * @param[in] mode: set up step mode
     */  
    void set_step_mode(uint16_t mode);        

    
    /**
     * @brief Sets the driver's decay mode (DECMOD).
     * @param[in] current: value of current (milliamps)
     */
    void set_current_milliamps_36v4(uint16_t current);

    
    /**
     * @brief Sets the driver's decay mode (DECMOD).
     */ 
    void set_decay_mode(hpsd_decay_mode mode);          
    
    
    /**
     * @brief Clears any status conditions that are currently latched in the driver.
     */
    void clear_status();                             

protected:
    uint16_t ctrl, torque, off, blank, decay, stall, drive;
    
    
    /**
     * @brief Writes the cached value of the CTRL register to the device.
     */
    void write_ctrl();   
    
    
    /**
     * @brief Writes the cached value of the TORQUE register to the device.
     */
    void write_torque(); 
    
    
    /**
     * @brief Writes the cached value of the OFF register to the device.
     */
    void write_off();    
    

    /**
     * @brief Writes the cached value of the BLANK register to the device.
     */
    void write_blank();  
    

    /**
     * @brief Writes the cached value of the DECAY register to the device.
     */
    void write_decay(); 
    
    
    /**
     * @brief Writes the cached value of the STALL register to the device.
     */
    void write_stall(); 
    
    
    /**
     * @brief Writes the cached value of the DRIVE register to the device.
     */
    void write_drive(); 

public:
    drv8711_spi driver;
};

#endif

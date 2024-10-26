#ifndef GPIO_MANAGER_H
#define GPIO_MANAGER_H

/************************************************************************************************/
/*                            GPIO Manager Class Definition                                     */
/************************************************************************************************/

#include "ConfigManager.h"

/**
 * @class GPIOManager
 * @brief Manages the initialization and configuration of GPIO pins.
 * 
 * This class is responsible for setting up GPIO pins and initializing their states 
 * based on the values stored in the configuration manager. It provides methods 
 * for initializing pins and setting default values.
 */
class GPIOManager {
public:
    GPIOManager();
    void GPinitGPIOs();
};

#endif // GPIO_MANAGER_H
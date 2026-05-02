#pragma once
#include <LovyanGFX.hpp>

#define CONFIG_EINK_SPI_CLK 12
#define CONFIG_EINK_SPI_MOSI 11
#define CONFIG_EINK_SPI_MISO 13
#define CONFIG_EINK_SPI_CS 10
#define CONFIG_EINK_SPI_DC 9
#define CONFIG_EINK_RST 8
#define CONFIG_EINK_BUSY 7

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_IT8951 _panel_instance;
    lgfx::Bus_SPI _bus_instance;

public:
    LGFX(void);
};
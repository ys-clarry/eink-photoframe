#include "LGFX_Display.hpp"

LGFX::LGFX(void)
{
    { // SPI bus config
        auto cfg = _bus_instance.config();
        cfg.spi_host = SPI2_HOST;
        cfg.spi_mode = 0;
        cfg.freq_write = 80000000;
        cfg.freq_read = 16000000;
        cfg.spi_3wire = true;
        cfg.use_lock = true;
        cfg.dma_channel = 1;
        cfg.pin_sclk = CONFIG_EINK_SPI_CLK;
        cfg.pin_mosi = CONFIG_EINK_SPI_MOSI;
        cfg.pin_miso = CONFIG_EINK_SPI_MISO;
        cfg.pin_dc = CONFIG_EINK_SPI_DC;
        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
    }
    { // Panel config ... change if resolution is different
        auto cfg = _panel_instance.config();
        cfg.pin_cs = CONFIG_EINK_SPI_CS;
        cfg.pin_rst = CONFIG_EINK_RST;
        cfg.pin_busy = CONFIG_EINK_BUSY;
        cfg.memory_width = 1872;
        cfg.memory_height = 1404;
        cfg.panel_width = 1872;
        cfg.panel_height = 1404;
        cfg.offset_x = 0;
        cfg.offset_y = 0;
        cfg.offset_rotation = 0;
        cfg.dummy_read_pixel = 8;
        cfg.dummy_read_bits = 1;
        cfg.readable = true;
        cfg.invert = true;
        cfg.rgb_order = false;
        cfg.dlen_16bit = false;
        cfg.bus_shared = false;
        _panel_instance.config(cfg);
    }
    setPanel(&_panel_instance);
}
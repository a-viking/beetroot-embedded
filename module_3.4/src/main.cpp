#include <cstdint>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
    class NoteFq {
        public:
        static constexpr uint32_t PAUSE = 0;
        static constexpr uint32_t A3    = 220;
        static constexpr uint32_t C4    = 262;
        static constexpr uint32_t C4_sh = 277;
        static constexpr uint32_t D4    = 294;
        static constexpr uint32_t D4_sh = 311;
        static constexpr uint32_t E4    = 330;
        static constexpr uint32_t F4    = 349;
        static constexpr uint32_t F4_sh = 370;
        static constexpr uint32_t G4    = 392;
        static constexpr uint32_t G4_sh = 415;
        static constexpr uint32_t A4    = 440;
        static constexpr uint32_t A4_sh = 466;
        static constexpr uint32_t B4    = 494;

        static constexpr uint32_t C5    = 523;
        static constexpr uint32_t D5    = 587;
        static constexpr uint32_t E5    = 659;
        static constexpr uint32_t F5    = 698;
        static constexpr uint32_t G5    = 784;
        static constexpr uint32_t A5    = 880;
        static constexpr uint32_t B5    = 988;

        static constexpr uint32_t C6    = 1047;
        static constexpr uint32_t D6    = 1175;
        static constexpr uint32_t E6    = 1319;
        static constexpr uint32_t F6    = 1397;
        static constexpr uint32_t G6    = 1568;
        static constexpr uint32_t A6    = 1760;
        static constexpr uint32_t B6    = 1976;
    };

    constexpr gpio_num_t kOutputGpio = GPIO_NUM_5;
    constexpr ledc_mode_t kMode = LEDC_LOW_SPEED_MODE;
    constexpr ledc_timer_t kTimer = LEDC_TIMER_0;
    constexpr ledc_channel_t kChannel = LEDC_CHANNEL_0;
    constexpr uint32_t kPeriodCounts = 1024;

    void configurePwm() {
        ledc_timer_config_t timer{};
        timer.speed_mode = kMode;
        timer.timer_num = kTimer;
        timer.duty_resolution = LEDC_TIMER_10_BIT;
        timer.freq_hz = 440;
        timer.clk_cfg = LEDC_USE_APB_CLK;
        ESP_ERROR_CHECK(ledc_timer_config(&timer));

        ledc_channel_config_t output{};
        output.gpio_num = kOutputGpio;
        output.speed_mode = kMode;
        output.timer_sel = kTimer;
        output.channel = kChannel;
        output.duty = 0;
        output.hpoint = 0;
        ESP_ERROR_CHECK(ledc_channel_config(&output));
    }

    void setDuty(uint32_t highCounts) {
        ESP_ERROR_CHECK(ledc_set_duty(kMode, kChannel, highCounts));
        ESP_ERROR_CHECK(ledc_update_duty(kMode, kChannel));
    }

    void playTone(uint32_t frequencyHz, uint32_t durationMs) {
        ESP_ERROR_CHECK(ledc_set_freq(kMode, kTimer, frequencyHz));
        setDuty(kPeriodCounts / 2);
        vTaskDelay(pdMS_TO_TICKS(durationMs));
        setDuty(0);
    }

    struct Note {
        uint32_t frequency; // Hz
        uint32_t duration;  // ms
    };

    // Du hast
    // (A4, 125), (G4, 125), (D4, 125), (0, 125), (F4, 125), (0, 250), (E4, 125), (0, 125), (A3, 125), (0, 125), (D4, 125), (F4, 125), (G4, 125), (E4, 125), (0, 375),
    // (A4, 125), (G4, 125), (D4, 125), (F4, 125), (E4, 125), (0, 250), (A3, 125), (D4, 125), (F4, 125), (G4, 125), (E4, 125), (0, 375)
    constexpr Note kMelody[] = {
        {NoteFq::A5, 125},
        {NoteFq::G5, 125},
        {NoteFq::D5, 125},
        {NoteFq::PAUSE, 125},
        {NoteFq::F5, 125},
        {NoteFq::PAUSE, 125},
        {NoteFq::E5, 125},
        {NoteFq::PAUSE, 125},
        {NoteFq::A4, 125},
        {NoteFq::PAUSE, 125},
        {NoteFq::D5, 125},
        {NoteFq::F5, 125},
        {NoteFq::G5, 125},
        {NoteFq::PAUSE, 125},
        {NoteFq::E5, 125},
        {NoteFq::PAUSE, 250},

        {NoteFq::A5, 125},
        {NoteFq::G5, 125},
        {NoteFq::D5, 125},
        {NoteFq::F5, 125},
        {NoteFq::E5, 125},
        {NoteFq::A4, 125},
        {NoteFq::D5, 125},
        {NoteFq::F5, 125},
        {NoteFq::G5, 125},
        {NoteFq::E5, 125},
        {NoteFq::PAUSE, 375},
    };
}

extern "C" void app_main()
{
    configurePwm();
    while (true) {
        for (const Note& note : kMelody) {
            if (note.frequency == NoteFq::PAUSE) {
                setDuty(0);
                vTaskDelay(pdMS_TO_TICKS(note.duration));
            } else {
                playTone(note.frequency, note.duration);
            }
            vTaskDelay(pdMS_TO_TICKS(20));
        }
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

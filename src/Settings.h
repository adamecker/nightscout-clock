#ifndef Settings_h
#define Settings_h

#include <Arduino.h>

#include <vector>
#include "SettingsAlarm.h"
#include "SettingsSchedule.h"
#include "enums.h"

// Big text face: the value takes early_stale_color once a reading is early_stale_minutes old.
struct BigTextFaceSettings {
    bool early_stale_enabled = false;
    DISPLAY_COLOR early_stale_color = DISPLAY_COLOR::CYAN;
    int early_stale_minutes = 6;
};

// Simple (dark) and Big text (dark) faces: the color of the number while the reading is fresh.
struct SimpleDarkFaceSettings {
    DISPLAY_COLOR value_color = DISPLAY_COLOR::WHITE;
};

// Unicorn face: a moving mane moves its colors at `speed` in the `flow` style while the reading is fresh.
struct UnicornFaceSettings {
    bool mane_moving = false;
    ANIMATION_SPEED speed = ANIMATION_SPEED::NORMAL;
    MANE_FLOW flow = MANE_FLOW::DOWN;
};

class Settings {
public:
    String ssid;
    String wifi_password;
    String hostname;
    String nightscout_url;
    String nightscout_api_key;
    bool nightscout_simplified_api;
    BG_UNIT bg_units;
    int bg_low_warn_limit;
    int bg_high_warn_limit;
    int bg_low_urgent_limit;
    int bg_high_urgent_limit;
    DISPLAY_COLOR bg_color_urgent_low = DISPLAY_COLOR::RED;
    DISPLAY_COLOR bg_color_low = DISPLAY_COLOR::YELLOW;
    DISPLAY_COLOR bg_color_normal = DISPLAY_COLOR::GREEN;
    DISPLAY_COLOR bg_color_high = DISPLAY_COLOR::YELLOW;
    DISPLAY_COLOR bg_color_urgent_high = DISPLAY_COLOR::RED;
    BRIGHTNES_MODE brightness_mode;

// Dragon face: the flame moves at `speed` while the reading is fresh.
struct DragonFaceSettings {
    ANIMATION_SPEED speed = ANIMATION_SPEED::NORMAL;
};

// Race car face: the race moves at `speed` while the reading is fresh.
struct RaceCarFaceSettings {
    ANIMATION_SPEED speed = ANIMATION_SPEED::NORMAL;
};
    int brightness_level;
    int default_clockface;
    bool face_cycle_enabled = false;
    std::vector<int> inactive_faces;
    int face_cycle_interval_seconds = 60;
    bool face_schedule_enabled = false;
    std::vector<FaceScheduleEntry> face_schedule;
    bool school_mode_active = false;
    bool unicorn_mode = false;
    std::vector<int> school_mode_faces = {0, 1, 2, 4, 5, 9};
    BG_SOURCE bg_source;
    String dexcom_username;
    String dexcom_password;
    DEXCOM_SERVER dexcom_server;
    String librelinkup_email;
    String librelinkup_password;
    String librelinkup_region;
    String librelinkup_patient_id;
    // Medtrum Easy Follow
    String medtrum_email;
    String medtrum_password;
    String tz_libc_value;
    TIME_FORMAT time_format;
    bool alarm_urgent_low_enabled;
    int alarm_urgent_low_mgdl;
    int alarm_urgent_low_snooze_minutes;
    std::vector<AlertWindow> alarm_urgent_low_alert_windows;
    bool alarm_low_enabled;
    int alarm_low_mgdl;
    int alarm_low_snooze_minutes;
    std::vector<AlertWindow> alarm_low_alert_windows;
    bool alarm_high_enabled;
    int alarm_high_mgdl;
    int alarm_high_snooze_minutes;
    std::vector<AlertWindow> alarm_high_alert_windows;
    String alarm_high_melody;
    String alarm_low_melody;
    String alarm_urgent_low_melody;
    bool additional_wifi_enable;
    String additional_wifi_type;
    String additional_wifi_ssid;
    String additional_wifi_username;
    String additional_wifi_password;
    bool custom_hostname_enable;
    String custom_hostname;
    bool custom_nodatatimer_enable;
    int custom_nodatatimer;
    int bg_data_too_old_threshold_minutes = 20;
    DISPLAY_COLOR data_old_color = DISPLAY_COLOR::GRAY;
    BigTextFaceSettings face_big_text;
    UnicornFaceSettings face_unicorn;
    int alarm_high_volume;
    int alarm_low_volume;
    int alarm_urgent_low_volume;
    SimpleDarkFaceSettings face_simple_dark;
    DragonFaceSettings face_dragon;
    RaceCarFaceSettings face_race_car;
    bool alarm_intensive_mode;
    int alarm_repeat_interval_seconds = 300;
    bool web_auth_enable;
    String web_auth_password;
    // Automatic self-updates: daily check of the release manifest, applied without interaction.
    bool ota_auto_update = false;
    int ota_auto_update_hour = 3;
    // Optional heartbeat: the clock POSTs a small JSON status on a schedule.
    // Works with healthchecks.io, ntfy.sh, or any webhook receiver.
    String healthcheck_url;
    int healthcheck_interval_hours = 1;
};

#endif

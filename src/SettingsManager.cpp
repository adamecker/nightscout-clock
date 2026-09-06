#include "SettingsManager.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Preferences.h>

#include "globals.h"

#define CONFIG_JSON_BAK "/config.bak"

namespace {
bool isValidFaceCycleInterval(int intervalSeconds) {
    return intervalSeconds == 10 || intervalSeconds == 30 || intervalSeconds == 60 ||
           intervalSeconds == 120 || intervalSeconds == 180 || intervalSeconds == 300;
}
}  // namespace

// The getter for the instantiated singleton instance
SettingsManager_& SettingsManager_::getInstance() {
    static SettingsManager_ instance;
    return instance;
}

// Initialize the global shared instance
SettingsManager_& SettingsManager = SettingsManager.getInstance();

void SettingsManager_::setup() {
    bool mounted = false;
    // Layer 1: Retry mounting up to 3 times with settling delays
    for (int attempt = 1; attempt <= 3; attempt++) {
        if (LittleFS.begin(false)) {
            mounted = true;
            DEBUG_PRINTF("LittleFS mounted successfully on attempt %d\n", attempt);
            break;
        }
        DEBUG_PRINTF("LittleFS mount attempt %d failed, waiting...\n", attempt);
        LittleFS.end();
        delay(150);
    }

    if (!mounted) {
        DEBUG_PRINTLN("LittleFS unmountable after 3 tries. Mounting with formatOnFail...");
        LittleFS.begin(true);
    }
}

bool copyFile(const char* srcPath, const char* destPath) {
    File srcFile = LittleFS.open(srcPath, "r");
    if (!srcFile) {
        DEBUG_PRINTLN("Failed to open source file");
        return false;
    }

    File destFile = LittleFS.open(destPath, "w");
    if (!destFile) {
        DEBUG_PRINTLN("Failed to open destination file");
        srcFile.close();
        return false;
    }

    while (srcFile.available()) {
        char data = srcFile.read();
        destFile.write(data);
    }

    srcFile.close();
    destFile.close();

    DEBUG_PRINTLN("File copied successfully");
    return true;
}

void SettingsManager_::factoryReset() {
    copyFile(CONFIG_JSON_FACTORY, CONFIG_JSON);
    LittleFS.end();
    ESP.restart();
}

// Helper: Safely reads and parses a JSON file, trimming any power-cut trailing garbage
static JsonDocument* safeReadJsonFile(const char* path) {
    if (!LittleFS.exists(path)) {
        return NULL;
    }

    File file = LittleFS.open(path, "r");
    if (!file || file.isDirectory()) {
        DEBUG_PRINTF("Failed to open %s for reading\n", path);
        return NULL;
    }

    String content = file.readString();
    file.close();

    if (content.length() == 0) {
        return NULL;
    }

    // Layer 2: Trim trailing null or garbage bytes after the final '}'
    int lastBrace = content.lastIndexOf('}');
    if (lastBrace != -1 && lastBrace < (int)content.length() - 1) {
        content = content.substring(0, lastBrace + 1);
    }

    auto doc = new JsonDocument();
    DeserializationError error = deserializeJson(*doc, content);
    if (error) {
        DEBUG_PRINTF("JSON error in %s: %s\n", path, error.c_str());
        delete doc;
        return NULL;
    }

    return doc;
}

JsonDocument* SettingsManager_::readConfigJsonFile() {
    // 1. Try reading the primary config file
    JsonDocument* doc = safeReadJsonFile(CONFIG_JSON);
    if (doc != NULL) {
        return doc;
    }

    // 2. Layer 3: If primary failed, try loading from the backup file
    DEBUG_PRINTLN("Primary config unreadable, trying backup /config.bak...");
    doc = safeReadJsonFile(CONFIG_JSON_BAK);
    if (doc != NULL) {
        DEBUG_PRINTLN("Restoring primary config from /config.bak...");
        copyFile(CONFIG_JSON_BAK, CONFIG_JSON);
        return doc;
    }

    // 3. Fallback: Only copy factory default if both primary and backup failed
    DEBUG_PRINTLN("Both primary and backup failed. Falling back to factory template...");
    copyFile(CONFIG_JSON_FACTORY, CONFIG_JSON);
    return safeReadJsonFile(CONFIG_JSON);
}

bool SettingsManager_::loadSettingsFromFile() {
    auto doc = readConfigJsonFile();
    if (doc == NULL) {
        DEBUG_PRINTLN("Warning: Could not read config file, checking NVS fallback...");

        // Layer 4: Load Wi-Fi credentials from crash-proof NVS storage
        Preferences netPrefs;
        netPrefs.begin("custom_net", true);
        settings.ssid = netPrefs.getString("ssid", "");
        settings.wifi_password = netPrefs.getString("pass", "");
        netPrefs.end();

        settings.bg_low_warn_limit = 70;
        settings.bg_high_warn_limit = 180;
        settings.bg_low_urgent_limit = 55;
        settings.bg_high_urgent_limit = 250;
        settings.bg_units = BG_UNIT::MGDL;
        settings.brightness_mode = BRIGHTNES_MODE::AUTO_LINEAR;
        settings.brightness_level = 5;
        settings.default_clockface = 0;
        settings.bg_source = BG_SOURCE::NO_SOURCE;
        settings.tz_libc_value = "PST8PDT,M3.2.0,M11.1.0";
        settings.time_format = TIME_FORMAT::HOURS_12;
        settings.alarm_urgent_low_enabled = false;
        settings.alarm_low_enabled = false;
        settings.alarm_high_enabled = false;
        settings.additional_wifi_enable = false;
        settings.custom_hostname_enable = false;
        settings.custom_nodatatimer_enable = false;
        settings.web_auth_enable = false;

        return true; // Never trigger showFatalError
    }

    settings.ssid = (*doc)["ssid"].as<String>();
    settings.wifi_password = (*doc)["password"].as<String>();

    // Mirror Wi-Fi credentials into NVS for safe keeping
    if (settings.ssid.length() > 0) {
        Preferences netPrefs;
        netPrefs.begin("custom_net", false);
        netPrefs.putString("ssid", settings.ssid);
        netPrefs.putString("pass", settings.wifi_password);
        netPrefs.end();
    }

    settings.bg_low_warn_limit = (*doc)["low_mgdl"].as<int>();
    settings.bg_high_warn_limit = (*doc)["high_mgdl"].as<int>();
    settings.bg_low_urgent_limit = (*doc)["low_urgent_mgdl"].as<int>();
    settings.bg_high_urgent_limit = (*doc)["high_urgent_mgdl"].as<int>();
    settings.bg_units = (*doc)["units"].as<String>() == "mmol" ? BG_UNIT::MMOLL : BG_UNIT::MGDL;

    String brightness_mode = (*doc)["brightness_mode"].as<String>();
    if (brightness_mode == "manual") {
        settings.brightness_mode = BRIGHTNES_MODE::MANUAL;
    } else if (brightness_mode == "auto_linear") {
        settings.brightness_mode = BRIGHTNES_MODE::AUTO_LINEAR;
    } else if (brightness_mode == "auto_dimmed") {
        settings.brightness_mode = BRIGHTNES_MODE::AUTO_DIMMED;
    } else {
        DEBUG_PRINTLN(
            "Unknown brightness mode in config: " + brightness_mode + ", defaulting to AUTO_LINEAR");
        settings.brightness_mode = BRIGHTNES_MODE::AUTO_LINEAR;
    }

    settings.brightness_level = (*doc)["brightness_level"].as<int>() - 1;
    settings.default_clockface = (*doc)["default_face"].as<int>();

    settings.face_cycle_enabled = (*doc)["face_cycle_enabled"] | false;
    settings.face_cycle_interval_seconds = (*doc)["face_cycle_interval_seconds"] | 60;
    if (!isValidFaceCycleInterval(settings.face_cycle_interval_seconds)) {
        DEBUG_PRINTLN("Invalid face cycle interval in config, defaulting to 60 seconds");
        settings.face_cycle_interval_seconds = 60;
    }

    settings.face_cycle_faces.clear();
    bool faceAlreadyAdded[6] = {};
    if ((*doc)["face_cycle_faces"].is<JsonArray>()) {
        for (JsonVariant face : (*doc)["face_cycle_faces"].as<JsonArray>()) {
            if (!face.is<int>()) {
                continue;
            }

            int faceId = face.as<int>();
            if (faceId >= 0 && faceId < 6 && !faceAlreadyAdded[faceId]) {
                settings.face_cycle_faces.push_back(faceId);
                faceAlreadyAdded[faceId] = true;
            }
        }
    }
    if (settings.face_cycle_faces.empty()) {
        int fallbackFace = settings.default_clockface >= 0 && settings.default_clockface < 6
                               ? settings.default_clockface
                               : 0;
        settings.face_cycle_faces.push_back(fallbackFace);
    }
    if (settings.face_cycle_enabled && settings.face_cycle_faces.size() < 2) {
        DEBUG_PRINTLN("Too few valid faces in config, disabling face cycling");
        settings.face_cycle_enabled = false;
    }

    String data_source = (*doc)["data_source"].as<String>();
    if (data_source == "nightscout") {
        settings.bg_source = BG_SOURCE::NIGHTSCOUT;
    } else if (data_source == "dexcom") {
        settings.bg_source = BG_SOURCE::DEXCOM;
    } else if (data_source == "medtronic") {
        settings.bg_source = BG_SOURCE::MEDTRONIC;
    } else if (data_source == "api") {
        settings.bg_source = BG_SOURCE::API;
    } else if (data_source == "librelinkup") {
        settings.bg_source = BG_SOURCE::LIBRELINKUP;
    } else if (data_source == "medtrum") {
        settings.bg_source = BG_SOURCE::MEDTRUM;
    } else {
        settings.bg_source = BG_SOURCE::NO_SOURCE;
    }
    settings.medtrum_email = (*doc)["medtrum_email"].as<String>();
    settings.medtrum_password = (*doc)["medtrum_password"].as<String>();
    settings.dexcom_username = (*doc)["dexcom_username"].as<String>();
    settings.dexcom_password = (*doc)["dexcom_password"].as<String>();
    String dexcomServerStr = (*doc)["dexcom_server"].as<String>();
    if (dexcomServerStr == "us") {
        settings.dexcom_server = DEXCOM_SERVER::US;
    } else if (dexcomServerStr == "ous") {
        settings.dexcom_server = DEXCOM_SERVER::NON_US;
    } else if (dexcomServerStr == "jp") {
        settings.dexcom_server = DEXCOM_SERVER::JAPAN;
    } else {
        DEBUG_PRINTLN("Unknown Dexcom server in config, defaulting to NON_US");
        settings.dexcom_server = DEXCOM_SERVER::NON_US;
    }

    settings.librelinkup_email = (*doc)["librelinkup_email"].as<String>();
    settings.librelinkup_password = (*doc)["librelinkup_password"].as<String>();
    settings.librelinkup_region = (*doc)["librelinkup_region"].as<String>();
    settings.librelinkup_patient_id = (*doc)["librelinkup_patient_id"].as<String>();

    settings.nightscout_url = (*doc)["nightscout_url"].as<String>();
    settings.nightscout_api_key = (*doc)["api_secret"].as<String>();
    settings.nightscout_simplified_api = (*doc)["nightscout_simplified_api"].as<bool>();

    settings.tz_libc_value = (*doc)["tz_libc"].as<String>();
    settings.time_format =
        (*doc)["time_format"].as<String>() == "12" ? TIME_FORMAT::HOURS_12 : TIME_FORMAT::HOURS_24;

    // read alarms data
    settings.alarm_urgent_low_enabled = (*doc)["alarm_urgent_low_enabled"].as<bool>();
    settings.alarm_urgent_low_mgdl = (*doc)["alarm_urgent_low_value"].as<int>();
    settings.alarm_urgent_low_snooze_minutes = (*doc)["alarm_urgent_low_snooze_interval"].as<int>();
    settings.alarm_urgent_low_silence_interval =
        (*doc)["alarm_urgent_low_silence_interval"].as<String>();
    settings.alarm_low_enabled = (*doc)["alarm_low_enabled"].as<bool>();
    settings.alarm_low_mgdl = (*doc)["alarm_low_value"].as<int>();
    settings.alarm_low_snooze_minutes = (*doc)["alarm_low_snooze_interval"].as<int>();
    settings.alarm_low_silence_interval = (*doc)["alarm_low_silence_interval"].as<String>();
    settings.alarm_high_enabled = (*doc)["alarm_high_enabled"].as<bool>();
    settings.alarm_high_mgdl = (*doc)["alarm_high_value"].as<int>();
    settings.alarm_high_snooze_minutes = (*doc)["alarm_high_snooze_interval"].as<int>();
    settings.alarm_high_silence_interval = (*doc)["alarm_high_silence_interval"].as<String>();
    settings.alarm_high_melody = (*doc)["alarm_high_melody"].as<String>();
    settings.alarm_low_melody = (*doc)["alarm_low_melody"].as<String>();
    settings.alarm_urgent_low_melody = (*doc)["alarm_urgent_low_melody"].as<String>();
    settings.alarm_intensive_mode = (*doc)["alarm_intensive_mode"].as<bool>();

    // Additional WiFi
    settings.additional_wifi_enable = (*doc)["additional_wifi_enable"].as<bool>();
    settings.additional_wifi_type = (*doc)["additional_wifi_type"].as<String>();
    settings.additional_ssid = (*doc)["additional_ssid"].as<String>();
    settings.additional_wifi_username = (*doc)["additional_wifi_username"].as<String>();
    settings.additional_wifi_password = (*doc)["additional_wifi_password"].as<String>();

    // Custom hostname
    settings.custom_hostname_enable = (*doc)["custom_hostname_enable"].as<bool>();
    settings.custom_hostname = (*doc)["custom_hostname"].as<String>();

    // Custom No Data Timer
    settings.custom_nodatatimer_enable = (*doc)["custom_nodatatimer_enable"].as<bool>();
    settings.custom_nodatatimer = (*doc)["custom_nodatatimer"].as<int>();
    if (settings.custom_nodatatimer_enable == true && settings.custom_nodatatimer > 5 &&
        settings.custom_nodatatimer <= 60) {
        settings.bg_data_too_old_threshold_minutes = settings.custom_nodatatimer;
    } else {
        settings.bg_data_too_old_threshold_minutes = 20;  // default value
        if (settings.custom_nodatatimer_enable == true) {
            DEBUG_PRINTLN("Custom No Data Timer value is invalid, using default value of 20 minutes.");
        }
    }

    // Web interface authentication
    settings.web_auth_enable = (*doc)["web_auth_enable"].as<bool>();
    settings.web_auth_password = (*doc)["web_auth_password"].as<String>();

    delete doc;

    this->settings = settings;
    return true;
}

bool SettingsManager_::saveSettingsToFile() {
    auto doc = readConfigJsonFile();
    if (doc == NULL)
        return false;

    (*doc)["ssid"] = settings.ssid;
    (*doc)["password"] = settings.wifi_password;

    (*doc)["low_mgdl"] = settings.bg_low_warn_limit;
    (*doc)["high_mgdl"] = settings.bg_high_warn_limit;
    (*doc)["low_urgent_mgdl"] = settings.bg_low_urgent_limit;
    (*doc)["high_urgent_mgdl"] = settings.bg_high_urgent_limit;

    (*doc)["units"] = settings.bg_units == BG_UNIT::MMOLL ? "mmol" : "mgdl";

    (*doc)["brightness_mode"] = settings.brightness_mode == BRIGHTNES_MODE::AUTO_LINEAR   ? "auto_linear"
                                : settings.brightness_mode == BRIGHTNES_MODE::AUTO_DIMMED ? "auto_dimmed"
                                                                                          : "manual";
    (*doc)["brightness_level"] = settings.brightness_level + 1;
    (*doc)["default_face"] = settings.default_clockface;
    (*doc)["face_cycle_enabled"] = settings.face_cycle_enabled;
    (*doc)["face_cycle_interval_seconds"] = settings.face_cycle_interval_seconds;
    (*doc).remove("face_cycle_faces");
    JsonArray faceCycleFaces = (*doc)["face_cycle_faces"].to<JsonArray>();
    for (int faceId : settings.face_cycle_faces) {
        faceCycleFaces.add(faceId);
    }

    String data_source = "no_source";
    switch (settings.bg_source) {
        case BG_SOURCE::NIGHTSCOUT:
            data_source = "nightscout";
            break;
        case BG_SOURCE::DEXCOM:
            data_source = "dexcom";
            break;
        case BG_SOURCE::MEDTRONIC:
            data_source = "medtronic";
            break;
        case BG_SOURCE::API:
            data_source = "api";
            break;
        case BG_SOURCE::LIBRELINKUP:
            data_source = "librelinkup";
            break;
        case BG_SOURCE::MEDTRUM:
            data_source = "medtrum";
            break;
        default:
            data_source = "no_source";
            break;
    }
    (*doc)["data_source"] = data_source;
    (*doc)["medtrum_email"] = settings.medtrum_email;
    (*doc)["medtrum_password"] = settings.medtrum_password;

    (*doc)["dexcom_username"] = settings.dexcom_username;
    (*doc)["dexcom_password"] = settings.dexcom_password;
    switch (settings.dexcom_server) {
        case DEXCOM_SERVER::US:
            (*doc)["dexcom_server"] = "us";
            break;
        case DEXCOM_SERVER::NON_US:
            (*doc)["dexcom_server"] = "ous";
            break;
        case DEXCOM_SERVER::JAPAN:
            (*doc)["dexcom_server"] = "jp";
            break;
        default:
            DEBUG_PRINTLN("Unknown Dexcom server, defaulting to US");
            (*doc)["dexcom_server"] = "ous";
            break;
    }

    (*doc)["librelinkup_email"] = settings.librelinkup_email;
    (*doc)["librelinkup_password"] = settings.librelinkup_password;
    (*doc)["librelinkup_region"] = settings.librelinkup_region;
    (*doc)["librelinkup_patient_id"] = settings.librelinkup_patient_id;

    (*doc)["nightscout_url"] = settings.nightscout_url;
    (*doc)["api_secret"] = settings.nightscout_api_key;
    (*doc)["nightscout_simplified_api"] = settings.nightscout_simplified_api;

    (*doc)["tz_libc"] = settings.tz_libc_value;
    (*doc)["time_format"] = settings.time_format == TIME_FORMAT::HOURS_12 ? "12" : "24";

    // save alarms data
    (*doc)["alarm_urgent_low_enabled"] = settings.alarm_urgent_low_enabled;
    (*doc)["alarm_urgent_low_value"] = settings.alarm_urgent_low_mgdl;
    (*doc)["alarm_urgent_low_snooze_interval"] = settings.alarm_urgent_low_snooze_minutes;
    (*doc)["alarm_urgent_low_silence_interval"] = settings.alarm_urgent_low_silence_interval;
    (*doc)["alarm_low_enabled"] = settings.alarm_low_enabled;
    (*doc)["alarm_low_value"] = settings.alarm_low_mgdl;
    (*doc)["alarm_low_snooze_interval"] = settings.alarm_low_snooze_minutes;
    (*doc)["alarm_low_silence_interval"] = settings.alarm_low_silence_interval;
    (*doc)["alarm_high_enabled"] = settings.alarm_high_enabled;
    (*doc)["alarm_high_value"] = settings.alarm_high_mgdl;
    (*doc)["alarm_high_snooze_interval"] = settings.alarm_high_snooze_minutes;
    (*doc)["alarm_high_silence_interval"] = settings.alarm_high_silence_interval;
    (*doc)["alarm_high_melody"] = settings.alarm_high_melody;
    (*doc)["alarm_low_melody"] = settings.alarm_low_melody;
    (*doc)["alarm_urgent_low_melody"] = settings.alarm_urgent_low_melody;
    (*doc)["alarm_intensive_mode"] = settings.alarm_intensive_mode;

    // Additional WiFi
    (*doc)["additional_wifi_enable"] = settings.additional_wifi_enable;
    (*doc)["additional_wifi_type"] = settings.additional_wifi_type;
    (*doc)["additional_ssid"] = settings.additional_wifi_ssid;
    (*doc)["additional_wifi_username"] = settings.additional_wifi_username;
    (*doc)["additional_wifi_password"] = settings.additional_wifi_password;

    // Custom hostname
    (*doc)["custom_hostname_enable"] = settings.custom_hostname_enable;
    (*doc)["custom_hostname"] = settings.custom_hostname;

    // Custom No Data Timer
    (*doc)["custom_nodatatimer_enable"] = settings.custom_nodatatimer_enable;
    (*doc)["custom_nodatatimer"] = settings.custom_nodatatimer;

    // Web interface authentication
    (*doc)["web_auth_enable"] = settings.web_auth_enable;
    (*doc)["web_auth_password"] = settings.web_auth_password;

    if (trySaveJsonAsSettings(*doc) == false)
        return false;

    delete doc;

    return true;
}

bool SettingsManager_::trySaveJsonAsSettings(JsonDocument doc) {
    DEBUG_PRINTLN(doc.as<String>());
    auto file = LittleFS.open(CONFIG_JSON, FILE_WRITE);
    if (!file) {
        DEBUG_PRINTLN("Failed to open config file for writing");
        return false;
    }

    auto result = file.print(doc.as<String>());
    file.close();
    if (!result) {
        return false;
    }

    // Automatically create a backup file every time settings are saved
    copyFile(CONFIG_JSON, CONFIG_JSON_BAK);

    return true;
}

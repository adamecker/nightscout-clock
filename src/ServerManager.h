#ifndef ServerManager_h
#define ServerManager_h

#include <Arduino.h>
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <time.h>

class ServerManager_ {
private:
    bool apMode;
    AsyncWebServer* ws;
    AsyncStaticWebHandler* staticFilesHandler = nullptr;
    ServerManager_() = default;
    unsigned long lastTimeSync = 0;

    IPAddress startWifi();
    void setupWebServer(IPAddress ip);
    IPAddress setAPmode(String ssid, String psk);
    void saveConfigHandler();
    bool initTimeIfNeeded();
    void setTimezone();
    String getHostname();
    bool isWebAuthEnabled() const;
    bool isRequestAuthenticated(AsyncWebServerRequest* request) const;
    String generateAuthToken();
    String webAuthToken;
    unsigned long webAuthTokenIssuedMs = 0;

    // Authenticated network OTA update state (POST /api/update/firmware and
    // POST /api/update/filesystem stream uploads into the Arduino Update class)
    int otaUpdateCommand = -1;
    bool otaUpdateAuthFailed = false;
    size_t otaUpdateWritten = 0;
    String otaUpdateError;
    void handleUpdateUpload(AsyncWebServerRequest* request, const String& filename, size_t index,
                            uint8_t* data, size_t len, bool final, int command);
    void handleUpdateRequest(AsyncWebServerRequest* request);

public:
    static ServerManager_& getInstance();
    void setup();
    void tick();
    void stop();
    bool isConnected;
    bool isInAPMode;
    IPAddress myIP;
    DNSServer dnsServer;
    unsigned long getUtcEpoch();
    tm getTimezonedTime();
    bool tryGetTimezonedTime(tm& timeinfo);
    AsyncWebHandler addHandler(AsyncWebHandler* handler);
    void removeStaticFileHandler();
    void addStaticFileHandler();
    bool enforceAuthentication(AsyncWebServerRequest* request);
    int failedAttempts = 0;
    void reconnectWifi();
};

extern ServerManager_& ServerManager;

#endif
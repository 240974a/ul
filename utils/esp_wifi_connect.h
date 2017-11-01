namespace ul {
namespace ESP 
{
    void wifi_connect(const char* ssid, const char* password) {
        Serial.println("\n\nConnecting to WiFi...");
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        while (WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.println("Connection Failed! Rebooting in 5s...");
            delay(5000);
            ::ESP.restart();
        }
        Serial.print("connected, IP: ");
        Serial.println(WiFi.localIP());
    }
}}
